/// \file OddEven.h
/// \brief Interface for Batcher's odd-even sorting network.

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

#ifndef __OddEven_h__
#define __OddEven_h__

#include "SortingNetwork.h"

/// \brief Batcher's odd-even sorting network.
///
/// Batcher's odd-even sorting network has number of inputs a power of 2.
/// It sorts by recursively sorting each half of the channels, then applying
/// odd-even merge to merge the results. From
///
/// > K. E. Batcher, "Sorting networks and their applications", In Proc. AFIPS
/// > Spring Joint Computer Conference, Vol. 32, pp. 307–314, 1968.

class COddEvenSort: public CSortingNetwork{
  private:
    UINT m_nLog2n = 0; ///< Log base 2 of the number of inputs.
    void CreateComparators(); ///< Create comparators.

  public:
    COddEvenSort(const UINT); ///< Constructor.
    const std::wstring GetName() const; ///< Get name.
}; //COddEvenSort

#endif //__OddEven_h__
