/*! \file MeassureAreaElementEditor.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MEASSUREAREABUTTON_H_
#define MEASSUREAREABUTTON_H_

#include "PolygonElementEditor.h"
#include "Meassure.h"
#include "suri/ViewportEvent.h"
#include "suri/PaintEventInterface.h"
#include "suri/ObserverInterface.h"
#include "suri/Subject.h"

namespace suri {

class ViewerWidget;
class ViewportManagerInterface;
class ViewcontextManagerInterface;
class GuiGeometryCreator;
class VectorEditor;

/**
 * Clase que permite realizar mediciones de Area.
 *	Realiza la mediciones de area creando para dicho fin un vector de poligono
 * que agrega a la lista de elementos.
 * El resultado de la medicion se almacena en el arbol xml del Elemento creado.
 *	\note Hereda de PolygonElementEditor ya que necesita poder dibujar un poligono
 * para luego determinar el area encerrada en el mismo.
 * \note Hereda de Meassure para poder manejar los cambios de unidades.
 */
class MeassureAreaElementEditor : public Meassure,
                                  public ViewportEvent,
                                  public ObserverInterface,
                                  public Subject {
public:
   /**
    * Ctor.
    */
   MeassureAreaElementEditor(ViewerWidget* pViewer, int ListGroup,
                             ViewportManagerInterface* pViewportManager = NULL,
                             ViewcontextManagerInterface* pViewcontextManager = NULL);

   /**
    * Dtor.
    */
   virtual ~MeassureAreaElementEditor() {
      delete pVectorEditor_;
   }

   /**
    * Resetea el estado del editor.
    */
   virtual void Reset();

   /**
    * Se ejecuta cuando el usuario hace click sobre el visualizador.
    */
   virtual void SetPoint(double Pixel, double Line, wxMouseEvent& Event);

   /**
    * Se ejecuta cuando el usuario se mueve en el visualizador.
    */
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent& Event);

   /**
    * Se ejecuta cuando el usuario comienza drag sobre visualizador.
    */
   virtual void OnStartDrag(const Coordinates& InitialCoordinate);

   /**
    * Se ejecuta cuando el usuario finaliza drag.
    */
   virtual void OnEndDrag(const Coordinates& InitialCoordinate,
                          const Coordinates& CurrentCoordinate, wxMouseEvent& Event);

   /**
    * Se ejecuta cada vez que el usuario mueve el mouse mientras realiza drag.
    */
   virtual void OnDrag(const Coordinates& InitialCoordinate,
                       const Coordinates& CurrentCoordinate, wxMouseEvent& Event);

   /**
    * Al pintar se dibuja geometria en edicion.
    */
   virtual void OnPaint(wxPaintEvent&Event, wxDC &dc);

   /**
    * Indica que quiere recibir el evento OnPaint.
    */
   virtual PaintEventInterface::PaintEventFlags GetPaintFlags() const;

   /**
    *
    */
   virtual bool CanProcessEvent(wxEvent &Event) const;

   /**
    * Recibe notificaciones del control contenedor.
    */
   virtual void Update(NotificationInterface* pNotification);

   /**
    *
    */
   virtual void Start();

   /**
    *
    */
   virtual void Stop();

protected:
   /**
    * Salva la medicion llamando a SaveVector.
    */
   virtual void CloseFeature();

private:
   bool editing_;
   ViewerWidget* pViewer_;
   VectorEditor* pVectorEditor_;
   GuiGeometryCreator* pGuiGeomCreator_;

   /**
    *
    */
   bool IsEditing() { return editing_; }

   /**
    * Inicializa el editor.
    */
   void Initialize();

   /**
    * Calcula el area del poligono.
    */
   void CalculateMeassurement();

   /**
    * Envia una notificacion de que las medidas cambiaron junto con los valores deseados.
    */
   void SendMeasureChangedNotifcation(double Area, double Perimeter);
   /** Actualiza el visualizador activo */
   void RefreshViewer();
};

}  // namespace suri

#endif  // MEASSUREAREABUTTON_H_
