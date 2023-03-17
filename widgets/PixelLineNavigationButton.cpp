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
