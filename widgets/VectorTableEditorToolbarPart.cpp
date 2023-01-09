/*! \file VectorTableEditorToolbarPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
// Includes Suri
#include "VectorTableEditorToolbarPart.h"
// Includes Wx
// Includes App
// Defines

namespace suri {
const std::string VectorTableEditorToolbarPart::VECTOR_TABLE_TOOLBAR_XRC_NAME =
      wxT("ID_VECTOR_TABLE_TOOLBAR_PANEL"); /*! Nombre de XRC */
VectorTableEditorToolbarPart::VectorTableEditorToolbarPart(bool ShowEditColumnButtons) :
      Part(VECTOR_TABLE_TOOLBAR_XRC_NAME, wxT("Toolbar Edicion Vectorial")) {
   showEditColumnButtons_ = ShowEditColumnButtons;
}

VectorTableEditorToolbarPart::~VectorTableEditorToolbarPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool VectorTableEditorToolbarPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool VectorTableEditorToolbarPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool VectorTableEditorToolbarPart::RollbackChanges() {
   return true;
}

/** Inicializa el Part */
void VectorTableEditorToolbarPart::SetInitialValues() {
}

/** Actualiza el estado del Part */
void VectorTableEditorToolbarPart::Update() {
}

/** Permite a clases que hereden modificar el widget luego de crear la ventana */
bool VectorTableEditorToolbarPart::ConfigureWidget() {
   if (!showEditColumnButtons_) {
      GetWidget()->Hide("ID_ADDCOLUMN_BUTTON");
      GetWidget()->Hide("ID_DELETECOLUMN_BUTTON");
   }
   return true;
}
}
