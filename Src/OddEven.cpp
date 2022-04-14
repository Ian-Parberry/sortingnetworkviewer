/// \file OddEven.cpp
/// \brief Code for Batcher's odd-even sorting network.

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

#include "OddEven.h"
#include "Helpers.h"

/// Construct an odd-even sorting network with number of inputs a power of 2.
/// \param log2n Log base 2 of the number of inputs.

COddEvenSort::COddEvenSort(const UINT log2n):
  m_nLog2n(log2n)
{
  if(log2n < 1)return; //safety

  m_nInputs = 1 << log2n; 
  m_nDepth = log2n*(log2n + 1)/2;
  m_nSize = m_nInputs*log2n*(log2n - 1)/4 + m_nInputs - 1;

  CreateMatchArray(m_nInputs, m_nDepth);
  CreateComparators();
  CreateValueArray();
} //constructor

/// Create the comparators for odd-even sort.
/// Assumes that `m_nLog2n` has been set to log base 2 of the number of inputs,
/// `m_nInputs` has been set to the number of inputs and is a power of 2,
///  and that the matching array `m_nMatch` has been created and initialized.
/// This code was appropriated from the odd-even mergesort Wikipedia page 
/// [https://en.wikipedia.org/wiki/Batcher_odd-even_mergesort](https://en.wikipedia.org/wiki/Batcher_odd-even_mergesort).

void COddEvenSort::CreateComparators(){
  UINT nCurLevel = 0; //current level

  for(UINT i=1; i<=m_nLog2n; i++){
    const UINT p = 1<<(i - 1);

    for(UINT j=p; j>0; j/=2){
      for(UINT k=j%p; k<m_nInputs-j; k+=2*j){
        const UINT nUpperMin = min(j + k, m_nInputs - j);

        for(UINT nMin=k; nMin<nUpperMin; nMin++){ //channel for min
          const UINT nMax = nMin + j; //channel for max
          
          if((nMin >> i) == (nMax >> i))
            InsertComparator(nCurLevel, nMin, nMax);
        } //for
      } //for
            
      ++nCurLevel; //next level
    } //for
  } //for
} //CreateComparators

/// Construct a wide string name from the type of sorting network and the
/// number of inputs.
/// \return A wide string name.

const std::wstring COddEvenSort::GetName() const{
  return std::wstring(L"OddEven" + std::to_wstring(m_nInputs));
} //GetName
