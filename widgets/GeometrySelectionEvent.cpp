/** \file GeometrySelectionEvent.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes suri
#include "GeometrySelectionEvent.h"
#include "VectorDatasourceSelectionSource.h"
#include "GeometryRenderer.h"

namespace suri {

/** Ctor */
GeometrySelectionEvent::GeometrySelectionEvent(
                                 ViewportManagerInterface *pViewportManager,
                                 ViewcontextManagerInterface *pViewcontextManager, // Default NULL?
                                 FeatureSelection* pFeatureSelection)  :
                                 ViewportEvent(pViewportManager, pViewcontextManager) {
   pFeatureSelection_ = pFeatureSelection;
}

/** Dtor */
GeometrySelectionEvent::~GeometrySelectionEvent() {
}

/**
 * Se ejecuta cuando el usuario hace click sobre el visualizador.
 * @param[in] Pixel pixel donde el usuario hizo click
 * @param[in] Line linea donde el usuario hizo click
 * @param[in] Event evento de mouse wx
 */
void GeometrySelectionEvent::SetPoint(double Pixel, double Line,
                                                        wxMouseEvent &Event) {
   ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   Coordinates viewportposition(Pixel, Line);
   Subset worldsubet = GetWorldSearchBox(viewportposition, pviewer->GetWorld());
   SelectGeometries(worldsubet);
}

/** Limpia el estado de la herramienta, aborta edicion */
void GeometrySelectionEvent::Reset() {
}

/**
 * Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse
 * @param[in] Pixel pixel donde el usuario hizo click
 * @param[in] Line linea donde el usuario hizo click
 * @param[in] Event evento de mouse wx
 */
void GeometrySelectionEvent::UpdatePoint(double Pixel,
                                    double Line, wxMouseEvent &Event) {
}

/**
 * Se ejecuta cuando el usuario comienza drag sobre visualizador
 * @param[in] InitialCoordinate coordenada donde el usuario inicio drag
 */
void GeometrySelectionEvent::OnStartDrag(const Coordinates &InitialCoordinate) {
   pFeatureSelection_->ClearSelection();
}

/**
 * Al realizar drag se actualiza geoemtria en edicion
 * @param[in] InitialCoordinate coordenada donde el usuario inicio drag
 * @param[in] CurrentCoordinate coordenada actual del mouse
 * @param[in] Event evento wx
 */
void GeometrySelectionEvent::OnDrag(const Coordinates &InitialCoordinate,
                          const Coordinates &CurrentCoordinate,
                          wxMouseEvent &Event) {
   rubberband_ = Subset(InitialCoordinate, CurrentCoordinate);
}

/**
 * Al finalizar drag se actualiza geometria en vector
 * @param[in] InitialCoordinate coordenada donde el usuario inicio drag
 * @param[in] CurrentCoordinate coordenada actual del mouse
 * @param[in] Event evento wx
 */
void GeometrySelectionEvent::OnEndDrag(const Coordinates &InitialCoordinate,
                       const Coordinates &CurrentCoordinate, wxMouseEvent &Event) {
   rubberband_ = Subset(InitialCoordinate, CurrentCoordinate);

   Subset worldsubset;
   ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   pviewer->GetWorld()->Transform(rubberband_.ul_, worldsubset.ul_);
   pviewer->GetWorld()->Transform(rubberband_.lr_, worldsubset.lr_);

   SelectGeometries(worldsubset);
}

/**
 * Al pintar se dibuja geometria en edicion
 * @param[in] Event evento de wx
 * @param[in] Dc wxDC del visualizador que se pinta.
 */
void GeometrySelectionEvent::OnPaint(wxPaintEvent &Event, wxDC &Dc) {
   ViewerWidget* pviewer = GetEventViewer(Event);
   GeometryCollection* pgeometries = pFeatureSelection_->
                           GetSelectedGeometries(pviewer->GetWorld());
   if (!pgeometries)
      return;

   GeometryRenderer renderer(&Dc, pviewer->GetWorld());
   renderer.Render(pgeometries, NULL);

   if (IsDragging()) {
      Dc.SetLogicalFunction(wxINVERT);
      Dc.SetBrush(wxBrush("red", wxTRANSPARENT));
      Dc.DrawRectangle(rubberband_.ul_.x_, rubberband_.ul_.y_,
                       rubberband_.lr_.x_ - rubberband_.ul_.x_,
                       rubberband_.lr_.y_ - rubberband_.ul_.y_);
   }
}

/** Indica que quiere recibir evento OnPaint */
PaintEventInterface::PaintEventFlags GeometrySelectionEvent::GetPaintFlags() const {
   return PaintEventInterface::Paint;
}

/**
 * Solo retorna true si el evento ocurre en un visualizador donde el datasource
 * de seleccion esta activo
 * Si no existe la capa con id de edicion en la lista, siempre retorna true.
 * @param[in] Evento evento que se desea analizar
 * @return bool que indica si puede procesar el evento
 */
bool GeometrySelectionEvent::CanProcessEvent(wxEvent &Event) const {
   if (pFeatureSelection_->GetWorkingDatasourceId() == SuriObject::NullUuid)
      return false;

   ViewerWidget* pviewer = GetEventViewer(Event);
   SuriObject::UuidType viewcontextid = pviewer ?
                  pviewer->GetViewcontextUuid() : SuriObject::NullUuid;
   ViewcontextInterface* pviewcontext = GetViewcontextManager()->GetSelectedViewcontext();
                                          //GetViewcontext(viewcontextid);
   if (!pviewcontext)
      return false;
   LayerInterface* player = pviewcontext->GetAssociatedLayer(
                     pFeatureSelection_->GetWorkingDatasourceId());

   // Si no existe la capa con id de edicion en la lista, siempre retorna true.
   // Se asume que cualquier capa que no esta en la lista se muestra
   // en todos los visualizadores desplegados.
   if (!player)
      return true;

   return player->GetElement() ? player->GetElement()->IsActive() : false;
}

/**
 * Selecciona las geometrias que intersecan el subset
 * @param[in] SelectionSubset subset que quiero usar para seleccionar. Esta
 * en coordenadas de mundo
 */
void GeometrySelectionEvent::SelectGeometries(
                                       Subset& SelectionSubset) const {
   ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   pFeatureSelection_->Select(SelectionSubset, pviewer->GetWorld());

   // Refresco todos los visualizadores.
   std::vector<SuriObject::UuidType> ids = GetViewportManager()->GetViewportIds();
   std::vector<SuriObject::UuidType>::iterator it = ids.begin();
   for (; it != ids.end(); ++it) {
      ViewerWidget* ptempviewer = AS_VW(GetViewportManager()->GetViewport(*it));
      ptempviewer->GetWindow()->Refresh();
   }
}

} /* namespace suri */
