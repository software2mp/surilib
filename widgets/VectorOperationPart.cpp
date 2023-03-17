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
#include <vector>
// Includes Suri
#include "VectorOperationPart.h"
#include "suri/DatasourceInterface.h"
#include "VectorDatasource.h"
#include "resources.h"

const std::string title = "Operaciones vectoriales geometricas";

namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(VectorOperationPartEvent, VectorOperationPart)
      IMPLEMENT_EVENT_CALLBACK(OnUnitSelection, OnUnitSelection(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnFirstLayerSelection, OnFirstLayerSelection(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnSecondLayerSelection, OnSecondLayerSelection(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Constructor */
VectorOperationPart::VectorOperationPart() : NEW_EVENT_OBJECT(VectorOperationPartEvent) {
   windowTitle_ = title;
}

/** Destructor */
VectorOperationPart::~VectorOperationPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Crea la ventana de la parte y contecta los eventos
 * @return true si pudo cargar la ventana de la herramienta.
 * @return false si fallo la carga de la ventana de la herramienta.
 */
bool VectorOperationPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_VECTOROPERATION_PANEL"));
   if (!pToolWindow_)
      return false;

   wxChoice* pfirstlayerchoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_FIRST_LAYER"), wxChoice);
   pfirstlayerchoice->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
                        wxCommandEventHandler(VectorOperationPartEvent::OnFirstLayerSelection),
                        NULL, pEventHandler_);

   wxChoice* psecondlayerchoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_SECOND_LAYER"), wxChoice);
   psecondlayerchoice->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
                        wxCommandEventHandler(VectorOperationPartEvent::OnSecondLayerSelection),
                        NULL, pEventHandler_);

   wxChoice* punitchoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_OPERATION"), wxChoice);
   punitchoice->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
                        wxCommandEventHandler(VectorOperationPartEvent::OnUnitSelection),
                        NULL, pEventHandler_);

   return true;
}

/**
 * Retorna el icono de la herramienta.
 */
void VectorOperationPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_VECTOR_OPERATIONS, ToolBitmap);
}

/**
 * Indica si el Part tiene cambios para salvar.
 * @return bool que indica si tuvo exito
 */
bool VectorOperationPart::HasChanged() {
   return modified_;
}

/** Salva los cambios realizados en el Part. */
bool VectorOperationPart::CommitChanges() {
   if (firstchoice_ == secondchoice_) {
      SHOW_ERROR(message_SAME_LAYER_ERROR);
      return false;
   }
   if (inputdatasources_[firstchoice_]->GetSpatialReference().compare(
         inputdatasources_[secondchoice_]->GetSpatialReference()) != 0)
      SHOW_WARNING(message_DIFFERENT_SR_WARNING);
   if (pPal_) {
      datasources_.clear();
      datasources_.push_back(inputdatasources_[firstchoice_]);
      datasources_.push_back(inputdatasources_[secondchoice_]);
      pPal_->AddAttribute<VectorOperation::OperationType>(
            ProcessAdaptLayer::VectorOperationTypeKeyAttr, operation_);
      pPal_->AddAttribute< std::vector<DatasourceInterface*> >(ProcessAdaptLayer::
                                                               VectorOperationDatasourcesKeyAttr,
                                                               datasources_);
      modified_ = false;
   }
   return true;
}

/**
 * Restaura los valores originales del Part.
 * @return bool que indica si tuvo exito
 */
bool VectorOperationPart::RollbackChanges() {
   return true;
}

/** Carga los choice del part */
void VectorOperationPart::LoadChoiceFields() {
   pPal_->GetAttribute<std::vector<DatasourceInterface*> >(
         ProcessAdaptLayer::InputDatasourcesKeyAttr, inputdatasources_);

   wxChoice* pfirstlayerchoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_FIRST_LAYER"), wxChoice);
   wxChoice* psecondlayerchoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_SECOND_LAYER"), wxChoice);
   wxArrayString elements;
   std::vector<DatasourceInterface*>::iterator it = inputdatasources_.begin();
   std::vector<VectorDatasource*> selectedFeatures;
   for (; it < inputdatasources_.end(); it++) {
      VectorDatasource* pvector = VectorDatasource::Create(*it);
      if (pvector) {
         elements.Add(wxT(pvector->GetName()));
         pvector = GetSelectedVectorDatasource(pvector->GetElement());
         if (pvector) selectedFeatures.push_back(pvector);
      }
   }
   std::vector<VectorDatasource*>::const_iterator it2 = selectedFeatures.begin();
   for (; it2 != selectedFeatures.end(); ++it2) {
      elements.push_back( wxT( (*it2)->GetName() ) );
      inputdatasources_.push_back(*it2);
   }
   pfirstlayerchoice->Append(elements);
   pfirstlayerchoice->Select(0);
   firstchoice_ = 0;
   psecondlayerchoice->Append(elements);
   psecondlayerchoice->Select(1);
   secondchoice_ = 1;
}

/** Accion de seleccionar una opcion en el Choice de unidades */
void VectorOperationPart::OnUnitSelection(wxCommandEvent &Event) {
   wxChoice* punitchoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_OPERATION"), wxChoice);
   switch (punitchoice->GetCurrentSelection()) {
      case VectorOperation::Union:
         operation_ = VectorOperation::Union;
         break;
      case VectorOperation::Intersection:
         operation_ = VectorOperation::Intersection;
         break;
      case VectorOperation::Trim:
         operation_ = VectorOperation::Trim;
         break;
      default:
         break;
   }
   modified_ = true;
}

/** Accion de seleccionar una opcion en el Choice de primer capa */
void VectorOperationPart::OnFirstLayerSelection(wxCommandEvent &Event) {
   wxChoice* pfirstlayerchoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_FIRST_LAYER"), wxChoice);
   firstchoice_ = pfirstlayerchoice->GetCurrentSelection();
   modified_ = true;
}

/** Accion de seleccionar una opcion en el Choice de segunda capa */
void VectorOperationPart::OnSecondLayerSelection(wxCommandEvent &Event) {
   wxChoice* psecondlayerchoice = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE_SECOND_LAYER"), wxChoice);
   secondchoice_ = psecondlayerchoice->GetCurrentSelection();
   modified_ = true;
}

/** Inicializa el part */
void VectorOperationPart::SetInitialValues() {
   operation_ = VectorOperation::Union;
   modified_ = true;
   LoadChoiceFields();
}

/**
 * Devuelve si la parte contiene datos validos.
 * Devuelve true : siempre.
 */
bool VectorOperationPart::HasValidData() {
   return true;
}

} /** namespace suri */
