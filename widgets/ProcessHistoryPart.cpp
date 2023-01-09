/*! \file ProcessHistoryPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
