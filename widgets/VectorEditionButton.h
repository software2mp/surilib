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

#ifndef VECTOREDITIONBUTTON_H_
#define VECTOREDITIONBUTTON_H_

#include "VectorDatasourceSelectionSource.h"
#include "VectorEditorButton.h"
#include "suri/StateChangeNotification.h"
#include "suri/VectorEditionTask.h"

namespace suri {

// forwards
class VectorEditionTaskEvent;
class GeometryEditionEvent;

class VectorEditionButton : public VectorEditorButton {
public:
   /** Ctor **/
   VectorEditionButton(wxWindow *pToolbar, ViewerWidget *pViewer, const wxString &Bitmap,
         GuiGeometryCreator::GeometryType GeoType, wxString ButtonTooltip,
         DataViewManager* pDataViewManager_, int Flags);

   /** Destructor */
   virtual ~VectorEditionButton();

   /** Inicio edicion geometria */
   bool StartFeatureEdition(FeatureIdType FeatureId);
   /** Inicio de la edicion */
   void Start();
   /** Finaliza la edicion */
   void End();
   /** Indica si se encuentra activo el editor **/
   virtual bool IsActive() const;
   /**  Metodo que se llama cuando finaliza la edicion vectorial.*/
   virtual void OnVectorEditionFinished() {
      if (IsActive())
         DoSetValue(false);
      if (Configuration::GetParameter("app_short_name", "") != wxT("SoPI")) {
         End();
         pDataViewManager_->GetDatasourceManager()->UnblockAllDatasource();
      }
   }

   /** Evento que habilita el boton segun el retorno de IsEnabled */
   /** Hoy es viernes 13 asi que para no ser menos seguimos agregando automagia negra */
   virtual void OnUIUpdate(wxUpdateUIEvent &Event) {
      VectorEditorButton::OnUIUpdate(Event);
      if (Configuration::GetParameter("app_short_name", "") != wxT("SoPI")) {
         if (taskended_ && pTask_) {
            OnVectorEditionFinished();
            StateChangeNotification notif("Deactivate", "null");
            Notify(&notif);
         }
      }
   }

private:
   /** Setea el estado del boton. */
   virtual bool DoSetValue(bool State);
   ViewportManagerInterface *pViewportManager_;
   ViewcontextManagerInterface *pViewcontextManager_;
   FeatureSelection* pFeatureSelection_;
   GeometryEditionEvent* pEventHandler_;
   VectorEditionTaskEvent* pPaintEventHandler_;
   VectorDatasourceSelectionSource* pSelectionSource_;
   DataViewManager* pDataViewManager_;
   VectorEditionTask* pTask_;
   Table* pTable_;
   VectorDatasource* pDatasource_;
   // Flag para acusar cuando en un start hubo algun problema,
   // volvemos a poner el boton en el estado inicial.
   bool error_;  // Flag para acusar cuando en un start hubo algun problema,
                 // volvemos a poner el boton en el estado inicial.

   friend class GeometryEditionEvent;  // esto es para no ser menos que
                                       // la implementacion para la creacion.
   bool taskended_;
};

} /** namespace suri */

#endif /* VECTOREDITIONBUTTON_H_ */
