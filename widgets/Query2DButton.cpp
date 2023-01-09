/*! \file Query2DButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "Query2DButton.h"
#include "suri/Viewer2D.h"

// Includes Wx
// Defines
// forwards

namespace suri {
/** ctor */
/**
 * @param[in] pToolbar window donde se debe agregar el boton
 * @param[in] pViewer viewer sobre el que se hacen clicks para activar
 * @param[in] pDataViewManager administrador de informacion de la aplicacion
 * hotlink
 */
Query2DButton::Query2DButton(wxWindow *pToolbar, Viewer2D *pViewer,
                             DataViewManager* pDataViewManager) :
      QueryButton(pToolbar, pViewer), pDataViewManager_(pDataViewManager) {
}
/** dtor */
Query2DButton::~Query2DButton() {
}

/** Analiza si el click puede pertenecer a un elemento valido. */
/**
 * @param[in] PosX posicion(X) sobre el que se hizo click
 * @param[in] PosY posicion(Y) sobre el que se hizo click
 */
bool Query2DButton::IsValid(int PosX, int PosY) {
   Coordinates coord;
   pViewer_->GetWorld()->V2WTransform(PosX, PosY, coord.x_, coord.y_);
   Subset world;
   pViewer_->GetWorld()->GetWorld(world);

   return world.IsInside(coord);
}

/** Retorna el box del elemento activo */
/**
 * @param[in] PosX lugar donde se hizo click x
 * @param[in] PosY lugar donde se hizo click y
 * @param[in] ElementSize tamanio del elemento
 * @param[out] QueryBox box con dimencion del elemento centrado en click
 * y en coordenadas de la imange.
 */
bool Query2DButton::GetQueryBox(int PosX, int PosY, int ElementSize, Subset &QueryBox) {
   if (!pViewer_ || !pViewer_->GetWorld()) {
      return false;
   }

   pViewer_->GetWorld()->V2WTransform(PosX - QUERY_TOLERANCE, PosY - QUERY_TOLERANCE,
                                      QueryBox.ul_.x_, QueryBox.ul_.y_);
   pViewer_->GetWorld()->V2WTransform(PosX + QUERY_TOLERANCE, PosY + QUERY_TOLERANCE,
                                      QueryBox.lr_.x_, QueryBox.lr_.y_);

   return true;
}
/** Activa la herramienta de consulta de pixel **/
void Query2DButton::Start() {
   if (!pDataViewManager_)
      return;
   pQuery_ = new Query(pDataViewManager_->GetViewportManager(),
                       pDataViewManager_->GetViewcontextManager(),
                       pDataViewManager_->GetLibraryManager());
   pViewer_->PushPaintEvent(pQuery_);
   pViewer_->PushMouseEvent(pQuery_);
}
/** Desactiva la herramienta de consulta de pixel **/
void Query2DButton::End() {
   if (pQuery_) {
      pViewer_->PopPaintEvent(pQuery_);
      pViewer_->PopMouseEvent(pQuery_);
   }
   delete pQuery_;
   pQuery_ = NULL;
}
#ifdef __PIXEL_INFO__
/** Metodo que se llama cuando se activa/desactiva el togglebutton **/
bool Query2DButton::DoSetValue(bool State) {
   if (State && !pQuery_)
      Start();
   else if (!State)
      End();

   return ToggleButton::DoSetValue(State);
}
#endif
}
