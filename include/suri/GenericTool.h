/*! \file GenericTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
