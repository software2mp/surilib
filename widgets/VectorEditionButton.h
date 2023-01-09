/*! \file VectorEditionButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
