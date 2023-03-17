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
// Includes Suri
#include "SearchWidget.h"
#include "suri/Widget.h"
#include "suri/messages.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(SearchWidgetEvent, SearchWidget)
   IMPLEMENT_EVENT_CALLBACK(OnButtonSearch, OnButtonSearch(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonDeleteFilters, OnButtonDeleteFilters(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonQueryBuilder, OnButtonQueryBuilder(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Constructor
 * @param SearchFilterApplierInterface
 * **/
SearchWidget::SearchWidget(SearchFilterApplierInterface* pFilterApplier) :
      NEW_EVENT_OBJECT(SearchWidgetEvent), pFilterApplier_(pFilterApplier) {
}

/** Destructor */
SearchWidget::~SearchWidget() {
   DELETE_EVENT_OBJECT;
}

/**
 * Se crea la ventana de herramienta en las clases derivadas
 * Crea la ventana, conecta los botones de control y muestra los requeridos
 * @return bool que indica si pudo crear la ventana (true siempre)
 */
bool SearchWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_SEARCH_WIDGET_PANEL"));
   if (pToolWindow_ == NULL) return false;
   // Conecto los eventos a los controles.
   GET_CONTROL(*pToolWindow_, "ID_SIMPLE_SEARCH_BUTTON", wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(SearchWidgetEvent::OnButtonSearch), NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_DELETE_FILTERS_BUTTON", wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(SearchWidgetEvent::OnButtonDeleteFilters), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_QUERY_BUTTON", wxBitmapButton)->Connect(
           wxEVT_COMMAND_BUTTON_CLICKED,
           wxCommandEventHandler(SearchWidgetEvent::OnButtonQueryBuilder), NULL,
           pEventHandler_);
   return true;
}

/** Metodo que es llamado cuando se produce el evento de click sobre el boton de busqueda. */
void SearchWidget::OnButtonSearch(wxCommandEvent &Event) {
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_SEARCH_WIDGET_TEXTCTRL"), wxTextCtrl);
   if (ptext) {
      std::string Condition = ptext->GetValue().c_str();
      pFilterApplier_->SetFilter(Condition);
   }
}

/** Metodo que es llamado cuando se produce el evento de
 * click sobre el boton eliminar filtros */
void SearchWidget::OnButtonDeleteFilters(wxCommandEvent &Event) {
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_SEARCH_WIDGET_TEXTCTRL"), wxTextCtrl);
   if (ptext) {
      ptext->Clear();
      std::string Condition = ptext->GetValue().c_str();
      pFilterApplier_->SetFilter(Condition);
   }
}

void SearchWidget::SetValue(const std::string Condition) {
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_SEARCH_WIDGET_TEXTCTRL"), wxTextCtrl);
   if (ptext) {
      ptext->SetValue(Condition);
      pFilterApplier_->SetFilter(Condition);
   }
}

void SearchWidget::OnButtonQueryBuilder(wxCommandEvent &Event) {
   pFilterApplier_->QueryBuilder();
}

} /** namespace suri */
