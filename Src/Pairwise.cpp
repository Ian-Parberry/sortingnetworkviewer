/// \file Pairwise.cpp
/// \brief Code for the pairwise sorting network.

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

#include "Pairwise.h"

/// Construct a pairwise sorting network with number of inputs a power of 2.
/// Makes 2 copies of a half-sized pairwise sorting network created
/// recursively and adds the required comparators before and after.
/// \param log2n Log base 2 of the number of inputs.

CPairwiseSort::CPairwiseSort(const UINT log2n){   
  if(log2n < 1)return; //safety

  m_nInputs = 1 << log2n;
  m_nDepth = log2n*(log2n + 1)/2;
  m_nSize = m_nInputs*log2n*(log2n - 1)/4 + m_nInputs - 1;

  CreateMatchArray(m_nInputs, m_nDepth);
  CreateComparators();
  CreateValueArray();
} //constructor

/// Create the comparators for pairwise sort. Assumes that `m_nInputs` has been
/// set to the number of inputs and is a power of 2 and that the matching array
/// `m_nMatch` has been created and initialized. This code was appropriated
/// from the pairwise sorting network Wikipedia page 
/// [https://en.wikipedia.org/wiki/Pairwise_sorting_network](https://en.wikipedia.org/wiki/Pairwise_sorting_network).

void CPairwiseSort::CreateComparators(){
  UINT nCurLevel = 0; //current level
  
  for(UINT i=1; i<m_nInputs; i<<=1){ //each iteration constructs a level
    for(UINT j=0; j<i; j++) //min-most channel used on this level
      for(UINT nMin=j; nMin<m_nInputs; nMin+=i<<1) //min channel
        InsertComparator(nCurLevel, nMin, nMin + i);

    ++nCurLevel; //next level
  } //for

  UINT k = 1; //smallest value of j in second for-loop below

  for(UINT i=m_nInputs>>2; i>0; i>>=1){
    for(UINT j=k; j>0; j>>=1){ //each iteration constructs a level
      const UINT nDelta = i*j; //distance between min and max channels
      UINT nMax = i + nDelta; //max channel
      UINT nCount = 0; //count mod i of how many times we have incremented nMax

      while(nMax < m_nInputs){ //each iteration inserts a group of comparators
        InsertComparator(nCurLevel, nMax - nDelta, nMax);
        ++nMax; ++nCount; //next comparator

        if(nCount >= i){ //skip over channels for next group of comparators
          nCount = 0;
          nMax += i;
        } //if
      } //while

      ++nCurLevel; //next level
    } //for

    k = 2*k + 1;
  } //for
} //CreateComparators

/// Construct a wide string name from the type of sorting network and the
/// number of inputs.
/// \return A wide string name.

const std::wstring CPairwiseSort::GetName() const{
  return std::wstring(L"Pairwise" + std::to_wstring(m_nInputs));
} //GetName
