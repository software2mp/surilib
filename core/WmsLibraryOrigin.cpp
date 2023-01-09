/*! \file WmsLibraryOrigin.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "WmsLibraryOrigin.h"
#include "suri/Configuration.h"

// Includes Wx

// Defines
#define WMS_LIBRARY_NAME "Servidores WMS"
// forwards

namespace suri {

std::string WmsLibraryOrigin::libraryFileName_ = "wmslibrary.lbr";

/**
 * Constructor
 * @param LibraryId id de la biblioteca a la que pertenece el origen
 */
WmsLibraryOrigin::WmsLibraryOrigin(
      const SuriObject::UuidType& LibraryId) :
                        TextFileLibraryItemOrigin(LibraryId, capabilites_,
                      Configuration::GetParameter("app_data_dir", wxT("./")).
                      append(libraryFileName_), USER) {
   AddCapability(READ);
   AddCapability(WRITE);
   AddCapability(APPEND);
   name_ = _(WMS_LIBRARY_NAME);
}

/** Destructor **/
WmsLibraryOrigin::~WmsLibraryOrigin() {
}
} /** namespace suri */
