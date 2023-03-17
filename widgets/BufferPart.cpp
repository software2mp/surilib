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
#include "BufferPart.h"
#include "suri/DataViewManager.h"
#include "suri/VectorEditionTable.h"
#include "SpatialReference.h"
#include "resources.h"
// Includes Wx
#include "wx/spinctrl.h"
#include "wx/regex.h"
// Defines
// forwards

const std::string title = "Configurar buffer";
const std::string metre = "Metros";
const std::string kilometre = "Kilometros";
const std::string degree = "Grados";
const std::string mile = "Millas";

namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(BufferPartEvent, BufferPart)
      IMPLEMENT_EVENT_CALLBACK(OnRadioButtonManual, OnRadioButtonManual(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnRadioButtonField, OnRadioButtonField(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnChoiceSelection, OnChoiceSelection(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnUnitSelection, OnUnitSelection(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnDistanceTexCtrlChanged, OnDistanceTexCtrlChanged(event),
                               wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Constructor */
BufferPart::BufferPart(DataViewManager *pDataViewManager,
                       World* pInputWorld) : NEW_EVENT_OBJECT(BufferPartEvent){
   pDataViewManager_ = pDataViewManager;
   pInputWorld_ = pInputWorld;
   windowTitle_ = title;
   operation_ = VectorOperation::Buffer;
}

/** Destructor */
BufferPart::~BufferPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Crea la ventana de la parte y contecta los eventos
 * @return true si pudo cargar la ventana de la herramienta.
 * @return false si fallo la carga de la ventana de la herramienta.
 */
bool BufferPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_BUFFER_PANEL"));
   if (!pToolWindow_)
      return false;

   GET_CONTROL(*pToolWindow_, "ID_RADIOBUTTON_MANUAL", wxRadioButton)->Connect(
                     wxEVT_COMMAND_RADIOBUTTON_SELECTED,
                     wxCommandEventHandler(BufferPartEvent::OnRadioButtonManual), NULL,
                     pEventHandler_);

   GET_CONTROL(*pToolWindow_, "ID_RADIOBUTTON_FIELD", wxRadioButton)->Connect(
                     wxEVT_COMMAND_RADIOBUTTON_SELECTED,
                     wxCommandEventHandler(BufferPartEvent::OnRadioButtonField), NULL,
                     pEventHandler_);

   GET_CONTROL(*pToolWindow_, "ID_TEXTCTRL_DISTANCE", wxTextCtrl)->Connect(
                     wxEVT_COMMAND_TEXT_UPDATED,
                     wxCommandEventHandler(BufferPartEvent::OnDistanceTexCtrlChanged), NULL,
                     pEventHandler_);

   wxChoice* ptypechoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_DISTANCE"), wxChoice);
   ptypechoice->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
                        wxCommandEventHandler(BufferPartEvent::OnChoiceSelection),
                        NULL, pEventHandler_);

   wxChoice* punitchoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_UNIT"), wxChoice);
   punitchoice->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
                        wxCommandEventHandler(BufferPartEvent::OnUnitSelection),
                        NULL, pEventHandler_);

   return true;
}

/**
 * Indica si el Part tiene cambios para salvar.
 * @return bool que indica si tuvo exito
 */
bool BufferPart::HasChanged() {
   return modified_;
}

/**
 * Retorna el icono de la herramienta.
 */
void BufferPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(tool_BUFFER_32, ToolBitmap);
}

/** Salva los cambios realizados en el Part. */
bool BufferPart::CommitChanges() {
   distance_ = GetInternalDistance();
   quantity_ = GetInternalQuantity();
   field_ = GetField();
   modified_ = false;
   reloadfield_ = false;
   if (pPal_) {
      pPal_->AddAttribute<double>(ProcessAdaptLayer::BufferDistanceKeyAttr, distance_);
      pPal_->AddAttribute<int>(ProcessAdaptLayer::BufferQuantityKeyAttr, quantity_);
      pPal_->AddAttribute<std::string>(ProcessAdaptLayer::BufferFieldKeyAttr, field_);
      pPal_->AddAttribute<bool>(ProcessAdaptLayer::BufferIsFromFieldKeyAttr, isfromfield_);
      pPal_->AddAttribute<double>(ProcessAdaptLayer::BufferConversionFactorKeyAttr,
                                  conversionfactor_);
      pPal_->AddAttribute<VectorOperation::OperationType>(
            ProcessAdaptLayer::VectorOperationTypeKeyAttr, operation_);
   }
   isfromfield_ = false;
   return true;
}

/**
 * Restaura los valores originales del Part.
 * @return bool que indica si tuvo exito
 */
bool BufferPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void BufferPart::SetInitialValues() {
   // Equivale al valor maximo de latitud porque
   // se utiliza el factor de conversion de la latitud
   distance_ = 0;
   quantity_ = USE_CONTROL(*pToolWindow_, "ID_SPINCTRL_BUFFER", wxSpinCtrl, GetValue(), 0);
   conversionfactor_ = 1;
   isfromfield_ = false;
   field_.clear();
   modified_ = false;
   LoadChoiceFields();
   SetConversionFactor();
}

/** Carga los choice del part */
void BufferPart::LoadChoiceFields() {
   std::vector<DatasourceInterface*> datasources;
   pPal_->GetAttribute<std::vector<DatasourceInterface*> >(
         ProcessAdaptLayer::InputDatasourcesKeyAttr, datasources);

   VectorEditorDriver* pdriver = new VectorEditorDriver(*datasources.begin());
   VectorEditionTable* pTable = new VectorEditionTable(*datasources.begin());
   pTable->SetDriver(pdriver);

   wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_DISTANCE"), wxChoice);

   wxArrayString datatypes;
   for (int i = 0; i < pTable->GetColumns(); i++) {
      Table::ColumnType type = pTable->GetColumnType(i);
      if (type == Table::FLOAT || type == Table::INT)
         datatypes.Add(wxT(pTable->GetColumnName(i)));
   }

   pchoice->Append(datatypes);
   pchoice->Select(0);

   wxChoice* punitchoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_UNIT"), wxChoice);
   wxArrayString dataunits;
   dataunits.Add(wxT(metre));
   dataunits.Add(wxT(kilometre));
   dataunits.Add(wxT(mile));
   punitchoice->Append(dataunits);
   punitchoice->Select(0);
   delete pTable;
}

/**
 * Obtiene la distancia
 * @return double indicado en el text control wx
 */
double BufferPart::GetInternalDistance() {
   wxRadioButton* pmanual = XRCCTRL(*pToolWindow_, wxT("ID_RADIOBUTTON_MANUAL"),
                                    wxRadioButton);
   if (pmanual->GetValue())
      return GetDistanceFromTextCtrl();
   else
      return distance_;
}

/**
 * Obtiene el nombre del campo seleccionado en el choice
 * @return string seleccionado en el choice
 */
std::string BufferPart::GetField() {
   wxRadioButton* pfield = XRCCTRL(*pToolWindow_, wxT("ID_RADIOBUTTON_FIELD"),
                                   wxRadioButton);
   if (pfield->GetValue())
      return GetFieldFromChoice();
   else
      return field_;
}

/**
 * Obtiene la distancia
 * @return double indicado en el text control wx
 */
double BufferPart::GetDistanceFromTextCtrl() {
   wxString value = USE_CONTROL(*pToolWindow_, wxT("ID_TEXTCTRL_DISTANCE"), wxTextCtrl,
                              GetValue(), wxT(message_NOT_APPLICABLE));
   double distance = 0;

   if (value)
      value.ToDouble(&distance);

   return distance;
}

/**
 * Obtiene el nombre del campo seleccionado en el choice
 * @return string seleccionado en el choice
 */
std::string BufferPart::GetFieldFromChoice() {
   reloadfield_ = false;
   std::vector<DatasourceInterface*> datasources;
   pPal_->GetAttribute<std::vector<DatasourceInterface*> >(
         ProcessAdaptLayer::InputDatasourcesKeyAttr, datasources);

   wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_DISTANCE"), wxChoice);

   VectorEditorDriver* pdriver = new VectorEditorDriver(*datasources.begin());
   VectorEditionTable* pTable = new VectorEditionTable(*datasources.begin());
   pTable->SetDriver(pdriver);

   std::string field = pchoice->GetStringSelection().c_str();

   delete pTable;
   return field;
}

/**
 * Obtiene la cantidad de anillos concentricos
 * @return int indicado en el control wx
 */
int BufferPart::GetInternalQuantity() {
   int value = USE_CONTROL(*pToolWindow_, "ID_SPINCTRL_BUFFER", wxSpinCtrl, GetValue(), 0);
   return value;
}

/** Accion de seleccionar el radio button manual */
void BufferPart::OnRadioButtonManual(wxCommandEvent &Event) {
   SetStatus(true);
   isfromfield_ = false;
}

/** Accion de seleccionar el radio button del campo */
void BufferPart::OnRadioButtonField(wxCommandEvent &Event) {
   SetStatus(false);
   isfromfield_ = true;
}

/** Habilita/Deshabilita los controles correspondientes al radio button */
void BufferPart::SetStatus(bool isManual) {
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_TEXTCTRL_DISTANCE"), wxTextCtrl);
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_DISTANCE"), wxChoice);
   if (ptext && pchoice && isManual) {
      ptext->Enable();
      pchoice->Disable();
   } else if (ptext && pchoice && !isManual) {
      ptext->Disable();
      pchoice->Enable();
   }
}

/** Accion de seleccionar una opcion en el Choice de distancias */
void BufferPart::OnChoiceSelection(wxCommandEvent &Event) {
   reloadfield_ = true;
}

/** Accion de seleccionar una opcion en el Choice de unidades */
void BufferPart::OnUnitSelection(wxCommandEvent &Event) {
   SetConversionFactor();
   modified_ = true;
}

/** Accion de ingresar un valor en el TextCtrl de distancia */
void BufferPart::OnDistanceTexCtrlChanged(wxCommandEvent &Event) {
   wxRegEx Expression(("^[0-9|.]*$"), wxRE_ICASE);
   if (!Expression.Matches(Event.GetString())) {
      wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_TEXTCTRL_DISTANCE"), wxTextCtrl);
      wxString aux = Event.GetString();
      ptext->SetValue(aux.RemoveLast());
      SHOW_ERROR(message_FLOAT_INPUT_ERROR);
      return;
   }
   modified_ = true;
}

/** Setea el factor de conversion de la unidad a metros */
void BufferPart::SetConversionFactor() {
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_UNIT"), wxChoice);
   std::string selectedunit = pchoice->GetStringSelection().c_str();
   conversionfactor_ = GetConversionFactorValue(selectedunit);
}

/** Retorna el factor de conversion de la unidad a metros */
double BufferPart::GetConversionFactor() {
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_UNIT"), wxChoice);
   std::string selectedunit = pchoice->GetStringSelection().c_str();
   return GetConversionFactorValue(selectedunit);
}

/** Retorna el valor del factor de conversion de la unidad a metros */
double BufferPart::GetConversionFactorValue(std::string SelectedUnit) {
   double conversionfactor;
   if (SelectedUnit.compare(kilometre) == 0)
      conversionfactor = 1000.0;
   else if (SelectedUnit.compare(mile) == 0)
      conversionfactor = 1609.344;
   else
      conversionfactor = 1;
  return conversionfactor;
}

/**
 * Devuelve si la parte contiene datos validos.
 * Devuelve true : siempre.
 */
bool BufferPart::HasValidData() {
   wxRadioButton* pfield = XRCCTRL(*pToolWindow_, wxT("ID_RADIOBUTTON_FIELD"), wxRadioButton);
   if (GetDistanceFromTextCtrl() > 0 || pfield->GetValue())
      return true;
   else
      return false;
}

} /** namespace suri */
