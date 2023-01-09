/*! \file VectorTableEditorPart.cpp */
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
#include "VectorTableEditorPart.h"

#include "CoregisterTableEditorToolbarPart.h"
#include "suri/messages.h"

// Includes Wx

// Includes App
// Defines

namespace suri {
const std::string VectorTableEditorPart::VECTOR_TABLE_EDITOR_XRC_NAME =
      wxT("ID_VECTOR_TABLE_EDITOR_PANEL"); /*! Nombre de XRC */
/** Ctor */
VectorTableEditorPart::VectorTableEditorPart() :
      PartCollection(VECTOR_TABLE_EDITOR_XRC_NAME, label_DATA_TABLE) {
}

/** Dtor */
VectorTableEditorPart::~VectorTableEditorPart() {
}
}  // Namespace suri
