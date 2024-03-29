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

#ifndef TOOL_H_
#define TOOL_H_

// Includes standard
#include <string>
#include <set>

// Includes Suri

// Includes Wx

// Includes App

// Defines

// Forwards
class wxWindow;

/** namespace suri */
namespace suri {
class LayerList;
class ViewerWidget;

/** Clase base de las herramientas de contexto */
/**
 * Es la clase base para las herramientas de contexto. Las herramientas pueden
 * ser graficas (tener GUI) o no (ej. PixelPosition). El contexto es el
 * encargado de administrar y eliminar las herramientas cuando no se usan.
 * La creacion de las herramientas dependera de la necesidad de la misma, por
 * ello las herramientas pueden crearse deste el View, Contexto u otra
 * Herramienta.
 */
class Tool {
   /** Ctor. de Copia. */
   Tool(const Tool &Tool);

public:
   /** ctor */
   Tool();
   /** dtor */
   virtual ~Tool();

   /** Retorna el string con el nombre de la herramienta */
   std::string GetToolName() const {
      return toolName_;
   }
   /** Retorna la ventana asociada a la herramienta */
   /**
    * @return Puntero a la ventana asociada a la herramienta o NULL en caso de
    *       de no tener ventana asociada.
    */
   virtual wxWindow *GetToolWindow() const {
      return NULL;
   }
   /** Metodo para indicarle a la herramienta que esta seleccionada */
   bool SetActive(bool Active = true);
   /** Informa si la herramienta esta activa. */
   bool IsActive();
   /** Metodo virtual para indicar que la herramienta se ha des/activado */
   virtual bool DoSetActive(bool Active) {
      return true;
   }
   /** Asigna la lista de elementos asociada a la herramienta */
   virtual void SetList(LayerList *pList);
   /** Metodo virtual que se llama al ejecutar SetList */
   virtual bool DoSetList(LayerList *pList) {
      return true;
   }
   /** Asigna el visualizador del contexto asociado a la herramienta */
   virtual void SetViewer(ViewerWidget *pViewer);
   /** Metodo virtual que se llama al ejecutar SetViwer. */
   virtual bool DoSetViewer(ViewerWidget *pViewer) {
      return true;
   }
   /** Indica si esta habilitado o no */
   virtual bool IsEnabled();
   /** Cambia estado de activacion de la herramienta */
   virtual void SetEnabled(const bool &Enabled);
   /** agrega ventanas a la lista de depnedentWindows */
   virtual void AddDependentWindow(wxWindow* Window);
   /** Metodo que permite actualizar el estado de la herramienta */
   virtual void Update() {
   }
protected:
   /** Des/Habilita los controles que afectan el funcionamiento */
   virtual void EnableControls(bool Enabled = true);
   std::string toolName_; /*! Nombre de la herramienta */
   bool active_; /*! Flag para indicar que la herramienta esta activa */
   bool enabled_; /*! Flag para indicar que la herramienta esta habilitada. */
   std::set<wxWindow*> dependentWindows_; /*! ventanas que habilita/deshabilita */
   LayerList *pList_; /*! Lista de elementos */
   ViewerWidget *pViewer_; /*! Visualizador asociado */
};
}  // namespace suri

#endif /*TOOL_H_*/

