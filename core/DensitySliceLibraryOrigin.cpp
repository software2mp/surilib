/*! \file DensitySliceLibraryOrigin.cpp */
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
#include "suri/DensitySliceLibraryOrigin.h"
#include "suri/Configuration.h"

// Includes Wx

// Defines
#define DENSITY_SLICE_LIBRARY_NAME "Tabla de intervalos"
// forwards

namespace suri {

std::string DensitySliceLibraryOrigin::libraryFileName_ = "densityslice.lbr";

/**
 * Constructor
 * @param LibraryId id de la biblioteca a la que pertenece el origen
 */
DensitySliceLibraryOrigin::DensitySliceLibraryOrigin(
      const SuriObject::UuidType& LibraryId) :
                        TextFileLibraryItemOrigin(LibraryId, capabilites_,
                      Configuration::GetParameter("app_data_dir", wxT("./")).
                      append(libraryFileName_), USER) {
   AddCapability(READ);
   AddCapability(WRITE);
   AddCapability(APPEND);
   name_ = _(DENSITY_SLICE_LIBRARY_NAME);
}

/** Destructor **/
DensitySliceLibraryOrigin::~DensitySliceLibraryOrigin() {
}
} /** namespace suri */
