/*! \file BandEnhancementPart.cpp */
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
#include "BandEnhancementPart.h"
// Includes Wx,
// Includes App
// Defines

namespace suri {
const std::string BandEnhancementPart::BAND_ENHANCEMENT_XRC_NAME =
      wxT("ID_BAND_ENHANCEMENT_PANEL"); /*! Id del panel */
/** Ctor */
BandEnhancementPart::BandEnhancementPart(RasterElement *pElement) :
      PartCollection(BandEnhancementPart::BAND_ENHANCEMENT_XRC_NAME,
                     wxT(caption_BAND_ENHANCEMENT_PART)) {
   Part *penhancement = new EnhancementSelectionPart(pElement);

   Part *rasteradjustment = new RasterAdjustmentsPart();

   AddPart(penhancement, wxT("ID_BAND_ENHANCEMENT_TABLE_PANEL"));
   AddPart(rasteradjustment, wxT("ID_BAND_ENHANCEMENT_ADJUSTMENTS_PANEL"));
}

/** Dtor */
BandEnhancementPart::~BandEnhancementPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool BandEnhancementPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool BandEnhancementPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool BandEnhancementPart::RollbackChanges() {
   return true;
}

/** Inicializa el Part */
void BandEnhancementPart::SetInitialValues() {
}

/** Actualiza el estado del Part */
void BandEnhancementPart::Update() {
}
}  // namespace suri
