/*! \file LatitudeLongitudeInputPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Suri
#include "LatitudeLongitudeInputPart.h"

/** namespace suri */
namespace suri {
/** Ctor */
LatitudeLongitudeInputPart::LatitudeLongitudeInputPart() :
      Part(pCoordInput = new LatitudeLongitudeInputWidget()) {
}

/** Dtor */
LatitudeLongitudeInputPart::~LatitudeLongitudeInputPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool LatitudeLongitudeInputPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool LatitudeLongitudeInputPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool LatitudeLongitudeInputPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void LatitudeLongitudeInputPart::SetInitialValues() {
}

/** retorna la coordenada que ingresaron, informa si es valida */
bool LatitudeLongitudeInputPart::SetCoordinate(Coordinates LatLongCoord) {
   if (!pCoordInput->SetCoordinate(LatLongCoord)) {
      return false;
   }
   return true;
}

/** Actualiza el estado de la parte */
void LatitudeLongitudeInputPart::Update() {
}
}
