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

#ifndef BASICLIBRARYITEMSELECTOR_H_
#define BASICLIBRARYITEMSELECTOR_H_

// Includes Estandar
// Includes Suri
#include "LibraryItemEditorPart.h"
#include "LibraryItemSelectionInterface.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/* Clase abstracta de tal forma que a traves de ella cualquier otra clase pueda incluir las
 * bibliotecas y capturar su comportamiento */

class BasicLibraryItemSelector : public LibraryItemEditorPart {
public:
   /* Constructor */
   BasicLibraryItemSelector(
         LibraryItemSelectionInterface* pItemSelector,
         const SuriObject::UuidType& LibraryId,
         LibraryManager* pManager,
         const std::string& WxWindowdId = "ID_LIBRARY_SELECTION_PANEL",
         const std::string& WxCguPanelId = "ID_LIBRARY_SELECTION_PANEL",
         bool FilterFavorites = false,
         const UniversalGraphicalComponentPartInterface::FeatureCombinationEnum& Feature =
               UniversalGraphicalComponentPartInterface::EditionWithFavorites,
               LibraryManager::AccessType Access = LibraryManager::ALL);
   /* Destructor */
   virtual ~BasicLibraryItemSelector();
   /**
    * Metodo auxiliar privado que es necesario implementar en las clases hijas.
    * Este metodo es el encargado de actualizar realmente la informacion en el part
    * Es llamado en el metodo SetActiveItem a modo de Template Method
    **/
   void UpdateFields(const LibraryItem* pItem, bool Modifiable = true);

protected:
   /* Puntero a LibraryItemSelector */
   LibraryItemSelectionInterface* pItemSelector_;
};

} /** namespace suri */

#endif /* BASICLIBRARYITEMSELECTOR_H_ */
