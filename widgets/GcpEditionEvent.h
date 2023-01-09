/** \file GcpEditionEvent.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GCPEDITIONEVENT_H_
#define GCPEDITIONEVENT_H_

// Includes suri
#include "suri/ViewportEvent.h"
#include "suri/CoregisterTask.h"

namespace suri {

/**
 * Permite editar graficamente los puntos de control
 */
class GcpEditionEvent : public ViewportEvent {
public:
   /** Ctor */
   GcpEditionEvent(ViewportManagerInterface *pViewportManager,
                              ViewcontextManagerInterface *pViewcontextManager,
                              CoregisterTask* pCoregisterTask, bool Editing = false);
   /** Dtor */
   virtual ~GcpEditionEvent();

   /** Limpia el estado de la herramienta, aborta edicion */
   virtual void Reset();
   /** Se ejecuta cuando el usuario hace click sobre el visualizador. */
   virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event);
   /** Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse */
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent &Event);
   /** Inicia la edicion de una geometria */
   virtual void OnLeftDoubleClick(wxMouseEvent&);

   /** Al iniciar drag se busca punto en geometrias en edicion */
   virtual void OnStartDrag(const Coordinates &InitialCoordinate);
   /** Al finalizar drag se actualiza geometria en vector */
   virtual void OnEndDrag(const Coordinates &InitialCoordinate,
                          const Coordinates &CurrentCoordinate, wxMouseEvent &Event);
   /** Al realizar drag se actualiza geoemtria en edicion */
   virtual void OnDrag(const Coordinates &InitialCoordinate,
                             const Coordinates &CurrentCoordinate,
                             wxMouseEvent &Event);

protected:
   /** Solo retorna true para los eventos que ocurren en visualizador de edicion */
   virtual bool CanProcessEvent(wxEvent &Event) const;
   /** Indica si el drag actual comenzo en el punto seleccionado */
   bool validDrag_;

   /** Task encargado de administrar la edicion. */
   CoregisterTask* pCoregisterTask_;

private:
   double dragXHolder_;
   double dragYHolder_;
   bool editing_;
};

} /* namespace suri */
#endif /* GCPEDITIONEVENT_H_ */
