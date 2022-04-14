/// \file itonic.cpp
/// \brief Code for Batcher's bitonic sorting network.

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

#include "Bitonic.h"

/// Construct a bitonic sorting network with number of inputs a power of 2.
/// This involves dabbling in max-min comparators, which
/// we will have to twist into min-max comparators before we're finished.
/// \param log2n Log base 2 of the number of inputs.

CBitonicSort::CBitonicSort(const UINT log2n){
  if(log2n < 1)return; //safety

  m_nInputs = 1 << log2n;
  m_nDepth = log2n*(log2n + 1)/2;
  m_nSize = m_nDepth*m_nInputs/2;

  std::vector<CComparator>* vecLevel = new std::vector<CComparator>[m_nDepth];

  CreateComparators(vecLevel); //create with min-max and max-min comparators
  MakeAllMinMax(vecLevel); //twist out any max-min comparators
  CreateMatchArray(vecLevel); //convert to matching array

  delete [] vecLevel;

  CreateValueArray();
} //constructor

/// Create an array of `std::vector`s of `CComparator` with one entry for each
/// level. The comparators may be min-max or max-min comparators. Assumes that
/// `m_nInputs` has been set to the number of inputs and is a power of 2 and
/// that the matching array `m_nMatch` has been created and initialized. 
/// This code was appropriated from the bitonic sorter Wikipedia page 
/// [https://en.wikipedia.org/wiki/Bitonic_sorter](https://en.wikipedia.org/wiki/Bitonic_sorter).
/// \param vecLevel [OUT] Array of `std::vectors` of `CComparator`.

void CBitonicSort::CreateComparators(std::vector<CComparator>* vecLevel){
  UINT nCurLevel = 0; //current level

  for(UINT i=2; i<=m_nInputs; i*=2) 
    for(UINT j=i/2; j>0; j/=2){
      for(UINT nMin=0; nMin<m_nInputs; nMin++){ //channel for min
        const UINT nMax = nMin ^ j; //channel for max

        if(nMax > nMin)
          vecLevel[nCurLevel].push_back(
            (nMin & i)? CComparator(nMin, nMax): CComparator(nMax, nMin));
      } //for

      ++nCurLevel; //next level
    } //for
} //CreateComparators

/// Make all comparators in an array of `std::vector`s of `CComparator` into
/// min-max comparators. Assumes that `m_nDepth` has been set and that the
/// array has exactly this number of entries.
/// \param vecLevel [IN, OUT] Array of `std::vectors` of `CComparator`.

void CBitonicSort::MakeAllMinMax(std::vector<CComparator>* vecLevel){
  for(UINT i=0; i<m_nDepth; i++) //for each level
    for(auto& p: vecLevel[i]) //for each comparator at that level
      if(p.m_nMax < p.m_nMin){
        std::swap(p.m_nMin, p.m_nMax);
        Twist(vecLevel, i + 1, p.m_nMin, p.m_nMax);
      } //if
} //MakeAllMinMax

/// Swap two channels below a certain depth in an array of `std::vector`s of
/// `CComparator`. That is, every comparator that is attached to one of
/// the channels gets attached to the other instead. Assumes that `m_nDepth`
/// has been set and that the array has exactly this number of entries.
/// \param vecLevel [IN, OUT] Array of `std::vector`s of `CComparator`.
/// \param nLevel Swap channels at this level and below.
/// \param nMin One channel to swap.
/// \param nMax The other channel to swap.

void CBitonicSort::Twist(std::vector<CComparator>* vecLevel, 
  UINT nLevel, UINT nMin, UINT nMax)
{
  for(UINT i=nLevel; i<m_nDepth; i++) //for each level
    for(auto& p: vecLevel[i]){ //for each comparator at that level
      //replace p.m_nMin if necessary
      if(p.m_nMin == nMin)p.m_nMin = nMax; else
      if(p.m_nMin == nMax)p.m_nMin = nMin;
      
      //replace p.m_nMax if necessary
      if(p.m_nMax == nMin)p.m_nMax = nMax; else
      if(p.m_nMax == nMax)p.m_nMax = nMin;
    } //for
} //Twist

/// Create the matching array and populate it with comparators from an array
/// of `std::vector`s of `CComparator`. Assumes that `m_nInputs` and `m_nDepth`
/// have been set.
/// \param vecLevel Array of `std::vector`s of min-max `CComparator`s.

void CBitonicSort::CreateMatchArray(std::vector<CComparator>* vecLevel){
  CComparatorNetwork::CreateMatchArray(m_nInputs, m_nDepth);
  
  for(UINT i=0; i<m_nDepth; i++) //for each level
    for(auto& p: vecLevel[i]) //for each comparator at that level
      InsertComparator(i, p.m_nMin, p.m_nMax); //insert into array
} //CreateMatchArray

/// Construct a wide string name from the type of sorting network and the
/// number of inputs.
/// \return A wide string name.

const std::wstring CBitonicSort::GetName() const{
  return std::wstring(L"Bitonic" + std::to_wstring(m_nInputs));
} //GetName