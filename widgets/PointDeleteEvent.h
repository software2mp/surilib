/** \file PointDeleteEvent.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef POINTDELETEEVENT_H_
#define POINTDELETEEVENT_H_

#include "GeometryEditionEvent.h"

namespace suri {

class VectorEditionTask;

/**
 * Administra eventos que permiten elimiar puntos.
 * Con doble click inicia la edicion de un feature.
 * Con click elimina puntos en el feature editado.
 */
class PointDeleteEvent : public GeometryEditionEvent {
public:
   /** Ctor */
   PointDeleteEvent(ViewportManagerInterface *pViewportManager,
                        ViewcontextManagerInterface *pViewcontextManager,
                        VectorEditionTask* pVectorEditionTask);
   /** Dtor */
   virtual ~PointDeleteEvent();

   /** Se ejecuta cuando el usuario hace click sobre el visualizador. */
   virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event);
   /** Al iniciar drag se busca punto en geometrias en edicion */
   virtual void OnStartDrag(const Coordinates &InitialCoordinate);
};

} /* namespace suri */
#endif /* POINTDELETEEVENT_H_ */
