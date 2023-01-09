/*! \file VectorSelectionButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORSELECTIONBUTTON_H_
#define VECTORSELECTIONBUTTON_H_

#include "VectorEditorButton.h"
#include "VectorElementEditor.h"

// Includes Estandar
// Includes Suri
#include "GeometrySelectionEvent.h"
#include "VectorDatasourceSelectionSource.h"
#include "VectorEditorButtonTool.h"

// Includes Wx
// Defines
// forwards
class VectorElementEditor;

/** namespace suri */
namespace suri {
// Forwards
class ViewerWidget;
class VectorElementEditor;
class DataViewManager;

/**
 * Clase que representa el boton para la seleccion de una geometria
 * */
class VectorSelectionButton : public VectorEditorButton {
public:
   /** ctor **/
   VectorSelectionButton(wxWindow *pToolbar, ViewerWidget *pViewer,
                         const wxString &Bitmap, wxString ButtonTooltip,
                         GuiGeometryCreator::GeometryType GeoType,
                         DataViewManager* pDataViewManager, int Flags);
   /** dtor **/
   virtual ~VectorSelectionButton();
   /** */
   void Start();
   /** */
   void End();
   /** Indica si se encuentra activo el editor **/
   virtual bool IsActive() const {
      return pEventHandler_ != NULL;
   }

private:
   /** Setea el estado del boton. */
   virtual bool DoSetValue(bool State);
   ViewportManagerInterface *pViewportManager_;
   ViewcontextManagerInterface *pViewcontextManager_;
   FeatureSelection* pFeatureSelection_;
   GeometrySelectionEvent* pEventHandler_;
   VectorDatasourceSelectionSource* pSelectionSource_;
   DataViewManager* pDataViewManager_;
   VectorEditionTask* pTask_;
};
}
#endif /* VECTORSELECTIONBUTTON_H_ */
