/// \file ComparatorNetwork.cpp
/// \brief Code for the comparator network CComparatorNetwork.

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

#include <fstream>
#include <sstream>
#include <string>

#include "Includes.h"
#include "Helpers.h"
#include "ComparatorNetwork.h"

/// Delete the memory used for the 2D array `m_nMatch`.

CComparatorNetwork::~CComparatorNetwork(){
  if(m_nMatch){ //safety
    for(UINT i=0; i<m_nDepth; i++)
      delete [] m_nMatch[i];
    delete [] m_nMatch;
  } //if
} //destructor

/// Read a comparator network from file. Create and input the matching array
/// `m_nMatch` and set `m_nInputs` to the number of inputs, `m_nDepth` to the
/// depth, and `m_nSize` to the size (number of comparators). The input file
/// must consist of a line of text for each layer of comparators. Each line
/// must consist of an even number of unsigned integer channel numbers in which
/// each consecutive pair \f$i, j\f$ indicates a comparator between channels
/// \f$i\f$ and \f$j\f$.
/// \param lpwstr Null terminated wide file name.
/// \return true if the input succeeded.

bool CComparatorNetwork::Read(LPWSTR lpwstr){
  std::ifstream infile(lpwstr); //input file stream
  bool bSuccess = (bool)infile; //should always be true

  if(bSuccess){
    const UINT nOldDepth = m_nDepth; //old depth 

    UINT nInputs = 0; //number of inputs seen so far
    UINT nDepth = 0; //depth seen so far
    UINT nSize = 0; //number of comparators seen so far

    std::vector<std::vector<UINT>> vMatching; //unprocessed matchings from input
    std::string strLine; //current input line
    std::vector<UINT> vEmptyVec; //the empty vector

    //load vMatching from file

    while(std::getline(infile, strLine)){ //for each line
      std::istringstream iss(strLine); //prepare for processing
      vMatching.push_back(vEmptyVec); //new level, empty so far
      UINT a, b; //pair of channels for a comparator

      while(iss >> a >> b){ //grab each comparator
        nInputs = max(max(nInputs, a), b); //adjust inputs seen
        vMatching[nDepth].push_back(a); //append to current level
        vMatching[nDepth].push_back(b); //append to current level
        nSize++; //one more comparator
      } //while

      nDepth++; //one more level
    } //while

    nInputs++; //number of inputs is one more than the maximum channel

    //process temporary array nMatch into m_nMatch

    if(bSuccess){
      CreateMatchArray(nInputs, nDepth);
      m_nSize = nSize;

      //convert matching from vMatching to match array m_nMatch
      
      for(UINT i=0; i<vMatching.size(); i++)
        for(UINT j=0; j<vMatching[i].size(); j+=2){
          const UINT j0 = vMatching[i][j]; //one end of comparator

          if(j < vMatching[i].size() - 1){
            const UINT j1 = vMatching[i][j + 1]; //the other end of comparator

            if(j0 < m_nInputs && j1 < m_nInputs){
              m_nMatch[i][j0] = j1;
              m_nMatch[i][j1] = j0;
            } //if
          } //if
        } //for
    } //if
  } //if

  return bSuccess;
} //Read

/// Insert a comparator between two channels at a certain level.
/// \param nLevel Level number.
/// \param i Channel index.
/// \param j Channel index.

void CComparatorNetwork::InsertComparator(UINT nLevel, UINT i, UINT j){
  if(nLevel < m_nDepth && i < m_nInputs && j < m_nInputs){
    m_nMatch[nLevel][i] = j;
    m_nMatch[nLevel][j] = i;
  } //if
} //InsertComparator

/// Prune down the number of inputs, deleting any comparators attached to the
/// deleted channels. Does nothing if the desired number of inputs is less 
/// then 2 or greater than or equal to the current number of inputs.
/// \param n Number of inputs desired.

void CComparatorNetwork::Prune(const UINT n){
  if(n < 2 || n >= m_nInputs)return; //safety

  //delete comparators attached to pruned channels

  for(UINT i=0; i<m_nDepth; i++)
    for(UINT j=0; j<n; j++)
      if(m_nMatch[i][j] >= n){
        m_nMatch[i][j] = j;
      } //if

  m_nInputs = n; //reset the mumber of inputs

  //recompute the size (number of comparators)

  m_nSize = 0;
  
  for(UINT i=0; i<m_nDepth; i++)
    for(UINT j=0; j<m_nInputs; j++)
      if(m_nMatch[i][j] > j)
        m_nSize++;
} //Prune

/// Set the number of inputs and the depth, then create a new matching
/// array, taking care to delete any old one that may exist.
/// \param nInputs Number of inputs.
/// \param nDepth Depth.

void CComparatorNetwork::CreateMatchArray(UINT nInputs, UINT nDepth){
  if(m_nMatch){ //there's already one, so delete it
    for(UINT i=0; i<nDepth; i++)
      delete [] m_nMatch[i];
    delete [] m_nMatch;
  } //if

  m_nInputs = nInputs;
  m_nDepth = nDepth;

  m_nMatch = new UINT*[m_nDepth];

  for(UINT i=0; i<m_nDepth; i++){
    m_nMatch[i] = new UINT[m_nInputs];
    for(UINT j=0; j<m_nInputs; j++)
      m_nMatch[i][j] = j; //default is unused
  } //for
} //CreateMatchArray

/// Compute the size, that is, number of comparators and stores it in `m_nSize`.
/// Assumes that the matching array `m_nMatch` has been created and initialized,
/// otherwise it sets `m_nSize` to zero.

void CComparatorNetwork::ComputeSize(){
  m_nSize = 0;

  if(m_nMatch)
    for(UINT i=0; i<m_nDepth; i++){
      for(UINT j=0; j<m_nInputs; j++)
        if(m_nMatch[i][j] > j)
          m_nSize++;
  } //for
} //ComputeSize

/// Reader function for the number of inputs.
/// \return Number of inputs.

const UINT CComparatorNetwork::GetNumInputs() const{
  return m_nInputs;
} //GetNumInputs

/// Reader function for the depth.
/// \return Depth.

const UINT CComparatorNetwork::GetDepth() const{
  return m_nDepth;
} //GetDepth

/// Reader function for the size (number of comparators).
/// \return Size.

const UINT CComparatorNetwork::GetSize() const{
  return m_nSize;
} //GetSize

/// First normal form test. A comparator network is in first normal form if the
/// first layer consists of comparators between channels \f$i\f$ and \f$i + 1\f$
/// for all even \f$0 \leq i < n\f$, where \f$n\f$ is the number of inputs.
/// This definition is from the following paper.
/// 
/// > I. Parberry, "A computer-assisted optimal depth lower bound for nine-input
/// > sorting networks}, _Mathematical Systems Theory_, Vol. 24, No. 1,
/// > pp. 101-116, 1991.
/// 
/// \return True if in first normal form.

const bool CComparatorNetwork::FirstNormalForm() const{
  if(m_nMatch == nullptr)return false; //bail and fail
  if(m_nMatch[0] == nullptr)return false; //bail and fail

  bool ok = true; //true if nothing is inconsistent with first normal form so far
  
  for(UINT j=0; j<m_nInputs-1 && ok; j+=2) //comparators on consecutive channels
    ok = ok && m_nMatch[0][j] == j + 1 && m_nMatch[0][j + 1] == j;

  if(ok && odd(m_nInputs)) //last channel is empty
    ok = ok && m_nMatch[0][m_nInputs - 1] == m_nInputs - 1;

  return ok;
} //FirstNormalForm
