/*! \file GeoReferenceTableEditorToolbarPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
// Includes Suri
#include "GeoReferenceTableEditorToolbarPart.h"

// Includes Wx
// Includes App
// Defines

namespace suri {
const std::string GeoReferenceTableEditorToolbarPart::VECTOR_GEOREFERENCE_TABLE_TOOLBAR_XRC_NAME =
      wxT("ID_GEOREFERENCE_TABLE_TOOLBAR_PANEL"); /*! Nombre de XRC */
/** Ctor */
GeoReferenceTableEditorToolbarPart::GeoReferenceTableEditorToolbarPart() :
      PartCollection(VECTOR_GEOREFERENCE_TABLE_TOOLBAR_XRC_NAME,
                     wxT("Toolbar Georeferenciacion")) {
}

/** Dtor */
GeoReferenceTableEditorToolbarPart::~GeoReferenceTableEditorToolbarPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool GeoReferenceTableEditorToolbarPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool GeoReferenceTableEditorToolbarPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool GeoReferenceTableEditorToolbarPart::RollbackChanges() {
   return true;
}

/** Inicializa el Part */
void GeoReferenceTableEditorToolbarPart::SetInitialValues() {
}

/** Actualiza el estado del Part */
void GeoReferenceTableEditorToolbarPart::Update() {
}
}
