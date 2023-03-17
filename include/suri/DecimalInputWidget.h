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
