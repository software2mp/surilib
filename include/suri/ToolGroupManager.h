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

#ifndef TOOLGROUPMANAGER_H_
#define TOOLGROUPMANAGER_H_

// Includes standard
#include <set>
#include <map>
#include <vector>

// Includes suri
#include "wxmacros.h"

// Includes App
class wxCommandEvent;
class wxWindow;

/** namespace suri */
namespace suri {

// Forwards
class Button;
class LayerList;
class ViewerWidget;
class Choice;
class Slider;
class Widget;

/** Declara la clase ToolGroupManagerEvent */
DECLARE_EVENT_OBJECT(ToolGroupManagerEvent);

/** Clase que maneja el set de botones que agrega cada herramienta. */
/**
 * Agrega y quita botones al menu de la herramienta. Des/Habilita los botones, e
 * informa el estado.
 * Conecta con el evento asociado al click sobre el boton.
 */
class ToolGroupManager {
public:
   /** Declara friend la clase de evento ToolGroupManagerEvent */
   FRIEND_EVENT_OBJECT(ToolGroupManagerEvent);
   /** Posicion de los botones de herramientas */
   typedef enum {
      Invisible = -2, /*!< Herramienta que no agrega boton a la barra */
      Last = -1, /*!< Posicion a la derecha de los botones */
      First = 0
   /*!< Posicion a la izquierda de los botones */} ButtonPositionType;
   /** Si se agrega nuevo tipo se debe agregar if */
   typedef int ButtonGroupType;
   static const ButtonGroupType StandAlone; /*!< Boton independiente (no toggleable, simultaneo) */
   static const ButtonGroupType Default;
   static const ButtonGroupType LeftMouse;
   static const ButtonGroupType RightMouse;
   static const ButtonGroupType Navigation;
   static const ButtonGroupType Edition;

private:
   /** Ctor. de Copia. */
   ToolGroupManager(const ToolGroupManager&);
   /** Contiene un set de botones */
   typedef std::set<Button*> ButtonSetType;
   /** Contiene set con ToolGroupManager asociados */
   typedef std::set<ToolGroupManager*> ToolGroupManagerSetType;
   /** Contiene un ButtonSetType y un Button */
   typedef struct {
      ButtonSetType toolSet_; /*! tipo de boton */
      Button *pLastSelected_; /*! ultimo boton seleccionado */
   } ButtonStateType;
   /** Mapa que contiene un ButtonGroupType y un ButtonStateType asociado */
   typedef std::map<ButtonGroupType, ButtonStateType> ButtonToolMapType;
   ButtonToolMapType toolMap_; /*! Mapa ordenado por grupo de botones */
   /** tools que administra el toolgroup manager
    * (ademas de botones que se tienen por separado) **/
   std::vector<Widget*> tools_;

public:
   /** Constructor */
   ToolGroupManager();
   /** Destructor */
   virtual ~ToolGroupManager() {
   }
   /** Agrega el ToolGroupManager al grupo al que pertenee este ToolGroupManager */
   void Link(ToolGroupManager *pToolGroupManager);
   /** Sale del grupo en el que se encuentra */
   void Unlink();
   /** retorna el boton activo del grupo seleccionado */
   Button *GetActiveButtonTool(ButtonGroupType Group = StandAlone);
   /** Agrega un boton de herramienta \TODO(Javier - Sin TCK): Analizar y hacer protected */
   void AddButtonTool(Button *pButton, int Position = Last, ButtonGroupType Group = StandAlone);
   /** Elimina boton de la herramienta */
   void RemoveButtonFromTool(Button *pButton);
   /** guarda los elementos que necesitan los botones al cargarlos al tool. */
   void InitializeToolbar(wxWindow *pToolbar_, ViewerWidget *pViewerWidget,
                          LayerList *pElementList);
   /*** Agrega un control grafico de tipo choice a la posicion que se indica por parametro **/
   void AddChoiceTool(Choice* pChoice, int Position = Last);
   /*** Agrega un control grafico de tipo slider a la posicion que se indica por parametro **/
   void AddSliderTool(Slider* pSlider, int Position = Last);


protected:
   /** Metodo virtual que se llama luego que el boton se activo correctamente */
   virtual void ButtonActivated(Button *pButton) {
   }
   /** Metodo virtual que se llama luego que el boton se desactivo */
   virtual void ButtonDeactivated(Button *pButton) {
   }
   /** Guarda el set como grupo al que pertenece, se deslinkea del actual si existe */
   void SetToolGroupManagerList(ToolGroupManagerSetType *toolGroupSet_);
   /** Desactiva los botones del tool e informa su estado */
   bool DeactivateButtons(ButtonGroupType Group);
   /** Mantiene los botones de herramientas en estado */
   void OnButtonClicked(wxCommandEvent &Event);
   /** Metodo que se llama cuando se ocasiona un evento sobre un tool
    * (control grafico, salvo boton) **/
   void OnToolClicked(wxCommandEvent &Event);
   /** Metodo que se llama cuando ocurre un evento sobre un control
    * grafico de tipo scroll (ej. slider) **/
   void OnScrollToolEvent(wxScrollEvent &Event);
   /** metodo que retorna la cantidad de botones en la herramienta */
   int GetButtonCount();
   /** retorna pToolbar_ */
   wxWindow* GetToolbar() {
      return pToolbar_;
   }
   /** metodo virtual que se llama cuando se agrega boton a tool */
   virtual void DoOnAddButton(Button *pButton) {
   }
   /** metodo virtual que se llama cuando se elimina boton a tool */
   virtual void DoOnRemoveButton(Button *pButton) {
   }
   /** metodo virtual que se llama cuando se hace click sobre boton de tool */
   virtual void DoOnButtonClicked(wxCommandEvent &Event) {
   }
   /** agrega boton a grupo */
   virtual void AddButtonToGroup(Button *pButton, ButtonGroupType Group);
   /** Desactiva el boton en todos los grupos a los que pertenece */
   virtual bool DeactivateButton(Button *pButton);
   /** Activa el boton en todos los grupos a los que pertenece */
   virtual bool ActivateButton(Button *pButton);

   // Propiedades
   ViewerWidget *pViewerWidget_; /*! Widget del viewer */
   LayerList *pElementList_; /*! Lista de elementos que se le setea a los botones cuando se crean */
   wxWindow *pToolbar_; /*! Toolbar donde se agregan los botones */
   ToolGroupManagerSetType *pLinkedToolsSet_; /*! Set con ToolGroupManagers  asociados */
   EVENT_OBJECT_PROPERTY(ToolGroupManagerEvent); /*! Objeto para conectar eventos */
};

}

#endif /*TOOLGROUPMANAGER_H_*/
