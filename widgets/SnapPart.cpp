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

// Includes Suri
#include "SnapPart.h"
#include "suri/SnapTool.h"
#include "VertexSnapStrategy.h"
#include "SegmentSnapStrategy.h"
#include "VertexSegmentSnapStrategy.h"

// Includes Wx
#include "wx/choice.h"
#include "wx/spinctrl.h"

namespace suri {
namespace ui {

// Defines
#define VERTEX "Vertice"
#define SEGMENT "Segmento"
#define VERTEX_SEGMENT "Vertice y segmento"
#define OK_BUTTON "ID_OK_BUTTON"
#define CANCEL_BUTTON "ID_CANCEL_BUTTON"
#define MODE_CHOICE "ID_MODE_CHOICE"
#define UNIT_CHOICE "ID_UNIT_CHOICE"
#define TOLERANCE_SPINCTRL "ID_TOLERANCE_SPINCTRL"
#define MAP_UNITS "Unidades de mapa"
#define PIXELS "Pixeles"
#define SNAP_PANEL "ID_SNAP_PANEL"

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(SnapPartEvent, SnapPart)
   IMPLEMENT_EVENT_CALLBACK(OnOkButton, OnOkButton(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnCancelButton, OnCancelButton(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** constructor */
SnapPart::SnapPart() : pSnapTool_(NULL), NEW_EVENT_OBJECT(SnapPartEvent) {
   windowTitle_ = tooltip_SNAP_PROPERTIES;
}

/** Destructor */
SnapPart::~SnapPart() {
   DELETE_EVENT_OBJECT;
}

/** Implementacion del metodo de creacion de ventana de Widget */
bool SnapPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_, wxT(SNAP_PANEL));

   if (!pToolWindow_)
      return false;

   InitializeModeValues();
   InitializeUnitValues();
   ConnectEvents();

   return true;
}

/**
 * Inicializa el choice correspondiente a los modos
 * @param[in] Event evento
 */
void SnapPart::InitializeModeValues() {
   wxArrayString modes;
   modes.Add(wxT(VERTEX));
   modes.Add(wxT(SEGMENT));
   modes.Add(wxT(VERTEX_SEGMENT));
   GET_CONTROL(*pToolWindow_, wxT(MODE_CHOICE), wxChoice)->Append(modes);
   GET_CONTROL(*pToolWindow_, wxT(MODE_CHOICE), wxChoice)->SetSelection(0);
   modes.Clear();
}

/**
 * Inicializa el choice correspondiente a las unidades
 * @param[in] Event evento
 */
void SnapPart::InitializeUnitValues() {
   wxArrayString units;
   units.Add(wxT(MAP_UNITS));
   units.Add(wxT(PIXELS));
   GET_CONTROL(*pToolWindow_, wxT(UNIT_CHOICE), wxChoice)->Append(units);
   GET_CONTROL(*pToolWindow_, wxT(UNIT_CHOICE), wxChoice)->SetSelection(0);
   units.Clear();
}

/** Conecta los eventos a los controles */
void SnapPart::ConnectEvents() {
   GET_CONTROL(*pToolWindow_, OK_BUTTON, wxButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(SnapPartEvent::OnOkButton), NULL, pEventHandler_);

   GET_CONTROL(*pToolWindow_, CANCEL_BUTTON, wxButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(SnapPartEvent::OnCancelButton), NULL, pEventHandler_);
}

/**
 * Evento click sobre boton aceptar
 * @param[in] Event evento
 */
void SnapPart::OnOkButton(wxCommandEvent &Event) {
   if (!pSnapTool_)
      EndModal(wxID_OK);
   SetMode();
   SetUnit();
   SetTolerance();
   if (pSnapTool_->CreateBufferGeometries())
      EndModal(wxID_OK);
}

/**
 * Evento click sobre boton cancelar
 * @param[in] Event evento
 */
void SnapPart::OnCancelButton(wxCommandEvent &Event) {
   EndModal(wxID_CANCEL);
}

/** Setea el modo en la herramienta de snapping */
void SnapPart::SetMode() {
   wxChoice* pmode = XRCCTRL(*pToolWindow_, wxT(MODE_CHOICE), wxChoice);
   if (pmode->GetString(pmode->GetSelection()).compare(VERTEX) == 0) {
      pSnapTool_->SetMode(new suri::VertexSnapStrategy());
   } else if (pmode->GetString(pmode->GetSelection()).compare(SEGMENT) == 0) {
      pSnapTool_->SetMode(new suri::SegmentSnapStrategy());
   } else {
      pSnapTool_->SetMode(new suri::VertexSegmentSnapStrategy());
   }
}

/** Setea la unidad en la herramienta de snapping */
void SnapPart::SetUnit() {
   wxChoice* punit = XRCCTRL(*pToolWindow_, wxT(UNIT_CHOICE), wxChoice);
   if (punit->GetString(punit->GetSelection()).compare(VERTEX) == 0) {
      pSnapTool_->SetUnit(SnapTool::Map);
   } else {
      pSnapTool_->SetUnit(SnapTool::Pixel);
   }
}

/** Setea la tolerancia en la herramienta de snapping */
void SnapPart::SetTolerance() {
   int tolerance = USE_CONTROL(*pToolWindow_, TOLERANCE_SPINCTRL, wxSpinCtrl, GetValue(), 0);
   pSnapTool_->SetTolerance(tolerance);
}

/** Setea un puntero a la herramienta de snapping existente */
void SnapPart::SetTool(SnapTool* Tool) {
   pSnapTool_ = Tool;
}

} /** namespace ui */
} /** namespace suri */
