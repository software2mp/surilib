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

#include "suri/Tool.h"

// Includes standard

// Includes Suri
#include "suri/messages.h"

// Includes Wx
#include "wx/wx.h"

// Includes App

// Defines

// Forwards

/** namespace suri */
namespace suri {
/**
 * Ctor. default
 * @return instancia de la clase Tool
 */
Tool::Tool() :
      toolName_(caption_UNKNOWN_TOOL), active_(false), enabled_(false), pList_(NULL),
      pViewer_(NULL) {
}

/**
 *  Dtor.
 */
Tool::~Tool() {
}

/**
 * Metodo para Des/Activar la herramienta
 * Este metodo se llama para indicarle a la herramienta que ha sido
 * activada.
 * La Herramienta realiza las operaciones necesarias en el metodo virtual
 * DoSetActive()
 * @param[in] Active estado de activacion
 * @return resultado de invocar a DoSetActive().
 */
bool Tool::SetActive(bool Active) {
   bool retval;
   if ((retval = DoSetActive(Active))) {
      active_ = Active;
   }
   return retval;
}

/**
 * Informa si la herramienta esta activa.
 * @return informa si el tool esta activo.
 */
bool Tool::IsActive() {
   return active_;
}

/**
 * Asigna la lista de elementos asociada a la herramienta
 * Para actualizar el atributo DoSetList debe retornar true
 * @param[in] pList puntero a la nueva lista
 */
void Tool::SetList(LayerList *pList) {
   if (pList && DoSetList(pList)) {
      pList_ = pList;
   }
}

/**
 * Asigna el visualizador del contexto asociado a la herramienta
 * Para actualizar el atributo DoSetViewer debe retornar true
 * @param[in] pViewer puntero al nuevo viewer
 */
void Tool::SetViewer(ViewerWidget *pViewer) {
   if ((pViewer) && DoSetViewer(pViewer)) {
      pViewer_ = pViewer;
   }
}

/* Indica si esta habilitado o no
 * @return true si esta habilitada.
 * @return false si no esta habilitada
 */
bool Tool::IsEnabled() {
   return enabled_;
}

/*
 * Establece el estado de la herramienta (habilitado o no)
 * @param[in] Enabled estado a setear en la herramienta.
 */
void Tool::SetEnabled(const bool &Enabled) {
   enabled_ = Enabled;
}

/**
 * agrega ventanas a la lista de depnedentWindows
 * @param[in] Window ventana a agregar.
 */
void Tool::AddDependentWindow(wxWindow* Window) {
   dependentWindows_.insert(Window);
}

/**
 * Des/Habilita los controles que afectan el funcionamiento
 * \post Las ventanas en dependentWindows_ toman estado SetEnabled(Status)
 *       despues de pasar por DoSetActive (que le da tiempo para modificar
 *       el estado).
 *  @param[in]	Enabled: estado de los controles.
 */
void Tool::EnableControls(bool Enabled) {
   std::set<wxWindow*>::iterator it = dependentWindows_.begin();
   while (it != dependentWindows_.end()) {
      (*it)->Enable(Enabled);
      it++;
   }
}
}  // namespace suri
