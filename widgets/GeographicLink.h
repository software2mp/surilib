/*! \file GeographicLink.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOGRAPHICLINK_H_
#define GEOGRAPHICLINK_H_

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
class GeographicLink : public ViewportLinkImpl {
public:
   GeographicLink(bool LinkCenter = true);
   virtual ~GeographicLink();

   virtual bool IsCentered() const;
   virtual bool Execute();
protected:
private:
   bool linkCenter_;
};

} /** namespace suri */

#endif /* GEOGRAPHICLINK_H_ */
