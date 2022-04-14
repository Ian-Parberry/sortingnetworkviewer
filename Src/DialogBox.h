/// \file DialogBox.h
/// \brief Interface for CDialogBox.

// MIT License
//
// Copyright (c) 2022 Ian Parberry
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __DialogBox_h__
#define __DialogBox_h__

#include "Includes.h"

/// \brief Custom dialog box.
///
/// Custom dialog box for getting the number of inputs from the user for
/// a generated sorting network.

class CDialogBox{
  private:  
    static UINT m_nNumInputs; ///< Number of inputs.

  public:
    HRESULT GetNumInputs(HWND, UINT&); ///< Get number of inputs.
    static BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM); ///< Dialog box procedure.
}; //CDialogBox

#endif //__DialogBox_h__
