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

#include <string>

#include "suri/ProcessHistoryPart.h"
#include "resources.h"
#include "suri/DatasourceInterface.h"
#include "wx/listctrl.h"
#include "suri/XmlFunctions.h"

#define KEY_COLUMN 0
#define VALUE_COLUMN 1

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(ProcessHistoryPartEventHandler, ProcessHistoryPart)
   IMPLEMENT_EVENT_CALLBACK(OnExportButtonClicked, OnExportButtonClicked(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

/** ctor **/
ProcessHistoryPart::ProcessHistoryPart(DatasourceInterface* pDatasource) :
                  pDatasource_(pDatasource),
                  pEventHandler_(new ProcessHistoryPartEventHandler(this)) {
   windowTitle_ = _(caption_PROCESS_HISTORY);
}

/** dtor **/
ProcessHistoryPart:: ~ProcessHistoryPart() {
   delete pEventHandler_;
}

/**
 * Indica si el Part tiene cambios para salvar.
 */
bool ProcessHistoryPart::HasChanged() {
   return false;
}

/**
 * Salva los cambios realizados en el Part.
 */
bool ProcessHistoryPart::CommitChanges() {
   return true;
}

/**
 * Restaura los valores originales del Part.
 */
bool ProcessHistoryPart::RollbackChanges() {
   return false;
}

/**
 * Inicializa el part.
 */
void ProcessHistoryPart::SetInitialValues() {
   wxListCtrl* plist = XRCCTRL(*pToolWindow_, wxT("ID_PROCESS_HISTORY_LIST"), wxListView);
   // Columna clave
   plist->InsertColumn(KEY_COLUMN, wxEmptyString, wxLIST_FORMAT_LEFT);

   // Columna valor
   plist->InsertColumn(VALUE_COLUMN, wxEmptyString, wxLIST_FORMAT_LEFT, 400);

   wxXmlNode* phistnode = pDatasource_->GetElement()->GetNode(PROCESS_HISTORY_NODE);
   wxXmlNode* pchild = phistnode ? phistnode->GetChildren() : NULL;
   int ix = 0;
   while (pchild) {
      plist->InsertItem(ix, wxEmptyString, 0);
      plist->SetItem(ix, KEY_COLUMN, _("PROCESAMIENTO"));
      plist->SetItemBackgroundColour(ix, *wxLIGHT_GREY);
      wxXmlNode* pparam = pchild->GetChildren();
      while (pparam) {
         ++ix;
         plist->InsertItem(ix, wxEmptyString, 0);
         plist->SetItem(ix, KEY_COLUMN, pparam->GetName());
         // TODO(Gabriel - TCK #7139): Pensar como expreso los atributos complejos.
         plist->SetItem(ix, VALUE_COLUMN, pparam->GetNodeContent());
         pparam = pparam->GetNext();
      }
      pchild = pchild->GetNext();
      ++ix;
   }
}

/**
 * Actualiza el estado de la parte.
 */
void ProcessHistoryPart::Update() {
}

/**
 * Crea la ventana de la parte.
 */
bool ProcessHistoryPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_, wxT("ID_PROCESS_HISTORY_PANEL"));
   GET_CONTROL(*pToolWindow_,
         wxT("ID_EXPORT_HISTORY_BTN"), wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(ProcessHistoryPartEventHandler::OnExportButtonClicked), NULL,
         pEventHandler_);
   return true;
}

/**
 * Retorna el icono de la herramienta.
 */
void ProcessHistoryPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_RASTER_METADATA, ToolBitmap);
}

/** Metodo que es llamado cuando se presiona el boton de exportar historial **/
void ProcessHistoryPart::OnExportButtonClicked(wxCommandEvent& Event) {
   std::string filename;
   wxFileDialog dlg(NULL, _(caption_PROCESS_HISTORY), wxT(""), wxT(""),
                    _("Archivos XML (*.xml)|*.xml"),
                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
   if (dlg.ShowModal() != wxID_OK) return;
   filename = dlg.GetPath();

   wxXmlNode* phistnode = pDatasource_->GetElement()->GetNode(PROCESS_HISTORY_NODE);
   wxXmlDocument doc;
   doc.SetRoot(new wxXmlNode(*phistnode));
   doc.Save(filename);
}

}  /** namespace suri **/
