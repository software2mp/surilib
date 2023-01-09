/*! \file BandEnhancementPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef BANDENHANCEMENTPART_H_
#define BANDENHANCEMENTPART_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"
#include "suri/PartCollection.h"
#include "ColorTableSelectionPart.h"
#include "EnhancementSelectionPart.h"
#include "RasterAdjustmentsPart.h"

// Includes Wx

// Includes App

// Defines

namespace suri {
/** Pantalla de realce. */
/* Pantalla de realce.
 *
 */
class BandEnhancementPart : public suri::PartCollection {
public:
   /** Ctor. */
   BandEnhancementPart(RasterElement *pElement);
   /** Dtor. */
   virtual ~BandEnhancementPart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado del Part */
   virtual void Update();

private:
   static const std::string BAND_ENHANCEMENT_XRC_NAME; /*! Nombre de xrc de Part */
protected:
};
}  // namespace suri

#endif /* BANDENHANCEMENTPART_H_ */
