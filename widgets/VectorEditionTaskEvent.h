/** \file VectorEditionTaskEvent.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTOREDITIONTASKEVENT_H_
#define VECTOREDITIONTASKEVENT_H_

// Includes suri
#include "suri/ViewportEvent.h"
#include "suri/VectorEditionTask.h"
#include "FeatureSelectionObserver.h"

namespace suri {

class VectorEditionTaskEvent : public ViewportEvent,
                         public suri::FeatureSelectionObserver {
public:
   /** Ctor */
   VectorEditionTaskEvent(ViewportManagerInterface *pViewportManager,
                              ViewcontextManagerInterface *pViewcontextManager,
                              VectorEditionTask* pVectorEditionTask);
   /** Dtor */
   virtual ~VectorEditionTaskEvent();

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

   /** Para saber si al ingresar a la edición, se agregó algun punto o se arrepintió. */
   virtual void SetClosedWithoutEdition(bool value);
   virtual bool GetClosedWithoutEdition();
private:
   /** Task encargado de administrar la edicion. */
   VectorEditionTask* pVectorEditionTask_;
   bool closedBeforeSetAPoint_;
};

} /* namespace suri */
#endif /* VECTOREDITIONTASKEVENT_H_ */
