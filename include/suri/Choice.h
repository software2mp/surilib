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

#ifndef CHOICE_H_
#define CHOICE_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/Widget.h"

// Includes Wx
// Defines
// forwards

namespace suri {
// forwards
class LabelText;

// forwards
DECLARE_EVENT_OBJECT(ChoiceEventHandler);

/*!
 * Clase que representa un control grafico de tipo choice.
 * Permite obtener e indicar la seleccion.
 */
class Choice : public Widget {
   /** Constructor copia **/
   Choice(const Choice&);

public:
   /** Constructor **/
   Choice(wxWindow *pViewerToolbar, const std::string& Tooltip, int Flag);
   /** Destructor **/
   virtual ~Choice();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Obtiene la seleccion activa en el control grafico. */
   std::string GetSelection() const;
   /** Indica la seleccion activa en el control grafico. */
   void SetSelection(const std::string& Selection);
   /** Setea la propiedad enabled_ */
   virtual void SetEnabled(bool Enabled = true);
   /** Verifica si el boton se puede o no habilitar */
   virtual bool IsEnabled() const;
   /** Agrega una opcion al choice **/
   virtual void AddChoiceOption(const std::string& Option);
   /** Metodo que indica si posee un label asociado **/
   bool HasLabel() const;
   /** Metodo que obtiene el label **/
   LabelText* GetLabel() const;

protected:
   EVENT_OBJECT_PROPERTY(ChoiceEventHandler);
   FRIEND_EVENT_OBJECT(ChoiceEventHandler);
   /** Metodo que se llama cuando cambia la seleccion en el control grafico.  */
   virtual void OnSelectionChanged(wxCommandEvent &Event);
   /** Metodo que se llama cuando se ejecuta CreateToolWindow **/
   virtual void OnCreateToolWindow();
   /** Referencia al control grafico **/
   wxChoice* pChoice_;
   /** bool que indica si esta habilitado o no el choice **/
   bool enabled_;
   /** label asociado al control grafico **/
   std::string label_;
};

} /** namespace suri */

#endif /* CHOICE_H_ */
