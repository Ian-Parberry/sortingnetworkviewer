/// \file RenderableComparatorNet.cpp
/// \brief Code for the renderable comparator network CRenderableComparatorNet.

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


#include "Includes.h"
#include "RenderableComparatorNet.h"
#include "WindowsHelpers.h"

///< Delete the bitmap.

CRenderableComparatorNet::~CRenderableComparatorNet(){
  delete m_pBitmap;
} //destructor

/// Compute the bitmap height when drawn in vertical draw mode. Note that this
/// does not actually draw the comparator network to a bitmap, but it goes
/// through the motions and tallies up the height that would be used.
/// \return Bitmap height in pixels.

Gdiplus::REAL CRenderableComparatorNet::ComputeBitmapHeight(){
  bool* bUsed = new bool[m_nInputs]; //whether channels are used at the current level
  Gdiplus::REAL fHeight = m_fYDelta + m_fYDelta2; //height of comparator network so far

  for(UINT i=0; i<m_nDepth; i++){
    for(UINT j=0; j<m_nInputs; j++) //mark all channels unused
      bUsed[j] = false;

    //make a pass through the current level printing non-overlapping comparators
    
    for(UINT pass=0; pass<m_nInputs/2; pass++){
      bool bPassUsed = false; //whether unprinted comparator found in this level
      
      for(UINT j=0; j<m_nInputs; j++){ //for rach channel
        const UINT dest = m_nMatch[i][j]; //other end of comparator

        if(dest < m_nInputs && dest > j && !bUsed[j]){ //can print without overlap
          bPassUsed = bUsed[j] = bUsed[dest] = true; //mark that we've printed it
          j = dest; //so that later comparators can't overlap
        } //if
      } //for

      if(bPassUsed)fHeight += m_fYDelta; //current pass found a comparator
    } //for

    fHeight += m_fYDelta2; //next level
  } //for

  delete [] bUsed;
  return fHeight;
} //ComputeBitmapHeight

/// Draw a comparator. The behaviour of this function depends on the value of
/// `m_eExportType`. If it is `m_eExportType::Png`, then we draw the comparator
/// to the bitmap pointed to by `m_pBitmap` via the graphics object
/// pointed to by `m_pGraphics`. Otherwise we output the vector graphics
/// commands to draw the comparator (a line and two filled circles) to the file
/// pointed to by `m_pOutput`.
/// \param src Source (min) channel.
/// \param dest Destination (max) channel.
/// \param fDist Distance along channel to comparator in pixels.
/// \param bRed True if comparator is to be drawn red (PNG only).

void CRenderableComparatorNet::DrawComparator(
  const UINT src, const UINT dest, const float fDist, bool bRed)
{
  float fSrcy = 0, fDesty = 0, fSrcx = 0, fDestx = 0; //end points for PNG, SVG 
  int vx = 0, vy = 0; //axis for TeX
  const float r = m_fDiameter/2.0f; //circle radius for connectors

  switch(m_eDrawStyle){
    case eDrawStyle::Vertical:
      fSrcx  = m_fDiameter/2 + src*m_fXDelta;
      fDestx = m_fDiameter/2 + dest*m_fXDelta;
      fSrcy = fDesty = fDist;
      vx = 1; vy = 0;
      break;

    case eDrawStyle::Horizontal:
      fSrcx = fDestx = fDist;
      fSrcy  = m_fDiameter/2 +  src*m_fXDelta;
      fDesty = m_fDiameter/2 + dest*m_fXDelta;
      vx = 0; vy = -1;
      break;
  } //switch
  
  const int nSrcx = (UINT)std::round(fSrcx);
  const int nSrcy = (UINT)std::round(fSrcy);

  const int nDestx = (UINT)std::round(fDestx);
  const int nDesty = (UINT)std::round(fDesty);

  switch(m_eExportType){
    case eExport::Png: 
      if(bRed){
        if(m_pGraphics && m_pRedPen && m_pRedBrush){
          const float d = m_fDiameter; //shorthand for diameter

          m_pGraphics->FillEllipse(m_pRedBrush,  fSrcx - r,  fSrcy - r, d, d);
          m_pGraphics->FillEllipse(m_pRedBrush, fDestx - r, fDesty - r, d, d);
          m_pGraphics->DrawLine(m_pRedPen, fSrcx, fSrcy, fDestx, fDesty);
        } //if
      } //if

      else{
        if(m_pGraphics && m_pPen && m_pBrush){
          const float d = m_fDiameter; //shorthand for diameter

          m_pGraphics->FillEllipse(m_pBrush,  fSrcx - r,  fSrcy - r, d, d);
          m_pGraphics->FillEllipse(m_pBrush, fDestx - r, fDesty - r, d, d);
          m_pGraphics->DrawLine(m_pPen, fSrcx, fSrcy, fDestx, fDesty);
        } //if
      } //else
    break;

    case eExport::Svg:
      if(m_pOutput){
        fprintf_s(m_pOutput, "<circle cx=\"%d\" cy=\"%d\"/>", nSrcx,  nSrcy);
        fprintf_s(m_pOutput, "<circle cx=\"%d\" cy=\"%d\"/>", nDestx, nDesty); 
        fprintf_s(m_pOutput, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\"/>\n",
          nSrcx, nSrcy, nDestx, nDesty);
      } //if
    break;

    case eExport::TeX: 
      if(m_pOutput){
        const int d = (UINT)std::round(m_fDiameter);
        const UINT nLen = abs(nDestx - nSrcx + nDesty - nSrcy);

        fprintf_s(m_pOutput, "\\put(%d,-%d){\\circle*{%d}}\n", nSrcx, nSrcy, d);
        fprintf_s(m_pOutput, "\\put(%d,-%d){\\circle*{%d}}\n", nDestx, nDesty, d); 
        fprintf_s(m_pOutput, "\\put(%d,-%d){\\line(%d,%d){%u}}\n", nDestx, nDesty,
          vx, vy, nLen); 
      } //if
    break;
  } //switch
} //DrawComparator

/// Draw all comparators. Calls `DrawComparator()` once for each comparator.
/// The behaviour of this function depends on the value of
/// `m_eExportType`. If it is `m_eExportType::Png`, then we draw the comparators
/// to the bitmap pointed to by `m_pBitmap` via the graphics object
/// pointed to by `m_pGraphics`. Otherwise we output the vector graphics
/// commands to draw the comparators (lines and filled circles) to the file
/// pointed to by `m_pOutput`.

void CRenderableComparatorNet::DrawComparators(){
  bool* bUsed = new bool[m_nInputs]; //whether channels are used at the current level
  float fLen = m_fYDelta + m_fYDelta2; //distance along channel

  for(UINT i=0; i<m_nDepth; i++){ //for each level
    for(UINT j=0; j<m_nInputs; j++) //mark all channels unused
      bUsed[j] = false;

    //make a pass through the current level printing comparators that don't overlap
    
    for(UINT pass=0; pass<m_nInputs/2; pass++){
      bool bPassUsed = false; //whether we found an unprinted comparator in this level
      
      switch(m_eDrawStyle){
        case eDrawStyle::Vertical:
          for(UINT j=0; j<m_nInputs; j++){
            const UINT dest = m_nMatch[i][j];

            if(dest < m_nInputs && dest > j && !bUsed[j]){ //can print without overlap
              const bool bRed = !m_bUsed[i][j]; //whether to be drawn red
              DrawComparator(dest, j, fLen, bRed); //draw comparator
              bPassUsed = bUsed[j] = bUsed[dest] = true; //mark that we've printed it
              j = dest; //so that later comparators can't overlap
            } //if
          } //for
          break;

        case eDrawStyle::Horizontal:
          for(int j=m_nInputs-1; j>=0; j--){
            const UINT dest = m_nMatch[i][j]; 

            if(dest < m_nInputs && dest < (UINT)j && !bUsed[dest]){ //can print without overlap
              const bool bRed = !m_bUsed[i][j]; //whether to be drawn red
              DrawComparator(j, dest, fLen, bRed); //draw comparator
              bPassUsed = bUsed[j] = bUsed[dest] = true; //mark that we've printed it
              j = dest; //so that later comparators can't overlap
            } //if
          } //for
          break;
      } //switch

      if(bPassUsed)fLen += m_fYDelta; //the current pass found a comparator
    } //for

    fLen += m_fYDelta2; //next level
  } //for

  delete [] bUsed;
} //DrawComparators

/// Draw channels. The behaviour of this function depends on the value of
/// `m_eExportType`. If it is `m_eExportType::Png`, then we draw the channels
/// to the bitmap pointed to by `m_pBitmap` via the graphics object
/// pointed to by `m_pGraphics`. Otherwise we output the vector graphics
/// commands to draw the channels (a line for each channel) to the file
/// pointed to by `m_pOutput`.
/// \param fLen Length of channels in pixels.

void CRenderableComparatorNet::DrawChannels(const float fLen){
  float fSrcy = 0, fDesty = 0, fSrcx = 0, fDestx = 0; //end points for PNG, SVG
  int vx = 0, vy = 0; //axis for TeX

  switch(m_eDrawStyle){
    case eDrawStyle::Vertical:
      fSrcy = 0.0f; fDesty = fLen;
      fSrcx = m_fDiameter/2; fDestx = fSrcx;
      vx = 0; vy = -1;
      break;

    case eDrawStyle::Horizontal:
      fSrcx = 0.0f; fDestx = fLen;
      fSrcy = m_fDiameter/2; fDesty = fSrcy;
      vx = 1; vy = 0;
      break;
  } //switch

  const int nLen = (UINT)std::round(fLen);

  for(UINT i=0; i<m_nInputs; i++){ 
    const int nSrcx  = (UINT)std::round(fSrcx);
    const int nSrcy  = (UINT)std::round(fSrcy);

    const int nDestx = (UINT)std::round(fDestx);
    const int nDesty = (UINT)std::round(fDesty);

    switch(m_eExportType){
      case eExport::Png:
        if(m_pGraphics) //safety
          m_pGraphics->DrawLine(m_pPen, fSrcx, fSrcy, fDestx, fDesty);
        break;

      case eExport::Svg:
        if(m_pOutput) //safety
          fprintf_s(m_pOutput, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\"/>\n",
            nSrcx, nSrcy, nDestx, nDesty);
        break;

      case eExport::TeX: 
        if(m_pOutput) //safety
          fprintf_s(m_pOutput, "\\put(%d,-%d){\\line(%d,%d){%d}}\n",
            nSrcx, nSrcy, vx, vy, nLen); 
      break;
    } //switch

    switch(m_eDrawStyle){
      case eDrawStyle::Vertical:   fSrcx += m_fXDelta; fDestx = fSrcx; break;
      case eDrawStyle::Horizontal: fSrcy += m_fXDelta; fDesty = fSrcy; break;
    } //switch
  } //for
} //DrawChannels

/// Draw the comparator network in black with a transparent background to a new
/// `Gdiplus::Bitmap` of the right size. The comparator network is drawn either
/// vertically or horzontally depending on the draw mode `m_eDrawStyle`. The
/// calling function is responsible for deleting the bitmap when it is no
/// longer needed. Note that `m_eExportType` is set to `eExport::Png` so that
/// the calls to `DrawComparators()` and DrawChannels()` draw to the bitmap
/// pointed to by `m_pBitmap`.

/// \param d Draw style.

void CRenderableComparatorNet::Draw(const eDrawStyle d){
  m_eExportType = eExport::Png;
  m_eDrawStyle = d;

  const UINT w = (UINT)std::ceil((m_nInputs - 1)*m_fXDelta + m_fPenWidth + m_fDiameter);
  const UINT h = (UINT)std::ceil(ComputeBitmapHeight());

  switch(m_eDrawStyle){   
    case eDrawStyle::Vertical:   m_pBitmap = new Gdiplus::Bitmap(w, h); break;
    case eDrawStyle::Horizontal: m_pBitmap = new Gdiplus::Bitmap(h, w); break;
  } //switch

  m_pGraphics = new Gdiplus::Graphics(m_pBitmap);
  m_pGraphics->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
  m_pGraphics->Clear(Gdiplus::Color::Transparent);
  m_pPen = new Gdiplus::Pen(Gdiplus::Color::Black);
  m_pRedPen = new Gdiplus::Pen(Gdiplus::Color::Red);
  m_pPen->SetWidth(m_fPenWidth); 
  m_pRedPen->SetWidth(m_fPenWidth); 
  m_pBrush = new Gdiplus::SolidBrush(Gdiplus::Color::Black);
  m_pRedBrush = new Gdiplus::SolidBrush(Gdiplus::Color::Red);

  DrawChannels((float)h);
  DrawComparators();

  delete m_pGraphics;  m_pGraphics = nullptr; 
  delete m_pPen; m_pPen = nullptr;
  delete m_pRedPen; m_pRedPen = nullptr;
  delete m_pBrush; m_pBrush = nullptr;
  delete m_pRedBrush; m_pRedBrush = nullptr;
} //Draw

/// Export to an PNG file. This simply involves using GDI+ to save the
/// bitmap pointed to by `m_pBitmap` to a file.
/// \param lpwstr Null terminated wide file name.
/// \return S_OK if export succeeded, otherwise E_FAIL.

HRESULT CRenderableComparatorNet::ExportToPNG(LPWSTR lpwstr){
  CLSID clsid; //for PNG class id
  HRESULT hr = GetEncoderClsid((WCHAR*)L"image/png", &clsid);

  if(SUCCEEDED(hr)){ 
    if(m_pBitmap)
      m_pBitmap->Save(lpwstr, &clsid, nullptr);
    else hr = E_FAIL;
  } //if

  return hr;
} //ExportToPNG

/// Export to a TeX file. Note that `m_eExportType` is set to `eExport::TeX`
/// so that the calls to `DrawComparators()` and DrawChannels()` output
/// the necessary vector graphics commands in TeX format to the file pointed
/// to by `m_pOutput`.
/// \param lpwstr Null terminated wide file name.
/// \return S_OK if export succeeded, otherwise E_FAIL.

HRESULT CRenderableComparatorNet::ExportToTex(LPWSTR lpwstr){
  _wfopen_s(&m_pOutput, lpwstr,  L"wt");

  if(m_pOutput){
    m_eExportType = eExport::TeX;
    
    const UINT w = (UINT)std::ceil((m_nInputs - 1)*m_fXDelta + m_fPenWidth + m_fDiameter);
    const UINT h = (UINT)std::ceil(ComputeBitmapHeight());
    
    fprintf_s(m_pOutput, "\\setlength{\\unitlength}{0.5pt}\n");

    switch(m_eDrawStyle){   
      case eDrawStyle::Vertical:
        fprintf_s(m_pOutput, "\\begin{picture}(%u,%u)(0,-%u)\n", w, h, h);
        break;

      case eDrawStyle::Horizontal:
        fprintf_s(m_pOutput, "\\begin{picture}(%u,%u)(0,-%u)\n", h, w, w);
        break;
    } //switch

    fprintf_s(m_pOutput, "\\thicklines\n");
    
    //start drawing

    DrawChannels((float)h);
    DrawComparators();

    //end drawing

    fprintf_s(m_pOutput, "\\end{picture}\n"); //close the picture environment

    //clean up and exit

    fclose(m_pOutput);
    m_pOutput = nullptr; //safety
    return S_OK;
  } //if

  return E_FAIL;
} //ExportToTex

/// Export to an SVG file. Note that `m_eExportType` is set to `eExport::Svg`
/// so that the calls to `DrawComparators()` and DrawChannels()` output
/// the necessary vector graphics commands in SVG format to the file pointed
/// to by `m_pOutput`.
/// \param lpwstr Null terminated wide file name.
/// \return S_OK if export succeeded, otherwise E_FAIL.

HRESULT CRenderableComparatorNet::ExportToSVG(LPWSTR lpwstr){
  _wfopen_s(&m_pOutput, lpwstr,  L"wt");

  if(m_pOutput){
    m_eExportType = eExport::Svg;

    fprintf_s(m_pOutput, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"); //header

    const UINT w = (UINT)std::ceil((m_nInputs - 1)*m_fXDelta +
      m_fPenWidth + m_fDiameter);
    const UINT h = (UINT)std::ceil(ComputeBitmapHeight());

    //svg tag

    switch(m_eDrawStyle){   
      case eDrawStyle::Vertical:
        fprintf_s(m_pOutput, "<svg width=\"%u\" height=\"%u\" ", w + 8, h + 8);
        fprintf_s(m_pOutput, "viewBox=\"-4 -4 %u %u\" ", w + 8, h + 8);
        break;

      case eDrawStyle::Horizontal:
        fprintf_s(m_pOutput, "<svg width=\"%u\" height=\"%u\" ", h + 8, w + 8);
        fprintf_s(m_pOutput, "viewBox=\"-4 -4 %u %u\" ", h + 8, w + 8);
        break;
    } //switch

    fprintf_s(m_pOutput, "xmlns=\"http://www.w3.org/2000/svg\">\n");

    //style tag

    fprintf_s(m_pOutput, "<style>\n");
    fprintf_s(m_pOutput, "circle{fill:black;r:%0.1f}", m_fDiameter/2.0f);
    fprintf_s(m_pOutput, "line{stroke:black;stroke-width:%0.1f}\n", m_fPenWidth);
    fprintf_s(m_pOutput, "</style>\n");
    
    //start drawing

    DrawChannels((float)h);
    DrawComparators();

    //end drawing

    fprintf_s(m_pOutput, "</svg>\n"); //close the svg tag

    //clean up and exit

    fclose(m_pOutput);
    m_pOutput = nullptr; //safety
    return S_OK;
  } //if

  return E_FAIL;
} //ExportToSVG

/// Reader function for bitmap pointer.
/// \return Pointer to bitmap.

Gdiplus::Bitmap* CRenderableComparatorNet::GetBitmap(){
  return m_pBitmap;
} //GetBitmap