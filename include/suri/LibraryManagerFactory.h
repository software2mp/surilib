/** \file LibraryManagerFactory.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef LIBRARYMANAGERFACTORY_H_
#define LIBRARYMANAGERFACTORY_H_

// Includes standard
#include <string>
// Includes suri
#include "LibraryManager.h"

namespace suri {

/** Clase encargada de generar y configurar el administrador de bibliotecas
 *  de la aplicacion
 */
class LibraryManagerFactory {
public:
   /** Inicializa LibraryManager */
   static LibraryManager* CreateLibraryManager();
   /** Recarga todos los origenes en LibraryManager */
   static void ResetLibraryManager(LibraryManager* pManager);
   /** Codigo de bibliotecas existente **/
   static std::string SpatialReferenceLibraryCode;
   static std::string DatumLibraryCode;
   static std::string EllipsoidLibraryCode;
   static std::string DensitySliceCode;
   static std::string WmsLibraryCode;
   static std::string VectorstyleLibraryCode;
   static std::string SpectralSignLibraryCode;
   static std::string ConvolutionFiltersCode;
   static std::string EquationLibraryCode;
   static std::string IndexLibraryCode;

private:
   /** Agrega un origen de archivo a biblioteca indicada */
   static void AddTextFileOriginToLibrary(
         Library* pLibrary, const std::string &FileName,
         LibraryItemOrigin::OriginCategoryType Category, const std::string& OriginName);
   /** Agrega un origen en memoria a biblioteca indicada */
   static void AddMemoryOriginToLibrary(Library* pLibrary,
                                        LibraryItemOrigin::OriginCategoryType Category,
                                        const std::string& OriginName);

   /**
    * Carga los distintos origenes disponibles a la libreria.
    */
   static void LoadOrigins(Library* pLibrary, const std::string& LibCode);
};
} /* namespace suri */


#endif /* LIBRARYMANAGERFACTORY_H_ */
