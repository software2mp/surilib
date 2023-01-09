/*! \file DecimalCoordinateInputPart.cpp */
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
#include "DecimalCoordinateInputPart.h"

// Includes Wx
// Defines

namespace suri {
const std::string DecimalCoordinateInputPart::DECIMAL_COORD_INPUT_XRC_NAME =
      wxT("ID_DECIMAL_COORD_PANEL");

DecimalCoordinateInputPart::DecimalCoordinateInputPart() :
      Part(DECIMAL_COORD_INPUT_XRC_NAME) {
}

DecimalCoordinateInputPart::~DecimalCoordinateInputPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool DecimalCoordinateInputPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool DecimalCoordinateInputPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool DecimalCoordinateInputPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void DecimalCoordinateInputPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void DecimalCoordinateInputPart::Update() {
}
}
