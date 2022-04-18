/// \file CMain.cpp
/// \brief Code for the main class CMain.

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

#include "CMain.h"

#include "Defines.h"
#include "Helpers.h"
#include "WindowsHelpers.h"
#include "DialogBox.h"

#include "Bubblesort.h"
#include "OddEven.h"
#include "Bitonic.h"
#include "Pairwise.h"

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors

#pragma region Constructors and destructors

/// Initialize GDI+, create and initialize the menus.
/// \param hwnd Window handle.

CMain::CMain(const HWND hwnd):
  m_hWnd(hwnd)
{
  CreateMenus(); //create the menu bar
  SetDrawStyle(m_eDrawStyle); //check mark next to draw style in menu

  m_gdiplusToken = InitGDIPlus(); //initialize GDI+
} //constructor

/// Delete the sorting network and shut down GDI+.

CMain::~CMain(){
  delete m_pSortingNetwork;
  Gdiplus::GdiplusShutdown(m_gdiplusToken);
} //destructor

#pragma endregion Constructors and destructors

///////////////////////////////////////////////////////////////////////////////
// Drawing functions

#pragma region Drawing functions

/// If we have a comparator network loaded or generated, then draw its bitmap
/// to the window client area, scaled as necessary using GDI+. This function
/// should only be called from the Window procedure in response to a `WM_PAINT`
/// message.

void CMain::OnPaint(){  
  PAINTSTRUCT ps; //paint structure
  HDC hdc = BeginPaint(m_hWnd, &ps); //device context
  Gdiplus::Graphics graphics(hdc); //GDI+ graphics object
  graphics.Clear(Gdiplus::Color::White);  //clear to white

  Gdiplus::Bitmap* pBitmap = GetBitmap();

  if(pBitmap){
    const int w = pBitmap->GetWidth(); //bitmap width 
    const int h = pBitmap->GetHeight(); //bitmap height

    //get client area width and height

    RECT rectClient; //for client rectangle
    GetClientRect(m_hWnd, &rectClient); //get client rectangle
    const int nClientWidth  = rectClient.right - rectClient.left; //client width
    const int nClientHeight = rectClient.bottom - rectClient.top; //client height

     //compute the drawing scale for the bitmap

    const UINT margin = 20; //margin width

    const float xscale = float(nClientWidth - 2*margin)/w; //horizontal scale
    const float yscale = float(nClientHeight - 2*margin)/h; //vertical scale
    const float scale = min(min(xscale, yscale), 1); //actual scale

    //compute the destination rectangle

    Gdiplus::Rect rectDest; //destination rectangle

    rectDest.Width  = (int)std::floor(scale*w);
    rectDest.Height = (int)std::floor(scale*h);

    rectDest.X = max(margin, (nClientWidth - rectDest.Width)/2);
    rectDest.Y = max(margin, (nClientHeight - rectDest.Height)/2);

    //draw the bitmap to the window
  
    graphics.DrawImage(pBitmap, rectDest);
  } //if

  EndPaint(m_hWnd, &ps); //this must be done last
} //OnPaint

#pragma endregion Drawing functions

///////////////////////////////////////////////////////////////////////////////
// Menu functions

#pragma region Menu functions

/// Add the menus to the menu bar. Store handles to the ones that have
/// entries that are initially grayed out to prevent the user from selecting
/// them when there is no comparator network loaded.

void CMain::CreateMenus(){
  m_hMenuBar = CreateMenu(); //create menu bar

  CreateFileMenu(m_hMenuBar); //attach File menu to menu bar
  CreateGenerateMenu(m_hMenuBar); //attach Generate menu to menu bar
  CreateViewMenu(m_hMenuBar); //attach View menu to menu bar
  CreateHelpMenu(m_hMenuBar); //attach Help menu to menu bar

  SetMenu(m_hWnd, m_hMenuBar); //attach menu bar to app window
} //CreateMenus

/// Enable the menu items that were initially grayed out to prevent the user
/// from selecting them when there was no comparator network loaded.

void CMain::EnableMenus(){
  EnableMenuItem(m_hMenuBar, IDM_FILE_EXPORT_PNG, MF_ENABLED);
  EnableMenuItem(m_hMenuBar, IDM_FILE_EXPORT_TEX, MF_ENABLED);
  EnableMenuItem(m_hMenuBar, IDM_FILE_EXPORT_SVG, MF_ENABLED);
  EnableMenuItem(m_hMenuBar, IDM_FILE_VERIFY,     MF_ENABLED);
} //EnableMenus

#pragma endregion Menu functions

///////////////////////////////////////////////////////////////////////////////
// Other functions

/// Reader function for the bitmap pointer `m_pBitmap`.
/// \return The bitmap pointer `m_pBitmap`.

Gdiplus::Bitmap* CMain::GetBitmap(){
  return (m_pSortingNetwork? m_pSortingNetwork->GetBitmap(): nullptr);
} //GetBitmap

/// Attempt to read a comparator network from a file into a new instance of
/// `CSortingNetwork` and put a pointer to it in `m_pSortingNetwork`. If
/// successful, then enable the menu items that are grayed out by default
/// until a comparator network is successully loaded. Otherwise set
/// `m_pSortingNetwork` to `nullptr`.

void CMain::Read(){
  delete m_pSortingNetwork;
  m_pSortingNetwork = new CSortingNetwork;

  if(Load(m_hWnd, m_pSortingNetwork, m_wstrName) == S_OK) //file read succeeded
    EnableMenus();

  else{ //file read failed
    delete m_pSortingNetwork;
    m_pSortingNetwork = nullptr;

    EnableMenuItem(m_hMenuBar, IDM_FILE_EXPORT_PNG, MF_GRAYED);
    EnableMenuItem(m_hMenuBar, IDM_FILE_EXPORT_TEX, MF_GRAYED);
    EnableMenuItem(m_hMenuBar, IDM_FILE_EXPORT_SVG, MF_GRAYED);
    EnableMenuItem(m_hMenuBar, IDM_FILE_VERIFY,     MF_GRAYED);
  } //else
} //Read

/// Pop up a custom dialog box asking for the number of inputs. If successful
/// and in range, then generate a sorting network with that number of inputs.
/// \tparam t Class of sorting network to generate.

template<class t> void CMain::Generate(){
  UINT n = 0; //for number of inputs

  if(SUCCEEDED(CDialogBox().GetNumInputs(m_hWnd, n))){ //got number of inputs
    delete m_pSortingNetwork; //delete old sorting network
  
    t* p = new t(n); //sorting network
    m_wstrName = p->GetName(); //get name
    m_pSortingNetwork = p; //this is the new sorting network

    EnableMenus();
  } //if
} //Generate

template void CMain::Generate<CBubbleSortMin>(); ///< Generate min-bubblesort.
template void CMain::Generate<CBubbleSortMax>(); ///< Generate max-bubblesort.
template void CMain::Generate<CBubbleSort>(); ///< Generate bubblesort.

/// Pop up a custom dialog box asking for the number of inputs. If successful
/// and in range, then round the number of inputs up to the closest power of 2,
/// generate a sorting network of that size, then prune away unneeded channels
/// and comparators.
/// \tparam t Class of sorting network to generate.

template<class t> void CMain::GeneratePowerOf2(){
  UINT n = 0; //for number of inputs

  if(SUCCEEDED(CDialogBox().GetNumInputs(m_hWnd, n))){ //got number of inputs
    delete m_pSortingNetwork; //delete old sorting network
  
    t* p = new t(CeilLog2(n)); //sorting network with inputs rounded up to next power of 2
    if(!IsPowerOf2(n))p->Prune(n); //prune unneeded channels and comparators
    m_wstrName = p->GetName(); //get name
    m_pSortingNetwork = p; //this is the new sorting network

    EnableMenus();
  } //if
} //GeneratePowerOf2

template void CMain::GeneratePowerOf2<COddEvenSort>(); ///< Generate odd-even.
template void CMain::GeneratePowerOf2<CBitonicSort>(); ///< Generate bitonic.
template void CMain::GeneratePowerOf2<CPairwiseSort>(); ///< Generate pairwise.

/// If a comparator network exists, then draw it to a new bitmap of the
/// appropriate width and depth. Put a pointer to the bitmap into `m_pBitmap`.

void CMain::Draw(){
  if(m_pSortingNetwork) //safety
    m_pSortingNetwork->Draw(m_eDrawStyle);
} //Draw

/// Export an image of the comparator network.
/// \param t Export file type.
/// \return `S_OK` for success, `E_FAIL` for failure.

HRESULT CMain::Export(const eExport t){
  return ExportImage(t, m_hWnd, m_pSortingNetwork, m_wstrName);
} //Export

/// Pop up a message box that tells the user information about the comparator
/// network and whether or not it is a sorting network.
/// The latter will take time exponential in the number of inputs, which is
/// probably the best we can do because sorting network verification
/// is Co-NP-complete even for shallow sorting networks (see the following paper).
///
/// > I. Parberry. "Single-exception sorting networks and the computational complexity of 
/// > optimal sorting network verification", _Mathematical Systems Theory_, Vol. 23, No. 1,
/// >  pp. 81–93, 1990.
///
/// If the number of inputs to the sorting network is 30 or larger, the user
/// is given the option of whether or not to proceed with verification
/// via a Yes/No dialog box.

void CMain::Verify(){
  if(m_pSortingNetwork == nullptr)return; //bail out

  const std::string strInputs = std::to_string(m_pSortingNetwork->GetNumInputs());
  const std::string strDepth = std::to_string(m_pSortingNetwork->GetDepth());
  const std::string strSize = std::to_string(m_pSortingNetwork->GetSize());

  const std::string strDetails = "of size " + strSize + " and depth " + strDepth;

  UINT nIcon = 0; //for icon flag in message box
  std::string s; //for message box text

  bool bSorts = false; //true if it sorts
  bool bUnknown = false; //true if sorting status is unknown

  if(m_pSortingNetwork->GetNumInputs() < 30)
    bSorts = m_pSortingNetwork && m_pSortingNetwork->sorts();

  else{
    s = "Sorting network verification is Co-NP-complete.";
    s += " This may take a long time. Are you sure you want to proceed?";

    const int id = MessageBox(nullptr, s.c_str(), "Verify", MB_ICONQUESTION | MB_YESNO);
    
    if(id == IDYES)
      bSorts = m_pSortingNetwork && m_pSortingNetwork->sorts();
    else bUnknown = true;
  } //else

  if(bSorts){
    s = "This is a " + strInputs + "-input sorting network " + strDetails + ".";
    nIcon = MB_ICONINFORMATION;
  } //if

  else if(bUnknown){   
    s = "This is a " + strInputs + "-input comparator network " + strDetails + ".";
    nIcon = MB_ICONINFORMATION;
  } //else if

  else{
    s = "This is a " + strInputs + "-input comparator network " + strDetails +
      " that is not a sorting network.";
    nIcon = MB_ICONERROR;
  } //else

  if(m_pSortingNetwork && m_pSortingNetwork->FirstNormalForm())
    s += " It is in First Normal Form.";
  else s += " It is not in First Normal Form.";
  
  const UINT nUnused = m_pSortingNetwork->GetUnused();
  const std::string strUnused = std::to_string(nUnused);

  s += " There are " + (nUnused == 0? "no": strUnused) + " redundant comparators.";
  
  MessageBox(nullptr, s.c_str(), "Verify", nIcon | MB_OK);
} //Verify

/// Set the draw style and put a checkmark next to the corresponding menu item.
/// \param d Draw style enumerated type.

void CMain::SetDrawStyle(const eDrawStyle d){
  m_eDrawStyle = d;

  switch(d){   
    case eDrawStyle::Vertical:
      CheckMenuItem(m_hMenuBar, IDM_VIEW_VERTICAL,   MF_CHECKED);
      CheckMenuItem(m_hMenuBar, IDM_VIEW_HORIZONTAL, MF_UNCHECKED);
      break;

    case eDrawStyle::Horizontal: 
      CheckMenuItem(m_hMenuBar, IDM_VIEW_VERTICAL,   MF_UNCHECKED);
      CheckMenuItem(m_hMenuBar, IDM_VIEW_HORIZONTAL, MF_CHECKED);
      break;
    } //switch
} //SetDrawStyle