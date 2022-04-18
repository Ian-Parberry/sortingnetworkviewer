/// \file ComparatorNetwork.h
/// \brief Interface for the comparator network CComparatorNetwork.

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

#ifndef __ComparatorNetwork_h__
#define __ComparatorNetwork_h__

#include "Defines.h"

/// \brief Comparator network.
///
/// `CComparatorNetwork` implements a comparator network, which may or may not
/// sort. Each level of the comparator network is represented by a matching
/// stored in an array `m_nMatch`. There is a comparator between channels `j`
/// and `k` at level `i` iff `m_nMatch[i][j] == k && m_nMatch[i][k] == j`.
/// If there is no comparator on channel `j` at level `i`, then 
/// `m_nMatch[i][j] == j`.  This will allow for fast verification of whether
/// a comparator network is a sorting network.

class CComparatorNetwork{
  protected: 
    UINT** m_nMatch = nullptr; ///< Matchings at each level.
    bool** m_bUsed = nullptr; ///< Whether comparators are used when sorting.

    UINT m_nInputs = 0; ///< Number of inputs.
    UINT m_nDepth = 0; ///< Depth.
    UINT m_nSize = 0; ///< Size.

    void InsertComparator(UINT, UINT, UINT); ///< Insert comparator.
    void CreateMatchArray(UINT, UINT); ///< Create match array.
    void ComputeSize(); ///< Compute size.

  public: 
    ~CComparatorNetwork(); ///< Destructor.

    virtual bool Read(LPWSTR); ///< Read from file.
    void Prune(const UINT); ///< Prune down number of inputs.

    const UINT GetNumInputs() const; ///< Get number of inputs.
    const UINT GetDepth() const; ///< Get depth.
    const UINT GetSize() const; ///< Get size.

    const bool FirstNormalForm() const; ///< Test for first normal form.
}; //CComparatorNetwork

#endif //__ComparatorNetwork_h__