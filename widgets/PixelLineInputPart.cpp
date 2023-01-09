/*! \file PixelLineInputPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "PixelLineInputPart.h"
#include "suri/PixelLineInputWidget.h"

#include "suri/World.h"

namespace suri {

PixelLineInputPart::PixelLineInputPart() :
      Part(new PixelLineInputWidget()) {
}

PixelLineInputPart::~PixelLineInputPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool PixelLineInputPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool PixelLineInputPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool PixelLineInputPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void PixelLineInputPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void PixelLineInputPart::Update() {
}

}
