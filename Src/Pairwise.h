/// \file Pairwise.h
/// \brief Interface for the pairwise sorting network.

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

#ifndef __Pairwise_h__
#define __Pairwise_h__

#include "SortingNetwork.h"

/// \brief The pairwise sorting network.
///
/// The pairwise sorting network sorts by constructing a sequence of sorted
/// pairs, then using a custom sequence of comparators to sort them. From
///
/// > I. Parberry, "The pairwise sorting network", 
/// > _Parallel Processing Letters_, Vol. 2, No. 2,3, pp. 205-211, 1992.

class CPairwiseSort: public CSortingNetwork{
  private:
    void CreateComparators(); ///< Create comparators.

  public:
    CPairwiseSort(const UINT); ///< Constructor.
    const std::wstring GetName() const; ///< Get name.
}; //CPairwiseSort

#endif //__Pairwise_h__
