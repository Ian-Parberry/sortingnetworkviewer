/// \file WindowsHelpers.h
/// \brief Interface for some helpful Windows-specific functions.
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

#ifndef __WINDOWSHELPERS_H__
#define __WINDOWSHELPERS_H__

#include "Includes.h"
#include "RenderableComparatorNet.h"

///////////////////////////////////////////////////////////////////////////////
// Menu IDs

#pragma region Menu IDs

#define IDM_FILE_OPEN       1 ///< Menu id for Generate.
#define IDM_FILE_VERIFY     2 ///< Menu id for Verify.
#define IDM_FILE_EXPORT_PNG 3 ///< Menu id for Export PNG.
#define IDM_FILE_EXPORT_TEX 4 ///< Menu id for Export TeX.
#define IDM_FILE_EXPORT_SVG 5 ///< Menu id for Export SVG.
#define IDM_FILE_QUIT       6 ///< Menu id for Quit.

#define IDM_GENERATE_MINBUBBLE 7 ///< Menu id for Generate min-bubblesort.
#define IDM_GENERATE_MAXBUBBLE 8 ///< Menu id for Generate max-bubblesort.
#define IDM_GENERATE_BUBBLE    9 ///< Menu id for Generate min-bubblesort.
#define IDM_GENERATE_ODDEVEN  10 ///< Menu id for Generate odd-even.
#define IDM_GENERATE_BITONIC  11 ///< Menu id for Generate bitonic.
#define IDM_GENERATE_PAIRWISE 12 ///< Menu id for Generate pairwise.

#define IDM_VIEW_VERTICAL   13 ///< Menu id for vertical view.
#define IDM_VIEW_HORIZONTAL 14 ///< Menu id for horizontal view.

#define IDM_HELP_HELP  15 ///< Menu id for display help.
#define IDM_HELP_ABOUT 16 ///< Menu id for display About info.

#pragma endregion Menu IDs

///////////////////////////////////////////////////////////////////////////////
// Helper functions

#pragma region Helper functions

//initialization functions

void InitWindow(HINSTANCE, INT, WNDPROC); ///< Initialize window.
ULONG_PTR InitGDIPlus(); ///< Initialize GDI+.

//others

HRESULT GetEncoderClsid(const WCHAR*, CLSID*); ///< Get encoder CLSID.

HRESULT Load(HWND, CComparatorNetwork*, std::wstring&); ///< Load comparator network.
HRESULT ExportImage(const eExport, HWND, CRenderableComparatorNet*, std::wstring&); ///< Export.

void MinDragRect(HWND, WPARAM, RECT*, int, int); ///< Enforce minimum drag rectangle.

#pragma endregion Helper functions

///////////////////////////////////////////////////////////////////////////////
// Menu functions

#pragma region Menu functions

void CreateFileMenu(HMENU hParent); ///< Create `File` menu.
void CreateExportMenu(HMENU hParent); ///< Create `File` menu.
void CreateGenerateMenu(HMENU hParent); ///< Create `Generate` menu.
void CreateViewMenu(HMENU hParent); ///< Create `View` menu.
void CreateHelpMenu(HMENU hParent); ///< Create `Help` menu.

#pragma endregion Menu functions

#endif //__WINDOWSHELPERS_H__
