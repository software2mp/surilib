/*! \file Navigator.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <cmath>
#include <string>
#include <vector>

// Includes suri
#include "suri/Navigator.h"
#include "suri/World.h"
#include "suri/Dimension.h"
#include "logmacros.h"
#include "suri/CoordinatesTransformInterface.h"
#include "suri/NavigationWidget.h"
#include "suri/ViewerWidget.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/DataViewManager.h"
#include "suri/DynamicViewerWidget.h"
#include "CrossHairPaintEvent.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/Viewer2D.h"

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] pViewer viewer asociado
 */
Navigator::Navigator(DataViewManager* pDataViewManager) :
      inputSpatialReference_(wxT("")), pCoordTransform_(NULL), point_(Coordinates(0, 0)),
      validPoint_(false), pViewer_(NULL), zoom_(-1), pNavigationWidget_(NULL),
      pCrossHair_(NULL), pDataView_(pDataViewManager) {
}

/**
 * Destructor
 * Destruye el objeto para transformar coordenadas
 */
Navigator::~Navigator() {
   if (pCoordTransform_) {
      delete pCoordTransform_;
   }
   if (pCrossHair_) {
      delete pCrossHair_;
   }
}

/**
 * Establece el sistema de coordenadas en el que espera los puntos para navegar
 * @param[in] SpatialReference: Sistema de coordenadas en que va a
 * recivir/enviar la clase
 * @return informa si se pudo crear transformacion entre mundo y el sistema
 * de coordenadas pasado por referencia
 */
bool Navigator::SetSpatialReference(const std::string &SpatialReference) {
   // elimina sist de coor anterior
   if (pCoordTransform_) {
      delete pCoordTransform_;
   }
   inputSpatialReference_ = SpatialReference.c_str();
   // Genera el la nueva transformacion entre el sistema ingresado y el del mundo
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                inputSpatialReference_.c_str());
   params.AddValue<std::string>(
         TransformationFactory::kParamWktOut,
         pViewer_ && pViewer_->GetWorld() ? pViewer_->GetWorld()->GetSpatialReference().c_str() :
                                            wxT(""));
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   pCoordTransform_ = pfactory->Create(TransformationFactory::kExact, params);
   TransformationFactoryBuilder::Release(pfactory);
   return (pCoordTransform_ != NULL);
}

/**
 * Retorna el sistema de coordenadas en el que espera los puntos para navegar
 * @return sistema de coordenadas en el que espera los puntos para navegar
 */
std::string Navigator::GetSpatialReference() {
   return inputSpatialReference_;
}

/**
 * Si el punto esta en el window y no cambia zoom no hace nada, caso contrario
 * llama a CenterPoint
 * @param[in] Point Coordenadas del punto que se quiere mostrar
 * @param[in] Zoom Relacion entre tamano de window y world. 0=no modificar
 * @return bool que indica si el punto esta dentro del mundo
 */
bool Navigator::ShowPoint(const Coordinates &Point, double Zoom) {
   if (!UpdateCoordinatesTransform()) {
      return false;
   }
   // Transformo las coord ingresadas a sistema de coordenadas de mundo
   Coordinates pointaux(Point);
   pCoordTransform_->Transform(pointaux);
   if (Zoom < 0) {
      REPORT_DEBUG("D:Zoom invalido");
      return false;
   }

   // Verifico que las coordenadas esten en el mundo
   Subset subset;
   pViewer_->GetWorld()->GetWorld(subset);
   if (!subset.IsInside(pointaux)) {
      SHOW_STATUS(_(message_POINT_NOT_IN_WORLD));
      return false;
   }

   // Si las coordenadas estan en la ventana solo actulizo punto
   pViewer_->GetWorld()->GetWindow(subset);
   if ((subset.IsInside(pointaux)) && (Zoom == 0)) {
      // Actualizo punto y zoom de la clase
      point_ = Point;
      zoom_ = Zoom;
      validPoint_ = true;
      pViewer_->GetWorld()->SetChanged();
      pViewer_->GetWorld()->SendViewerUpdate();
   } else {
      // Si las coordenadas no estan en la ventana llamo a centrar punto
      return CenterPoint(Point, Zoom);
   }
   return true;
}

/**
 * Cambia la ventana para centrarla en Point con Zoom indicado
 * @param[in] Point Coordenadas del punto que se quiere mostrar
 * @param[in] Zoom Relacion entre tamano de window y world. 0=no modificar
 * @return bool que indica si el punto esta dentro del mundo
 */
bool Navigator::CenterPoint(const Coordinates &Point, double Zoom) {
   if (!UpdateCoordinatesTransform()) {
      return false;
   }

   Coordinates pointaux(Point);
   pCoordTransform_->Transform(pointaux);
   if (Zoom < 0) {
      REPORT_DEBUG("D:Zoom invalido");
      return false;
   }
   Subset world;
   pViewer_->GetWorld()->GetWorld(world);
   if (world.IsInside(pointaux)) {
      Subset newwindow;

      double newwidth;
      double newheight;
      // Obtengo ventana y mundo actuales
      Subset window;
      pViewer_->GetWorld()->GetWindow(window);
      Dimension dimwindow(window);
      Dimension dimworld(world);

      if (Zoom > 0) {
         // Calculo el area del mundo
         double worldarea = dimworld.GetWidth() * dimworld.GetHeight() / Zoom;
         // Calculo la relacion entre altura y ancho de window
         double relation = std::abs(dimwindow.GetWidth() / dimwindow.GetHeight());
         // Calculo nuevo ancho y alto
         newwidth = std::sqrt(worldarea * relation);
         newheight = std::sqrt(worldarea / relation);
      } else {
         newwidth = dimwindow.GetWidth();
         newheight = dimwindow.GetHeight();
      }

      // Asigno coordenadas a subset
      newwindow.ul_.x_ = -dimworld.XSign() * newwidth / 2 + pointaux.x_;
      newwindow.ul_.y_ = -dimworld.YSign() * newheight / 2 + pointaux.y_;
      newwindow.lr_.x_ = dimworld.XSign() * newwidth / 2 + pointaux.x_;
      newwindow.lr_.y_ = dimworld.YSign() * newheight / 2 + pointaux.y_;

      pViewer_->GetWorld()->SetWindow(newwindow);
      // Actualizo punto y zoom de la clase
      point_ = Point;
      zoom_ = Zoom;
      validPoint_ = true;
   } else {
      SHOW_STATUS(_(message_POINT_NOT_IN_WORLD));
      return false;
   }
   return true;
}

/**
 * Informa las coordenadas del ultimo Show/Center que tubo exito
 * @param[in] Point Coordenadas del ultimo Show/Center que tubo exito
 * @return bool que informa si el punto habia sido inicializado
 */
bool Navigator::GetLastPoint(Coordinates &Point) {
   // Solo retorna las coordenadas si existe transformacion y sigue siendo valido
   if ((!UpdateCoordinatesTransform()) || (!validPoint_)) {
      return false;
   }
   if (zoom_ != -1) {
      Point.x_ = point_.x_;
      Point.y_ = point_.y_;
      Point.z_ = point_.z_;
      return true;
   }
   return false;
}

/**
 * Informa las coordenadas del ultimo Show/Center que tubo exito en coordenadas
 * de mundo
 * @param[out] Point coordenadas del ultimo Show/Center que tubo exito
 * @return true si puede devolver una coordenada
 * @return false si no se ingreso ningun punto, o no se puede realizar la
 *          transformacion de coordenadas.
 */
bool Navigator::GetLastPointInWorldCoordinates(Coordinates &Point) {
   if ((!UpdateCoordinatesTransform()) || (!validPoint_)) {
      return false;
   }
   Coordinates pointaux(Point);
   // Tranforma coordenadas antes de retornarlas
   if (pCoordTransform_->Transform(pointaux)) {
      Point.x_ = pointaux.x_;
      Point.y_ = pointaux.y_;
      Point.z_ = pointaux.z_;
      return true;
   }
   return false;
}

/**
 * Informa las coordenadas del centro del window actual
 * @param[out] Point del centro del window actual
 * @return true si puede devolver la coordenada
 * @return false si no existe transformacion de coordenadas, o ya no es valida
 *                (distinta a la del mundo actual)
 */
bool Navigator::GetCenterPoint(Coordinates &Point) {
   UpdateViewer();
   // Solo retorna las coordenadas si existe transformacion y sigue siendo valido
   if (!UpdateCoordinatesTransform()) {
      return false;
   }
   Subset window;
   pViewer_->GetWorld()->GetWindow(window);
   Point.x_ = (window.ul_.x_ + window.lr_.x_) / 2;
   Point.y_ = (window.ul_.y_ + window.lr_.y_) / 2;
   if (!pCoordTransform_->Transform(Point, true)) {
      return false;
   }
   return true;
}

/**
 * Informa si el window contiene el punto
 * @param[in] Point coordenada para verificar si esta dentro del window
 * @return bool que indica si el punto esta dentro de window actual
 */
bool Navigator::IsShow(const Coordinates &Point) {
   if (!UpdateCoordinatesTransform()) {
      return false;
   }

   Coordinates pointaux(Point);
   pCoordTransform_->Transform(pointaux);
   Subset window;
   pViewer_->GetWorld()->GetWindow(window);
   return window.IsInside(pointaux);
}

/**
 * Informa si el punto esta en el centro del window con 1% de error
 * @param[in] Point coordenada para verificar si esta en el centro del window
 * @return bool que indica si el punto esta en el centro del window con 1% de error
 */
bool Navigator::IsCentred(const Coordinates &Point) {
   if (!UpdateCoordinatesTransform()) {
      return false;
   }

   Coordinates pointaux(Point);
   pCoordTransform_->Transform(pointaux);

   // Encuentro diferencia entre punto y centro
   Coordinates error;
   Subset window;
   pViewer_->GetWorld()->GetWindow(window);

   // Si la ventana tiene ancho o alto 0 es invalida
   if ((window.ul_.x_ == window.lr_.x_) || (window.ul_.y_ == window.lr_.y_)) {
      REPORT_DEBUG("D:Ventana invalida");
      return false;
   }

   error.x_ = (window.ul_.x_ + window.lr_.x_) / 2 - pointaux.x_;
   error.y_ = (window.ul_.y_ + window.lr_.y_) / 2 - pointaux.y_;
   // Si diferencia es mayor a 0.01 de ancho de ventana considero que no esta centrada
   if ((std::abs(error.x_ / (window.ul_.x_ - window.lr_.x_)) > 0.01)
         || (std::abs(error.y_ / (window.ul_.y_ - window.lr_.y_)) > 0.01)) {
      return false;
   }
   return true;
}

/**
 * Retorna NavigatorWidget que se usa para navegar.
 * @return NavigationWidget que permite ingresar coordenadas en lat/long
 */
NavigationWidget* Navigator::GetWidget() {
   return pNavigationWidget_;
}

/**
 * Actualiza el coordinates transform si se cambio el mundo
 * @return retorna si el CoordinatesTransform es valido
 */
bool Navigator::UpdateCoordinatesTransform() {
   UpdateViewer();
   // Si no tengo sistema de coordenadas de origen retorno error.
   if (inputSpatialReference_ == "") {
      REPORT_DEBUG("D:No se creo el coordinates transform");
      return false;
   }
   if (pCoordTransform_)
      delete pCoordTransform_;

   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                inputSpatialReference_.c_str());
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                pViewer_->GetWorld()->GetSpatialReference().c_str());
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   pCoordTransform_ = pfactory->Create(TransformationFactory::kExact, params);
   TransformationFactoryBuilder::Release(pfactory);

   // Si no se pudo crear salgo
   if (!pCoordTransform_) {
      REPORT_DEBUG("D:No se creo el coordinates transform");
      validPoint_ = false;
      return false;
   }
   return true;
}

/**
 * Guarda el NavigatorWidget que se usa para navegar.
 * @param[in] pNavigationWidget herramienta que se usa para navegar
 */
void Navigator::SetWidget(NavigationWidget* pNavigationWidget) {
   pNavigationWidget_ = pNavigationWidget;
}

/**
 * Metodo que resetea la coordenada anterior, y permite que se actualice el
 * estado a valores iniciales cuando se cambia de elemento activo.
 */
void Navigator::ResetCoordinate() {
   validPoint_ = false;
   point_.x_ = 0;
   point_.y_ = 0;
   point_.z_ = 0;
}

/**
 * Devuelve la transformacion que esta siendo utilizada.
 */
CoordinatesTransformation* Navigator::GetTransformation() {
   UpdateCoordinatesTransform();
   return pCoordTransform_;
}

/** Actualiza el visualizador activo */
void Navigator::UpdateViewer() {
   ViewerWidget *pviewer = dynamic_cast<Viewer2D*>(
         pDataView_->GetViewportManager()->GetSelectedViewport());
   DynamicViewerWidget *pdynamic = dynamic_cast<DynamicViewerWidget*>(
         pDataView_->GetViewportManager()->GetSelectedViewport());
   if (!pviewer && pdynamic)
      pviewer = pdynamic->GetViewer();
   if (pviewer == pViewer_)
      return;
   pViewer_ = pviewer;
   StopPaintEvent();
   StartPaintEvent();
   SetSpatialReference(pViewer_->GetWorld()->GetSpatialReference());
}

/** Agrega al visualizador activo un evento paint */
void Navigator::StartPaintEvent() {
   ViewerWidget* pviewer = dynamic_cast<Viewer2D*>(
         pDataView_->GetViewportManager()->GetSelectedViewport());
   DynamicViewerWidget *pdynamic = dynamic_cast<DynamicViewerWidget*>(
         pDataView_->GetViewportManager()->GetSelectedViewport());
   if (!pviewer && pdynamic)
      pviewer = pdynamic->GetViewer();
   if (!pViewer_)
      pViewer_ = pviewer;
   pCrossHair_ = new CrossHairPaintEvent(pviewer, pDataView_->GetViewportManager(),
                                         pDataView_->GetViewcontextManager(), this);

   pviewer->PushPaintEvent(pCrossHair_);
}

/** Elimina del visualizador activo un evento paint */
void Navigator::StopPaintEvent() {
   std::vector<SuriObject::UuidType> ids = pDataView_->GetViewportManager()->GetViewportIds();
   for (size_t i = 0; i < ids.size(); ++i) {
      ViewerWidget* pviewer = AS_VW(pDataView_->GetViewportManager()->GetViewport(ids[i]));
      pviewer->PopPaintEvent(pCrossHair_);
   }
   delete pCrossHair_;
   pCrossHair_ = NULL;
}
}
