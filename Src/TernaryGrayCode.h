/// \file TernaryGrayCode.h
/// \brief Interface for the ternary reflected Gray code generator class CTernaryGrayCode.

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

#ifndef __TernaryGrayCode_h__
#define __TernaryGrayCode_h__

#include "Defines.h"
#include "BinaryGrayCode.h"

/// \brief Ternary reflected Gray code generator.
///
/// A ternary Gray code generates all strings of n bits in such a way that
/// it is made up of the following bit pairs: 00 01 11, with an additional 
/// single bit if n is odd, and each string differs from the previous one in
/// exactly one bit. This class implements a nonrecursive version of the
/// algorithm from the papers
///
/// > I. Parberry, "A computer assisted optimal depth lower bound for
/// > nine-Input sorting networks", Proceedings of Supercomputing '89, 
/// > pp. 152-161, Reno, Nevada, 1989.
/// >
/// > I. Parberry, A computer assisted optimal depth lower bound for
/// > nine-Input sorting networks". _Mathematical Systems Theory_, Vol. 24,
/// > pp. 101-116, 1991.

class CTernaryGrayCode: public CBinaryGrayCode{
  protected:
    int* m_nDirection = nullptr; ///< Direction of ternary change.

  public:
    ~CTernaryGrayCode(); ///< Destructor.

    void Initialize(const UINT); ///< Get first code word.
    UINT Next(); ///< Get next code word.
}; //CTernaryGrayCode

#endif //__TernaryGrayCode_h__