/** \file LibraryItemOriginFactory.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYITEMORIGINFACTORY_H_
#define LIBRARYITEMORIGINFACTORY_H_

namespace suri {

class Library;
class LibraryItemOrigin;
class LibraryConfigDefinitionOrigin;

class LibraryItemOriginFactory {
public:
   /**
    * Dtor.
    */
   ~LibraryItemOriginFactory() {
   }

   /**
    * Crea una nueva instancia de LibraryItemOrigin basado en la definicion del origen.
    */
   static LibraryItemOrigin* Create(Library* pLibrary,
                                    const LibraryConfigDefinitionOrigin* pOrigin,
                                    const std::string& FavoritesIndexFileName);

private:
   /**
    * Ctor.
    */
   LibraryItemOriginFactory() {
   }

   /**
    * Crea una nueva instancia de LibraryItemOrigin de tipo texto plano.
    */
   static LibraryItemOrigin* CreateTextfile(
         Library* pLibrary, const LibraryConfigDefinitionOrigin* pOrigin,
         const std::string& FavoritesIndexFileName);

   /**
    * Crea una nueva instancia de LibraryItemOrigin para carga de datos desde gdal.
    */
   static LibraryItemOrigin* CreateGdal(Library* pLibrary,
                                        const LibraryConfigDefinitionOrigin* pOrigin,
                                        const std::string& FavoritesIndexFileName);
};

} // namespace suri

#endif  // LIBRARYITEMORIGINFACTORY_H_
