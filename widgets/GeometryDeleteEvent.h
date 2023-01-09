/** \file GeometryDeleteEvent.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOMETRYDELETEEVENT_H_
#define GEOMETRYDELETEEVENT_H_

// Includes suri
#include "suri/ViewportEvent.h"

namespace suri {

// fordwards
class VectorEditionTask;

/**
 * Clase encargada de eliminar geometrias
 */
class GeometryDeleteEvent : public ViewportEvent {
public:
   /** Ctor */
   GeometryDeleteEvent(ViewportManagerInterface *pViewportManager,
                        ViewcontextManagerInterface *pViewcontextManager,
                        VectorEditionTask* pVectorEditionTask);
   /** Dtor */
   virtual ~GeometryDeleteEvent();

   /** Limpia el estado de la herramienta, aborta edicion */
   virtual void Reset();
   /** Se ejecuta cuando el usuario hace click sobre el visualizador. */
   virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event);
   /** Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse */
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent &Event);

private:
   /** Solo retorna true para los eventos que ocurren en visualizador de creacion */
   virtual bool CanProcessEvent(wxEvent &Event) const;
   /** Task encargado de administrar la edicion. */
   VectorEditionTask* pVectorEditionTask_;
};

} /* namespace suri */
#endif /* GEOMETRYDELETEEVENT_H_ */
