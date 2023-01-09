/*! \file RasterAdjustmentsPart.cpp */
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
#include "RasterAdjustmentsPart.h"
// Includes Wx
// Includes App
// Defines

namespace suri {
const std::string RasterAdjustmentsPart::RASTER_ADJUSTMENTS_XRC_NAME =
      wxT("ID_ADJUST_TOOL"); /*! Id del panel */
RasterAdjustmentsPart::RasterAdjustmentsPart() :
      Part(RASTER_ADJUSTMENTS_XRC_NAME) {
}

RasterAdjustmentsPart::~RasterAdjustmentsPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool RasterAdjustmentsPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool RasterAdjustmentsPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool RasterAdjustmentsPart::RollbackChanges() {
   return true;
}

/** Inicializa el Part */
void RasterAdjustmentsPart::SetInitialValues() {
}

/** Actualiza el estado del Part */
void RasterAdjustmentsPart::Update() {
}
}
