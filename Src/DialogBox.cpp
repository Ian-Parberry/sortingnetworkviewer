/// \file DialogBox.cpp
/// \brief Code for CDialogBox.

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

#include "DialogBox.h"
#include "resource.h"

UINT CDialogBox::m_nNumInputs = 0; ///< Number of inputs.

/// Pop up a custom modal dialog box and get an unsigned integer from an edit
/// box. Fail if something goes wrong or the requested number of inputs is
/// less than 2.
/// \param hwnd Window handle.
/// \param n [OUT] Reference to result.
/// \return S_OK if successful, otherwise E_FAIL.

HRESULT CDialogBox::GetNumInputs(HWND hwnd, UINT& n){
  DialogBox(nullptr, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, (DLGPROC)DlgProc);

  if(m_nNumInputs < 2)
    return E_FAIL;

  n = (UINT)m_nNumInputs;
  return S_OK;
} //GetNumInputs

/// If the user clicks on the `OK` button, then `m_nNumInputs` is set to
/// the value in the edit box. If anything goes wrong, `m_nNumInputs` is 
/// set to zero.
/// \param hDlg Dialog box handle.
/// \param iMsg Message code.
/// \param wp Parameter for message.
/// \param lp Second parameter for message if needed.
/// \return TRUE If message is handled.

BOOL CALLBACK CDialogBox::DlgProc(HWND hDlg, UINT iMsg, WPARAM wp, LPARAM lp){
  BOOL bOK = TRUE;

  switch(iMsg){ 
    case WM_INITDIALOG:
      SetDlgItemInt(hDlg, IDC_EDIT1, 16, FALSE); //default number of inputs
      return TRUE;

    case WM_COMMAND: 
      switch(LOWORD(wp)){ 
        case IDOK: //user specifies the number of inputs
          m_nNumInputs = GetDlgItemInt(hDlg, IDC_EDIT1, &bOK, FALSE);
          if(!bOK)m_nNumInputs = 0; //something went wrong
          EndDialog(hDlg, wp); 
          return TRUE; 
 
        case IDCANCEL: //user declines
          m_nNumInputs = 0; 
          EndDialog(hDlg, wp); 
          return TRUE; 
       } //switch
    } //switch

    return FALSE;
} //DlgProc
