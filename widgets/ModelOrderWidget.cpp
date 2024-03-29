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
#include "ModelOrderWidget.h"
#include "ModelOrderChangeNotification.h"
#include "suri/Widget.h"
#include "suri/AuxiliaryFunctions.h"
#include "RmseRequestNotification.h"

// Includes Wx
#include "wx/spinctrl.h"
#include "wx/textctrl.h"
#include "wx/stattext.h"

// Defines
// forwards

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ModelOrderWidgetEvent, ModelOrderWidget)
      IMPLEMENT_EVENT_CALLBACK(OnSpinChange, OnSpinChange(event),
                               wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** ctor
 * @param TablePart donde se incluira el ModelOrder widget
 * **/
ModelOrderWidget::ModelOrderWidget(TablePart* pTablePart) :
      pTablePart_(pTablePart), order_(1), NEW_EVENT_OBJECT(ModelOrderWidgetEvent) {
   ModelOrderChangeNotification notification(this, order_);
   if (pTablePart_)
      pTablePart->Notify(&notification);
}

/** dtor */
ModelOrderWidget::~ModelOrderWidget() {
   DELETE_EVENT_OBJECT;
}

/** Creacion de la ventana */
bool ModelOrderWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                             wxT("ID_CORREGISTER_SELECT_PANEL"));
   if (pToolWindow_ == NULL)
      return false;
   wxSpinCtrl* pspin = XRCCTRL(*pToolWindow_, wxT("ID_MODEL_ORDER_SPINCTRL"), wxSpinCtrl);
   if (pspin) {
      pspin->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED,
                        wxCommandEventHandler(ModelOrderWidgetEvent::OnSpinChange), NULL,
                        pEventHandler_);
      RmseRequestNotification notification(this);
      pTablePart_->Notify(&notification);
   }
   return true;
}

/**
 * devuelve el titulo de la ventana
 * @return titulo de la ventana
 */
const wxString ModelOrderWidget::GetWindowTitle() const {
   return wxString("");
}

/**
 * setea el orden del polinomio aproximador
 * @param Orden del polinomio aproximador
 *
 */
void ModelOrderWidget::SetOrder(const int Order) {
   this->order_ = Order;
}
/**
 * Retorna el valor Rmse
 * @return error cuadratico medio
 */

double ModelOrderWidget::GetRmse() const {
   return rmse_;
}
/**
 * Retorna el valor del orden del polinomio aproximador
 * @return orden del polinimio
 */
int ModelOrderWidget::GetOrder() const {
   return order_;
}

/**
 * Setea y actualiza el valor del error cuadratico medio en el widget dados
 * por la actualizacion de los puntos gcp de la edicion de corregistro
 * @param Error cuadratico medio
 */
void ModelOrderWidget::SetRmse(double RmseModel) {
   this->rmse_ = RmseModel;
   UpdateRmseText(RmseModel);
}
/**
 * Evento que disparado por la mjodificacion del spin control asociado
 * al orden del polinomio
 * @param evento del spin control
 */
void ModelOrderWidget::OnSpinChange(wxCommandEvent &Event) {
   wxSpinCtrl* pspin = XRCCTRL(*pToolWindow_, wxT("ID_MODEL_ORDER_SPINCTRL"),
                               wxSpinCtrl);

   int order = pspin->GetValue();
   if (order != order_ && pTablePart_) {
      ModelOrderChangeNotification notification(this, order);
      pTablePart_->Notify(&notification);
      pTablePart_->UpdateTable();
      order_ = order;
   }

}
/**
 * Metodo que actuliza el rmse text
 * @param toma el valor rmse para actualizar el campo de texto
 */
void ModelOrderWidget::UpdateRmseText(double RmseValue) {
   std::string rmsevalue = "rmse : ";
   rmsevalue.append(DoubleToString(RmseValue,3));
   GET_CONTROL(*pToolWindow_, "wxID_RMSE_STATIC_TEXT", wxStaticText)->SetLabel(rmsevalue.c_str());
}
} /** namespace suri */
