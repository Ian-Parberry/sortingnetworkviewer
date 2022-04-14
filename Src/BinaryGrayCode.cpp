/// \file BinaryGrayCode.cpp
/// \brief Code for the binary reflected Gray code generator CBinaryGrayCode.

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

#include "BinaryGrayCode.h"

/// Delete the memory in the Gray code word and stack.

CBinaryGrayCode::~CBinaryGrayCode(){
  delete [] m_nGrayCodeWord;
  delete [] m_nGrayCodeStack;
} //destructor

/// Initialize and set the Gray code generator to the first word in Gray code
/// order, which is the all-zero word.
/// \param n Number of bits in the Gray code word.

void CBinaryGrayCode::Initialize(const UINT n){ 
  if(m_nSize != n){
    delete [] m_nGrayCodeWord;
    delete [] m_nGrayCodeStack;

    const UINT nSize = n + 3U; //we need 3 extra entries

    m_nGrayCodeWord = new UINT[nSize];
    m_nGrayCodeStack = new UINT[nSize];

    m_nSize = n;
  } //if

  m_nZeros = n; //all zeros
  
  for(UINT i=0; i<=n+2; i++){
    m_nGrayCodeWord[i] = 0; //all zeros
    m_nGrayCodeStack[i] = i + 1; //recursion stack initial conditions
  } //for
} //Initialize

/// Get the next binary word in binary reflected Gray code order, which will
/// differ from the previous one in exactly one bit.
/// \return Index of the changed bit in the range 1 through m_nSize. 
/// Out of range means we're finished.

UINT CBinaryGrayCode::Next(){
  const UINT i = m_nGrayCodeStack[0]; //bit to change

  m_nGrayCodeStack[0] = 1; 
  m_nGrayCodeWord[i] ^= 1; //change bit
  m_nGrayCodeStack[i - 1] = m_nGrayCodeStack[i];
  m_nGrayCodeStack[i] = i + 1;
  m_nZeros += 1 - 2*m_nGrayCodeWord[i]; //adjust zero count

  return i; //return bit changed
} //Next
