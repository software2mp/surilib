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

#ifndef GUIIMPL_H_
#define GUIIMPL_H_

// Includes Estandar
#include <string>
#include <map>

// Includes Suri
#include "suri/GuiInterface.h"
#include "suri/Command.h"
#include "suri/ToolGroupManager.h"
// Includes Wx

// Defines

// forwards
class wxMenuItem;

namespace suri {

// forwards
class Widget;
class GuiStateChangeMediator;

/*!
 * Clase encargada de la administracion
 * de herramientas y menues para la aplicacion haciendo uso de los ToolGroupManagers
 * asociados.
 */
class GuiImpl : public GuiInterface {
public:
   explicit GuiImpl(Widget *pGuiWidget);
   virtual ~GuiImpl();
   /** Agrega la herramienta en la ubicacion de los Commands asociados**/
   virtual bool AddTool(ToolInterface* pTool);
   /** Elimina la herramienta de las ubicaciones del os Commandas asociados **/
   virtual bool RemoveTool(ToolInterface* pTool);
   virtual void AddToolbar(Widget *pWidget);
   /** Elimina el Toolbar en la ubicacion indicada por parametro**/
   virtual void RemoveToolbar(const std::string& LocationId);
   virtual void InitializeToolbar(wxWindow *pToolBar, const std::string& LocationId);
   static const std::string CallerLocationToken;
   static const std::string PositionLocationToken;
   virtual void EnableTopMenu(bool Enable, int Position,
                              const std::vector<std::string>& ValidNames);
   /** Configura el GuiChangeMediator. */
   void SetGuiStateChangeMediator(GuiStateChangeMediator* pGuiStateMediator);
   void DisableMenuNoElement();
   void EnableMenu(bool Enable);
   void EnableMenuRemoteLayer(bool Enable);
protected:
   /** Metodo que agrega un menu en la ubicacion descripta por el command**/
   virtual void AddMenu(Command *pCommand, const std::string& MenuLocation);
   /** Elimina un menu **/
   virtual void RemoveMenu(const std::string& MenuLocation);
   /** Agrega widget **/
   virtual void AddWidget(Widget *pWidget);
   /** Elimina widget **/
   virtual void RemoveWidget();
   /** Obtiene el Id del item de menu en Location */
   int GetLocationId(const std::string &Location);
   /** Crea un item de menu a partir del Command */
   wxMenuItem *CreateMenuFromCommand(Command *pCommand, const std::string& MenuLocation);
private:
   /** constantes para la definicion de las posiciones
    * para la descripcion de la ubicacion**/
   static const int LocationPosition;
   static const int LocationDescriptionPosition;

   /** Metodo auxilar que dada una ubicacion agrega el command a todos los
    *  toolgroupmanagerasociados
    */
   void ToolGroupToolAddition(Command *pCommand, const std::string& CommandType,
                              const std::string& Location);
   /** Metodo auxiliar que maneja de una herramienta en funcion al command asociado**/
   void ManageToolAddition(Command *pCommand, ToolGroupManager* pTgm,
                           const std::string& CommandType, const std::string& Location,
                           int Position = suri::ToolGroupManager::Last);
   /** Mapa con relacion ID, toolgroupManager **/
   typedef std::map<std::string, ToolGroupManager*> ToolGroupMapType;
   ToolGroupMapType toolGroupMap_;
   typedef std::multimap<std::string, ToolGroupManager*> CallerToolGroupMapType;
   CallerToolGroupMapType callerToolGroupMultiMap_;
   Widget *pGuiWidget_;
   /** Permite modificar el estado de la gui */
   GuiStateChangeMediator* pGuiStateMediator_;
   std::map<std::string, wxMenu*> menu_;
   bool isMenuEnable_;
   bool isRemoteLayer_;
   std::vector<std::string> validtools_;
};

} /* namespace suri */

#endif /* GUIIMPL_H_ */
