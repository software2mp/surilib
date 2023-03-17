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

#ifndef GENERICTOOL_H_
#define GENERICTOOL_H_

#include "suri/ToolInterface.h"

// Includes Estandar
#include <string>
#include <map>
#include <list>

// Includes Suri
#include "suri/Command.h"
#include "suri/DataViewManager.h"
#include "suri/PaintEventInterface.h"
#include "suri/MouseEventInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

// forwards
class Widget;
class EventInterface;

namespace core {
class CommandExecutionHandlerInterface;
class CommandCreatorInterface;
}

/*!
 * Tool Generico
 */
class GenericTool : public ToolInterface {
public:
   /** Constructor */
   GenericTool(DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~GenericTool();

   virtual void Initialize();
   virtual void Finalize();

   virtual std::list<Command*> GetCommands() const;
   virtual Command* GetCommand(const Command::CommandIdType &Id) const;
   virtual std::list<MouseEventInterface*> GetMouseEvents() const;
   virtual std::list<PaintEventInterface*> GetPaintEvents() const;
   virtual std::multimap<std::string, Widget*> GetWidgets() const;

   virtual bool IsActive(const Command* pToolCommand) const;
   virtual void SetActive(Command* pToolCommand, bool Active = true);
   virtual bool IsEnabled(const Command* pToolCommand) const;
   virtual void SetEnabled(Command* pToolCommand, bool Enable = true);

   /** Retorna el id del comando en la interfaz grafica */
   virtual std::string GetCommandGuiId(const Command* pCommand) const;
   /**
    * Implementacion de ejecucion de acciones de una herramienta en donde se recorre los
    * administradores de ejecucion disponibles, handlers,  (una especie de cadena de responsabilidades).
    **/
   virtual void Execute(const Command *pToolCommand);
   /** Agrega un administrador de ejecucion de acciones de la herramienta. **/
   void AddCommandExecutionHandler(core::CommandExecutionHandlerInterface* pHandler);
   /** Agrega un creador de comandos en la cadena de responsabilidades **/
   void AddCommandCreator(core::CommandCreatorInterface* pCreator);

   void SetConfigurationVariableName(const std::string& Name) {
      configurationVariableName_ = Name;
   }

   std::string GetConfigurationVariableName() {
      return configurationVariableName_;
   }

   virtual int GetId(const std::string &CommandName) const;
   virtual void SetMouseEvent(MouseEventInterface* pEvent);
   virtual void SetPaintEvent(PaintEventInterface* pEvent);

protected:
   virtual std::list<std::string> GetCommandNames() const;
   virtual Command *CreateCommand(const std::string &CommandName);
   virtual void DestroyCommand(Command* &pCommand) const {
   }
   virtual Widget *CreateWidget(const std::string &Widget) {
      return NULL;
   }
   virtual void SetCommand(Command* pCommand);
   virtual void SetEvent(EventInterface* pEvent);
   virtual void SetWidget(Widget* pWidget, const std::string &Location);
   /** Inicializa el estado de Enable y Active */
   virtual void InitializeState(const std::string &CommandName);
   /** Exporta los metadatos de una capa a un archivo */
   bool ExportMetadataToFile(DatasourceInterface* pDatasource, std::string Filename) const;
   /** Devuelve el DatasourceManager */
   virtual DatasourceManagerInterface *GetDatasourceManager() const;
   /** Devuelve el ViewportManager */
   virtual ViewportManagerInterface *GetViewportManager() const;
   /** Devuelve el ViewcontextManager */
   virtual ViewcontextManagerInterface *GetViewcontextManager() const;
   /** Devuelve el GroupManager */
   virtual GroupManagerInterface *GetGroupManager() const;
   /** Devuelve el SelectionManager */
   virtual TreeSelectionManagerInterface *GetSelectionManager() const;
   /** Devuelve el LibraryManager */
   virtual LibraryManager *GetLibraryManager() const;
   /** Devuelve el TaskManager */
   virtual TaskManager *GetTaskManager() const;
   /** Devuelve el FeatureSelection */
   virtual FeatureSelection *GetFeatureSelection() const;
   /** Devuelve el DataView */
   virtual DataViewManager *GetDataViewManager() const;

   std::string configurationVariableName_;

private:
   DataViewManager* pDataViewManager_; /*! Puntero al DataViewManager */
   bool active_;
   bool enabled_;
   bool finalized_;
   std::list<Command*> commandList_;
   std::set<const Command*> activeCommands_;
   std::set<const Command*> enabledCommands_;
   std::list<MouseEventInterface*> mouseEventList_;
   std::list<PaintEventInterface*> paintEventList_;
   std::list<EventInterface*> eventList_;
   std::multimap<std::string, Widget*> widgetList_;
   std::map<std::string, int> commandNameIds_;
   std::list<core::CommandExecutionHandlerInterface*> handlers_;
   core::CommandCreatorInterface* pCommandCreator_;
};

} /* namespace suri */

#endif /* GENERICTOOL_H_ */
