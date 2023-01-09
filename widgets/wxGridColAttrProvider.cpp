/*! \file wxGridColAttrProvider.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <map>
#include <string>
#include <limits>

// Includes Suri
#include "wxGridColAttrProvider.h"
#include "wxGridColorCellEditor.h"
#include "wxGridColorCellRenderer.h"
#include "wxHotLinkEditor.h"
#include "wxHotLinkCellRenderer.h"
#include "wxVectorStyleCellRenderer.h"
#include "wxSimpleVectorStyleCellEditor.h"
#include "suri/SuriObject.h"
#include "wxCellEditorWidgetInterface.h"
#include "wxWidgetCellEditor.h"
#include "wxQueryBuilderCellEditor.h"
#include "TablePartConfigurator.h"

// Includes Wx
#include "wx/grid.h"

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
wxGridColAttrProvider::wxGridColAttrProvider(Table* pTable,
                                             wxGenericTableBase* pGenericTable,
                                             const Option& Options) :
      pTable_(pTable), pGenericTable_(pGenericTable) {
   for (int col = 0, lencol = pGenericTable_->GetNumberCols(); col < lencol; ++col) {
      int fixcol = pGenericTable->GetFixedColumnIndex(col);
      if (fixcol == std::numeric_limits<int>::max())
         continue;
      wxGridCellAttr* pcellattribute = new wxGridCellAttr();
      pcellattribute->SetKind(wxGridCellAttr::Col);
      Table::ColumnType type = pTable_->GetColumnType(fixcol);
      if (pGenericTable->IsReadOnlyColumn(pTable_->GetColumnName(fixcol))) {
         pcellattribute->SetReadOnly(true);
      }
      switch (type) {
         case Table::INT:
            pcellattribute->SetEditor(new wxGridCellNumberEditor());
            break;
         case Table::COLOR_RGBA_HEXA:
            pcellattribute->SetEditor(new wxGridColorCellEditor(pGenericTable_));
            pcellattribute->SetRenderer(new wxGridColorCellRenderer());
            break;
         case Table::STRING:
            if (pTable->HasRestrictions(fixcol)) {
               wxArrayString options;
               std::map < std::string, std::string > restr = pTable->GetRestrictions(fixcol);
               std::map<std::string, std::string>::iterator it;
               for (it = restr.begin(); it != restr.end(); it++) {
                  options.Add(_(it->second.c_str()));
               }
               pcellattribute->SetEditor(new wxGridCellChoiceEditor(options));
            }
            break;
         case Table::HOTLINK: {
            pcellattribute->SetEditor(new wxHotLinkEditor(pGenericTable_));
            pcellattribute->SetTextColour(wxColour(0, 0, 255));
            // pcellattribute->SetRenderer(new wxHotLinkCellRenderer());
            break;
         }
         case Table::VSTYLE: {
            /** Por ahora no se soporte utilizar la biblioteca **/
            wxSimpleVectorStyleCellEditor* pstyleeditor =
                  new wxSimpleVectorStyleCellEditor();
            pcellattribute->SetEditor(
                  new wxWidgetCellEditor(pGenericTable_, pstyleeditor));
            pcellattribute->SetRenderer(new wxVectorStyleCellRenderer);
            break;
         }
         case Table::QBUILDER: {
            std::string url = Options.GetOption(TablePartConfigurator::VectorDatasourceUrl);
            wxQueryBuilderCellEditor* pqedtir = new wxQueryBuilderCellEditor(url);
            pcellattribute->SetEditor(new wxWidgetCellEditor(pGenericTable_, pqedtir));
            break;
         }
         case Table::FLOAT: {
            /** TCK #10759: se setea en 8 la cantidad
             * de decimales para las columnas de tipo float */
            pcellattribute->SetEditor(new wxGridCellFloatEditor(1, 8) );
            pcellattribute->SetRenderer(new wxGridCellFloatRenderer(1, 8) );
            break;
         }
         case Table::BOOL:
         case Table::DATE:
         case Table::FILE:
         case Table::URL:
         case Table::FORMATTED_STRING:
         default:
            break;
      }
      SetColAttr(pcellattribute, col);
   }
}

/**
 * Destructor
 */
wxGridColAttrProvider::~wxGridColAttrProvider() {
   // No es necesario hacer nada en el destructor
}

/**
 * Setting attributes
 */
void wxGridColAttrProvider::SetAttr(wxGridCellAttr* pAttr, int Row, int Column) {
   // No se hace nada para evitar que la logica de filas o celdas moleste a la de columnas
   // Se llama al metodo del padere, con el propocito de que se mantenga los colores de
   // texto para las celdas de hipervinculo.
   // todo : revisar si hay una alternativa
   wxGridCellAttrProvider::SetAttr(pAttr, Row, Column);
   return;
}

/**
 * Set attribute for the specified row
 */
void wxGridColAttrProvider::SetRowAttr(wxGridCellAttr* pAttr, int Row) {
   // No se hace nada para evitar que la logica de filas o celdas moleste a la de columnas
   return;
}

} /** namespace suri */
