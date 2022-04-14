/// \file Helpers.cpp
/// \brief Code for useful helper functions.

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

#include "Helpers.h"

/// Parity test.
/// \param n A number.
/// \return true if n is odd, false otherwise.

bool odd(const UINT n){
  return n & 1; //obvious, really
} //odd

/// Power of 2 test.
/// \param n A number.
/// \return true if n is a power of 2, false otherwise.

bool IsPowerOf2(const UINT n){
  return n && !(n & (n - 1)); //tricksy
} //IsPowerOf2

/// Compute the ceiling of log base 2. For speed, this relies on the Windows
/// API function `_BitScanReverse`, which uses the corresponding native
/// machine-code instruction to find the index of the most significant 1 in the
/// binary representation of an unsigned integer. In retrospect this is
/// probably overkill for this application but you have to admit that it's cool.
/// \param n A number.
/// \return Log base 2 of the smallest power of 2 greater than or equal to n.

UINT CeilLog2(const UINT n){ ///< Ceiling of log base 2.
  if(n == 0)return 0; //safety

  DWORD k = 0; //for index of most significant 1
  _BitScanReverse(&k, n); //find index of most significant 1

  return IsPowerOf2(n)? k: k + 1;
} //NextPowerOf2
