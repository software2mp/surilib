/*! \file WmsLibraryOrigin.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WMSLIBRARYORIGIN_H_
#define WMSLIBRARYORIGIN_H_

// Includes Estandar
// Includes Suri
#include "suri/LibraryItemOrigin.h"
#include "suri/TextFileLibraryItemOrigin.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase que representa un origin de items de biblioteca para tabla de intervalos
 */
class WmsLibraryOrigin : public TextFileLibraryItemOrigin {
   /** ctor copia **/
   WmsLibraryOrigin(const WmsLibraryOrigin&);

public:
   /** constructor **/
   explicit WmsLibraryOrigin(const SuriObject::UuidType& LibraryId);
   /** dtor **/
   virtual ~WmsLibraryOrigin();

private:
   /** Filename para la biblioteca**/
   static std::string libraryFileName_;
};

} /** namespace suri */

#endif /* WMSLIBRARYORIGIN_H_ */
