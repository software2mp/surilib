/*! \file SearchWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
