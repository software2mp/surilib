/*! \file DecimalInputWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DECIMALINPUTWIDGET_H_
#define DECIMALINPUTWIDGET_H_

// Includes Estandar

// Includes Suri
#include "suri/LatitudeLongitudeInputWidget.h"

// Includes Wx
#include "wx/event.h"
#include "wx/xrc/xmlres.h"
#include "wx/hyperlink.h"

// Defines
#define __NOT_USE_LATITIUDE_PART__

// forwards

namespace suri {

#ifdef __NOT_USE_LATITIUDE_PART__
// forwards
/*!
 * Clase para el ingreso de coordenadas en Sistema Decimal
 * Aprovecha las funcionalidades ya implementadas en LatitudeLongitudeInputWidget
 * para validacion y convierte las mismas a decimal
 */
class DecimalInputWidget  : public LatitudeLongitudeInputWidget {
   /** ctor copia **/
   DecimalInputWidget(const DecimalInputWidget&);

public:
   /** ctor **/
   explicit DecimalInputWidget(bool Validate = true);
   /** dtor **/
   virtual ~DecimalInputWidget();

   /** Creacion de la ventana */
   virtual bool CreateToolWindow();
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool GetCoordinate(Coordinates& DecimalCoord);
   /** Despliega la coordenada interna en los controles */
   virtual void DisplayCoordinates();

private:
   /** Metodo auxiliar que se encarga de validar la nueva coordenada y actualizar
    * el valor de la coordenada guardada */
   virtual bool ValidateAndUpdateCoordinates(Coordinates& DecimalCoordinate,
                                     Coordinates& NewCoordinate,
                                     Coordinates& ReferenceCoordinate);
   /** Metodo que se llama cuando se configura el sistema de referencia. Permite
    *  realizar alguna accion luego de configurar el sistema de referencia
    *  de trabajo
    */
   virtual void OnSpatialReferenceSet();
   wxString latitudeStringValue_, longitudeStringValue_;
};
} /** namespace suri */
#else
   typedef LatitudeLongitudeInputWidget DecimalInputWidget;
#endif

#endif /* DECIMALINPUTWIDGET_H_ */
