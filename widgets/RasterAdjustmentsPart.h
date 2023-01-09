/*! \file RasterAdjustmentsPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERADJUSTMENTSPART_H_
#define RASTERADJUSTMENTSPART_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"
// Includes Wx

// Includes App

// Defines

namespace suri {
/** suri::RasterAdjustmentsPart: comment */
/*
 *
 */
class RasterAdjustmentsPart : public suri::Part {
public:

   /** Ctor */
   RasterAdjustmentsPart();
   /** Dtor */
   virtual ~RasterAdjustmentsPart();

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
   static const std::string RASTER_ADJUSTMENTS_XRC_NAME; /*! ID del panel */
protected:
};
}

#endif /* RASTERADJUSTMENTSPART_H_ */
