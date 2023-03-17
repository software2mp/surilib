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

#ifndef LIBRARYITEMEDITORDEBUGPART_H_
#define LIBRARYITEMEDITORDEBUGPART_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "LibraryItemEditorPart.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
/** Declara la clase TextChangedEvent */
DECLARE_EVENT_OBJECT(TextChangedEvent);
/** Declara la clase ValidDataCheckEvent */
DECLARE_EVENT_OBJECT(ValidDataCheckEvent);
class LibraryItemEditorNotificator;

/*!
 * Clase que representa un editor de un item el cual es de uso para debug.
 * Ademas se tomara como ejemplo de implementacion para desarrollar los editores
 * futuros.
 * Posee una comunicacion bidireccional con el CGU.
 */
class LibraryItemEditorDebugPart : public LibraryItemEditorPart {
public:
   LibraryItemEditorDebugPart(
         const SuriObject::UuidType& LibraryId,
         LibraryManager* pManager,
         const UniversalGraphicalComponentPartInterface::FeatureCombinationEnum& Feature =
               UniversalGraphicalComponentPartInterface::EditionWithFavorites,
         const std::string& WxWindowdId = "ID_LIBRARY_DEBUG_PANEL",
         const std::string& WxCguPanelId = "ID_CGU_PANEL");

   virtual ~LibraryItemEditorDebugPart();
   /** Metodo auxiliar que obtiene el texto del wxtextctrl y genera un item asociado **/
   virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields();

private:
   FRIEND_EVENT_OBJECT(TextChangedEvent);FRIEND_EVENT_OBJECT(ValidDataCheckEvent);EVENT_OBJECT_PROPERTY(TextChangedEvent);
   ValidDataCheckEvent* pCheckEventHandler_;
   /**
    *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
    *  Este metodo es el que realmente tienen que implementar las clases hijas para
    * lograr la especializacion a la hora de iniciar el part (metodo que se llama
    * en CreateToolWindow, a modo de Template Method).
    **/
   virtual bool ConfigureFeatures();
   /**
    * Metodo auxiliar privado que es necesario implementar en las clases hijas.
    * Este metodo es el encargado de actualizar realmente la informacion en el part
    * Es llamado en el metodo SetActiveItem a modo de Template Method
    **/
   virtual void UpdateFields(const LibraryItem* pItem, bool Modifiable = true);
   /** Evento UIUpdate */
   void OnUIUpdate(wxUpdateUIEvent &Event);
   /** Evento para el check de valid data **/
   void OnValidDataCheckEvent(wxCommandEvent &Event);
   /** Metodo auxiliar que dado un item muestra sus atributos
    *  en un string
    * @param pItem item que se desea convertir a string
    * @return string con el contenido del item
    */
   std::string ConvertItemToString(const LibraryItem* pItem) const;
   /** item seleccionado **/
   const LibraryItem* pItemSelected_;
};

} /** namespace suri */

#endif /* LIBRARYITEMEDITORDEBUGPART_H_ */
