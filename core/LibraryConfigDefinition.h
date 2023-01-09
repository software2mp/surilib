/** \file LibraryConfigDefinition.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYCONFIGDEFINITION_H_
#define LIBRARYCONFIGDEFINITION_H_

// Includes Estandar
#include <vector>
#include <string>

namespace suri {

// Forwards
class LibraryConfigDefinitionOrigin;

/**
 * Contiene la informacion de un item de configuracion para una biblioteca en particular.
 */
class LibraryConfigDefinition {
public:
   /**
    * Ctor.
    */
   explicit LibraryConfigDefinition(const std::string& Name);

   /**
    * Dtor.
    */
   ~LibraryConfigDefinition();

   /** Configura el archivo de indice de favoritos */
   void SetFavoritesIndexFileName(const std::string& FileName);
   /** Obtiene el filename del indice de favoritos*/
   std::string GetFavoritesIndexFileName() const;
   /**
    * Agrega un origen a la definicion.
    */
   void AddOrigin(LibraryConfigDefinitionOrigin* pOrigin);

   /**
    * Devuelve todos los origenes disponibles.
    */
   std::vector<LibraryConfigDefinitionOrigin*> GetOrigins() const;

   /**
    * Devuelve todos los origenes disponibles para un determinado nivel.
    */
   std::vector<LibraryConfigDefinitionOrigin*> GetOriginsByLevel(int Level) const;
   /**
    * Devuelve el nombre de la definicion.
    */
   std::string GetName() const;

private:
   std::string name_;
   std::vector<LibraryConfigDefinitionOrigin*> origins_;
   std::string favoritesIndexFileName_;
};

}  // namespace suri

#endif  // LIBRARYCONFIGDEFINITION_H_
