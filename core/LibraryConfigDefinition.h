/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
