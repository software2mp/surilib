/*! \file NavigationPart.cpp */
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
// Includes App
// Defines
#include "NavigationPart.h"
#include "UniversalCoordinateInputPart.h"

namespace suri {
const std::string NavigationPart::NAVIGATION_XRC_NAME = wxT("ID_NAVIGATION_PANEL");

NavigationPart::NavigationPart() :
      PartCollection(NAVIGATION_XRC_NAME) {
}

NavigationPart::~NavigationPart() {
}

void NavigationPart::ShowWidget(bool doShow) {
   pWidget_->Show("ID_NAVIGATION_PANEL", doShow);
}

}
