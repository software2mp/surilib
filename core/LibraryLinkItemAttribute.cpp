/*! \file LibraryLinkItemAttribute.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/LibraryLinkItemAttribute.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** String que representa la clave para el id del item al cual hace referencia**/
const std::string LibraryLinkItemAttribute::ItemIdKey = "id";
/** String que representa la clave para el codigo de biblioteca al que pertence el item **/
const std::string LibraryLinkItemAttribute::CodeKey = "libcode";

/** Constructor**/
LibraryLinkItemAttribute::LibraryLinkItemAttribute(const std::string& ItemId,
                                                   const std::string& LibraryCode) :
      LibraryItemAttribute("liblink"), itemId_(ItemId), libcode_(LibraryCode) {
}

LibraryLinkItemAttribute::~LibraryLinkItemAttribute() {
}

/** Obtiene el id del item al cual hace referencia **/
std::string LibraryLinkItemAttribute::GetItemId() const {
   return itemId_;
}
/** Obtiene el codigo de la biblioteca al cual pertenece el item **/
std::string LibraryLinkItemAttribute::GetLibraryCode() const {
   return libcode_;
}
} /** namespace suri */
