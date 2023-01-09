/** \file GeoreferenceEditionEvent.h
 *
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/ViewportEvent.h"
#include "suri/GeoreferenceTask.h"

#ifndef GEOREFERENCEEDITIONEVENT_H_
#define GEOREFERENCEEDITIONEVENT_H_

namespace suri {

class ViewportManagerInterface;
class ViewcontextManagerInterface;
class GeoreferenceTask;

/**
 * Clase para manejar los eventos para la edicion y toma de puntos.
 */
class GeoreferenceEditionEvent : public ViewportEvent {
public:
   /**
    * Ctor.
    */
   GeoreferenceEditionEvent(ViewportManagerInterface* pViewportManager,
                            ViewcontextManagerInterface* pViewcontextManager,
                            GeoreferenceTask* pGeoreferenceTask, bool Editing = false);

   /**
    * Dtor.
    */
   virtual ~GeoreferenceEditionEvent();

   /**
    * Limpia el estado de la herramienta, aborta edicion.
    */
   virtual void Reset();

   /**
    * Se ejecuta cuando el usuario hace click sobre el visualizador.
    */
   virtual void SetPoint(double Pixel, double Line, wxMouseEvent& Event);

   /**
    * Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse.
    */
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent& Event);

   /**
    * Inicia la edicion de una geometria.
    */
   virtual void OnLeftDoubleClick(wxMouseEvent& Event);

   /**
    * Al iniciar drag se busca punto en geometrias en edicion.
    */
   virtual void OnStartDrag(const Coordinates& InitialCoordinate);

   /**
    * Al finalizar drag se actualiza geometria en vector.
    */
   virtual void OnEndDrag(const Coordinates& InitialCoordinate,
                          const Coordinates& CurrentCoordinate, wxMouseEvent& Event);

   /**
    * Al realizar drag se actualiza geoemtria en edicion.
    */
   virtual void OnDrag(const Coordinates& InitialCoordinate,
                       const Coordinates& CurrentCoordinate, wxMouseEvent& Event);

protected:
   bool validDrag_;  // Indica si el drag actual comenzo en el punto seleccionado.
   GeoreferenceTask* pGeoreferenceTask_;  // Task encargado de administrar la edicion.

   /**
    * Solo retorna true para los eventos que ocurren en visualizador de edicion.
    */
   virtual bool CanProcessEvent(wxEvent& Event) const;

private:
   double dragXHolder_;
   double dragYHolder_;
   bool editing_;
};

}  // namespace suri

#endif  // GEOREFERENCEEDITIONEVENT_H_
