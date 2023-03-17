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

#ifndef LIBRARYCONFIGDEFINITIONORIGIN_H_
#define LIBRARYCONFIGDEFINITIONORIGIN_H_

#include <string>

namespace suri {

class LibraryConfigDefinitionOriginSource;

/**
 * Contiene la informacion necesaria para la carga del origen de datos.
 */
class LibraryConfigDefinitionOrigin {
public:
   static const int kCapabilitiesRead = 1;
   static const int kCapabilitiesWrite = 2;

   static const int kLevelSystem = 1;
   static const int kLevelUser = 2;
   static const int kLevelProject = 3;

   /**
    * Ctor.
    */
   LibraryConfigDefinitionOrigin();

   /**
    * Dtor.
    */
   ~LibraryConfigDefinitionOrigin();

   /**
    * Devuelve el nombre del origen.
    */
   std::string GetName() const;

   /**
    * Establece el nombre del origen.
    */
   void SetName(const std::string& Name);

   /**
    * Devuelve los permisos disponibles sobre el origen.
    */
   int GetCapabilities() const;

   /**
    * Establece los permisos sobre el origen.
    */
   void SetCapabilities(int Capabilities);

   /**
    * Devuelve el nivel del origen.
    */
   int GetLevel() const;

   /**
    * Establece el nivel del origen.
    */
   void SetLevel(int Level);

   /**
    * Devuelve la fuente del origen.
    */
   LibraryConfigDefinitionOriginSource* GetSource() const;

   /**
    * Establece la fuente del origen.
    */
   void SetSource(LibraryConfigDefinitionOriginSource* pSource);

private:
   std::string name_;  // nombre del origen
   int capabilities_;  // permisos (lectura y/o escritura)
   int level_;  // nivel (usuario, sistema o proyecto)
   LibraryConfigDefinitionOriginSource* pSource_;  // fuente de los datos
};

}  // namespace suri

#endif  // LIBRARYCONFIGDEFINITIONORIGIN_H_
