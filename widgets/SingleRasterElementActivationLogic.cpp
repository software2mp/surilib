/*! \file SingleRasterElementActivationLogic.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>

// Includes Suri
#include "SingleRasterElementActivationLogic.h"
#include "suri/World.h"
#include "suri/messages.h"
#include "suri/ViewerWidget.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

// Includes Wx

// Includes App

// Defines

// --------------- CLASE DE SELECCION DE ELEMENTOS DEL CONTEXTO ---------------
/**
 * Este metodo es el que proporciona la logica de activado/desactivado de
 * elementos del contexto.
 * Valida que no se puedan activar Vectores si no hay al menos 1 imagen activa.
 * Valida que no se esten activas mas de 1 imagen a la vez.
 * @param[in]	ElementList: lista de elementos del contexto.
 * @param[in]	ElementData: datos del estado y ubicacion en la lista del elemento.
 * @param[in]	Status: estado a aplicar al elemento(activo/desactivo).
 * @param[out]	pElement: puntero al elemento que se desea activar/desactivar.
 */
void SingleRasterElementActivationLogic::SetActivation(
      suri::Element *pElement, suri::LayerList::ElementListType & ElementList,
      suri::LayerList::ElementSelectionType & ElementData, bool Status) {
   const_cast<SingleRasterElementActivationLogic*>(this)->elementList_.clear();
   if (!pElement) {
      return;
   }

   switch (ElementData.find(pElement)->second.group_) {
      // Anotacion y vector se activan o desactivan sin modificaciones al resto
      case AnotationPosition:
      case VectorPosition: {
         bool canactivate = false;
         // recorro todos los elementos de la lista
         suri::LayerList::ElementListType::iterator it = ElementList.begin();
         while (it != ElementList.end()) {
            suri::Element *ptemp = *it;
            // si se esta activando algo que no es raster, y hay un raster activo, lo permito
            if (Status && ElementData[ptemp].group_ == RasterPosition
                  && ptemp->IsActive()) {
               canactivate = true;
            }
            it++;
         }
         // si no habia un raster activo, falla
         if (!canactivate && Status) {
            //message_ELEMENT_MUST_BE_DISPLAYED_s
            SHOW_ERROR("Debe haber un elemento de tipo %s desplegado.", requiredElement_.c_str());
            // no se traduce, lo
            // hace SHOW_
         } else {
            pElement->Activate(Status);
         }
         break;
      }
      case RasterPosition: {
         // recorro todos los elementos de la lista
         suri::LayerList::ElementListType::iterator it = ElementList.begin();
         while (it != ElementList.end()) {
            suri::Element *ptemp = *it;
            // Si estoy activando un raster
            // el elemento actual (del recorrido de la lista) es un raster
            // y esta activo, entonces lo desactivo
            if (Status) {
               // si esta activo y no es el actual
               if (ptemp->IsActive() && ptemp != pElement) {
                  // si no es raster, ademas me lo guardo para reactivarlo mas tarde
                  if (ElementData[ptemp].group_ == RasterPosition) {
                     ptemp->PreventNextViewerUpdate();
                     ptemp->Activate(false);
                  }
               }
            } else {  // Estoy desactivando. Si el raster esta activo, desactivo todos los elementos
               if (pElement->IsActive() && ptemp != pElement) {
                  ptemp->PreventNextViewerUpdate();
                  ptemp->Activate(false);
               }
            }
            it++;
         }
         // obtengo los sistemas de referencia del elemento nuevo
         // y del viewer para compararlos
         suri::World *pviewerworld = pViewer_ ? pViewer_->GetWorld() : NULL;
         std::string viewersr = pviewerworld ? pviewerworld->GetSpatialReference() : "";
         std::string rastersr = pElement->GetSpatialReference().c_str();
         suri::ParameterCollection params;
         params.AddValue<std::string>(suri::TransformationFactory::kParamWktIn, viewersr);
         params.AddValue<std::string>(suri::TransformationFactory::kParamWktOut, rastersr);
         suri::TransformationFactory* pfactory = suri::TransformationFactoryBuilder::Build();
         suri::CoordinatesTransformation *pct = pfactory->Create(suri::TransformationFactory::kExact,
                                                          params);
         suri::TransformationFactoryBuilder::Release(pfactory);
         // hago que el viewer se resetee para aceptar la imagen nueva
         // solo si se dan las condiciones de Ventana-Mundo/Extent
         // (sistemas de referencia iguales e interseccion entre extent y ventana)
         if (pct && pct->IsIdentity() && pviewerworld) {
            suri::Subset viewerwindow, elementextent;
            pviewerworld->GetWindow(viewerwindow);
            pElement->GetElementExtent(elementextent);
            if (!suri::Intersect(viewerwindow, elementextent).IsValid()) {
               pViewer_->ResetViewer();
            }
         } else {
            pViewer_->ResetViewer();
         }
         delete pct;
         pElement->Activate(Status);
         pViewer_->SetWindowTitle(pElement->GetName());
         break;
      }
      default:
         break;
   }
}
// --------------- CLASE DE SELECCION DE ELEMENTOS DEL CONTEXTO ---------------
