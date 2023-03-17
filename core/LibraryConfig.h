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

#ifndef LIBRARYCONFIG_H_
#define LIBRARYCONFIG_H_

#include <string>

// forward wx
class wxXmlDocument;
class wxXmlNode;

namespace suri {

class LibraryConfigDefinition;

/**
 * Carga la configuracion de bibliotecas para que este disponible para la aplicacion.
 */
class LibraryConfig {
public:
   /**
    * Devuelve el LibraryConfigDefinition correspondiente la nombre dado.
    */
   LibraryConfigDefinition* GetDefinitionByName(const std::string& DefName);
   /** Definicion de nombres de nodos de configuracion de bibliotecas **/
   static std::string LibraryConfigNodeName;
   static std::string LibrariesDefNodeName;
   static std::string LibraryDefNodeName;
   static std::string LibraryOriginsNodeName;
   static std::string LibraryOriginNodeName;
   static std::string LibraryFavIndexNodeName;
   static std::string LibrarySourceProperty;
   static std::string LibraryPathProperty;
   static std::string LibraryTypeProperty;
   static std::string LibraryNameProperty;
   static std::string LibraryItemTypeProperty;
   static std::string LibraryCapabilitiesProperty;
   static std::string LibraryLevelProperty;
   static std::string LibraryConfigFileName;

   static LibraryConfig* GetInstance() {
      static LibraryConfig* plibconfig = new LibraryConfig();
      return plibconfig;
   }

private:
   /** Ctor. */
   LibraryConfig();

   /** Dtor. */
   ~LibraryConfig();
   /** Carga los origenes que contiene el nodo <origin> **/
   void LoadOrigins(LibraryConfigDefinition* pLibConfigDef,
                    wxXmlNode* pOriginsNode) const;
   /** Carga los favoritos que se encuentran configurados en el indice
    *  de favoritos
    */
   void LoadFavorites(LibraryConfigDefinition* pLibConfigDef,
                                   wxXmlNode* pOriginsNode) const;
};

}  // namespace suri

#endif  // LIBRARYCONFIG_H_
