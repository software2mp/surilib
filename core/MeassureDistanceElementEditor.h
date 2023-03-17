/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#ifndef MEASSUREDISTANCEBUTTON_H_
#define MEASSUREDISTANCEBUTTON_H_

#include "LineElementEditor.h"
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

/** Clase que permite realizar mediciones de Distancias. */
/**
 *	Realiza la mediciones de distancia creando para dicho fin un vector de linea
 * que agrega a la lista de elementos.
 * El resultado de la medicion se almacena en el arbol xml del Elemento creado.
 *	\note Hereda de LineElementEditor ya que necesita poder dibujar una linea
 * para luego determinar su longitud.
 * \note Hereda de Meassure para poder manejar los cambios de unidades.
 */
class MeassureDistanceElementEditor : public Meassure,
                                      public ViewportEvent,
                                      public ObserverInterface,
                                      public Subject {
public:
   /**
    * Ctor.
    */
   MeassureDistanceElementEditor(
         ViewerWidget* pViewer, int ListGroup,
         ViewportManagerInterface* pViewportManager = NULL,
         ViewcontextManagerInterface* pViewcontextManager = NULL);
   /**
    * Dtor.
    */
   virtual ~MeassureDistanceElementEditor() {
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
    * Calcula la longitud de la linea.
    */
   void CalculateMeassurement();

   /**
    * Envia una notificacion de que las medidas cambiaron junto
    * con los valores deseados.
    */
   void SendMeasureChangedNotifcation(double Distance);
   /** Actualiza el visualizador activo */
   void RefreshViewer();
};
}  // namespace suri

#endif /*MEASSUREDISTANCEBUTTON_H_*/
