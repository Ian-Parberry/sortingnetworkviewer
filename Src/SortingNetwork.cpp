/// \file SortingNetwork.cpp
/// \brief Code for the sorting network CSortingNetwork.

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

#include "SortingNetwork.h"

/// Delete the value table `m_nValue`, the usage array `m_bUsed`,
/// and the Gray code generator.

CSortingNetwork::~CSortingNetwork(){
  if(m_nValue){ //safety
    for(UINT i=0; i<m_nDepth; i++)
      delete [] m_nValue[i];
    delete [] m_nValue;
  } //if

  if(m_bUsed){ //safety
    for(UINT i=0; i<m_nDepth; i++)
      delete [] m_bUsed[i];
    delete [] m_bUsed;
  } //if

  delete m_pGrayCode;
} //destructor

/// Set the values on every channel between two levels to zero.
/// \param firstlayer First level to set to zero.
/// \param lastlayer Last level to set to zero.

void CSortingNetwork::initValues(const UINT firstlayer, const UINT lastlayer){
  for(UINT i=firstlayer; i<=lastlayer; i++) //for each layer in range
    for(UINT j=0; j<m_nInputs; j++) //for each channel
      m_nValue[i][j] = 0; //set the value on this channel at that layer to zero
} //initValues

/// Set the usage flag on every channel to a fixed value.
/// \param b Value of flag.

 void CSortingNetwork::initUsage(){
   const bool bFirstNormalForm = FirstNormalForm();

  for(UINT j=0; j<m_nInputs; j++) //for each channel in first layer
    m_bUsed[0][j] = bFirstNormalForm; //set the value on this channel at that layer to b

  for(UINT i=1; i<m_nDepth; i++) //for each layer after the first
    for(UINT j=0; j<m_nInputs; j++) //for each channel
      m_bUsed[i][j] = false; //set the value on this channel at that layer to b
 } //initUsage

/// Initialize the network for the sorting test, that is, make the
/// Gray code word for input be all zeros, and the values on every channel at
/// every level be zero.

void CSortingNetwork::initSortingTest(){ 
  delete m_pGrayCode;
  m_pGrayCode = FirstNormalForm()? new CTernaryGrayCode: new CBinaryGrayCode;

  m_pGrayCode->Initialize(m_nInputs); //initialize the Gray code to all zeros.
  initValues(0, m_nDepth - 1); //initialize the network values to all zeros.
  initUsage(); //mark all comparators unused
} //initSortingTest

/// Flip value and propagate down the comparator network.
/// \param j Flip value in this channel.
/// \param firstlayer Flip value at this layer.
/// \param lastlayer Propagate change down to this layer.
/// \return Channel whose value is flipped after the last layer.

UINT CSortingNetwork::flipinput(UINT j, const UINT firstlayer, const UINT lastlayer){
  for(UINT i=firstlayer; i<=lastlayer; i++){ //for each layer in range
    m_nValue[i][j] ^= 1; //flip the value on channel j at that level

    UINT k = m_nMatch[i][j]; //find the channel to which it is joined via a comparator, if any
    
    if(0 <= k && k < m_nInputs)
      if((m_nValue[i][k] && j>k) || !(m_nValue[i][k] || j>k)){
        m_bUsed[i][j] = m_bUsed[i][k] = true; //mark both channels used
        j = k;
      } //if
  } //for

  return j;
} //flipinput

/// Check whether sorting network sorts when the current input has channel flipped.
/// \param delta Index of channel to flip.
/// \return true if it still sorts when channel is flipped.

bool CSortingNetwork::stillsorts(const int delta){
  return flipinput(delta - 1, 0, m_nDepth - 1) == 
    m_pGrayCode->m_nZeros + m_pGrayCode->m_nGrayCodeWord[delta] - 1;
} //stillsorts

/// Check whether sorting network sorts all inputs. Set `m_bSorts` to `true`
/// if it does.
/// \return true if it sorts.

bool CSortingNetwork::sorts(){ 
  UINT i=0; //index of bit to flip
  m_bSorts = true; //assume it sorts until we find otherwise
  initSortingTest(); //intialize input and values in comparator network to zero
  
  while(m_bSorts && i<=m_nInputs){ //bail if it doesn't sort, or we've tried all binary inputs
    i = m_pGrayCode->Next(); //next bit to flip in Gray code order
    m_bSorts = m_bSorts && (i>m_nInputs || stillsorts(i)); //check whether it still sorts when this bit is flipped
  } //while

  return m_bSorts;
} //sorts

/// Get number of unused comparators. Assumes that function `sorts()` has
/// already been run. Returns zero otherwise.
/// \return Number of unused comparators.

const UINT CSortingNetwork::GetUnused() const{
  UINT count = 0;

  if(m_bUsed){
    for(UINT i=0; i<m_nDepth; i++) //for each level
      for(UINT j=0; j<m_nInputs; j++) //for each channel
        if(m_nMatch[i][j] > j && !m_bUsed[i][j])
          count++;
  } //if

  return count;
} //GetUnused

/// Create and initialize value array to all zeros. Assumes that `m_nInputs`
/// and `m_nDepth` have been set to the correct values.

void CSortingNetwork::CreateValueArray(){
  m_nValue = new UINT*[m_nDepth];

  for(UINT i=0; i<m_nDepth; i++){
    m_nValue[i] = new UINT[m_nInputs];

    for(UINT j=0; j<m_nInputs; j++)
      m_nValue[i][j] = 0;
  } //for
} //CreateValueArray

/// Create and initialize usage array to all usde. Assumes that `m_nInputs`
/// and `m_nDepth` have been set to the correct values.

void CSortingNetwork::CreateUsageArray(){
  m_bUsed = new bool*[m_nDepth];

  for(UINT i=0; i<m_nDepth; i++){
    m_bUsed[i] = new bool[m_nInputs];

    for(UINT j=0; j<m_nInputs; j++)
      m_bUsed[i][j] = true;
  } //for
} //CreateUsageArray

/// Read a sorting network and create and initialize the value array `m_nValue`
/// for use in sorting verification. Uses `CComparatorNetwork::Read()` to do
/// the heavy lifting.
/// \param lpwstr Null terminated wide file name.
/// \return true if the input succeeded.

bool CSortingNetwork::Read(LPWSTR lpwstr){
  const bool ok = CComparatorNetwork::Read(lpwstr); //read from file

  if(ok){
    CreateValueArray(); //create and initialize value array to all zeros
    CreateUsageArray(); //create usage array
  } //if

  return ok;
} //Read