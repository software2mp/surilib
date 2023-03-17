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
#include <vector>
#include <map>
#include <string>

// Includes suri
#include "suri/ProcessNotebookWidget.h"
#include "suri/ProcessInterface.h"
#include "suri/messages.h"
#include "suri/LibraryManager.h"
#include "suri/RasterProcess.h"
#include "RasterProcessPartProvider.h"
#include "FileExporterProcessPartProvider.h"
#include "BandMathProcessPartProvider.h"
#include "ClassificationProcessPartProvider.h"
#include "FilterProcessPartProvider.h"
#include "IndexProcessPartProvider.h"
#include "ReprojectionProcessPartProvider.h"
#include "StaticsExporterProcessPartProvider.h"
#include "ConfusionMatrixProcessPartProvider.h"
#include "VectorProcessPartProvider.h"
#include "FileVectorExporterProcessPartProvider.h"
#include "VectorOperationProcessPartProvider.h"
#include "SharpeningProcessPartProvider.h"
#include "TasseledCapTransformationPartProvider.h"
#include "PrincipalComponentAnalysisPartProvider.h"
#include "VisualizationExporterProcessPartProvider.h"
#include "MosaicProcessPartProvider.h"
#include "RasterizationProcessPartProvider.h"
#include "HistogramMatchingProcessPartProvider.h"
#include "suri/Configuration.h"

namespace suri {

#define WANTED_WIDTH 600
#define WANTED_HEIGHT 300

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ProcessNotebookWidgetEvent, ProcessNotebookWidget)
      IMPLEMENT_EVENT_CALLBACK(OnUIUpdate, OnUIUpdate(event), wxUpdateUIEvent)
      IMPLEMENT_EVENT_CALLBACK(OnPageChanging, OnPageChanging(event), wxNotebookEvent)
      IMPLEMENT_EVENT_CALLBACK(OnClose, OnClose(event), wxCloseEvent)
      IMPLEMENT_EVENT_CALLBACK(OnButtonCancel,OnButtonCancel(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Ctor. */
ProcessNotebookWidget::ProcessNotebookWidget(ProcessInterface* pProcess,
                                             const std::string& ProcessName,
                                             LibraryManager* pLibraryManager) :
      NotebookWidget(_(ProcessName.c_str()),wxNB_LEFT,Listbook,
                       SUR_BTN_ALL_NO_APPLY,SUR_BTN_ALL_NO_APPLY),
      NEW_EVENT_OBJECT(ProcessNotebookWidgetEvent) {
   // Tamanio preferido de ventana
   wantedWidth_ = WANTED_WIDTH;
   wantedHeight_ = WANTED_HEIGHT;
   pProcess_ = pProcess;
   processName_ = _(ProcessName.c_str());
   widgetCreated_ = false;
   pLibraryManager_ = pLibraryManager;
}

/** Dtor. */
ProcessNotebookWidget::~ProcessNotebookWidget() {
   DELETE_EVENT_OBJECT;
}

/**
 * Evento que habilita las partes dependiendo si hay o no alguna banda
 * seleccionada.
 * @param[in] Event evento UIUpdate
 */
void ProcessNotebookWidget::OnUIUpdate(wxUpdateUIEvent & Event) {
   NotebookWidget::OnUIUpdate(Event);

   // Actualiza los parts
   RasterProcess* pprocess = dynamic_cast<RasterProcess*>(pProcess_);
   if (widgetCreated_ && pprocess)
      pprocess->DataUpdate();

   Event.SetUpdateInterval(250);
}

/**
 * Evento que maneja el cambio entre dos parts de un proceso.
 * @param[in] Event evento BookCtrlBase
 */
void ProcessNotebookWidget::OnPageChanging(wxBookCtrlBaseEvent & Event) {
   Event.Skip(false);
   // ignora el evento con 1 sola pagina presente
   if (!(pBookCtrl_->GetPageCount() > 1)) {
      return;
   }
   int pageselected = Event.GetOldSelection();
   Part *ppart = GetPart(pageselected);
   if (!ppart) {
      REPORT_DEBUG("D: Parte invalida");
      return;
   }
   if (ppart->HasChanged()) {
      if (ppart->HasValidData()) {
         ppart->CommitChanges();
      }
      else {
         ppart->RollbackChanges();
         ppart->GetWidget()->GetWindow()->Layout();
      }
   }
}

/**
 * Evento que maneja el botón Cerrar en un proceso.
 * @param[in] Event CloseEvent
 */
void ProcessNotebookWidget::OnClose(wxCloseEvent& Event){
   VerifyExit(Event);
}

/**
 *Evento que maneja el botón 'Cancelar' en un proceso
 *@param[in] CommandEvent wxCommandEvent
 */
void ProcessNotebookWidget::OnButtonCancel(wxCommandEvent& Event){
   VerifyExit(Event);
}


/**
 * Llama al metodo que agrega las partes. Conecta el evento temporal.
 * @return true: si puede agregar las partes.
 * @return false: si no puede agregar las partes.
 */
bool ProcessNotebookWidget::CreateToolWindow() {
   if (!NotebookWidget::CreateToolWindow()) {
      return false;
   }

   ProcessPartProviderInterface* pprovider = GetPartProvider(
         pProcess_->GetProcessName());
   std::vector<Part*> parts;
   if (!pprovider || !pprovider->GetParts(parts))
      return false;

   bool returnvalue = true;
   std::vector<Part*>::iterator it = parts.begin();
   for (; it != parts.end(); it++)
      returnvalue = returnvalue && InsertPart(*it);

   // Conecto el evento
   if (returnvalue)
      pToolWindow_->Connect(
            wxEVT_UPDATE_UI,
            wxUpdateUIEventHandler(ProcessNotebookWidgetEvent::OnUIUpdate), NULL,
            pEventHandler_);

   widgetCreated_ = true;

   return true;
}

/**
 * Metodo que sirve para definir se se habilita o no el boton ok.
 * @return true : si todas las partes tienen datos validos.
 * @return false : si alguna parte no tiene datos validos.
 */
bool ProcessNotebookWidget::EnableButtonOk() {
   return PartsValidData();
}

/**
 * Aplica todos los cambios en las partes y ejecuta el metodo RunProcess para
 * generar el elemento de salida.
 * @return true si el proceso se ejecuto correctamente
 * @return false : si no pudo aplicar los cambio en las partes o no pudo salvar
 */
bool ProcessNotebookWidget::ApplyChanges() {
   if (!PartContainerWidget::ApplyChanges()) {
      SHOW_WARNING(_(message_PROCESS_EXECUTION_ERROR));
      return false;
   }
   if (!pProcess_->RunProcess()) {
      SHOW_WARNING(_(message_PROCESS_EXECUTION_ERROR));
      return false;
   }
   return true;
}

/** Obtiene el part provider correspondiente al nombre del proceso**/
ProcessPartProviderInterface* ProcessNotebookWidget::GetPartProvider(
      const std::string& ProcessName) const {
   ProcessPartProviderInterface* pprovider = NULL;
   std::vector<CreatorFunc*> creators;
   GetPartProviderCreators(creators);
   std::vector<CreatorFunc*>::iterator it = creators.begin();
   for (; !pprovider && it != creators.end(); ++it) {
      pprovider = (*it)(ProcessName, pProcess_->GetProcessAdaptLayer());
   }
   return pprovider;
}

/** Obtiene los creadores de los partproviders */
void ProcessNotebookWidget::GetPartProviderCreators(
      std::vector<CreatorFunc*>& Creator) const {
   Creator.push_back(RasterProcessPartProvider::Create);
   Creator.push_back(FileExporterProcessPartProvider::Create);
   Creator.push_back(BandMathProcessPartProvider::Create);
   Creator.push_back(ClassificationProcessPartProvider::Create);
   Creator.push_back(FilterProcessPartProvider::Create);
   Creator.push_back(IndexProcessPartProvider::Create);
   Creator.push_back(ReprojectionProcessPartProvider::Create);
   Creator.push_back(StaticsExporterProcessPartProvider::Create);
   Creator.push_back(ConfusionMatrixProcessPartProvider::Create);
   Creator.push_back(VectorProcessPartProvider::Create);
   Creator.push_back(FileVectorExporterProcessPartProvider::Create);
   Creator.push_back(VectorOperationProcessPartProvider::Create);
   Creator.push_back(SharpeningProcessPartProvider::Create);
   Creator.push_back(TasseledCapTransformationPartProvider::Create);
   Creator.push_back(PrincipalComponentAnalysisPartProvider::Create);
   Creator.push_back(VisualizationExporterProcessPartProvider::Create);
   Creator.push_back(MosaicProcessPartProvider::Create);
   Creator.push_back(HistogramMatchingProcessPartProvider::Create);
   Creator.push_back(RasterizationProcessPartProvider::Create);
}

void ProcessNotebookWidget::VerifyExit(wxEvent& Event) {
   Event.Skip(false);
   int res = SHOWQUERY(GetWindow(), _(question_CONFIRM_EXIT_PROCESS),
                       wxYES | wxNO | wxICON_QUESTION);
   if (res == wxID_YES) {
      EndModal (res);
   }
}

}  // namespace
