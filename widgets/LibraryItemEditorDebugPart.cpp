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

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "LibraryItemEditorDebugPart.h"
#include "LibraryItemEditorNotificator.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
#include "wx/textctrl.h"

// Defines
#define ATTRIBUTE_TOKEN "\n"
#define ATTR_NAME_VALE_TOKEN "="
// forwards

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(TextChangedEvent, LibraryItemEditorDebugPart)
      IMPLEMENT_EVENT_CALLBACK(OnUIUpdate, OnUIUpdate(event), wxUpdateUIEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ValidDataCheckEvent, LibraryItemEditorDebugPart)
      IMPLEMENT_EVENT_CALLBACK(OnValidDataCheckEvent, OnValidDataCheckEvent(event),
                               wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */


/** ctor **/
LibraryItemEditorDebugPart::LibraryItemEditorDebugPart(
      const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
      const UniversalGraphicalComponentPartInterface::
                                    FeatureCombinationEnum& Feature,
                                    const std::string& WxWindowdId,
                                    const std::string& WxCguPanelId) :
      LibraryItemEditorPart(LibraryId, pManager, WxWindowdId, WxCguPanelId, Feature),
      NEW_EVENT_OBJECT(TextChangedEvent),
      pCheckEventHandler_(new ValidDataCheckEvent(this)) {
}

/** dtor **/
LibraryItemEditorDebugPart::~LibraryItemEditorDebugPart() {
   DELETE_EVENT_OBJECT;
   delete pCheckEventHandler_;
}

/**
 *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
 *  Este metodo es el que realmente tienen que implementar las clases hijas para
 * lograr la especializacion a la hora de iniciar el part (metodo que se llama
 * en CreateToolWindow, a modo de Template Method).
 **/
bool LibraryItemEditorDebugPart::ConfigureFeatures() {
   pItemSelected_ = pCgu_->GetSelectedItem();
   UpdateFields(pItemSelected_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_ATRIBUTE_TEXT", wxTextCtrl)->Connect(
            wxEVT_UPDATE_UI, wxUpdateUIEventHandler(TextChangedEvent::OnUIUpdate), NULL,
            pEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_VALID_CHECKBOX", wxCheckBox)->Connect(
            wxEVT_COMMAND_CHECKBOX_CLICKED,
            wxCommandEventHandler(ValidDataCheckEvent::OnValidDataCheckEvent), NULL,
            pCheckEventHandler_);
   wxCheckBox* pcheckvalidator = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                         wxT("ID_VALID_CHECKBOX"), wxCheckBox);
   validData_ = pcheckvalidator->IsChecked();
   return true;
}

/**
 * Metodo auxiliar privado que es necesario implementar en las clases hijas.
 * Este metodo es el encargado de actualizar realmente la informacion en el part
 * Es llamado en el metodo SetActiveItem a modo de Template Method
 **/
void LibraryItemEditorDebugPart::UpdateFields(const LibraryItem* pItem, bool Modifiable) {
   pItemSelected_ = pItem;
   std::string itemstring = ConvertItemToString(pItem);
   wxTextCtrl* ptext = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                   wxT("ID_ATRIBUTE_TEXT"), wxTextCtrl);
   ptext->SetValue(itemstring);
   modified_ = false;
}

/** Metodo auxiliar que dado un item muestra sus atributos
 *  en un string
 * @param pItem item que se desea convertir a string
 * @return string con el contenido del item
 */
std::string LibraryItemEditorDebugPart::ConvertItemToString(
      const LibraryItem* pItem) const {
   std::string itemstring;
   if (pItem) {
      LibraryItem::MapAttribute itemattributes = pItem->GetAttributes();
      LibraryItem::MapAttribute::const_iterator consit = itemattributes.begin();
      for (; consit != itemattributes.end(); ++consit) {
         if (consit->second->GetValue().compare(
               pItem->GetPrincipalAttribute()->GetValue()) != 0) {
            itemstring.append(consit->first);
            itemstring.append(ATTR_NAME_VALE_TOKEN);
            itemstring.append(consit->second->GetValue());
            itemstring.append(ATTRIBUTE_TOKEN);
         }
      }
   }
   return itemstring;
}

/**
 * @param[in] Event: Evento.
 */
void LibraryItemEditorDebugPart::OnUIUpdate(wxUpdateUIEvent &Event) {
   wxTextCtrl* ptext = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                     wxT("ID_ATRIBUTE_TEXT"), wxTextCtrl);
   modified_ = ptext->IsModified();
   if (modified_)
      pNotificator_->NotifyItemInformationChangedFromEditor(HasValidData());
   Event.SetUpdateInterval(250);
   Event.Skip();
}

/**  Metodo que obtiene el texto del wxtextctrl y genera
 *   un vector con los atributos
 *   @return vector con atributos
 *   @return NULL en caso de que no se haya modificado los atributos (o sea el texto)
 *   @return NULL en caso de que la informnacion del part no sea valida
 *   **/
std::vector<LibraryItemAttribute*>*
LibraryItemEditorDebugPart::CreateAttributesFromFields() {
   wxTextCtrl* ptext = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_ATRIBUTE_TEXT"), wxTextCtrl);
   int lines = ptext->GetNumberOfLines();
   std::vector<LibraryItemAttribute*>* pattributesvalues = NULL;
   if (HasValidData()) {
      pattributesvalues = new std::vector<LibraryItemAttribute*>();
      for (int i = 0; i < lines; ++i) {
         std::string line = ptext->GetLineText(i).c_str();
         std::vector < std::string > attribute = tokenizer(line, ATTR_NAME_VALE_TOKEN);
         if (attribute.size() == 2) {
            pattributesvalues->push_back(
                  new LibraryItemAttribute(attribute[0], attribute[1]));
         }
      }
   }
   return pattributesvalues;
}

/** Evento para el check de valid data **/
void LibraryItemEditorDebugPart::OnValidDataCheckEvent(wxCommandEvent &Event) {
   wxCheckBox* pcheckvalidator = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                         wxT("ID_VALID_CHECKBOX"), wxCheckBox);
   validData_ =  pcheckvalidator->IsChecked();
}
}
/** namespace suri */
