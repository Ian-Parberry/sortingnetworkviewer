/// \file BinaryGrayCode.h
/// \brief Interface for the binary reflected Gray code generator CBinaryGrayCode.

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

#ifndef __BinaryGrayCode_h__
#define __BinaryGrayCode_h__

#include "Defines.h"
#include "Includes.h"

/// \brief Binary reflected Gray code generator.
///
/// A binary Gray code generates all strings of n bits in such a way that
/// each string differs from the previous one in exactly one bit.
/// This class implements a nonrecursive version of the binary reflected Gray
/// code generation algorithm from the following paper:
/// 
/// > Bitner, Ehrlich, and Reingold, "Efficient generation of the Binary
/// > Reflected Gray Code and its applications", _Communications of the ACM_,
/// > Vol. 19, No. 9, pp 517-521, 1976.

class CBinaryGrayCode{
  public:
    UINT m_nZeros = 0; ///< Number of zeros in the code word.
    UINT m_nSize = 0; ///< Size of the code word.
    UINT* m_nGrayCodeWord = nullptr; ///< Current code word.
    UINT* m_nGrayCodeStack = nullptr; ///< Stack to remove recursion.

  public:
    ~CBinaryGrayCode(); ///< Destructor.

    virtual void Initialize(const UINT); ///< Get first code word.
    virtual UINT Next(); ///< Get next code word.
}; //CBinaryGrayCode

#endif //__BinaryGrayCode_h__