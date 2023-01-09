/** \file GeometryEditionEvent.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOMETRYEDITIONEVENT_H_
#define GEOMETRYEDITIONEVENT_H_

// Includes standard
#include <set>

// Includes suri
#include "suri/ViewportEvent.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/ViewcontextManagerInterface.h"
#include "GuiGeometryEditor.h"

namespace suri {

// fordwards
class VectorEditionTask;
class GeometryCollection;
class VectorEditionButton;

/**
 * Se encarga de manejar los eventos de usuario al editar una geometria
 */
class GeometryEditionEvent : public ViewportEvent {
public:
   /** Ctor */
   GeometryEditionEvent(ViewportManagerInterface *pViewportManager,
                        ViewcontextManagerInterface *pViewcontextManager,
                        VectorEditionTask* pVectorEditionTask,
                        VectorEditionButton* pEditionButton = NULL);

   /** Dtor */
   virtual ~GeometryEditionEvent();

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
   /** Busca los puntos de geometria editada en subset cercano a pixel linea recibidos */
   virtual std::set<SuriObject::UuidType> FindPointsNearCoordinate(
                                              const Coordinates &Coord);
   /** Solo retorna true para los eventos que ocurren en visualizador de edicion */
   virtual bool CanProcessEvent(wxEvent &Event) const;
   /** Indica si el drag actual comenzo en el punto seleccionado */
   bool validDrag_;

   /** Task encargado de administrar la edicion. */
   VectorEditionTask* pVectorEditionTask_;

private:
   double dragXHolder_;
   double dragYHolder_;

   VectorEditionButton* pEditionButton_;
};

} /* namespace suri */

#endif /* GEOMETRYEDITIONEVENT_H_ */
