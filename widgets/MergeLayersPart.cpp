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
#include "MergeLayersPart.h"
#include "suri/DatasourceInterface.h"
#include "VectorDatasource.h"
#include "suri/VectorEditor.h"
#include "suri/messages.h"
#include "suri/AuxiliaryFunctions.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(MergeLayersPartEvent, MergeLayersPart)
      IMPLEMENT_EVENT_CALLBACK(OnFieldSortUpClick, OnFieldSortUpClick(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnFieldSortDownClick, OnFieldSortDownClick(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnAddFieldClick, OnAddFieldClick(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnRevokeFieldClick, OnRevokeFieldClick(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

MergeLayersPart::MergeLayersPart(
      std::vector<DatasourceInterface*>& DatasourceVector) :
      datasourceVector_(DatasourceVector), NEW_EVENT_OBJECT(MergeLayersPartEvent) {
   windowTitle_ = "Seleccion de Campos";
   operation_ = VectorOperation::MergeLayers;
}

MergeLayersPart::~MergeLayersPart() {
   DELETE_EVENT_OBJECT;
}

/** Indica si el Part tiene cambios para salvar. */
bool MergeLayersPart::HasChanged() {
   return modified_;
}

/** Salva los cambios realizados en el Part. */
bool MergeLayersPart::CommitChanges() {
   modified_ = false;
   if (pPal_) {
      SerializableString ss(fileName_);
      pPal_->AddSerializableAttribute<SerializableString>(
            ProcessAdaptLayer::OutputFileNameKeyAttr, ss);
      pPal_->AddAttribute<VectorOperation::OperationType>(
            ProcessAdaptLayer::VectorOperationTypeKeyAttr, operation_);
      std::string mergefields = ConvertMapToString();
      pPal_->AddAttribute<std::string>(ProcessAdaptLayer::MergeFieldsMapKey,
                                       mergefields);
      hasvaliddata_ = true;
   }
   return true;
}

/** Restaura los valores originales del Part. */
bool MergeLayersPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void MergeLayersPart::SetInitialValues() {
   fileName_.clear();
   hasvaliddata_ = true;
   modified_ = true;
}

/**
 * Retorna el icono de la herramienta.
 */
void MergeLayersPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_VECTOR_OPERATIONS, ToolBitmap);
}

/** Implementacion del metodo de creacion de ventana de Widget */
bool MergeLayersPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_, "ID_FIELDLAYER_SELECTION_PANEL");
   wxTreeCtrl* ptree = XRCCTRL(*pToolWindow_, wxT("ID_FIELD_TREECTRL"), wxTreeCtrl);
   wxListCtrl* plist = XRCCTRL(*pToolWindow_, wxT("ID_FIELD_SELECTED"), wxListCtrl);

   // Conecto los controles con sus eventos.
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_ADD_FIELD_BUTTON", wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(MergeLayersPartEvent::OnAddFieldClick), NULL,
         pEventHandler_);

   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_REVOKE_FIELD_BUTTON", wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(MergeLayersPartEvent::OnRevokeFieldClick), NULL,
         pEventHandler_);

   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_FIELD_SORT_UP_BUTTON", wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(MergeLayersPartEvent::OnFieldSortUpClick), NULL,
         pEventHandler_);

   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_FIELD_SORT_DOWN_BUTTON", wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(MergeLayersPartEvent::OnFieldSortDownClick), NULL,
         pEventHandler_);

   plist->DeleteAllColumns();
   plist->InsertColumn(NUMBER_COL, "#");
   plist->SetColumnWidth(NUMBER_COL, 0);
   plist->InsertColumn(COLUMN_COL, "Campo");
   plist->InsertColumn(ORIGIN_COL, "Origen");
   if (ptree != NULL) {
      wxBitmap itembitmap;
      wxImageList *pimageinlist = new wxImageList(16, 16);

      GET_BITMAP_RESOURCE(wxT(icon_DRIVE_16), itembitmap);
      pimageinlist->Add(itembitmap);

      GET_BITMAP_RESOURCE(wxT(icon_VECTOR_ELEMENT_16), itembitmap);
      pimageinlist->Add(itembitmap);

      ptree->AssignImageList(pimageinlist);
      wxTreeItemId rootnodeix = ptree->AddRoot(wxT("Capas seleccionadas"), 0);
      std::vector<DatasourceInterface*>::iterator it = datasourceVector_.begin();
      for (; it != datasourceVector_.end(); ++it) {
         VectorEditor editor;
         editor.OpenVector((*it)->GetUrl());
         editor.OpenLayer(0);
         wxTreeItemId layern = ptree->AppendItem(rootnodeix, (*it)->GetName(), 1);
         for (int i = 0; i < editor.GetFieldCount(); ++i) {
            ptree->AppendItem(layern, editor.GetFieldName(i));
         }
      }
      ptree->ExpandAll();
   }
   return true;
}

/** Mueve hacia arriba un campo seleccionado en la capa destino */
void MergeLayersPart::OnFieldSortUpClick(wxCommandEvent &Event) {
}

/** Mueve hacia abajo un campo seleccionado en la capa destino */
void MergeLayersPart::OnFieldSortDownClick(wxCommandEvent &Event) {
}

/** Agrega un campo de la capa origen a la capa destino */
void MergeLayersPart::OnAddFieldClick(wxCommandEvent &Event) {
   wxTreeCtrl* ptree = XRCCTRL(*pToolWindow_, wxT("ID_FIELD_TREECTRL"), wxTreeCtrl);
   wxTreeItemId item = ptree->GetSelection();
   if (ptree->ItemHasChildren(item)) {
      SHOW_ERROR(message_INVALID_COLUMN_FIELD);
      return;
   }
   wxString column = ptree->GetItemText(item);
   wxString origin = ptree->GetItemText(ptree->GetItemParent(item));
   wxString dsid;
   std::vector<DatasourceInterface*>::iterator it = datasourceVector_.begin();
   for (; it != datasourceVector_.end(); ++it) {
      if ((*it)->GetName().compare(origin.c_str()) == 0)
         dsid = (*it)->GetId().c_str();
   }
   if (!AppendRowToList(dsid, column, origin))
      SHOW_ERROR(message_DUPLICATED_ITEM);
}

/** Agrega una fila a la lista
 * @param[in] Dsid id de la fuente de datos
 * @param[in] Column nombre de la columna
 * @param[in] Origin nombre de la fuente de datos
 * @return true si pudo agregar el campo, false en caso contrario
 */
bool MergeLayersPart::AppendRowToList(wxString Dsid, wxString Column, wxString Origin) {
   wxListCtrl* plist = XRCCTRL(*pToolWindow_, wxT("ID_FIELD_SELECTED"), wxListCtrl);
   if (!CanAppendRow(Dsid, Column))
      return false;
   modified_ = true;
   // Agrego el item a la lista
   wxString hiddencol = Dsid + "," + Column;
   long itemindex = plist->InsertItem(NUMBER_COL, hiddencol);
   plist->SetItem(itemindex, COLUMN_COL, Column);
   plist->SetItem(itemindex, ORIGIN_COL, Origin);
   // Agrego el item al mapa
   fieldsmultipmap_.insert(
         std::make_pair<std::string, std::string>(Dsid.c_str(), Column.c_str()));
   return true;
}

/**
 * Verifica que no haya registros duplicados
 * @param[in] Dsid id de la fuente de datos
 * @param[in] Column nombre de la columna
 * @return true si no hay duplicados, false en caso contrario
 */
bool MergeLayersPart::CanAppendRow(wxString Dsid, wxString Column) {
   bool success = true;
   wxListCtrl* plist = XRCCTRL(*pToolWindow_, wxT("ID_FIELD_SELECTED"), wxListCtrl);
   long item = -1;
   for (;;) {
      item = plist->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);
      if (item == -1 || !success)
         break;
      std::vector<std::string> CompleteItem = tokenizer(plist->GetItemText(item).c_str(), ",");
      if (CompleteItem[0].compare(Dsid.c_str()) == 0
            && CompleteItem[1].compare(Column.c_str()) == 0)
         success = false;
   }
   return success;
}

/** Quita un campo de la capa destino */
void MergeLayersPart::OnRevokeFieldClick(wxCommandEvent &Event) {
   wxListCtrl* plist = XRCCTRL(*pToolWindow_, wxT("ID_FIELD_SELECTED"), wxListCtrl);
   wxArrayLong rownumberstodelete;
   // Agrego los items seleccionados al array
   long item = -1;
   for (;;) {
      item = plist->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if (item == -1)
         break;
      EraseFieldFromMap(tokenizer(plist->GetItemText(item).c_str(), ","));
      rownumberstodelete.Add(item);
   }
   // Recorro el array y borro los items
   for (long n = (rownumberstodelete.GetCount() - 1); 0 <= n; n--)
      plist->DeleteItem(rownumberstodelete[n]);
   // Limpio el array para evitar memory leaks
   rownumberstodelete.Clear();
}

/**
 * Arma un string con las fuentes de datos y los campos del mapa
 * @return string con los campos seleccionados
 */
std::string MergeLayersPart::ConvertMapToString() {
   std::string mergefields;
   for (std::multimap<std::string, std::string>::iterator it = fieldsmultipmap_.begin();
         it != fieldsmultipmap_.end(); ++it) {
      mergefields += (*it).first + "=" + (*it).second + ',';
   }
   // Borro la ultima coma del string
   mergefields = mergefields.substr(0, mergefields.size() - 1);
   return mergefields;
}

/**
 * Devuelve si la parte contiene datos validos.
 * Devuelve true : siempre.
 */
bool MergeLayersPart::HasValidData() {
   return hasvaliddata_;
}

/**
 * Borra el item seleccionado del mapa
 * @param[in] Contiene el id de la fuente de datos seleccionada y el campo
 */
void MergeLayersPart::EraseFieldFromMap(std::vector<std::string> CompleteItem) {
   std::pair<std::multimap<std::string, std::string>::iterator,
         std::multimap<std::string, std::string>::iterator> ret;

   ret = fieldsmultipmap_.equal_range(CompleteItem[0]);

   // Recorro los items para la fuente de datos y borro el campo seleccionado
   for (std::multimap<std::string, std::string>::iterator mit = ret.first;
         mit != ret.second; ++mit) {
      if (mit->second == CompleteItem[1]) {
         fieldsmultipmap_.erase(mit);
         break;
      }
   }
}
} /** namespace suri */
