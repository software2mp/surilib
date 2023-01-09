/*! \file wxQueryBuilderCellEditor.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "wxQueryBuilderCellEditor.h"

// Includes Estandar
// Includes Suri
#include "QueryBuilderPart.h"
#include "suri/Table.h"
#include "DefaultTable.h"
#include "suri/DatasourceInterface.h"
#include "suri/VectorEditorDriver.h"
#include "VectorDatasource.h"

// Includes Wx
// Defines
// forwards

namespace suri {

wxQueryBuilderCellEditor::wxQueryBuilderCellEditor(const std::string& VectorUrl) :
      pTable_(NULL), pQueryEditor_(NULL) {
   pDatasource_ = DatasourceInterface::Create(
         "VectorDatasource", VectorUrl);
   if (pDatasource_) {
      VectorEditorDriver* pdriver = new VectorEditorDriver(pDatasource_);
      pTable_ = new DefaultTable();
      pTable_->SetDriver(pdriver);
   }
}

wxQueryBuilderCellEditor::~wxQueryBuilderCellEditor() {
   delete pTable_;
   delete pDatasource_;
}

/** Metodo virtual que inicia la edicion del atributo de la celda **/
bool wxQueryBuilderCellEditor::BeginEdition() {
   /** Por ahora no se soporte utilizar la biblioteca.
    *  Necsito generar una instancia aca porque partcontainer la destruye **/

   pQueryEditor_ = new QueryBuilderPart(pTable_, &currentValue_);
   if (pQueryEditor_->CreateTool() && pQueryEditor_->ShowModal(true) == wxID_OK) {
      return true;
   }
    return false;
}
/** Obtiene el valor resultante de la edicion de la grilla */
std::string wxQueryBuilderCellEditor::GetValue() {
   return currentValue_;
}
/** Configura el valor a editar **/
void wxQueryBuilderCellEditor::SetValue(const std::string& Value) {
   currentValue_ = Value;
}
} /** namespace suri */
