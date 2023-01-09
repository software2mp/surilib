/** \file GeometryCreationEvent.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOMETRYCREATIONEVENT_H_
#define GEOMETRYCREATIONEVENT_H_

// Includes suri
#include "suri/ViewportEvent.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/ViewcontextManagerInterface.h"
#include "GuiGeometryCreator.h"

namespace suri {

// fordwards
class VectorEditionTask;
class SnapTool;

/**
 * Se encarga de administrar los eventos de ususario al crear geometria.
 */
class GeometryCreationEvent : public ViewportEvent {
public:
   /** Ctor */
   GeometryCreationEvent(ViewportManagerInterface *pViewportManager,
                           ViewcontextManagerInterface *pViewcontextManager,
                           VectorEditionTask* pVectorEditionTask,
                           bool SimpleFeatureCreation = false);
   /** Dtor */
   virtual ~GeometryCreationEvent();

   /** Limpia el estado de la herramienta, aborta edicion */
   virtual void Reset();
   /** Se ejecuta cuando el usuario hace click sobre el visualizador. */
   virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event);
   /** Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse */
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent &Event);
   /** Al finalizar drag se actualiza geometria en vector */
   virtual void OnEndDrag(const Coordinates &InitialCoordinate,
                          const Coordinates &CurrentCoordinate, wxMouseEvent &Event);

private:
   /** Solo retorna true para los eventos que ocurren en visualizador de creacion */
   virtual bool CanProcessEvent(wxEvent &Event) const;
   void SnapPoint(SnapTool* pSnapTool, Coordinates &ViewportPosition);
   /** Task encargado de administrar la creacion. */
   VectorEditionTask* pVectorEditionTask_;
   bool simpleFeatureCreation_;
   bool featureCreated_;
};

} /* namespace suri */
#endif /* GEOMETRYCREATIONEVENT_H_ */
