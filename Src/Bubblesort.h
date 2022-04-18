/// \file Bubblesort.h
/// \brief Interface for the bubblesort sorting network.

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

#ifndef __Bubblesort_h__
#define __Bubblesort_h__

#include "SortingNetwork.h"

/// \brief Min-bubblesort.
///
/// Perform bubblesort from min to max. That is, first bubble the smallest
/// value to the first channel, then the second-smallest value to the second
/// channel, etc., making as much use of parallelism as possible.

class CBubbleSortMin: public CSortingNetwork{
  private:
    void CreateComparators(); ///< Create comparators.

  public:
    CBubbleSortMin(const UINT); ///< Constructor.
    const std::wstring GetName() const; ///< Get name.
}; //CBubbleSortMin

/// \brief Max-bubblesort.
///
/// Perform bubblesort from max to min. That is, first bubble the largest
/// value to the last channel, then the second-largest value to the second
/// last channel, etc., making as much use of parallelism as possible.

class CBubbleSortMax: public CSortingNetwork{
  private:
    void CreateComparators(); ///< Create comparators.

  public:
    CBubbleSortMax(const UINT); ///< Constructor.
    const std::wstring GetName() const; ///< Get name.
}; //CBubbleSortMax

/// \brief Bubblesort.
///
/// A highly parallelized version of Bubblesort.

class CBubbleSort: public CSortingNetwork{
  private:
    void CreateComparators(); ///< Create comparators.

  public:
    CBubbleSort(const UINT); ///< Constructor.
    const std::wstring GetName() const; ///< Get name.
}; //CBubbleSort

#endif //__Bubblesort_h__
