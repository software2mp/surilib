/** \file CoregisterTaskEvent.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes suri
#include "CoregisterTaskEvent.h"
#include "GeometryRenderer.h"

namespace suri {

/** Ctor */
CoregisterTaskEvent::CoregisterTaskEvent (ViewportManagerInterface *pViewportManager,
                                 ViewcontextManagerInterface *pViewcontextManager,
                                 CoregisterTask* pCoregisterTask) :
                                 ViewportEvent(pViewportManager, pViewcontextManager) {
   pCoregisterTask_ = pCoregisterTask;
   pGcpStyle_ = VectorStyle::Create("VECTORSTYLE[1,\"Punto real\",SYMBOL[\"Suri-symbol\",2,COLOR[233,183,10,255],SIZE[2]]]");
   pSelectionStyle_ = VectorStyle::Create("VECTORSTYLE[3,\"Poligono rojo\",BRUSH[\"Suri-brush\",0,COLOR[0,0,0,255]],PEN[\"Suri-pen\",1,COLOR[32,89,108,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",2,COLOR[63,113,154,255],SIZE[2]]]");
   pEditionStyle_ = VectorStyle::Create("VECTORSTYLE[1,\"Punto seleccionado\",SYMBOL[\"Suri-symbol\",2,COLOR[217,0,0,255],SIZE[2]]]");
   
}

/** Dtor */
CoregisterTaskEvent::~CoregisterTaskEvent () {
   VectorStyle::Destroy(pGcpStyle_);
   VectorStyle::Destroy(pSelectionStyle_);
   VectorStyle::Destroy(pEditionStyle_);
}

/** Limpia el estado de la herramienta, aborta edicion */
void CoregisterTaskEvent::Reset() {
}

/** Se ejecuta cuando el usuario hace click sobre el visualizador. */
void CoregisterTaskEvent::SetPoint(double Pixel, double Line,
                                                        wxMouseEvent &Event) {
}

/** Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse */
void CoregisterTaskEvent::UpdatePoint(double Pixel, double Line,
                                                        wxMouseEvent &Event) {
}

/**
 * Al pintar se dibuja geometria en edicion
 * Centraliza el renderizado de geometrias sobre el visualizador para asegurar
 * que se renderizen en el orden correcto:
 *  i. Geometrias modificadas/creadas
 *  i. Geometrias seleccionadas
 *  i. Geometria que se esta modificando/creando
 * @param[in] Event evento wx de pintado
 * @param[out] Dc donde se deben renderizar las geometrias de corregistro.
 */
void CoregisterTaskEvent::OnPaint(wxPaintEvent &Event, wxDC &Dc) {
   CoregisterTask::WorkingImageType imagetype;
   ViewerWidget* pviewer = GetEventViewer(Event);
   if (!pCoregisterTask_->IsActive() ||
               !pCoregisterTask_->GetImageType(pviewer->GetWorld(), imagetype))
      return;

   GeometryRenderer renderer(&Dc, pviewer->GetWorld());
   std::string gcpcolumnname = pCoregisterTask_->GetGcpColumnName(imagetype);
   VectorDataLayer datalayer(pCoregisterTask_->GetTable(), gcpcolumnname);

   if (!datalayer.GetLayerSrs().empty())
      REPORT_DEBUG("D: ERROR - La tabla de corregistro tiene srs");

   // Obtengo la ventana en pixel y linea de imagen
   Subset searchwindow;
   pviewer->GetWorld()->GetWindow(searchwindow);
   pCoregisterTask_->GetSpatialModel(imagetype)->InverseTransform(searchwindow.lr_);
   pCoregisterTask_->GetSpatialModel(imagetype)->InverseTransform(searchwindow.ul_);

   // Renderizo las geometrias que caen dentro de la ventana.
   std::vector<FeatureIdType> fids = datalayer.IntersectLayer(searchwindow,
                                                       datalayer.GetLayerSrs());
   RenderGcps(imagetype, fids, pGcpStyle_, renderer);

   // Renderizo las geometrias seleccionadas
   GeometryCollection* pselection =
         pCoregisterTask_->GetFeatureSelector()->GetSelectedGeometries(pviewer->GetWorld());
   renderer.Render(pselection, NULL);

   // Renderizo geometria en edicion.
   if (pCoregisterTask_->IsEditingFeature() &&
                     pCoregisterTask_->GetEditedGeometry(gcpcolumnname) != NULL) {
      std::vector<Geometry*> overlay;
      Geometry* pgeom = pCoregisterTask_->GetEditedGeometry(gcpcolumnname)->Clone();
      pCoregisterTask_->AddSpatialReference(imagetype, pgeom);
      overlay.push_back(pgeom);
      renderer.Render(overlay, pEditionStyle_);
      delete pgeom;
   }
}

/** Indica que quiere recibir evento OnPaint */
PaintEventInterface::PaintEventFlags CoregisterTaskEvent::GetPaintFlags() const {
   return PaintEventInterface::Paint;
}

/** Indica que no se quieren recibir eventos de mouse */
MouseEventInterface::MouseEventFlags CoregisterTaskEvent::GetMouseFlags() const {
   return MouseEventInterface::NoneFlag;
}

/** Solo retorna true para los eventos que ocurren en visualizador de edicion */
bool CoregisterTaskEvent::CanProcessEvent(wxEvent &Event) const {
   ViewerWidget* pviewer = GetEventViewer(Event);
   CoregisterTask::WorkingImageType pimagetype;
   return pCoregisterTask_->IsActive() &&
            pCoregisterTask_->GetImageType(pviewer->GetWorld(), pimagetype);
}

/**
 * Metodo que es llamado cuando se actualiza la seleccion.
 */
void CoregisterTaskEvent::SelectionUpdated() {
   if (pCoregisterTask_->IsActive())
      RefreshViewers(GetViewportManager()->GetViewportIds());
}

/** 
 * Renderiza los gcps con el estilo default indicado 
 * @param[in] ImageType imagen sobre la que se va a renderizar
 * @param[in] GcpIds vector con el id de los gcps a renderizar
 * @param[in] pDefaultStyle estilo default
 * @param[out] Renderer renderizador
 */
void CoregisterTaskEvent::RenderGcps(CoregisterTask::WorkingImageType ImageType,
                                       std::vector<FeatureIdType> &GcpIds,
                                                VectorStyle* pDefaultStyle,
                                                 GeometryRenderer &Renderer) {
   std::string gcpcolumnname = pCoregisterTask_->GetGcpColumnName(ImageType);
   VectorDataLayer datalayer(pCoregisterTask_->GetTable(), gcpcolumnname);
   GeometryCollection* pgeomcollection = new GeometryCollection(
               pCoregisterTask_->GetWorld(ImageType)->GetSpatialReference());
   pgeomcollection->SetStyle(pDefaultStyle->Clone());
   std::vector<FeatureIdType>::iterator it = GcpIds.begin();
   for (; it != GcpIds.end(); ++it) {
      Geometry* pgeom = datalayer.GetGeometry(*it);
      if(pgeom && (!pCoregisterTask_->AddSpatialReference(ImageType, pgeom)
                                 || !pgeomcollection->AppendGeometry(pgeom)))
         delete pgeom;
   }
   Renderer.Render(pgeomcollection, NULL);
   delete pgeomcollection;
}

} /* namespace suri */
