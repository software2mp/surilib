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

// Includes standard
#include <list>
#include <string>

// Includes Suri
#include "VectorTableStructureEditionPart.h"
#include "TablePart.h"
#include "TableTool.h"
#include "DefaultTable.h"
#include "suri/VectorTableStructureDriver.h"
#include "suri/BufferedDriver.h"
#include "suri/DataTypes.h"
#include "resources.h"
#include "VectorLayer.h"

// Includes Wx
// Includes App
// Define
#define ID_FIELD_CHOICE "ID_FIELD_CHOICE"
#define ID_VECTOR_GRID "ID_VECTOR_GRID"
#define ID_TABLE_CONTAINER "ID_TABLE_CONTAINER"
#define ID_FIELD_CHOICE_SELECT "ID_FIELD_CHOICE_SELECT"

namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ComboSelectionEvent, VectorTableStructureEditionPart)
      IMPLEMENT_EVENT_CALLBACK(OnComboSelectionEventHandler,
      OnComboSelectionEventHandler(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

const std::string VectorTableStructureEditionPart::VECTOR_TABLE_STRUCTURE_EDITION_XRC_NAME =
      wxT("ID_VECTOR_TABLE_STRUCTURE_EDITION_PANEL"); /*! Id del panel */

/**
 * Constructor
 * @param pVectorDatasource fuente de datos
 * @param pLayer capa activa
 */
VectorTableStructureEditionPart::VectorTableStructureEditionPart(
      VectorDatasource* pVectorDatasource, VectorLayer* pLayer) :
      Part(true, false), NEW_EVENT_OBJECT(ComboSelectionEvent),
      pVectorDatasource_(pVectorDatasource), pTable_(NULL), pLayer_(pLayer) {
   windowTitle_ = _(caption_TABLE_STRUCTURE_EDITOR_PART);
}

/** Dtor */
VectorTableStructureEditionPart::~VectorTableStructureEditionPart() {
   DELETE_EVENT_OBJECT;
}

/** Implementacion del metodo de creacion de ventana de Widget */
bool VectorTableStructureEditionPart::CreateToolWindow() {
   if (pToolWindow_)
      return false;
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(
         pParentWindow_, wxT(VECTOR_TABLE_STRUCTURE_EDITION_XRC_NAME));
   // Se debe configurar el table Part para que aparezca en el Part

   pTable_ = new DefaultTable();
   VectorTableStructureDriver* pdriver = new VectorTableStructureDriver(
         pVectorDatasource_);
   BufferedDriver* pbuffer = new BufferedDriver(pdriver);
   pTable_->SetDriver(pbuffer);
   TablePart* ppart = new TablePart(pTable_);
   TableTool* ptool = new TableTool(NULL, ppart);
   ppart->SetTableTool(ptool);
   ppart->SetSelectionNotifier(NULL);
   ptool->SetTablePart(ppart);
   AddControl(ppart, wxT(ID_TABLE_CONTAINER));
   // El choice debe poseer los campos disponibles para hipervinculo
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT(ID_FIELD_CHOICE_SELECT), wxChoice);
   pchoice->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(ComboSelectionEvent::OnComboSelectionEventHandler), NULL,
         pEventHandler_);
   for (int row = 0; row < pTable_->GetRows(); ++row) {
      std::string fieldtype;
      pTable_->GetCellValue(VectorTableStructureDriver::FieldTypeColumn, row,
                            fieldtype);
      // Si no es un tipo de dato entero se encuentra disponible para hotlink
      if (IsCharType(fieldtype)) {
         std::string fieldname;
         pTable_->GetCellValue(VectorTableStructureDriver::FieldNameColumn, row,
                               fieldname);
         pchoice->Append(fieldname.c_str());
      }
   }
   VectorElement* pvecelement = dynamic_cast<VectorElement*>(pVectorDatasource_->GetElement());
   if (pvecelement) {
      std::list<std::string> hotlinklist;
      pvecelement->GetHotlinkNodesList(hotlinklist);
      if (hotlinklist.size() > 0) {
         pchoice->SetStringSelection(hotlinklist.front().c_str());
      }
   }
   return true;
}

/** Indica si el Part tiene cambios para salvar. */
bool VectorTableStructureEditionPart::HasChanged() {
   return modified_;
}

/** Salva los cambios realizados en el Part. */
bool VectorTableStructureEditionPart::CommitChanges() {
   // En caso de definir un campo como hipervinculo esa informacion debe
   // guardarse en el xml como metadato
   if (HasChanged() && HasValidData()) {
      wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT(ID_FIELD_CHOICE_SELECT), wxChoice);
      std::string name = pchoice->GetStringSelection().c_str();
      VectorElement* pvecelement = dynamic_cast<VectorElement*>(pVectorDatasource_->GetElement());
      pvecelement->SetHotLinkFieldName(name);
      VectorElement* playerelement = dynamic_cast<VectorElement*>(pLayer_->GetElement());
      playerelement->SetHotLinkFieldName(name);
   }
   return true;
}

/** Restaura los valores originales del Part. */
bool VectorTableStructureEditionPart::RollbackChanges() {
   return true;
}

/** Inicializa el Part */
void VectorTableStructureEditionPart::SetInitialValues() {
}

/** Actualiza el estado del Part */
void VectorTableStructureEditionPart::Update() {
}

/** Metodo que captura el evento de cambio del choice **/
void VectorTableStructureEditionPart::OnComboSelectionEventHandler(
      wxCommandEvent &Event) {
   modified_ = true;
}

/**
 * Devuelve el icono de la parte
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho de icono
 * @param[in] Y alto de icono
 */
void VectorTableStructureEditionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
      GET_BITMAP_RESOURCE(icon_VECTOR_TABLE_EDITOR, ToolBitmap);
}
} /** namespace suri **/
