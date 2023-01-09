/*! \file MeassureWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MEASSUREWIDGET_H_
#define MEASSUREWIDGET_H_

// Includes standard

// Includes Suri
#include "suri/Widget.h"
#include "VectorElementEditor.h"
#include "MeassureAreaElementEditor.h"
#include "PixelVectorButton.h"
#include "suri/ToolGroupManager.h"
#include "suri/ObserverInterface.h"
#include "suri/Part.h"
#include "suri/Subject.h"
#include "suri/Widget.h"

namespace suri {

class ViewerWidget;
class DataViewManager;

DECLARE_EVENT_OBJECT(MeassureWidgetEvent);

/**
 * Herramienta para realizar mediciones rapidas.
 *
 * Permite generar vectores temporales para medir su area/distancia.
 * Los vectores generados no se agregan a la lista, una vez finalizada la medicion
 * se pierde el vector.
 * El widget muestra el resultado de la medicion mientras se realiza.
 */
class MeassureWidget : public ToolGroupManager, public Widget, public ObserverInterface, public Subject {
public:
   FRIEND_EVENT_OBJECT(MeassureWidgetEvent);

   static const int kAreaMode = 0;
   static const int kDistanceMode = 1;

   static const int km2 = 0;
   static const int m2 = 1;
   static const int ha = 2;

   static const int km = 0;
   static const int m = 1;
   static const int mi = 2;

   double area, perimdist;

   /**
    * Ctor.
    */
   MeassureWidget(wxWindow* pParent, ViewerWidget* pViewer = NULL, int VectorGroup = -1,
                  VectorEditor* pVectorEditor = NULL, Widget* pPart = NULL,
                  DataViewManager* pDataViewManager = NULL);

   /**
    * Dtor.
    */
   virtual ~MeassureWidget();

   /**
    * Carga el widget de medicion.
    */
   virtual bool CreateToolWindow();

   /**
    * Recibe notificaciones de las medidas tomadas.
    */
   virtual void Update(NotificationInterface* pNotification);

   /**
    * Despliega el panel para mostrar las mediciones.
    */
   void ShowWidget();

   /**
    * Oculta el panel para mostrar las mediciones.
    */
   void HideWidget();

protected:
   /**
    * Metodo que se llama cuando el boton se activo correctamente.
    */
   virtual void ButtonActivated(Button *pButton);

private:
   EVENT_OBJECT_PROPERTY(MeassureWidgetEvent);

   ViewerWidget* pViewer_; /*! Viewer sobre el que se realizaran las mediciones */
   int vectorGroup_;
   VectorEditor* pVectorEditor_;
   DatasourceManagerInterface* pDatasourceManager_;
   Widget* pPart_;
   DataViewManager* pDataViewManager_;
   ToggleButton* pActivatedButton_;
   std::string geometry_;

   /**
    * Configura el widget para mostrar medidas de area y perimetro o
    * para mostrar medidas de distancia.
    */
   void ConfigControls(int Mode);

   /**
    * Maneja el evento de click en el boton para borrar geometria.
    */
   void OnDeleteClicked(wxCommandEvent &Event);

   /**
    * Maneja el evento de click en el boton para borrar geometria.
    */
   void OnChoisePerimDistClicked(wxCommandEvent &Event);

   /**
    * Maneja el evento de click en el boton para borrar geometria.
    */
   void OnChoiseAreaClicked(wxCommandEvent &Event);
   /** Manejo de evento que guarda la medicion realizada **/
   void OnSaveMeassureClicked(wxCommandEvent &Event);
   /** Metodo auxiliar que obtiene el archivo destino en donde se guardara la medicion **/
   std::string GetOutputFileName();
   /**
    * Calcula la medida tomada en la unidad seleccionada.
    */
   double CalculateSelection(double value, int unit, int mode);
   /**
    * Chequea que el valor del label ID_MEASSUREWIDGET_PANEL_PERIM_DIST
    *  que contiene la distancia sea 0.
    */
   bool IsZero(wxStaticText *pperimdist);
};

}  // namespace suri

#endif /* MEASSUREWIDGET_H_ */
