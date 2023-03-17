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
