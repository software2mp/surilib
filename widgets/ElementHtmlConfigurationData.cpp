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

#include "ElementHtmlConfigurationData.h"

// Includes standard

// Includes Suri
#include "wxSimpleHtmlTreeCtrl.h"

// Includes Wx

// Includes App

// Defines

// Forwards

namespace suri {

/** Ctor */
ElementHtmlConfigurationData::ElementHtmlConfigurationData(
      Element* pElement, wxSimpleHtmlTreeCtrl* pWxTreeCtrl) :
      HtmlConfigurationData(""), pElement_(pElement), pWxTreeCtrl_(pWxTreeCtrl) {
   if (pElement_ == NULL)
      Leaf(false);
   else {
      pElement_->RegisterViewer(this);
      Update();
   }
}

/** Dtor */
ElementHtmlConfigurationData::~ElementHtmlConfigurationData() {
   if (pElement_ != NULL) pElement_->UnregisterViewer(this);
   pElement_ = NULL;
   pWxTreeCtrl_ = NULL;
}

/**
 * Retorna elemento asociado al nodo
 * @return Element con el que se configura el item. NULL si es un nodo interno
 * del arbol.
 */
Element* ElementHtmlConfigurationData::GetElement() const {
   return pElement_;
}

/** Actualiza el control con el contenido del elemento */
void ElementHtmlConfigurationData::Update() {
   if (!pElement_) return;

   pWxTreeCtrl_->BeginBatch();

   SetName(pElement_->GetName().c_str());
   if (pElement_->IsActive())
      SetState(HtmlConfigurationData::ON);
   else
      SetState(HtmlConfigurationData::OFF);
   SetIcon(pElement_->GetIcon().c_str());

   pWxTreeCtrl_->EndBatch();
}

/**
 * Evento de update de View
 * @param[in]  pModel: modelo que llama al Update.
 */
void ElementHtmlConfigurationData::Update(Model *pModel) {
   Update();
}

/**
 * Recibe el aviso de delete
 * @param[in]  pModel: modelo que se elimina.
 */
void ElementHtmlConfigurationData::OnModelDelete(Model *pModel) {
   pElement_ = NULL;
}

}

