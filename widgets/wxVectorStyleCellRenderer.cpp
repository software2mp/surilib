/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
