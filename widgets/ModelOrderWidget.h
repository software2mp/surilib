/*! \file ModelOrderWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MODELORDERWIDGET_H_
#define MODELORDERWIDGET_H_

// Includes Estandar
#include <string>
// Includes Suri
#include "suri/Widget.h"
#include "ModelOrderChangeNotification.h"
#include "TablePart.h"
// Includes Wx
#include "wx/event.h"
#include "wx/spinctrl.h"
// Defines
// forwards

namespace suri {

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(ModelOrderWidgetEvent);

/**
 * Widget para el manejo de cambio de orden de modelo de transformacion.
 * Posee una referencia a la clase que lo contiene de manera de poder notificar
 * el cambio sobre el spin para la correcta actualizacion de la tabla.
 */
class ModelOrderWidget : public Widget {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(ModelOrderWidgetEvent);
   explicit ModelOrderWidget(TablePart *pTablePart);
   virtual ~ModelOrderWidget();
   /** Creacion de Ventana  */
   virtual bool CreateToolWindow();
   /** Titulo de la ventana (en caso de pParent!=NULL */
   virtual const wxString GetWindowTitle() const;
   /** Configura el orden del modelo de transformacion.  */
   void SetOrder(const int Order);
   /** Obtiene el rmse del modelo de transformacion existente.   */
   double GetRmse() const;
   /** Obtiene el orden del modelo de transformacion.  */
   int GetOrder() const;
   /** Configura el rmse del modelo de transformacion existente. */
   void SetRmse(double RmseModel);
   /** Metodo que es llamado cuando se produce el evento de cambio de valor sobre el spin. */
   void OnSpinChange(wxCommandEvent &Event);

private:
   TablePart *pTablePart_;
   double rmse_;
   int order_;
   EVENT_OBJECT_PROPERTY(ModelOrderWidgetEvent);
   /** Metodo que actuliza el rmse text */
   void UpdateRmseText(double RmseValue);
};

} /** namespace suri */

#endif /* MODELORDERWIDGET_H_ */
