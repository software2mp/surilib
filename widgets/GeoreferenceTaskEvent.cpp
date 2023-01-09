/** \file GeoreferenceTaskEvent.cpp
 *
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Suri
#include "GeoreferenceTaskEvent.h"
#include "GeometryRenderer.h"
#include "suri/RasterSpatialModel.h"
#include "Point.h"

namespace suri {

/**
 * Constructor
 * @param pViewportManager
 * @param pViewcontextManager
 * @param pGeoreferenceTask
 */
GeoreferenceTaskEvent::GeoreferenceTaskEvent(ViewportManagerInterface *pViewportManager,
                                             ViewcontextManagerInterface *pViewcontextManager,
                                             GeoreferenceTask* pGeoreferenceTask)
   : ViewportEvent(pViewportManager, pViewcontextManager) {
   pGeoreferenceTask_ = pGeoreferenceTask;
   pGcpStyle_ =
      VectorStyle::Create(
                          "VECTORSTYLE[1,\"Punto real\",SYMBOL[\"Suri-symbol\",2,COLOR[233,183,10,255],SIZE[2]]]");
   pSelectionStyle_ =
      VectorStyle::Create(
                          "VECTORSTYLE[3,\"Poligono rojo\",BRUSH[\"Suri-brush\",0,COLOR[0,0,0,255]],PEN[\"Suri-pen\",1,COLOR[32,89,108,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",2,COLOR[63,113,154,255],SIZE[2]]]");
   pEditionStyle_ =
      VectorStyle::Create(
                          "VECTORSTYLE[1,\"Punto seleccionado\",SYMBOL[\"Suri-symbol\",2,COLOR[217,0,0,255],SIZE[2]]]");
}

/**
 * Dtor.
 */
GeoreferenceTaskEvent::~GeoreferenceTaskEvent() {
   VectorStyle::Destroy(pGcpStyle_);
   VectorStyle::Destroy(pSelectionStyle_);
   VectorStyle::Destroy(pEditionStyle_);
}

/**
 * Limpia el estado de la herramienta, aborta edicion.
 */
void GeoreferenceTaskEvent::Reset() {
}

/**
 * Se ejecuta cuando el usuario hace click sobre el visualizador.
 */
void GeoreferenceTaskEvent::SetPoint(double Pixel, double Line, wxMouseEvent &Event) {
}

/**
 * Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse.
 */
void GeoreferenceTaskEvent::UpdatePoint(double Pixel, double Line, wxMouseEvent &Event) {
}

/**
 * Al pintar se dibuja geometria en edicion.
 */
void GeoreferenceTaskEvent::OnPaint(wxPaintEvent&Event, wxDC &Dc) {
   ViewerWidget* pviewer = GetEventViewer(Event);

   if (!pGeoreferenceTask_->IsActive())
      return;

   GeometryRenderer renderer(&Dc, pviewer->GetWorld());
   std::string gcpcolumnname = pGeoreferenceTask_->GetGcpColumnName();
   VectorDataLayer datalayer(pGeoreferenceTask_->GetTable(), gcpcolumnname);

   if (!datalayer.GetLayerSrs().empty())
      REPORT_DEBUG("D: ERROR - La tabla de georreferenciacion tiene srs");

   // Obtengo la ventana en pixel y linea de imagen
   Subset searchwindow;
   pviewer->GetWorld()->GetWindow(searchwindow);
   pGeoreferenceTask_->GetSpatialModel()->InverseTransform(searchwindow.lr_);
   pGeoreferenceTask_->GetSpatialModel()->InverseTransform(searchwindow.ul_);

   // Renderizo las geometrias que caen dentro de la ventana.
   std::vector<FeatureIdType> fids = datalayer.IntersectLayer(searchwindow,
                                                              datalayer.GetLayerSrs());
   RenderGcps(fids, pGcpStyle_, renderer);

   // Renderizo las geometrias seleccionadas
   GeometryCollection* pselection =
         pGeoreferenceTask_->GetFeatureSelector()->GetSelectedGeometries(
               pviewer->GetWorld());
   renderer.Render(pselection, NULL);

   // Renderizo geometria en edicion.
   if (pGeoreferenceTask_->IsEditingFeature()
         && pGeoreferenceTask_->GetEditedGeometry(gcpcolumnname) != NULL) {
      std::vector<Geometry*> overlay;
      Geometry* pgeom = pGeoreferenceTask_->GetEditedGeometry(gcpcolumnname)->Clone();
      pGeoreferenceTask_->AddSpatialReference(pgeom);
      overlay.push_back(pgeom);
      renderer.Render(overlay, pEditionStyle_);
      delete pgeom;
   }
}

/**
 * Indica que quiere recibir evento OnPaint.
 */
PaintEventInterface::PaintEventFlags GeoreferenceTaskEvent::GetPaintFlags() const {
   return PaintEventInterface::Paint;
}

/**
 * Indica que no se quieren recibir eventos de mouse.
 */
MouseEventInterface::MouseEventFlags GeoreferenceTaskEvent::GetMouseFlags() const {
   return MouseEventInterface::NoneFlag;
}

/**
 * Solo retorna true para los eventos que ocurren en visualizador de edicion.
 */
bool GeoreferenceTaskEvent::CanProcessEvent(wxEvent &Event) const {
   return true;
}

/**
 * Metodo que es llamado cuando se actualiza la seleccion.
 */
void GeoreferenceTaskEvent::SelectionUpdated() {
   if (pGeoreferenceTask_->IsActive())
      RefreshViewers(GetViewportManager()->GetViewportIds());
}

/**
 * Renderiza los Gcps indicados con el estilo default indicado.
 */
void GeoreferenceTaskEvent::RenderGcps(std::vector<FeatureIdType> &GcpIds,
                                       VectorStyle* pDefaultStyle,
                                       GeometryRenderer &Renderer) {
   std::string gcpcolumnname = pGeoreferenceTask_->GetGcpColumnName();
   VectorDataLayer datalayer(pGeoreferenceTask_->GetTable(), gcpcolumnname);
   GeometryCollection* pgeomcollection = new GeometryCollection(
         pGeoreferenceTask_->GetWorld()->GetSpatialReference());
   pgeomcollection->SetStyle(pDefaultStyle->Clone());
   std::vector<FeatureIdType>::iterator it = GcpIds.begin();
   for (; it != GcpIds.end(); ++it) {
      Geometry* pgeom = datalayer.GetGeometry(*it);
      if(pgeom && (!pGeoreferenceTask_->AddSpatialReference(pgeom)
                                 || !pgeomcollection->AppendGeometry(pgeom)))
         delete pgeom;
   }
   Renderer.Render(pgeomcollection, NULL);
   delete pgeomcollection;
}

}  // namespace suri
