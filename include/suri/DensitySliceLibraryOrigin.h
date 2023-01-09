/*! \file DensitySliceLibraryOrigin.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DENSITYSLICELIBRARYORIGIN_H_
#define DENSITYSLICELIBRARYORIGIN_H_

// Includes Estandar
// Includes Suri
#include "LibraryItemOrigin.h"
#include "TextFileLibraryItemOrigin.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase que representa un origin de items de biblioteca para tabla de intervalos
 */
class DensitySliceLibraryOrigin : public TextFileLibraryItemOrigin {
   /** ctor copia **/
   DensitySliceLibraryOrigin(const DensitySliceLibraryOrigin&);

public:
   /** constructor **/
   explicit DensitySliceLibraryOrigin(const SuriObject::UuidType& LibraryId);
   /** dtor **/
   virtual ~DensitySliceLibraryOrigin();

private:
   /** Filename para la biblioteca**/
   static std::string libraryFileName_;
};

} /** namespace suri */

#endif /* DENSITYSLICELIBRARYORIGIN_H_ */
