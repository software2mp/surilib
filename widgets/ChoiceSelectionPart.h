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

#ifndef CHOICESELECTIONPART_H_
#define CHOICESELECTIONPART_H_

// Includes standar
#include <map>

// Includes suri
#include "suri/Part.h"
#include "suri/RasterElement.h"

/** namespace suri */
namespace suri {

// Forwards
class RasterLayer;

/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(ChoiceSelectionPartEvent);

/** Permite seleccionar un valor en un Choice, activar/desactivar y un boton refresh oculto. */
/**
 * Part que recibe un elemento raster y permite seleccionar de una lista, al .
 * realizar commit se utiliza el valor seleccionado para modificar el nodo que
 * corresponda del elemento.
 */
class ChoiceSelectionPart : public Part {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(ChoiceSelectionPartEvent);
   /** Ctor. legacy */
   explicit ChoiceSelectionPart(RasterElement * pElement);
   /** Ctor. */
   explicit ChoiceSelectionPart(RasterLayer * pRasterLayer);
   /** Dtor **/
   virtual ~ChoiceSelectionPart();
   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Devuelve si hubo cambios */
   virtual bool HasChanged();
   /** Aplica los cambios al elemento */
   virtual bool CommitChanges();
   /** Carga los datos del elemento al control wx */
   virtual bool RollbackChanges();
   /** Carga los valores iniciales */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Des/habilita la ventana de la herramienta */
   virtual void SetEnabled(bool State);
   /** Retorna la posicion del item seleccionado en el choice */
   int GetSelectedItem();
   /** Actualiza el contenido del choice usando los atributos de la clase. */
   void ConfigureChoiceList();
   /** Modifica estado de activacion del checkbox */
   void ActivateChoice(bool State);
   /** Des/habilita la lista de realces y el boton refrescar(si esta desplegado) */
   void ActivateControls(bool State);
   /** Informa si el checkbox dentro del part esta activo. */
   bool IsChoiceActive();
   /** Modifica el estado de refresh_ a true y llama a updata */
   void Refresh();
protected:
   /**
    * Configura el contenido de choiceContent_ y selecciona el item que
    * corresponda al estado actual del elemento
    */
   virtual bool ResetList()=0;
   /**
    * Modifica/Crea/Elimina el nodo del elemento cuyo contenido representa el
    * choice, con la opcion que selecciono el usuario.
    */
   virtual bool ConfigureElement()=0;

   /** Modifica el estado de activacion de los controles del part(ActivateControls) */
   void OnCheck(wxCommandEvent &Event);
   /** Modifica el estado de refresh de la herramienta */
   void OnListSelection();
   /** Funcion que se llama en update y permite modificar comportamiento */
   virtual void OnUpdate() {
   }

   RasterElement* pElement_; /*! Puntero al elemento */
   RasterLayer* pRasterLayer_; /*! Puntero a la capa */
   std::vector<wxString> choiceContent_; /*! Contenido del choice(traducido) */
   bool isActive_; /*! Estado del item en el elemento */
   int selectedItem_; /*! Posicion del item configurado en el elemento. */
   bool refresh_; /*! Le indica al choice que se desea refrescar. */
private:
   EVENT_OBJECT_PROPERTY(ChoiceSelectionPartEvent); /*! Objeto que conecta */
   /* los eventos */
};
}

#endif /* CHOICESELECTIONPART_H_ */
