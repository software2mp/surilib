/** \file GeoreferenceTaskEvent.h
 *
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOREFERENCETASKEVENT_H_
#define GEOREFERENCETASKEVENT_H_

#include <vector>

#include "suri/ViewportEvent.h"
#include "suri/GeoreferenceTask.h"
#include "FeatureSelectionObserver.h"
#include "GeometryRenderer.h"

namespace suri {

class VectorStyle;

/**
 *
 */
class GeoreferenceTaskEvent : public ViewportEvent,
                              public FeatureSelectionObserver {
public:
   /**
    * Ctor.
    */
   GeoreferenceTaskEvent(ViewportManagerInterface *pViewportManager,
         ViewcontextManagerInterface *pViewcontextManager,
         GeoreferenceTask* pGeoreferenceTask);

   /**
    * Dtor.
    */
   virtual ~GeoreferenceTaskEvent();

   /**
    * Limpia el estado de la herramienta, aborta edicion.
    */
   virtual void Reset();

   /**
    * Se ejecuta cuando el usuario hace click sobre el visualizador.
    */
   virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event);

   /**
    * Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse.
    */
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent &Event);

   /**
    * Al pintar se dibuja geometria en edicion.
    */
   virtual void OnPaint(wxPaintEvent&Event, wxDC &Dc);

   /**
    * Indica que quiere recibir evento OnPaint.
    */
   virtual PaintEventFlags GetPaintFlags() const;

   /**
    * Indica que no se quieren recibir eventos de mouse.
    */
   virtual MouseEventFlags GetMouseFlags() const;

   /**
    * Solo retorna true para los eventos que ocurren en visualizador de edicion.
    */
   virtual bool CanProcessEvent(wxEvent &Event) const;

   /**
    * Metodo que es llamado cuando se actualiza la seleccion.
    */
   virtual void SelectionUpdated();

private:
   GeoreferenceTask* pGeoreferenceTask_;  // Task encargado de administrar la edicion.
   VectorStyle* pGcpStyle_;
   VectorStyle* pSelectionStyle_;
   VectorStyle* pEditionStyle_;

   /**
    * Renderiza los Gcps indicados con el estilo default indicado.
    */
   void RenderGcps(std::vector<FeatureIdType> &GcpIds, VectorStyle* pDefaultStyle,
                   GeometryRenderer &Renderer);
};

}

#endif  // GEOREFERENCETASKEVENT_H_
