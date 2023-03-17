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

#ifndef ELEMENTHTMLCONFIGURATIONDATA_H_
#define ELEMENTHTMLCONFIGURATIONDATA_H_

// Includes standard
#include <map>

// Includes Suri
#include "suri/Element.h"
#include "View.h"
#include "HtmlConfigurationData.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

class wxSimpleHtmlTreeCtrl;

/**
 * Clase que genera la configuracion de un nodo de wxSimpleHtmlTreeCtrl
 * a partir del elemento de la lista.
 */
class ElementHtmlConfigurationData : public HtmlConfigurationData, View {
public:
   /** Ctor */
   ElementHtmlConfigurationData(Element* pElement, wxSimpleHtmlTreeCtrl* pWxTreeCtrl);
   /** Dtor */
   virtual ~ElementHtmlConfigurationData();
   /** Retorna elemento asociado al nodo */
   Element* GetElement() const;
   /** Actualiza el control con el contenido del elemento */
   void Update();

protected:
   /** Recibe el update */
   virtual void Update(Model *pModel);
   /** Recibe el aviso de delete */
   virtual void OnModelDelete(Model *pModel);

private:
   Element* pElement_; /*! Elemento asociado al nodo */
   wxSimpleHtmlTreeCtrl* pWxTreeCtrl_; /*! Permite actualizar interfaz despues */
   /* de Update */
};

}  //namespace

#endif /* ELEMENTHTMLCONFIGURATIONDATA_H_ */
