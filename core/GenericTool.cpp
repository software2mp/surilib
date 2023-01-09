/*! \file GenericTool.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014444-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <utility>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <set>

// Includes Suri
#include "suri/GenericTool.h"
#include "suri/Configuration.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/CommandExecutionHandlerInterface.h"
#include "suri/CommandCreatorInterface.h"
#include "suri/ToolSupport.h"
#include "XmlMetadataHandler.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
GenericTool::GenericTool(DataViewManager* pDataViewManager) :
      configurationVariableName_(""), pDataViewManager_(pDataViewManager), active_(true),
      enabled_(true), finalized_(true), pCommandCreator_(NULL) {
   commandNameIds_.insert(std::make_pair(SuriObject::NullUuid, SuriObject::NullIntUuid));
}

/**
 * Destructor
 */
GenericTool::~GenericTool() {
   if (!finalized_) {
      REPORT_ERROR("D: Clase derivada de GenericTool no ha llamado a Finalize() en el dtor.");
      Finalize();
   }
	delete pCommandCreator_;
	// memory leak sobre list<CommandExecutionHandlerInterface*>
}

void GenericTool::Initialize() {
   // si ya se inicializo, salgo
   if (!finalized_)
      return;
   std::list < std::string > list = GetCommandNames();
   std::list<std::string>::iterator it = list.begin();
   while (it != list.end()) {
      std::string commandname = *it;
      Command *pcommand = CreateCommand(commandname);
      if (pcommand) {
         SetCommand(pcommand);
         if (pcommand->GetId() == SuriObject::NullIntUuid) {
            pcommand->SetId(SuriObject::CreateIntId(SuriObject::CreateId()));
         }
         std::pair<std::string, int> commandpair(commandname, pcommand->GetId());
         commandNameIds_.insert(commandpair);
         InitializeState(commandname);
      }
      ++it;
   }
   finalized_ = false;
}

void GenericTool::InitializeState(const std::string &CommandName) {
   Command *pcommand = GetCommand(GetId(CommandName));
   SetEnabled(pcommand, pcommand->IsEnabled());
   SetActive(pcommand, false);
}

void GenericTool::Finalize() {
   std::list<Command*> commands = GetCommands();
   std::list<Command*>::iterator it = commands.begin();
   while (it != commands.end()) {
      DestroyCommand(*it);
      ++it;
   }
   finalized_ = true;
}

std::list<Command*> GenericTool::GetCommands() const {
   return commandList_;
}

Command* GenericTool::GetCommand(const Command::CommandIdType &Id) const {
   std::list<Command*>::const_iterator it = commandList_.begin();
   while (it != commandList_.end() && (*it)->GetId() != Id)
      ++it;
   if (it == commandList_.end())
      return NULL;
   return *it;
}

std::list<MouseEventInterface*> GenericTool::GetMouseEvents() const {
   return mouseEventList_;
}

std::list<PaintEventInterface*> GenericTool::GetPaintEvents() const {
   return paintEventList_;
}

std::multimap<std::string, Widget*> GenericTool::GetWidgets() const {
   return widgetList_;
}

bool GenericTool::IsActive(const Command *pToolCommand) const {
   std::set<const Command*>::const_iterator it = activeCommands_.begin();
   bool found = false;
   for ( ; !found && it != activeCommands_.end(); ++it) {
      found = ((*it)->GetId() == pToolCommand->GetId());
   }
   return found;
}

void GenericTool::SetActive(Command *pToolCommand, bool Active) {
   if (Active) {
      activeCommands_.insert(pToolCommand);
   } else {
      activeCommands_.erase(pToolCommand);
      std::set<const Command*>::iterator it = activeCommands_.begin();
       for ( ; it != activeCommands_.end(); ++it) {
          if ((*it)->GetId() == pToolCommand->GetId()) {
             activeCommands_.erase(it);
             break;
          }
       }
   }
}

bool GenericTool::IsEnabled(const Command *pToolCommand) const {
   std::set<const Command*>::const_iterator it = enabledCommands_.begin();
   bool found = false;
   for ( ; !found && it != enabledCommands_.end(); ++it) {
      found = ((*it)->GetId() == pToolCommand->GetId());
   }
   return found;
}

void GenericTool::SetEnabled(Command *pToolCommand, bool Enable) {
   if (Enable) {
      enabledCommands_.insert(pToolCommand);
   } else {
      std::set<const Command*>::iterator it = enabledCommands_.begin();
      for ( ; it != enabledCommands_.end(); ++it) {
         if ((*it)->GetId() == pToolCommand->GetId()) {
            enabledCommands_.erase(it);
            break;
         }
      }
   }
}

/**
 * Retorna el id del comando en la interfaz grafica
 * @param[in] pCommand Comando a analizar
 * @return id del comando en la interfaz grafica <tool_name>|<command_name>
 */
std::string GenericTool::GetCommandGuiId(const Command* pCommand) const {
   std::string name;
   std::map<std::string, int>::const_iterator it = commandNameIds_.begin();
   for ( ; it != commandNameIds_.end() && it->second != pCommand->GetId(); ++it);
   if (it == commandNameIds_.end())
      return "";

   return configurationVariableName_ + "|" + it->first;
}

std::list<std::string> GenericTool::GetCommandNames() const {
   std::string navigationcommands;
   // si no se configuro, no entrega nombres. Esto previene que exista
   // una variable tool__commands que sea leida por este codigo
   if (!configurationVariableName_.empty())
      navigationcommands = suri::Configuration::GetParameter(
            std::string("tool_") + configurationVariableName_ + "_commands", "");

   std::vector < std::string > commandnames = tokenizer(navigationcommands, ",");
   return std::list < std::string > (commandnames.begin(), commandnames.end());
}

void GenericTool::SetCommand(Command *pCommand) {
   if (!pCommand)
      return;
   commandList_.push_back(pCommand);
}

void GenericTool::SetEvent(EventInterface* pEvent) {
   eventList_.push_back(pEvent);
}

void GenericTool::SetMouseEvent(MouseEventInterface *pEvent) {
   mouseEventList_.push_back(pEvent);
}

void GenericTool::SetPaintEvent(PaintEventInterface *pEvent) {
   paintEventList_.push_back(pEvent);
}

void GenericTool::SetWidget(Widget* pWidget, const std::string &Location) {
   widgetList_.insert(std::make_pair(Location, pWidget));
}

int GenericTool::GetId(const std::string &CommandName) const {
   std::map<std::string, int>::const_iterator it = commandNameIds_.find(CommandName);
   if (it != commandNameIds_.end())
      return it->second;
   return SuriObject::NullIntUuid;
}

/**
 * Devuelve el DatasourceManager
 * @return DatasourceManager
 */
DatasourceManagerInterface *GenericTool::GetDatasourceManager() const {
   return (pDataViewManager_) ? pDataViewManager_->GetDatasourceManager() : NULL;
}

/**
 * Devuelve el ViewportManager
 * @return ViewportManager
 */
ViewportManagerInterface *GenericTool::GetViewportManager() const {
   return (pDataViewManager_) ? pDataViewManager_->GetViewportManager() : NULL;
}

/**
 * Devuelve el ViewcontextManager
 * @return ViewcontextManager
 */
ViewcontextManagerInterface *GenericTool::GetViewcontextManager() const {
   return (pDataViewManager_) ? pDataViewManager_->GetViewcontextManager() : NULL;
}

/** Devuelve el GroupManager */
GroupManagerInterface *GenericTool::GetGroupManager() const {
   return (pDataViewManager_) ? pDataViewManager_->GetGroupManager() : NULL;
}

/** Devuelve el SelectionManager */
TreeSelectionManagerInterface *GenericTool::GetSelectionManager() const {
   return (pDataViewManager_) ? pDataViewManager_->GetSelectionManager() : NULL;
}

/** Devuelve el LibraryManager */
LibraryManager *GenericTool::GetLibraryManager() const {
   return (pDataViewManager_) ? pDataViewManager_->GetLibraryManager() : NULL;
}

/** Devuelve el TaskManager */
TaskManager *GenericTool::GetTaskManager() const {
   return (pDataViewManager_) ? pDataViewManager_->GetTaskManager() : NULL;
}

/** Devuelve el FeatureSelection */
FeatureSelection *GenericTool::GetFeatureSelection() const {
   return (pDataViewManager_) ? pDataViewManager_->GetFeatureSelection() : NULL;
}

/** Devuelve el DataView */
DataViewManager *GenericTool::GetDataViewManager() const {
   return pDataViewManager_;
}

/** 
 * Implementacion de ejecucion de acciones de una herramienta en donde se recorre los 
 * administradores de ejecucion disponibles, handlers,  (una especie de cadena de responsabilidades).
 **/
void GenericTool::Execute(const Command *pToolCommand) {
   bool commandhandled = false;
	std::list<core::CommandExecutionHandlerInterface*>::iterator it = handlers_.begin();
	for (; !commandhandled && it != handlers_.end(); ++it) {
		commandhandled = (*it)->Execute(pToolCommand, this);
	}
}

/** Agrega un administrador de ejecucion de acciones de la herramienta. **/
void GenericTool::AddCommandExecutionHandler(core::CommandExecutionHandlerInterface* pHandler) {
	handlers_.push_back(pHandler);
}

/** Agrega un creador de comandos en la cadena de responsabilidades **/
void GenericTool::AddCommandCreator(core::CommandCreatorInterface* pCreator) {
	if (pCommandCreator_)
		pCommandCreator_->AddSuccessor(pCreator);
	else
		pCommandCreator_ = pCreator;
}

Command *GenericTool::CreateCommand(const std::string &CommandName) {
	return pCommandCreator_ ? pCommandCreator_->CreateCommand(CommandName, this) : NULL;
}


/**
 * Exporta los metadatos de una capa a un archivo
 * @param[in] pDatasource fuente de datos que posee el elemento con los metadatos a exportar
 * @return true en caso de que pueda exportar correctamente los metadatos, false caso contrario
 */
bool GenericTool::ExportMetadataToFile(DatasourceInterface* pDatasource,
                                       std::string Filename) const {
   MetadataExporterInterface* pexporter = new XmlMetadataHandler();
   return pexporter->ExportMetadata(pDatasource, Filename);
}
} /* namespace suri */
