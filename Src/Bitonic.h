/// \file Bitonic.h
/// \brief Interface for Batcher's bitonic sorting network.

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

#ifndef __Bitonic_h__
#define __Bitonic_h__

#include "SortingNetwork.h"

/// \brief A min-max or max-min comparator used in bitonic sort.
///
/// This can be either a min-max comparator (when the index of the min channel
/// is less than the index of the max channel) or a max-min comparator (when 
/// the index of the min channel is greater than the index of the max channel).
/// Max-min channels are necessary during the construction of the bitonic
/// sorting network.

class CComparator{
  public:
    UINT m_nMin = 0; ///< Channel index of minimum.
    UINT m_nMax = 0; ///< Channel index of maximum.

    /// \brief Constructor.
    ///
    /// \param nMin Channel index of minimum.
    /// \param nMax Channel index of maximum.

    CComparator(UINT nMin, UINT nMax): m_nMin(nMin), m_nMax(nMax){}; 
}; //CComparator

/// \brief Batcher's bitonic sorting network.
///
/// Batcher's bitonic sorting network has number of inputs a power of 2.
/// It sorts by recursively sorting each half of the channels, one upward
/// (min to max) and one downward (max to min) and then using
/// bitonic merge to merge the results. From
///
/// > K. E. Batcher, "Sorting networks and their applications", In Proc. AFIPS
/// > Spring Joint Computer Conference, Vol. 32, pp. 307-314, 1968.

class CBitonicSort: public CSortingNetwork{
  private:
    void CreateComparators(std::vector<CComparator>*); ///< Create comparators.
    void MakeAllMinMax(std::vector<CComparator>*); ///< Make all comparators min-max.
    void Twist(std::vector<CComparator>*, UINT, UINT, UINT); ///< Twist channels.
    void CreateMatchArray(std::vector<CComparator>*); ///< Create match array.

  public:
    CBitonicSort(const UINT); ///< Constructor.
    const std::wstring GetName() const; ///< Get name.
}; //CBitonicSort

#endif //__Bitonic_h__
