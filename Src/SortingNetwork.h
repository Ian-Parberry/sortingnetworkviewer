/// \file SortingNetwork.h 
/// \brief Interface for the sorting network CSortingNetwork.

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

#ifndef __SortingNetwork_h__
#define __SortingNetwork_h__

#include "WindowsHelpers.h"

#include "TernaryGrayCode.h"
#include "RenderableComparatorNet.h"

/// \brief Sorting network
///
/// `CSortingNetwork` combines a renderable comparator network with a binary
/// Gray code generator to test whether the comparator network sorts based on 
/// the _Zero-One Principle_, which says that a comparator network is a sorting 
/// network if and only if it sorts all inputs made up of only zeros and ones. 
/// See Knuth Volume 3 for the details. Using a Gray code generator instead of 
/// a standard binary string generator speeds up the test.

class CSortingNetwork: public CRenderableComparatorNet{
  protected: 
    CBinaryGrayCode *m_pGrayCode = nullptr; ///< Gray code generator.
    UINT** m_nValue = nullptr; ///< Values at each level when sorting.

    void initSortingTest(); ///< Initialize the sorting test.
    bool stillsorts(const int delta); ///< Does it still sort when a bit is changed?

    UINT flipinput(UINT j, const UINT firstlayer, const UINT lastlayer); ///< Recompute network values when a bit is changed.
    void initValues(const UINT firstlayer, const UINT lastlayer); ///< Initialize the network values to the all zero input.
    void CreateValueArray(); ///< Make value array.

  public:
    ~CSortingNetwork(); ///< Destructor.

    bool Read(LPWSTR); ///< Read from file.
    bool sorts(); ///< Does it sort?
}; //CSortingNetwork

#endif //__SortingNetwork_h__