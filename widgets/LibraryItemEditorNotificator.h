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

#ifndef LIBRARYITEMEDITORNOTIFICATOR_H_
#define LIBRARYITEMEDITORNOTIFICATOR_H_

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "LibraryItemEditorPart.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase que se encarga de realizar la comunicacion bidireccional
 * entre el Componente Grafico universal y
 * el Editor de biblioteca
 */
class LibraryItemEditorNotificator {
public:
   /** ctor **/
   LibraryItemEditorNotificator(LibraryItemEditorPart* pEditor = NULL,
                                UniversalGraphicalComponentPartInterface* pCgu = NULL);
   /** dtor **/
   virtual ~LibraryItemEditorNotificator();
   /** Notifica que se selecciono un nuevo item en el CGU.
    *  Si el item pasado por parametro es NULL significa que
    *  no se encuentra seleccionado ningun item **/
   void NotifyItemSelectionFromCgu(const LibraryItem* pItem = NULL,
                                   bool FirstSelection = false, bool Modifiable = true);
   /** Notifica que se selecciono un item desde el editor, indicandole el
    *  atributo principal. En caso de que no exista el item en el CGU
    *  Es resposabilidad del CGU tratarlo**/
   void NotifyItemSelectionFromEditor(const std::string& PrincipalAttribute);
   /** Notifica al cgu que el editor ha cambiado informacion del item, notificando
    * ademas la validez de la informacion*/
   void NotifyItemInformationChangedFromEditor(bool IsValidData);
   /** Indica la referencia al editor in situ*/
   void SetLibraryItemEditorDebugPart(LibraryItemEditorPart* pEditor);
   /** Indica la referencia al cgu*/
   void SetUniversalGraphicalComponentPart(UniversalGraphicalComponentPartInterface* pCgu);
   /** Obtiene los atributos modificados desde el editor
    *  @return vector con atributos
    *  @return NULL en caso de que no se haya modificado ningun atributo */
   std::vector<LibraryItemAttribute*>* GetAttributesModifiedFromEditor();

   /** Obtiene los atributos eliminados. */
   std::vector<LibraryItemAttribute*>* GetAttributesRemovedFromEditor();

   /** Metodo que consulta la validez de informacion del part del editor. **/
   bool AskEditorForValidData() const;
   /** Metodo que consulta el estado del check del cgu **/
   bool AskCguForCheckState() const;

private:
   LibraryItemEditorPart* pEditor_;
   UniversalGraphicalComponentPartInterface* pCgu_;
};

} /** namespace suri */

#endif /* LIBRARYITEMEDITORNOTIFICATOR_H_ */
