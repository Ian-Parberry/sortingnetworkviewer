/// \file TernaryGrayCode.cpp
/// \brief Code for the ternary reflected Gray code generator CTernaryGrayCode.

#include "TernaryGrayCode.h"

CTernaryGrayCode::~CTernaryGrayCode(){
  delete [] m_nDirection;
} //destructor

/// Reset Gray code generator to the first word in Gray code order, the
/// all-zero word.
/// \param n Number of bits in the Gray code word.

void CTernaryGrayCode::Initialize(const UINT n){ 
  CBinaryGrayCode::Initialize(n); //reset as for the binary version
  
  delete [] m_nDirection;
  m_nDirection = new int[m_nSize + 3];

  for(UINT i=0; i<m_nSize+3; i++) //reset the direction array too
    m_nDirection[i] = 0;
} //Initialize

/// Get the next binary word in ternary reflected Gray code order, which will
/// differ from the previous one in exactly one bit.
/// \return Index of the bit that has changed, in the range 1..INPUTS. 
/// Out of range means we're finished.

UINT CTernaryGrayCode::Next(){
  UINT i = m_nGrayCodeStack[0]; 
  m_nGrayCodeStack[0] = 1;
  UINT j = 2*i - m_nGrayCodeWord[2*i - m_nDirection[i]];
  m_nGrayCodeWord[j] ^= 1;

  if(m_nGrayCodeWord[2*i] == m_nGrayCodeWord[2*i - 1]){
    m_nDirection[i] ^= 1;
    m_nGrayCodeStack[i-1] = m_nGrayCodeStack[i];
    m_nGrayCodeStack[i] = i + 1;
  } //if

  m_nZeros += 1 - 2*m_nGrayCodeWord[j]; 

  return j;
} //Next