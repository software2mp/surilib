/*! \file RegistrationCodeInputPart.cpp */
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
#include "suri/Part.h"
#include "RegistrationCodeInputPart.h"
// Includes Wx

// Includes App
#include "suri/messages.h"
// Defines

/** namespace suri */
namespace suri {
const std::string RegistrationCodeInputPart::REGISTER_XRC_NAME =
      wxT("ID_REGISTER_PANEL"); /*! Titulo de ventana de codigo */

RegistrationCodeInputPart::RegistrationCodeInputPart() :
      Part(REGISTER_XRC_NAME, _(caption_REGISTRATION_PART)) {
}

/** Dtor */
RegistrationCodeInputPart::~RegistrationCodeInputPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool RegistrationCodeInputPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool RegistrationCodeInputPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool RegistrationCodeInputPart::RollbackChanges() {
   return true;
}

/** Inicializa el Part */
void RegistrationCodeInputPart::SetInitialValues() {
}

/** Actualiza el estado del Part */
void RegistrationCodeInputPart::Update() {
}
}  // namespace suri
