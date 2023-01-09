/*! \file ViewerPropertiesPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "suri/ViewerPropertiesPart.h"
#include "suri/DynamicViewerWidget.h"
#include "Viewer2DProperties.h"
#include "Viewer3DProperties.h"
#include "suri/LibraryManager.h"

// Includes Wx
#include "wx/choicebk.h"

// Defines

namespace suri {

// forwards
class DataViewManager;

ViewerPropertiesPart::ViewerPropertiesPart(DynamicViewerWidget* pViewer,
                                           LibraryManager* pLibManager,
                                           DataViewManager* pDataViewManager) :
      p2dProperties_(new Viewer2DProperties(pViewer, pLibManager)),
      p3dProperties_(new Viewer3DProperties(pViewer, pDataViewManager)), pViewer_(pViewer) {
}

ViewerPropertiesPart::~ViewerPropertiesPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool ViewerPropertiesPart::HasChanged() {
   wxChoicebook* pchoicebook = XRCCTRL(*pToolWindow_, wxT("ID_CHOICEBOOK"), wxChoicebook);
   int page = pchoicebook->GetSelection();
   return page == 0 ? p2dProperties_->HasChanged() : p3dProperties_->HasChanged();
   return true;
}

/** Salva los cambios realizados en el Part. */
bool ViewerPropertiesPart::CommitChanges() {
   wxChoicebook* pchoicebook = XRCCTRL(*pToolWindow_, wxT("ID_CHOICEBOOK"), wxChoicebook);
   int page = pchoicebook->GetSelection();
   return (page == 0) ? p2dProperties_->CommitChanges() : p3dProperties_->CommitChanges();
}

/** Restaura los valores originales del Part. */
bool ViewerPropertiesPart::RollbackChanges() {
   return false;
}

/** Inicializa el part */
void ViewerPropertiesPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void ViewerPropertiesPart::Update() {
}

/** Implementacion del metodo de creacion de ventana de Widget */
bool ViewerPropertiesPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
         wxT("ID_VISUALIZATION_PROPERTIES_PART"));
   if (!pToolWindow_)
      return false;
   AddControl(p2dProperties_->GetWidget(), wxT("ID_2D_VISUALIZATION_PROPERTIES_PANEL"));
   AddControl(p3dProperties_->GetWidget(), wxT("ID_3D_VISUALIZATION_PROPERTIES_PANEL"));
   wxChoicebook* pchoicebook = XRCCTRL(*pToolWindow_, wxT("ID_CHOICEBOOK"), wxChoicebook);
   DynamicViewerWidget* dynamicviewer = dynamic_cast<DynamicViewerWidget*>(pViewer_);
   int mode = dynamicviewer->GetVisualizationMode();
   if (mode < 2)
      pchoicebook->SetSelection(0);
   else
      pchoicebook->SetSelection(1);
   return true;
}
} /** namespace suri */
