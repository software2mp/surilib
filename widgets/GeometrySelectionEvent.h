/** \file GeometrySelectionEvent.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOMETRYSELECTIONEVENT_H_
#define GEOMETRYSELECTIONEVENT_H_

// Includes suri
#include "suri/ViewportEvent.h"
#include "suri/FeatureSelection.h"
#include "suri/Subset.h"

namespace suri {

/**
 * Maneja los eventos sobre los visualizadores para que se puedan
 * seleccionar features.
 * Todos los features seleccionados deben pertenecer al mismo datasource.
 */
class GeometrySelectionEvent : public ViewportEvent  {
public:
   /** Ctor */
   GeometrySelectionEvent(ViewportManagerInterface *pViewportManager,
                                  ViewcontextManagerInterface *pViewcontextManager,
                                  FeatureSelection* pFeatureSelection);
   /** Dtor */
   virtual ~GeometrySelectionEvent();

   /** Limpia el estado de la herramienta, aborta edicion */
   virtual void Reset();
   /** Se ejecuta cuando el usuario hace click sobre el visualizador. */
   virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event);
   /** Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse */
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent &Event);

   /** Se ejecuta cuando el usuario comienza drag sobre visualizador */
   virtual void OnStartDrag(const Coordinates &InitialCoordinate);
   /** Al finalizar drag se actualiza geometria en vector */
   virtual void OnEndDrag(const Coordinates &InitialCoordinate,
                          const Coordinates &CurrentCoordinate, wxMouseEvent &Event);
   /** Al realizar drag se actualiza geoemtria en edicion */
   virtual void OnDrag(const Coordinates &InitialCoordinate,
                             const Coordinates &CurrentCoordinate,
                             wxMouseEvent &Event);

   /** Al pintar se dibuja geometria en edicion */
   virtual void OnPaint(wxPaintEvent &Event, wxDC &Dc);
   /** Indica que quiere recibir evento OnPaint */
   virtual PaintEventFlags GetPaintFlags() const;

protected:
   /** Solo retorna true para los eventos que ocurren en visualizador de creacion */
   virtual bool CanProcessEvent(wxEvent &Event) const;
   /** Selecciona las geometrias que intersectan el subset */
   virtual void SelectGeometries(Subset& SelectionSubset) const;

   /** Administra la seleccion de features */
   FeatureSelection* pFeatureSelection_;
   /** Subset que contiene el drag en pixel y linea */
   Subset rubberband_;
};

} /* namespace suri */
#endif /* GEOMETRYSELECTIONEVENT_H_ */
