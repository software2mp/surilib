/*! \file FavoritesIndexFile.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FAVORITESINDEXFILE_H_
#define FAVORITESINDEXFILE_H_

// Includes Estandar
#include <string>
#include <set>

// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Clase que representa el archivo de indice de favoritos
 * Permite cargar el indice de favoritos
 * Es la clase encargada de administrar el archivo (alta, eliminacion)
 */
class FavoritesIndexFile {
public:
   /** Constructor **/
   FavoritesIndexFile(const std::string FileName);
   virtual ~FavoritesIndexFile();
   /** Carga los indices en el set pasado por parametro **/
   bool LoadFavoritesIndex(std::set<std::string>& FavoritesIndex);
   /** Elimina el item con id pasado por parametro del archivo de favoritos **/
   bool RemoveItemFromFavorite(const std::string& ItemId);
   /** Agrega el item con id pasado por parametro al archivo de favoritos **/
   bool AddItemFromFavorite(const std::string& ItemId);
   /** Reemplaza el contenido del archivo por el indice pasado por parametro **/
   bool ReplaceIndex(std::set<std::string>& FavoritesIndex);

private:
   std::string filename_;
};

} /** namespace suri */

#endif /* FAVORITESINDEXFILE_H_ */
