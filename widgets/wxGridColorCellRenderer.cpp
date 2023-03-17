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
#include <string>

// Includes Suri
#include "wxGridColorCellRenderer.h"
#include "suri/VectorStyle.h"
#include "wxmacros.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
wxGridColorCellRenderer::wxGridColorCellRenderer() {
}

/**
 * Destructor
 */
wxGridColorCellRenderer::~wxGridColorCellRenderer() {
}

/**
 * Pinta el rectangulo de la celda con el valor asociado a la celda
 * En modo Debug, ademas de pintar la celda del color asociado, escribe con letras en hexadecimal
 * dicho valor
 * @param[in] Grid grilla sobre la cual se va a renderizar
 * @param[in] Attribute atributo de la grilla sobre el cual se actua
 * @param[in] Dc Dc
 * @param[in] Cell rectangulo con las dimensiones de la celda
 * @param[in] Row numero de fila
 * @param[in] Column numero de columna
 * @param[in] IsSelected booleano que informa si la celda esta o no seleccionada
 */
void wxGridColorCellRenderer::Draw(wxGrid& Grid, wxGridCellAttr& Attribute, wxDC& Dc,
                                   const wxRect& Cell, int Row, int Column, bool IsSelected) {
   std::string color = WS2S(Grid.GetCellValue(Row, Column));
   VectorStyle::Color vscolor(color);
   wxBrush brush(wxColour(vscolor.red_, vscolor.green_, vscolor.blue_, vscolor.alpha_));
   Dc.SetBrush(brush);
   Dc.DrawRectangle(Cell.x, Cell.y, Cell.width, Cell.height);
#ifdef __DEBUG__
   // Imprime el RGB del color en la celda, para validacion
   Dc.DrawLabel(color, Cell);
#endif
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
wxSize wxGridColorCellRenderer::GetBestSize(wxGrid& Grid, wxGridCellAttr& Attribute, wxDC& Dc,
                                            int Row, int Column) {
   return wxDefaultSize;
}

/**
 * Clonador de la clase wxGridColorCellRenderer
 * @return wxGridColorCellRenderer* Devuelve un puntero a una nueva instancia con una copia
 *                                  profunda del objeto actual
 */
wxGridColorCellRenderer* wxGridColorCellRenderer::Clone() const {
   return new wxGridColorCellRenderer();
}

} /** namespace suri */
