/*! \file wxHotLinkEditor.cpp */
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
#include "wxHotLinkEditor.h"
#include "wxGenericTableBase.h"
#include "wxmacros.h"
#include "suri/Table.h"
#include "suri/VectorStyle.h"
#include "wxGridColorCellEditor.h"
#include "wxGridColAttrProvider.h"
#include "wxGridColorCellRenderer.h"
// Includes Wx
// Defines

namespace suri {

wxHotLinkEditor::wxHotLinkEditor(wxGenericTableBase* pTable) : pTable_(pTable) {
}

wxHotLinkEditor::~wxHotLinkEditor() {
}

/**
 * Lo que se efectua cuando comienza la edicion de la celda
 * @param[in] Row numero de la fila de la celda a editar
 * @param[in] Column numero de la columna de la celda a editar
 * @param[in] pGrid puntero a la grilla en edicion
 */
void wxHotLinkEditor::BeginEdit(int Row, int Column, wxGrid* pGrid) {
   wxGridCellTextEditor::BeginEdit(Row, Column, pGrid);
}
/**
 * Metodo invocado al finalizar la edicion de una celda
 */
bool wxHotLinkEditor::EndEdit(int Row, int Column, wxGrid* pGrid) {

   wxGridCellAttr* pattr = new wxGridCellAttr(wxColour(0,0,255), wxColour(255,255,255),
                                              wxFont(10,wxFONTFAMILY_DEFAULT,
                                                     wxFONTSTYLE_NORMAL ,
                                                     wxFONTWEIGHT_LIGHT),
                                              0, 0);
   if (pattr && pTable_->GetAttrProvider()) {
      pTable_->GetAttrProvider()->SetAttr(pattr, Row, Column);
   }
   wxGridCellTextEditor::EndEdit(Row, Column, pGrid);
   return true;
}

} /** namespace suri */
