/// \file Main.cpp
/// \brief The window procedure WndProc(), and wWinMain().

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

#ifdef _DEBUG
  #include <vld.h> //Visual Leak Detector from http://vld.codeplex.com/
#endif

#include "Includes.h"

#include "CMain.h"

#include "OddEven.h"
#include "Bitonic.h"
#include "Pairwise.h"
#include "Bubblesort.h"

static CMain* g_pMain = nullptr; ///< Pointer to the main class.

/// \brief Window procedure.
///
/// This is the handler for messages from the operating system.
/// \param hWnd Window handle.
/// \param message Message code.
/// \param wParam Parameter for message.
/// \param lParam Second parameter for message if needed.
/// \return 0 If message is handled.

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
  UINT nMenuId = 0; //menu identifier for menu command messages
  static bool bResizing = false;

  switch(message){
    case WM_CREATE: //window has been created
      g_pMain = new CMain(hWnd); //create the main class
      return 0;

    case WM_DESTROY: //window has been removed from the screen
      delete g_pMain; //delete the main class
      PostQuitMessage(0); //ready to shut down
      return 0;
      
    case WM_SIZING: //user is resizing the window
	    MinDragRect(hWnd, wParam, (RECT*)lParam, 320); //enforce minimum size
	    return 0;

    case WM_PAINT: //window needs to be redrawn
      g_pMain->OnPaint();
      return 0;
 
    case WM_COMMAND: //user has selected a command from the menu
      nMenuId = LOWORD(wParam); //menu id

      switch(nMenuId){  
        case IDM_FILE_OPEN: //open and read a comparator network file
          g_pMain->Read();
          g_pMain->Draw();
          InvalidateRect(hWnd, nullptr, FALSE);
          break;

        case IDM_FILE_EXPORT_PNG: //export to PNG file 
          g_pMain->Export(eExport::Png);
          break;

        case IDM_FILE_EXPORT_TEX: //export to TeX file 
          g_pMain->Export(eExport::TeX);
          break;

        case IDM_FILE_EXPORT_SVG: //export to SVG file  
          g_pMain->Export(eExport::Svg);
          break;

        case IDM_FILE_VERIFY: //verify that it sorts
          if(g_pMain->Verify()){ //redundant comparators trigger redraw
            g_pMain->Draw();
            InvalidateRect(hWnd, nullptr, FALSE);
          } //if
          break;

        case IDM_FILE_QUIT: //so long, farewell, auf weidersehn, goodbye!
          SendMessage(hWnd, WM_CLOSE, 0, 0);
          break;

        //---------------------------------------------------

        case IDM_GENERATE_MINBUBBLE: //generate min-bubblesort sorting network 
          g_pMain->Generate<CBubbleSortMin>();
          g_pMain->Draw();
          InvalidateRect(hWnd, nullptr, FALSE);
          break;

        case IDM_GENERATE_MAXBUBBLE: //generate max-bubblesort sorting network  
          g_pMain->Generate<CBubbleSortMax>();
          g_pMain->Draw();
          InvalidateRect(hWnd, nullptr, FALSE);
          break;

        case IDM_GENERATE_BUBBLE: //generate bubblesort sorting network  
          g_pMain->Generate<CBubbleSort>();
          g_pMain->Draw();
          InvalidateRect(hWnd, nullptr, FALSE);
          break;

        case IDM_GENERATE_ODDEVEN: //generate odd-even sorting network 
          g_pMain->GeneratePowerOf2<COddEvenSort>();
          g_pMain->Draw();
          InvalidateRect(hWnd, nullptr, FALSE);
          break;

        case IDM_GENERATE_BITONIC: //generate bitonic sorting network 
          g_pMain->GeneratePowerOf2<CBitonicSort>();
          g_pMain->Draw();
          InvalidateRect(hWnd, nullptr, FALSE);
          break;

        case IDM_GENERATE_PAIRWISE: //generate pairwise sorting network 
          g_pMain->GeneratePowerOf2<CPairwiseSort>();
          g_pMain->Draw();
          InvalidateRect(hWnd, nullptr, FALSE);
          break;

        //---------------------------------------------------

        case IDM_VIEW_VERTICAL: //vertical view mode
          g_pMain->SetDrawStyle(eDrawStyle::Vertical);
          g_pMain->Draw();
          InvalidateRect(hWnd, nullptr, FALSE);
          break;

        case IDM_VIEW_HORIZONTAL: //horizontal view mode
          g_pMain->SetDrawStyle(eDrawStyle::Horizontal);
          g_pMain->Draw();
          InvalidateRect(hWnd, nullptr, FALSE);
          break;

        //---------------------------------------------------

        case IDM_HELP_HELP: //show help
          ShellExecute(0, 0, 
            "https://ian-parberry.github.io/sortingnetworkviewer/html", 
            0, 0, SW_SHOW);
          break;

        case IDM_HELP_ABOUT:  //show ABout dialog box.
          MessageBox(nullptr, 
            "Copyright © Ian Parberry, 2022.\nSource code available under the MIT License from https://github.com/Ian-Parberry/sortingnetworkviewer/.", 
            "About", MB_ICONINFORMATION | MB_OK);
          break;
      } //switch

      return 0; //all is good

    default: 
      return DefWindowProc(hWnd, message, wParam, lParam); //not my message
  } //switch
} //WndProc

/// \brief Winmain.  
///
/// Initialize a window and start the message pump. 
/// \param hInst Handle to the current instance.
/// \param hPrev Unused.
/// \param lpStr Unused.
/// \param nShow Nonzero if window is to be shown.
/// \return 0 If this application terminates correctly, otherwise an error code.

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPWSTR lpStr, int nShow){
  UNREFERENCED_PARAMETER(hPrev); //nope
  UNREFERENCED_PARAMETER(lpStr); //nope nope

  InitWindow(hInst, nShow, WndProc); //create and show a window

  MSG msg; //current message

  while(GetMessage(&msg, nullptr, 0, 0)){ //message pump
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  } //while

  return (int)msg.wParam;
} //wWinMain
