/*! \file RasterLink.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERLINK_H_
#define RASTERLINK_H_

// Includes Estandar

// Includes Suri
#include "suri/ViewportLinkImpl.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 *
 */
class RasterLink : public suri::ViewportLinkImpl {
public:
   RasterLink(bool LinkCenter = true);
   virtual ~RasterLink();

   virtual bool IsCentered() const;
   virtual bool Execute();
protected:
private:
   bool linkCenter_;
};

} /** namespace suri */

#endif /* RASTERLINK_H_ */
