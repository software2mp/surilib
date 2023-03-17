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

#ifndef SLIDER_H_
#define SLIDER_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "Widget.h"

// Includes Wx
#include "wx/slider.h"

// Defines
// forwards

namespace suri {
// forwards
DECLARE_EVENT_OBJECT(SliderEventHandler);
class LabelText;

/*!
 * Clase que representa un control grafico de tipo slider.
 * Permite obtener e indicar el valor actual en el control grafico.
 */
class Slider : public suri::Widget {
   /** ctor copia **/
   Slider(const Slider&);
public:
   /** Constructor **/
   Slider(wxWindow *pViewerToolbar, const std::string& Tooltip, int Flag);
   /** Destructor **/
   virtual ~Slider();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Obtiene la seleccion activa en el control grafico. */
   int GetValue() const;
   /** Indica la seleccion activa en el control grafico. */
   void SetValue(int Value);
   /** Setea la propiedad enabled_ */
   virtual void SetEnabled(bool Enabled = true);
   /** Verifica si el boton se puede o no habilitar */
   virtual bool IsEnabled() const;
   /** Metodo que indica si posee un label asociado **/
   bool HasLabel() const;
   /** Metodo que obtiene el label **/
   LabelText* GetLabel() const;

protected:
   /** Metodo que se llama cuando se ejecuta CreateToolWindow **/
   virtual void OnCreateToolWindow();
   /** Metodo que se llama cuando cambia el valor actual en el control grafico.  */
   virtual void OnValueChanged(wxScrollEvent &Event);
   wxSlider* pSlider_;
   bool enabled_;
   EVENT_OBJECT_PROPERTY(SliderEventHandler);
   FRIEND_EVENT_OBJECT(SliderEventHandler);
   /** label asociado al control grafico **/
   std::string label_;
};

} /** namespace suri */

#endif /* SLIDER_H_ */
