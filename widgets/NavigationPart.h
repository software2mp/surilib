/*! \file NavigationPart.h */
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
// Includes Wx
// Defines
#ifndef NAVIGATIONPART_H_
#define NAVIGATIONPART_H_

#include "suri/PartCollection.h"

namespace suri {
/** Herramienta de navegacion */
/**
 *
 */
class NavigationPart : public PartCollection {
   static const std::string NAVIGATION_XRC_NAME;

public:
   NavigationPart();
   virtual ~NavigationPart();

   void ShowWidget(bool doShow);
};
}

#endif /* NAVIGATIONPART_H_ */
