/** \file CoregisterTaskEvent.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COREGISTERTASKEVENT_H_
#define COREGISTERTASKEVENT_H_

// Includes standard
#include <vector>

// Includes suri
#include "suri/ViewportEvent.h"
#include "suri/CoregisterTask.h"
#include "FeatureSelectionObserver.h"
#include "GeometryRenderer.h"

namespace suri {

// fordward
class VectorStyle;

/**
 * Se encarga de renderizar todas las geometrias relacionadas a corregistro:
 *  i. Geometrias modificadas/creadas
 *  i. Geometrias seleccionadas
 *  i. Geometria que se esta modificando/creando
 */
class CoregisterTaskEvent : public ViewportEvent,
                                 public suri::FeatureSelectionObserver {
public:
   /** Ctor */
   CoregisterTaskEvent(ViewportManagerInterface *pViewportManager,
                           ViewcontextManagerInterface *pViewcontextManager,
                           CoregisterTask* pCoregisterTask);
   /** Dtor */
   virtual ~CoregisterTaskEvent();

   /** Limpia el estado de la herramienta, aborta edicion */
   virtual void Reset();

   /** Se ejecuta cuando el usuario hace click sobre el visualizador. */
   virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event);
   /** Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse */
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent &Event);
   /** Al pintar se dibuja geometria en edicion */
   virtual void OnPaint(wxPaintEvent&Event, wxDC &Dc);
   /** Indica que quiere recibir evento OnPaint */
   virtual PaintEventFlags GetPaintFlags() const;
   /** Indica que no se quieren recibir eventos de mouse */
   virtual MouseEventFlags GetMouseFlags() const;

   /** Solo retorna true para los eventos que ocurren en visualizador de edicion */
   virtual bool CanProcessEvent(wxEvent &Event) const;

   /** Metodo que es llamado cuando se actualiza la seleccion. */
   virtual void SelectionUpdated();

private:
   /** Renderiza los Gcps indicados con el estilo default indicado */
   void RenderGcps(CoregisterTask::WorkingImageType ImageType,
                           std::vector<FeatureIdType> &GcpIds,
                           VectorStyle* pDefaultStyle,
                           GeometryRenderer &Renderer);

   /** Task encargado de administrar la edicion. */
   CoregisterTask* pCoregisterTask_;

   VectorStyle* pGcpStyle_;
   VectorStyle* pSelectionStyle_;
   VectorStyle* pEditionStyle_;
};

} /* namespace suri */
#endif /* COREGISTERTASKEVENT_H_ */
