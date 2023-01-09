/*! \file FavoritesIndexFile.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <fstream>

// Includes Suri
#include "FavoritesIndexFile.h"
#include "logmacros.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor
 *  @param[in] FileName archivo en donde se encuentra el indice de favoritos */
FavoritesIndexFile::FavoritesIndexFile(const std::string FileName) :
      filename_(FileName) {
}

FavoritesIndexFile::~FavoritesIndexFile() {
}
/** Carga los indices en el set pasado por parametro **/
bool FavoritesIndexFile::LoadFavoritesIndex(std::set<std::string>& FavoritesIndex) {
   if (filename_.empty())
      return false;
   std::string line;
   std::ifstream file(filename_.c_str());
   if (file.is_open()) {
      while (file.good()) {
         getline(file, line);
         if (!line.empty()) {
            FavoritesIndex.insert(line);
         }
      }
      file.close();
   }
   return true;
}

/** Elimina el item con id pasado por parametro del archivo de favoritos **/
bool FavoritesIndexFile::RemoveItemFromFavorite(const std::string& ItemId) {
   if (filename_.empty())
      return false;
   std::ifstream filein(filename_.c_str());
   std::string filetempout = filename_ + ".tmp";
   // archivo temporal
   std::ofstream fileout(filetempout.c_str());
   if (filein.is_open()) {
      std::string line;
      while (filein.good()) {
         getline(filein, line);
         bool found = ItemId.compare(line) == 0;
         if (!found) {
            line += "\n";
            fileout << line;
         }
      }
      filein.close();
      fileout.close();
      // Borro el archivo original
      remove(filename_.c_str());
      // Renombro el archivo de salida
      rename(filetempout.c_str(), filename_.c_str());
   }
   return true;
}

/** Agrega el item con id pasado por parametro al archivo de favoritos al final
 *  del archivo
 * Precondicion: El ItemId debe existir y ser valido **/
bool FavoritesIndexFile::AddItemFromFavorite(const std::string& ItemId) {
   if (filename_.empty())
      return false;
   bool added = false;
   std::ofstream file(filename_.c_str(), std::ofstream::app);
   if (file.is_open()) {
      file << ItemId << std::endl;
      added = true;
      file.close();
   }
   return added;
}

/** Reemplaza el contenido del archivo por el indice pasado por parametro **/
bool FavoritesIndexFile::ReplaceIndex(std::set<std::string>& FavoritesIndex) {
   return false;
}
} /** namespace suri */
