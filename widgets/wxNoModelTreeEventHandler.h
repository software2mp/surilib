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

#ifndef WXNOMODELTREEEVENTHANDLER_H_
#define WXNOMODELTREEEVENTHANDLER_H_

// Includes standard

// Includes Suri
#include "wxHtmlTreeEventHandler.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

//Includes suri
class wxSimpleHtmlTreeCtrl;

/**
 * Comportamiento default de wxHtmlTreeEventHandler.
 */
class wxNoModelTreeEventHandler : public wxHtmlTreeEventHandler {
public:
   //! Ctor
   wxNoModelTreeEventHandler(wxSimpleHtmlTreeCtrl* pHtmlTreeCtrl);
   //! Dtor
   ~wxNoModelTreeEventHandler();

   //! Se ejecuta cuando se modifican los elementos seleccionados
   virtual void DoOnSelectionChanged();
   //! Modifica el estado del nodo y el de sus hijos.
   virtual void DoOnStateChanged(const ItemId &Id,
                                 HtmlConfigurationData::State NewState);

   //! Valida si se puede mover nodo Id debajo de ParentId
   virtual bool ValidateNodeNewParent(const ItemId &Id, const ItemId &ParentId);
   //! Mueve un nodo despues de otro.
   virtual void DoOnMoveNodeAfter(const ItemId &Id, const ItemId &DestinationId);
   //! Mueve un nodo antes de otro.
   virtual void DoOnMoveNodeBefore(const ItemId &Id, const ItemId &DestinationId);
   //! Mueve un nodo debajo de otro.
   virtual void DoOnMoveNodeUnder(const ItemId &Id, const ItemId &DestinationId);
   //! Genera menu que se debera mostrar sobre la lista
   virtual wxMenu* CreateContextMenu(const ItemId &Id);
   //! Se llama cuando se selecciona item del menu contextual
   virtual void DoOnContextMenu(wxMenuEvent &Event);

protected:
   wxSimpleHtmlTreeCtrl* pHtmlTreeCtrl_;
};

}  //namespace

#endif /* WXNOMODELTREEEVENTHANDLER_H_ */
