/// \file RenderableComparatorNet.h
/// \brief Interface for the renderable comparator network CRenderableComparatorNet.

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

#ifndef __RenderableComparatorNet_h__
#define __RenderableComparatorNet_h__

#include "Defines.h"
#include "ComparatorNetwork.h"

/// \brief Renderable comparator network.
///
/// A comparator network that can be rendered to a `Gdiplus::Bitmap` or
/// exported in one of several graphics file formats. Uses GDI+, obviously.
 
class CRenderableComparatorNet: public CComparatorNetwork{
  protected:
    const Gdiplus::REAL m_fPenWidth = 2.0f; ///< Pen width in pixels.
    const Gdiplus::REAL m_fXDelta   = 24.0f; ///< Gap between channels in pixels.
    const Gdiplus::REAL m_fYDelta   = 16.0f; ///< Vertical comparator gap in pixels.
    const Gdiplus::REAL m_fYDelta2  = 8.0f; ///< Extra vertical gap between layers in pixels.
    const Gdiplus::REAL m_fDiameter = 8.0f; ///< Diameter of circles in pixels.
    
    Gdiplus::Bitmap* m_pBitmap = nullptr; ///< Pointer to a bitmap image.

    eDrawStyle m_eDrawStyle = eDrawStyle::Horizontal; ///< Drawing style.

    Gdiplus::Graphics* m_pGraphics = nullptr; ///< Pointer to graphics object.
    Gdiplus::Pen* m_pPen = nullptr; ///< Pointer to graphics pen.
    Gdiplus::SolidBrush* m_pBrush = nullptr; ///< Pointer to graphics brush.

    FILE* m_pOutput = nullptr; ///< File pointer.
    eExport m_eExportType = eExport::Png; ///< Export type.

    Gdiplus::REAL ComputeBitmapHeight(); ///< Compute bitmap height.

    void DrawChannels(const float fLen); ///< Draw channels.
    void DrawComparator(const UINT, const UINT, const float); ///< Draw a comparator.
    void DrawComparators(); ///< Draw all comparators.

  public:
    ~CRenderableComparatorNet(); ///< Destructor.

    void Draw(const eDrawStyle); ///< Draw to a `Gdiplus::Bitmap`.
    
    HRESULT ExportToPNG(LPWSTR); ///< Export in PNG format.
    HRESULT ExportToTex(LPWSTR); ///< Export in TeX format.
    HRESULT ExportToSVG(LPWSTR); ///< Export in SVG format.

    Gdiplus::Bitmap* GetBitmap(); ///< Get bitmap pointer.
}; //CRenderableComparatorNet

#endif //__RenderableComparatorNet_h__