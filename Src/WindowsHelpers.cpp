/// \file WindowsHelpers.cpp
/// \brief Code for some helpful Windows-specific functions.
///
/// These platform-dependent functions are hidden away so that the faint-of-heart
/// don't have to see them if they're offended by them. 

// MIT License
//
// Copyright (c) 2022 Ian Parberry
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include <shobjidl_core.h>
#include <atlbase.h>

#include "WindowsHelpers.h"
#include "Includes.h"
#include "SortingNetwork.h"
#include "resource.h"

/// Remove the path and the extension from a file name.
/// \param wstrPath File path, possibly with extension.
/// \return File name with path and extension removed (if any).

std::wstring FileNameBase(const std::wstring& wstrPath){
  const size_t n0 = wstrPath.find_last_of(L"\\");

  std::wstring wstrResult = (n0 == std::string::npos)? wstrPath:
    wstrPath.substr(n0 + 1, wstrPath.size() - n0);

  const size_t n1 = wstrResult.find_last_of(L".");

  if(n1 != std::string::npos)
    wstrResult = wstrResult.substr(0, n1);

  return wstrResult;
} //FileNameBase

/// Resize the drag rectangle provided by a WM_SIZING message to ensure a
/// minimum client area width and height.
/// \param hwnd Window handle.
/// \param wParam WMSZ message telling us which edge is being dragged on.
/// \param pRect [IN, OUT] Pointer to drag rectangle.
/// \param n Minimum width and height of client area.

void MinDragRect(HWND hwnd, WPARAM wParam, RECT* pRect, int n){ 
  RECT cr; //client rectangle
  RECT wr; //window rectangle, includes client rectangle and borders

  GetClientRect(hwnd, &cr);
  GetWindowRect(hwnd, &wr);

  //combined border width and height

  const int bw = (wr.right - wr.left) - (cr.right - cr.left); //border width
  const int bh = (wr.bottom - wr.top) - (cr.bottom - cr.top); //border height

  //new drag window width and height

  const int dw = max(n, pRect->right - pRect->left - bw) + bw; //new width
  const int dh = max(n, pRect->bottom - pRect->top - bh) + bh; //new height

  //enforce new  drag window width and height

  switch(wParam){ //which edge are we dragging on?
    case WMSZ_LEFT: //left edge
      pRect->left = pRect->right - dw;
      break;

    case WMSZ_RIGHT: //right edge
      pRect->right = pRect->left + dw;
      break;
      
    case WMSZ_TOP: //top edge
      pRect->top = pRect->bottom - dh;
      break;
      
    case WMSZ_BOTTOM: //bottom edge
      pRect->bottom = pRect->top + dh;
      break;
      
    case WMSZ_TOPRIGHT: //top right corner
      pRect->top = pRect->bottom - dh;
      pRect->right = pRect->left + dw;
      break;

    case WMSZ_TOPLEFT: //top left corner
      pRect->top = pRect->bottom - dh;
      pRect->left = pRect->right - dw;
      break;

    case WMSZ_BOTTOMRIGHT: //bottom right corner
      pRect->bottom = pRect->top + dh;
      pRect->right = pRect->left + dw;
      break;

    case WMSZ_BOTTOMLEFT: //bottom left corner
      pRect->bottom = pRect->top + dh;
      pRect->left = pRect->right - dw;
     break;
  } //switch
} //MinDragRect

///////////////////////////////////////////////////////////////////////////////
// Initialization functions

#pragma region Initialization

/// Create and initialize a window.
/// \param hInst Instance handle.
/// \param nShow 1 to show window, 0 to hide.
/// \param WndProc Window procedure.

void InitWindow(HINSTANCE hInst, INT nShow, WNDPROC WndProc){
  const char appname[] = "Sorting Network Verify and Draw";
   
  WNDCLASSEX wndClass = {0}; //extended window class structure

  wndClass.cbSize         = sizeof(WNDCLASSEX);
  wndClass.style          = CS_HREDRAW | CS_VREDRAW;
  wndClass.lpfnWndProc    = WndProc;
  wndClass.cbClsExtra     = 0;
  wndClass.cbWndExtra     = 0;
  wndClass.hInstance      = hInst;
  wndClass.hIcon          = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
  wndClass.hCursor        = LoadCursor(nullptr, IDC_ARROW);
  wndClass.hbrBackground  = nullptr;
  wndClass.lpszMenuName   = nullptr;
  wndClass.lpszClassName  = appname;
  wndClass.hIconSm        = 0;
   
  RegisterClassEx(&wndClass);

  const DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX| WS_THICKFRAME | WS_SYSMENU; 
  const DWORD dwStyleEx = WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME;

  const int w = 600; //window client area width.
  const int h = 600; //window client area height.
    
  RECT r;  
  r.left = 0; r.right = w; 
  r.top = 0; r.bottom = h + GetSystemMetrics(SM_CYMENU);
  AdjustWindowRectEx(&r, dwStyle, FALSE, dwStyleEx); 

  const HWND hwnd = CreateWindowEx(dwStyleEx, appname, appname, dwStyle, 
    CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top, 
    nullptr, nullptr, hInst, nullptr);
  
  ShowWindow(hwnd, nShow);
  UpdateWindow(hwnd);
} //InitWindow

/// Initialize GDI+ and get a GDI+ token.
/// \return A GDI+ token.

ULONG_PTR InitGDIPlus(){
  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken;
  Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
  return gdiplusToken;
} //InitGDIPlus

#pragma endregion Initialization

///////////////////////////////////////////////////////////////////////////////
// Save functions

#pragma region Save

/// Get an encoder clsid for an image file format.
/// \param format File format using wide characters.
/// \param pClsid [OUT] Pointer to clsid.
/// \return `S_OK` for success, `E_FAIL` for failure.

HRESULT GetEncoderClsid(const WCHAR* format, CLSID* pClsid){
  UINT num = 0; //number of image encoders
  UINT n = 0; //size of the image encoder array in bytes
  HRESULT hr = E_FAIL; //return result

  Gdiplus::ImageCodecInfo* pCodecInfo = nullptr; //for codec info
  if(FAILED(Gdiplus::GetImageEncodersSize(&num, &n)))return E_FAIL; //get sizes
  if(n == 0)return E_FAIL; //there are no encoders

  pCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(n)); //allocate codec info memory
  if(pCodecInfo == nullptr)return E_FAIL; //malloc failed (as if)
  if(FAILED(GetImageEncoders(num, n, pCodecInfo)))return E_FAIL; //get encoders

  for(UINT j=0; j<num && hr!=S_OK; j++) //for each encoder, while not found
    if(wcscmp(pCodecInfo[j].MimeType, format) == 0){ //found the codex we want
      *pClsid = pCodecInfo[j].Clsid; //return it
      hr = S_OK; //success
    } //if

  free(pCodecInfo); //clean up
  return hr;
} //GetEncoderClsid

/// Display a `Save` dialog box save an image of a comparator network to the
/// file that the user selects.
/// Export an image to a file.
/// \param t Export file type.
/// \param hwnd Window handle.
/// \param pNet Pointer to a renderable comparator network.
/// \param wstrName Default file name.
/// \return S_OK for success, E_FAIL for failure.

HRESULT ExportImage(const eExport t, HWND hwnd, CRenderableComparatorNet* pNet,
  std::wstring& wstrName)
{  
  COMDLG_FILTERSPEC filetypes[1];
  std::wstring wstrTitle = L"Export "; //for dialog box title
  std::wstring wstrDefaultExtension; //for dialog box default file extension

  //set file types, title, and default extension for export type

  switch(t){
    case eExport::Png:
      filetypes[0] =  {L"PNG Files", L"*.png"}; 
      wstrTitle += L"PNG"; wstrDefaultExtension = L"png";
      break;

    case eExport::Svg: 
      filetypes[0] =  {L"SVG Files", L"*.svg"};
      wstrTitle += L"SVG"; wstrDefaultExtension = L"svg";
      break;

    case eExport::TeX:
      filetypes[0] =  {L"TeX Files", L"*.tex"};
      wstrTitle += L"TeX"; wstrDefaultExtension = L"tex";
      break;
  } //switch

  wstrTitle += L" Image";

  //prepare for the dialog box

  CComPtr<IFileSaveDialog> pDlg; //pointer to save dialog box
  CComPtr<IShellItem> pItem; //item pointer
  LPWSTR pwsz = nullptr; //pointer to null-terminated wide string for result

  //fire up the save dialog box  
  
  HRESULT hr = pDlg.CoCreateInstance(__uuidof(FileSaveDialog));
 
  if(SUCCEEDED(hr)){ 
    pDlg->SetFileTypes(_countof(filetypes), filetypes); //set file types
    pDlg->SetTitle(wstrTitle.c_str()); //set title bar text
    pDlg->SetFileName(wstrName.c_str()); //set default file name
    pDlg->SetDefaultExtension(wstrDefaultExtension.c_str()); //set default extension  
 
    hr = pDlg->Show(hwnd); //show the dialog box

    if(SUCCEEDED(hr)){
      hr = pDlg->GetResult(&pItem); //get the result item

      if(SUCCEEDED(hr)){
        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pwsz); //get file name

        if(SUCCEEDED(hr)){ 
          switch(t){ //here's where the export happens
            case eExport::Png: hr = pNet->ExportToPNG(pwsz); break;
            case eExport::Svg: hr = pNet->ExportToSVG(pwsz); break;
            case eExport::TeX: hr = pNet->ExportToTex(pwsz); break;
          } //switch

          if(SUCCEEDED(hr))
            wstrName = FileNameBase(std::wstring(pwsz)); //set file name
        } //if
      } //if 
    } //if
  } //if

  CoTaskMemFree(pwsz); //clean up

  return hr;
} //ExportImage

/// Pop up a Windows `Open` dialog box for the user to pick a text file
/// and read the comparator network from there into a comparator network.
/// \param hwnd Window handle.
/// \param pNet [OUT] Pointer to a renderable comparator network.
/// \param wstrName [IN, OUT] File name without extension.
/// \return `S_OK` for success, `E_FAIL` for failure.

HRESULT Load(HWND hwnd, CComparatorNetwork* pNet,
  std::wstring& wstrName)
{
  COMDLG_FILTERSPEC filetypes[] = { //text files only
    {L"TXT Files", L"*.txt"}
  }; //filetypes

  HRESULT hr = S_OK; //success or failure

  CComPtr<IFileOpenDialog> pDlg; //pointer to save dialog box
  CComPtr<IShellItem> pItem; //item pointer
  LPWSTR pwsz = nullptr; //pointer to null-terminated wide string for result

  hr = pDlg.CoCreateInstance(__uuidof(FileOpenDialog)); //fire up the Open dialog box

  if(SUCCEEDED(hr)){ 
    pDlg->SetFileTypes(_countof(filetypes), filetypes); //set file types
    pDlg->SetTitle(L"Open Comparator Network"); //set title bar text
    pDlg->SetDefaultExtension(L"txt"); //set default extension

    hr = pDlg->Show(hwnd); //show the dialog box
 
    if(SUCCEEDED(hr)){ 
      hr = pDlg->GetResult(&pItem);

      if(SUCCEEDED(hr))
        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pwsz);
    } //if
    
    if(SUCCEEDED(hr))
      hr = pNet->Read(pwsz)? S_OK: E_FAIL; //read file into comparator network
  } //if
  
  if(SUCCEEDED(hr))
    wstrName = FileNameBase(std::wstring(pwsz)); //set file name

  CoTaskMemFree(pwsz); //clean up

  return hr;
} //Load

#pragma endregion Save

///////////////////////////////////////////////////////////////////////////////
// Create menu functions

#pragma region Create menu functions

/// Create the `File` menu.
/// \param hParent Handle to the parent menu.

void CreateFileMenu(HMENU hParent){
  HMENU hMenu = CreateMenu();
  
  AppendMenuW(hMenu, MF_STRING, IDM_FILE_OPEN,   L"Open...");
  AppendMenuW(hMenu, MF_STRING, IDM_FILE_VERIFY, L"Verify...");
  CreateExportMenu(hMenu); //create Export sub-menu
  AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);
  AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT,   L"Quit");
  
  AppendMenuW(hParent, MF_POPUP, (UINT_PTR)hMenu, L"&File");
  
  EnableMenuItem(hMenu, IDM_FILE_EXPORT_PNG, MF_GRAYED);
  EnableMenuItem(hMenu, IDM_FILE_EXPORT_TEX, MF_GRAYED);
  EnableMenuItem(hMenu, IDM_FILE_EXPORT_SVG, MF_GRAYED);
  EnableMenuItem(hMenu, IDM_FILE_VERIFY, MF_GRAYED);
} //CreateFileMenu

/// Create the `Export` menu.
/// \param hParent Handle to the parent menu.

void CreateExportMenu(HMENU hParent){
  HMENU hMenu = CreateMenu();
  
  AppendMenuW(hMenu, MF_STRING, IDM_FILE_EXPORT_PNG,   L"Png");
  AppendMenuW(hMenu, MF_STRING, IDM_FILE_EXPORT_SVG,   L"Svg");
  AppendMenuW(hMenu, MF_STRING, IDM_FILE_EXPORT_TEX,   L"TeX");
  
  AppendMenuW(hParent, MF_POPUP, (UINT_PTR)hMenu, L"Export");
} //CreateExportMenu

/// Create the `Generate` menu.
/// \param hParent Handle to the parent menu.

void CreateGenerateMenu(HMENU hParent){
  HMENU hMenu = CreateMenu();
  
  AppendMenuW(hMenu, MF_STRING, IDM_GENERATE_ODDEVEN,  L"Odd-even");
  AppendMenuW(hMenu, MF_STRING, IDM_GENERATE_BITONIC,  L"Bitonic");
  AppendMenuW(hMenu, MF_STRING, IDM_GENERATE_PAIRWISE, L"Pairwise");
  
  AppendMenuW(hParent, MF_POPUP, (UINT_PTR)hMenu, L"Generate");
} //CreateGenerateMenu

/// Create the `View` menu.
/// \param hParent Handle to the parent menu.

void CreateViewMenu(HMENU hParent){
  HMENU hMenu = CreateMenu();
  
  AppendMenuW(hMenu, MF_STRING, IDM_VIEW_HORIZONTAL, L"Horizontal");
  AppendMenuW(hMenu, MF_STRING, IDM_VIEW_VERTICAL, L"Vertical");

  AppendMenuW(hParent, MF_POPUP, (UINT_PTR)hMenu, L"&View");
} //CreateViewMenu

/// Create the `Help` menu.
/// \param hParent Handle to the parent menu.

void CreateHelpMenu(HMENU hParent){
  HMENU hMenu = CreateMenu();
  
  AppendMenuW(hMenu, MF_STRING, IDM_HELP_HELP,    L"Display help...");
  AppendMenuW(hMenu, MF_STRING, IDM_HELP_ABOUT,   L"About...");
  AppendMenuW(hParent, MF_POPUP, (UINT_PTR)hMenu, L"&Help");
} //CreateHelpMenu

#pragma endregion Create menu functions
