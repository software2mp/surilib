/*! \file wxHotLinkCellRenderer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "wxHotLinkCellRenderer.h"

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

wxHotLinkCellRenderer::wxHotLinkCellRenderer() {
   // TODO Auto-generated constructor stub

}

wxHotLinkCellRenderer::~wxHotLinkCellRenderer() {
   // TODO Auto-generated destructor stub
}


/**
 * Devuelve el size por defecto
 * @param[in] Grid grilla
 * @param[in] Attribute atributo
 * @param[in] Dc Dc
 * @param[in] Row numero de fila
 * @param[in] Column numero de columna
 * @return wxSize Devuelve el mejor size, que para este caso se considera el size por defecto
 */
wxSize wxHotLinkCellRenderer::GetBestSize(wxGrid& Grid, wxGridCellAttr& Attribute,
                                          wxDC& Dc, int Row, int Column) {
   return wxDefaultSize;
}

wxHotLinkCellRenderer* wxHotLinkCellRenderer::Clone() const {
   return new wxHotLinkCellRenderer();
}

void wxHotLinkCellRenderer::Draw(wxGrid& Grid, wxGridCellAttr& Attribute, wxDC& Dc,
                                 const wxRect& Cell, int Row, int Column, bool IsSelected) {
   wxBrush brush(wxColour(0,0,255));
   Dc.DrawRectangle(Cell.x, Cell.y, Cell.width, Cell.height);

}
} /** namespace suri */
