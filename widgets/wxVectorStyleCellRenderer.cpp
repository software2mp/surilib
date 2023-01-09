/*! \file wxVectorStyleCellRenderer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes Estandar
// Includes Suri
#include "suri/VectorStyle.h"
#include "wxmacros.h"
#include "wxVectorStyleCellRenderer.h"
#include "VectorRenderer.h"

// Includes Wx
#include "wx/dc.h"

// Defines
// forwards

namespace suri {

wxVectorStyleCellRenderer::wxVectorStyleCellRenderer() {
}

wxVectorStyleCellRenderer::~wxVectorStyleCellRenderer() {
}

/** Pinta el rectangulo de la celda con el valor asociado a la celda */
void wxVectorStyleCellRenderer::Draw(wxGrid& Grid, wxGridCellAttr& Attribute, wxDC& Dc,
                                     const wxRect& Cell, int Row, int Column,
                                     bool IsSelected) {
   std::string style = Grid.GetCellValue(Row, Column).c_str();
   wxBitmap bmp;
   VectorStyle* pvstyle = VectorStyle::Create(style);
   if (!pvstyle)
      return;
   VectorRenderer::PreviewStyle(pvstyle, bmp, Cell.width, Cell.height);
   wxBrush brush(wxColour(255, 255, 255, 255));
   Dc.SetBrush(brush);
   Dc.DrawRectangle(Cell.x, Cell.y, Cell.width, Cell.height);
   /** Dibuja el bmp del preview del estilo en la celda **/
   Dc.DrawBitmap(bmp, Cell.x, Cell.y, true);
   VectorStyle::Destroy(pvstyle);
}
/** Devuelve el size por defecto */
wxSize wxVectorStyleCellRenderer::GetBestSize(wxGrid& Grid, wxGridCellAttr& Attribute,
                                              wxDC& Dc, int Row, int Column) {
   return wxDefaultSize;
}
/** Clonador */
wxVectorStyleCellRenderer* wxVectorStyleCellRenderer::Clone() const {
   return new wxVectorStyleCellRenderer;
}
} /** namespace suri */
