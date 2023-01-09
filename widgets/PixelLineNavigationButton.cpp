/*! \file PixelLineNavigationButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/PixelLineNavigationButton.h"

// Includes suri
#include "suri/ViewerWidget.h"
#include "GoButton.h"
#include "suri/messages.h"
#include "resources.h"
#include "suri/PixelLineInputWidget.h"
#include "suri/CoordinatesTransformInterface.h"
#include "suri/RasterElement.h"

// Defines
/** Los grupos estan definidos en Context */
#define RASTERELEMENT_GROUP_POSITION 6

/** namespace suri */
namespace suri {
/**
 * Constructor
 * Crea y registra los viewer a la lista de elementos.
 * @param[in] pToolbar toolbar donde se inserta el boton
 * @param[in] pViewer viewer asociado al contexto
 */
PixelLineNavigationButton::PixelLineNavigationButton(wxWindow *pToolbar,
                                                     ViewerWidget* pViewer) :
      ToggleButton(pToolbar, wxT(tool_PIXEL_LINE_NAVIGATION_WIDGET),
                   wxT(tooltip_PIXEL_LINE_NAVIGATION_WIDGET), BUTTON_NULL),
      pViewer_(pViewer), pGoButton_(NULL), pInputWidget_(NULL), pListViewer_(NULL),
      pRasterElement_(NULL) {
   // Verifico que existan todos los elementos para la operacion
   if (pViewer_ && pViewer_->GetList()) {
      // Creo y registro los viewers
      pListViewer_ = new ListView<PixelLineNavigationButton>(
            this, &PixelLineNavigationButton::ListUpdate);
      pViewer_->GetList()->RegisterViewer(pListViewer_);
   } else {
      SetEnabled(false);
   }
}

/**
 * Destructor.
 */
PixelLineNavigationButton::~PixelLineNavigationButton() {
   delete pListViewer_;
}

/**
 * Setea el estado del boton. Ingresa/Saca boton de NavigationWidget.
 * Agrega a NavigatorWidget un PixelLineInputWidget
 * @param[in] State estado en el que va a estar el boton
 * @return true siempre
 */
bool PixelLineNavigationButton::DoSetValue(bool State) {
   // Verifico que existan todos los elementos para la operacion
   if (!pViewer_ || !pViewer_->GetNavigator()
         || !pViewer_->GetNavigator()->GetWidget()) {
      suri::ToggleButton::DoSetValue(false);
      return true;
   } else {
      suri::ToggleButton::DoSetValue(State);
   }

   // Evita que la pantalla titile si se llama a SetValue
   // varias veces con el mismo valor.
   if (State && pGoButton_) {
      return true;
   }

   // Elimina el boton anterior
   if (pGoButton_) {
      pViewer_->GetNavigator()->GetWidget()->RemoveButtonFromTool(pGoButton_);
      // Elimino referencia a pPliw porque ya no se usara. NavigationWidget lo elimina
      pInputWidget_ = NULL;
      // Elimino la ventana del boton en forma segura.
      pGoButton_->GetWindow()->Destroy();
   }

   if (State) {
      // Cargo herramienta para ingresar lat-long en NavigationWidget
      pInputWidget_ = NULL;
#ifdef __UNUSED_CODE__
      pInputWidget_ = new suri::PixelLineInputWidget(pViewer_->GetWorld());
      pRasterElement_ = GetRasterElement();
      pInputWidget_->SetCurrentRasterElement(pRasterElement_);
      pViewer_->GetNavigator()->GetWidget()->SetInputWidget(pInputWidget_);
#endif
      // Navigator va a utilizar coordenadas LatLong
      if (pRasterElement_) {
         pViewer_->GetNavigator()->SetSpatialReference(
               std::string(pRasterElement_->GetSpatialReference()));
      }

      // Agrega boton a NavigatorWidget
      pGoButton_ = new GoButton(pViewer_->GetNavigator()->GetWidget()->GetToolWindow(),
                                pViewer_);
      pViewer_->GetNavigator()->GetWidget()->AddButtonTool(pGoButton_,
                                                           ToolGroupManager::First);
   } else if (pGoButton_) {
      pGoButton_ = NULL;
      if (pViewer_->GetWorld()) {
         // Actualiza mundo para no mostrar la cruz
         pViewer_->GetWorld()->SetChanged();
         pViewer_->GetWorld()->SendViewerUpdate();
      }
   }
   return true;
}

/**
 * Obtiene el elemento raster activo de la lista.
 * @return pelement elemento raster activo de la lista.
 * @return NULL si no puede obtener la lista, o no hay elemento raster activo.
 * \ todo en caso de permitir mas de un elemento raster activo al mismo tiempo
 *        deberia devolver el elemento raster seleccionado.
 */
RasterElement* PixelLineNavigationButton::GetRasterElement() {
   // Verifico que existan todos los elementos para la operacion
   if (!pViewer_ || !pViewer_->GetList()) {
      return NULL;
   }
   // Obtiene el elemento raster activo, posicion 0 porque se supone que solo hay
   // un elemento raster activo
   return dynamic_cast<RasterElement*>(pViewer_->GetList()->GetActiveElement(
         0, RASTERELEMENT_GROUP_POSITION));
}

/**
 * Metodo de actualizacion de la lista
 * Verifica si cambio la imagen activa. Si cambio actualiza el elemento y la
 * referencia espacial en widget de ingreso de coordenadas ,
 * resetea las coordenadas del navigator para que la cruz no se muestre en la
 * nueva imagen.
 * @param[in] pModel model
 */
void PixelLineNavigationButton::ListUpdate(Model *pModel) {
   // Verifico que existan todos los elementos para la operacion
   if (!pViewer_ || !pViewer_->GetNavigator()) {
      return;
   }
#ifdef __UNUSED_CODE__
   RasterElement* pelement = GetRasterElement();
   // Solo se actualiza el input widget si el navegador activo es pixel linea
   // y si cambio el elemento activo de la lista
   if (pInputWidget_
         && pInputWidget_ == pViewer_->GetNavigator()->GetWidget()->GetInputWidget()
         && (pelement != pRasterElement_)) {
      // Reset a navigator para que no muestre la cruz sobre la nueva imagen
      pViewer_->GetNavigator()->ResetCoordinate();
      pRasterElement_ = pelement;
      // Cambio el elemento del widget
      pInputWidget_->SetCurrentRasterElement(pRasterElement_);
      // Cambia la referencia espacial
      if (pRasterElement_) {
         pViewer_->GetNavigator()->SetSpatialReference(
               std::string(pRasterElement_->GetSpatialReference()));
      }
   }
#endif
}
}
