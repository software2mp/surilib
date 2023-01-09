/*! \file CoregisterTableEditorToolbarPart.cpp */
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
#include "CoregisterTableEditorToolbarPart.h"
// Includes Wx
// Includes App
// Defines

namespace suri

{
const std::string CoregisterTableEditorToolbarPart::VECTOR_COREGISTER_TABLE_TOOLBAR_XRC_NAME =
      wxT("ID_COREGISTER_TABLE_TOOLBAR_PANEL"); /*! Nombre de XRC */
/** Ctor */
CoregisterTableEditorToolbarPart::CoregisterTableEditorToolbarPart() :
      Part(VECTOR_COREGISTER_TABLE_TOOLBAR_XRC_NAME, wxT("Toolbar Corregistro")) {
}

/** Dtor */
CoregisterTableEditorToolbarPart::~CoregisterTableEditorToolbarPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool CoregisterTableEditorToolbarPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool CoregisterTableEditorToolbarPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool CoregisterTableEditorToolbarPart::RollbackChanges() {
   return true;
}

/** Inicializa el Part */
void CoregisterTableEditorToolbarPart::SetInitialValues() {
}

/** Actualiza el estado del Part */
void CoregisterTableEditorToolbarPart::Update() {
}
}
