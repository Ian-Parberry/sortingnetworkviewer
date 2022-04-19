/// \file Bubblesort.cpp
/// \brief Code for the bubblesort sorting network.

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

#include "Bubblesort.h"
#include "Helpers.h"

////////////////////////////////////////////////////////////////////////////////
// CBubbleSortMax functions

/// Construct a max-bubblesort sorting network.
/// \param n The number of inputs.

CBubbleSortMax::CBubbleSortMax(const UINT n){   
  if(n < 2)return; //safety

  m_nInputs = n;
  m_nDepth = 2*n - 3;

  CreateMatchArray(m_nInputs, m_nDepth);
  CreateComparators();
  CreateValueArray();
  CreateUsageArray();
} //constructor

/// Create the comparators for max-bubblesort. Assumes that `m_nInputs` has been
/// set to the number of inputs and that the matching array
/// `m_nMatch` has been created and initialized. 

void CBubbleSortMax::CreateComparators(){
  for(UINT i=0; i<m_nInputs; i++) //for each level, first half
    for(UINT j=i&1; j<i+1; j+=2) //min-most channel used on this level
      InsertComparator(i, j, j + 1);
  
  for(UINT i=0; i<m_nInputs-2; i++){ //for each level, second half
    const UINT k = (m_nInputs&1)^(i&1);

    for(UINT j=k; j<m_nInputs-i-2; j+=2){ //min-most channel used on this level   
      InsertComparator(i + m_nInputs, j, j + 1);
    } //for
  } //for

  ComputeSize(); 
} //CreateComparators

/// Construct a wide string name from the type of sorting network and the
/// number of inputs.
/// \return A wide string name.

const std::wstring CBubbleSortMax::GetName() const{
  return std::wstring(L"BubblesortMax" + std::to_wstring(m_nInputs));
} //GetName

////////////////////////////////////////////////////////////////////////////////
// CBubbleSortMin functions

/// Construct a min-bubblesort sorting network.
/// \param n The number of inputs.

CBubbleSortMin::CBubbleSortMin(const UINT n){   
  if(n < 2)return; //safety

  m_nInputs = n;
  m_nDepth = 2*n - 3;

  CreateMatchArray(m_nInputs, m_nDepth);
  CreateComparators();
  CreateValueArray();
  CreateUsageArray();
} //constructor

/// Create the comparators for min-bubblesort. Assumes that `m_nInputs` has been
/// set to the number of inputs and that the matching array
/// `m_nMatch` has been created and initialized. 

void CBubbleSortMin::CreateComparators(){
  for(UINT i=0; i<m_nInputs; i++) //for each level, first half
    for(UINT j=i&1; j<i+1; j+=2){ //min-most channel used on this level
      UINT nMin = m_nInputs - j - 2;
      InsertComparator(i, nMin, nMin + 1);
    } //for
  
  for(UINT i=0; i<m_nInputs-2; i++){ //for each level, second half
    const UINT k = (m_nInputs&1)^(i&1);

    for(UINT j=k; j<m_nInputs-i-2; j+=2){ //min-most channel used on this level   
      UINT nMin = m_nInputs - j - 2;
      InsertComparator(i + m_nInputs, nMin, nMin + 1);
    } //for
  } //for

  ComputeSize(); 
} //CreateComparators

/// Construct a wide string name from the type of sorting network and the
/// number of inputs.
/// \return A wide string name.

const std::wstring CBubbleSortMin::GetName() const{
  return std::wstring(L"BubblesortMin" + std::to_wstring(m_nInputs));
} //GetName

////////////////////////////////////////////////////////////////////////////////
// CBubbleSort functions

/// Construct a bubblesort sorting network.
/// \param n The number of inputs.

CBubbleSort::CBubbleSort(const UINT n){   
  if(n < 2)return; //safety

  m_nInputs = n;
  m_nDepth = (n == 2)? 1: n;

  CreateMatchArray(m_nInputs, m_nDepth);
  CreateComparators();
  CreateValueArray();
  CreateUsageArray();
} //constructor

/// Create the comparators for bubblesort. Assumes that `m_nInputs` has been
/// set to the number of inputs and that the matching array
/// `m_nMatch` has been created and initialized. 

void CBubbleSort::CreateComparators(){
  for(UINT i=0; i<m_nInputs; i++) //for each level, first half
    for(UINT j=i&1; j<m_nInputs - 1; j+=2) //min-most channel used on this level
      InsertComparator(i, j, j + 1);

  ComputeSize(); 
} //CreateComparators

/// Construct a wide string name from the type of sorting network and the
/// number of inputs.
/// \return A wide string name.

const std::wstring CBubbleSort::GetName() const{
  return std::wstring(L"Bubblesort" + std::to_wstring(m_nInputs));
} //GetName
