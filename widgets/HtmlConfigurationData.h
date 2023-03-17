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

#ifndef HTMLCONFIGURATIONDATA_H_
#define HTMLCONFIGURATIONDATA_H_

// Includes standard
#include <string>

// Includes Suri
#include "suri/Command.h"

// Includes Wx

// Includes App

// Defines

namespace suri {
/**
 * Interfaz que brinda wxSimpleHtmlTreeCtrl para configurar html de un item
 */
class HtmlConfigurationData {
public:
   enum State {
      ON, OFF, BOTH
   };

   /** Ctor */
   HtmlConfigurationData(std::string Name, bool Leaf = true);
   /** Dtor */
   virtual ~HtmlConfigurationData();
   /** Retorna nombre de nodo */
   std::string GetName() const;
   /** Modifica nombre del nodo */
   void SetName(std::string Name);
   /** Retorna el nombre del icono */
   std::string GetIcon() const;
   /** Modifica el nombre del icono */
   void SetIcon(std::string Icon);
   /** Indica si es nodo hoja */
   bool IsLeaf() const;
   /** Modifica indicador de si es hoja */
   void Leaf(bool IsLeaf);
   /** Retorna estado del item. */
   State GetState() const;
   /** Configura estado del item. */
   void SetState(State State);
   /** Retorna command. */
   Command* GetExecuteOnState() const;
   /** Configura command. */
   void SetExecuteOnState(Command* pExecuteOnState);

protected:
   std::string name_; /*! Nombre del nodo */
   std::string icon_; /*! Path del icono asociado al nodo */
   State state_; /*! Indica estado del check. */
   bool isLeaf_; /*! Indica si el nodo es tipo hoja. */
   Command* pExecuteOnState_; /*! Se ejecuta cuando el usuario modifica estado de activacion desde */
   // interfaz.
};
}  // namespace

#endif /* HTMLCONFIGURATIONDATA_H_ */
