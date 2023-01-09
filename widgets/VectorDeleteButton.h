/*! \file VectorDeleteButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORDELETEBUTTON_H_
#define VECTORDELETEBUTTON_H_

// Includes Estandar
// Includes Suri
#include "GeometryDeleteEvent.h"
#include "VectorDatasourceSelectionSource.h"
#include "VectorEditorButton.h"

// Includes Wx
// Defines
// forwards
class VectorElementEditor;

namespace suri {

class ViewerWidget;
class VectorEditionTaskEvent;

/**
 * Clase que se encarga de borrar una geometria
 */

class VectorDeleteButton : public VectorEditorButton {
public:
   /** Constructor */
   VectorDeleteButton(wxWindow *pToolbar, ViewerWidget *pViewer, const wxString &Bitmap,
                      GuiGeometryCreator::GeometryType GeoType, wxString ButtonTooltip,
                      DataViewManager* pDataViewManager_, int Flags);
   /** Destructor */
   virtual ~VectorDeleteButton();

   /** Inicio de eliminacion  de geometria */
   void Start();
   /** Finalizacion de eliminacion de geometria */
   void End();
   /** Indica si se encuentra activo el editor **/
   virtual bool IsActive() const {
      return pDeleteEventHandler_ != NULL || pPaintEventHandler_ != NULL;
   }
   /**  Metodo que se llama cuando finaliza la edicion vectorial.*/
   virtual void OnVectorEditionFinished() {
      if (IsActive())
         DoSetValue(false);
   }

private:
   /** Setea el estado del boton. */
   virtual bool DoSetValue(bool State);
   ViewportManagerInterface *pViewportManager_;
   ViewcontextManagerInterface *pViewcontextManager_;
   FeatureSelection* pFeatureSelection_;
   GeometryDeleteEvent* pDeleteEventHandler_;
   VectorEditionTaskEvent* pPaintEventHandler_;
   VectorDatasourceSelectionSource* pSelectionSource_;
   DataViewManager* pDataViewManager_;
   VectorEditionTask* pTask_;
   Table* pTable_;
};
} // suri namespace



#endif /* VECTORDELETEBUTTON_H_ */

