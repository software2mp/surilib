/** \file LibraryConfigDefinition.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "LibraryConfigDefinition.h"
#include "LibraryConfigDefinitionOrigin.h"

namespace suri {

/**
 * Ctor.
 */
LibraryConfigDefinition::LibraryConfigDefinition(const std::string& Name) :
      name_(Name) {
}

/**
 * Dtor.
 */
LibraryConfigDefinition::~LibraryConfigDefinition() {

}

/**
 * Agrega un origen a la definicion.
 */
void LibraryConfigDefinition::AddOrigin(LibraryConfigDefinitionOrigin* pOrigin) {
   origins_.push_back(pOrigin);
}

/**
 * Devuelve todos los origenes disponibles.
 */
std::vector<LibraryConfigDefinitionOrigin*> LibraryConfigDefinition::GetOrigins() const {
   return origins_;
}

/**
 * Devuelve todos los origenes disponibles para un determinado nivel.
 */
std::vector<LibraryConfigDefinitionOrigin*> LibraryConfigDefinition::GetOriginsByLevel(
      int Level) const {
   std::vector<LibraryConfigDefinitionOrigin*> retvec;
   for (int ix = 0, lenix = origins_.size(); ix < lenix; ++ix) {
      if (origins_[ix]->GetLevel() == Level) {
         retvec.push_back(origins_[ix]);
      }
   }
   return retvec;
}

/**
 * Devuelve el nombre de la definicion.
 */
std::string LibraryConfigDefinition::GetName() const {
   return name_;
}
/** Configura el archivo de indice de favoritos */
void LibraryConfigDefinition::SetFavoritesIndexFileName(const std::string& FileName) {
   favoritesIndexFileName_ = FileName;
}

/** Obtiene el filename del indice de favoritos*/
std::string LibraryConfigDefinition::GetFavoritesIndexFileName() const {
   return favoritesIndexFileName_;
}
} // namespace suri
