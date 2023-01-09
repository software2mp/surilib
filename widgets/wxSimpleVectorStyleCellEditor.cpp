/*! \file wxSimpleVectorStyleCellEditor.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes Estandar
#include <string>

// Includes Suri
#include "wxSimpleVectorStyleCellEditor.h"
#include "SimpleVectorStylePart.h"
#include "suri/VectorStyle.h"
#include "suri/PartContainerWidget.h"

// Includes Wx
// Defines
// forwards

namespace suri {

wxSimpleVectorStyleCellEditor::wxSimpleVectorStyleCellEditor() :
      pStyleEditor_(NULL) {
}

wxSimpleVectorStyleCellEditor::~wxSimpleVectorStyleCellEditor() {
}

/** Metodo que inicia la edicion del atributo de la celda **/
bool wxSimpleVectorStyleCellEditor::BeginEdition() {
   /** Por ahora no se soporte utilizar la biblioteca.
    *  Necsito generar una instancia aca porque partcontainer la destruye **/
   pStyleEditor_ = new ui::SimpleVectorStylePart(&currentValue_);
   pStyleEditor_->SetVectorStyleWkt(currentValue_);
   PartContainerWidget* pcontainer = new PartContainerWidget(
         pStyleEditor_, _(pStyleEditor_->GetWindowTitle()));
   if (pcontainer->ShowModal(true) == wxID_OK) {
      return true;
   }
   return false;
}

/** Obtiene el valor resultante de la edicion de la grilla */
std::string wxSimpleVectorStyleCellEditor::GetValue() {
   return currentValue_;
}

/** Configura el valor a editar **/
void wxSimpleVectorStyleCellEditor::SetValue(const std::string& Value) {
   std::string wkt = Value;
   if (Value.empty()) {
      wkt = "VECTORSTYLE[3,\"Poligono rojo\",PEN[\"Suri-pen\",1,COLOR[255,0,0,255],WIDTH[1]]]";
   }
   currentValue_ = Value;
}
} /** namespace suri */
