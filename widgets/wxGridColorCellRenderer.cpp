/*! \file wxGridColorCellRenderer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
