/*! \file MapEditionTool.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/MapEditionTool.h"
#include "suri/ToolCommand.h"
#include "suri/ChoiceCommand.h"
#include "suri/ParameterCollection.h"
#include "resources.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/MapEditionTask.h"
#include "suri/RasterElement.h"
#include "suri/ToolSupport.h"
#include "SpatialReference.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
MapEditionTool::MapEditionTool(DataViewManager* pDataViewManager, wxWindow* pParent) :
      GenericTool(pDataViewManager), pParent_(pParent), pMapTask_(NULL),
      pTaskConfig_(NULL), pScaleCommand_(NULL), blockedDatasource_(SuriObject::NullUuid) {
   configurationVariableName_ = "map_edition";
   Initialize();
}

/** Destructor **/
MapEditionTool::~MapEditionTool() {
   Finalize();
}

/** Ejecucion del comando */
void MapEditionTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("ShowLegend")) {
      ExecuteShowLegend(pToolCommand->IsActive());
   } else if (id == GetId("ShowScale")) {
      ExecuteShowScale(pToolCommand->IsActive());
   } else if (id == GetId("ShowNorth")) {
      ExecuteShowNorth(pToolCommand->IsActive());
   } else if (id == GetId("ShowGrid")) {
      ParameterCollection* pparam = pToolCommand->GetOutputParameters();
      std::string selection;
      pparam->GetValue<std::string>(ChoiceCommand::SelectionParameterKey, selection);
      ExecuteShowGrid(selection);
   } else if (id == GetId("TaskMenu")) {
      ExecuteTaskMenu(pToolCommand->IsActive());
   }
}

/** Creacion del Comando */
Command* MapEditionTool::CreateCommand(const std::string &CommandName) {
   Command* pcommand = NULL;
   if (CommandName.compare("ShowLegend") == 0) {
      pcommand = new ToolCommand(this);
      pcommand->SetType("ToggleButton");
      pcommand->SetGroupId(0);
      pcommand->SetDescription(_(caption_LEGEND_BUTTON));
      pcommand->SetDecoration(button_MAP_LEGEND);
      pcommand->SetLocation("ID_MAP_TOOL_PANEL");
   } else if (CommandName.compare("ShowScale") == 0) {
      pcommand = new ToolCommand(this);
      pcommand->SetType("ToggleButton");
      pcommand->SetGroupId(0);
      pcommand->SetDescription(_(caption_SCALE_BUTTON));
      pcommand->SetDecoration(button_MAP_SCALE);
      pcommand->SetLocation("ID_MAP_TOOL_PANEL");
      pScaleCommand_ = pcommand;
   } else if (CommandName.compare("ShowNorth") == 0) {
      pcommand = new ToolCommand(this);
      pcommand->SetType("ToggleButton");
      pcommand->SetGroupId(0);
      pcommand->SetDescription(_(caption_NORTH_BUTTON));
      pcommand->SetDecoration(button_MAP_NORTH_ARROW);
      pcommand->SetLocation("ID_MAP_TOOL_PANEL");
   } else if (CommandName.compare("ShowGrid") == 0) {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Choice");
      pcommand->SetGroupId(0);
      pcommand->SetDescription(_(label_GRID));
      ParameterCollection* pparams = new ParameterCollection;
      pparams->AddValue<std::string>(ChoiceCommand::LabelParameterKey, "Grilla");
      pparams->AddValue<std::string>(ChoiceCommand::InitialSelectionParameterKey,
                                     label_NO_GRID);
      std::vector < std::string > *pvalues = new std::vector<std::string>;
      pvalues->push_back(_(label_NO_GRID));
      pvalues->push_back(_(label_1_DEGREE));
      pvalues->push_back(_(label_2_DEGREES));
      pvalues->push_back(_(label_5_DEGREES));
      pvalues->push_back(_(label_10_DEGREES));
      pvalues->push_back(_(label_20_DEGREES));
      pvalues->push_back(_(label_30_DEGREES));
      pparams->AddValue<std::vector<std::string>*>(
            ChoiceCommand::OptionInputParameterKey, pvalues);
      pcommand->SetInputParameters(pparams);
      pcommand->SetLocation("ID_MAP_TOOL_PANEL");
   } else if (CommandName.compare("TaskMenu") == 0) {
      pcommand = new ToolCommand(this);
      pcommand->SetType("ToggleButton|Menu");
      pcommand->SetGroupId(-64);
      pcommand->SetDescription(_(tooltip_TASK_MENU));
      pcommand->SetDecoration(tool_EDIT_TASK);
      pcommand->SetLocation("ID_MAP_TOOL_PANEL|ID_EDIT_MAP_MENUTIEM");
      pcommand->SetAssociatedTaskId("MapEditionTask");
   }
   return pcommand;
}

/** Destruccion del Comando */
void MapEditionTool::DestroyCommand(Command* &pCommand) const {
}

/** Creacion del Widget */
Widget* MapEditionTool::CreateWidget(const std::string &Widget) {
   return NULL;
}

/** Inicia la tarea de edicion de mapa */
bool MapEditionTool::StartTask() {
   ViewcontextManagerInterface* pvmgr = GetViewcontextManager();
   RasterElement* praster =
         ToolSupport::GetRaster(pvmgr, pvmgr->GetSelectedViewcontextId());
   if (!praster)
      return false;
   World* pworld = new World;
   Subset subset;
   praster->GetElementExtent(subset);
   LayerInterface* player = pvmgr->GetSelectedViewcontext()->GetLayerByElementId(praster->GetUid());
   if (player) {
      blockedDatasource_ = player->GetAssociatedDatasourceId();
      GetDatasourceManager()->BlockDatasource(blockedDatasource_);
   }
   std::string sr = praster->GetSpatialReference().c_str();
   pworld->SetSpatialReference(sr.c_str());
   pworld->SetWorld(subset);
   pworld->SetWindow(subset);
   if (!pMapTask_)
      pMapTask_ = new MapEditionTask(pworld, pParent_);
   ParameterCollection params;
   params.AddValue<DataViewManager*>(TaskConfigBuilder::kDataViewParam, GetDataViewManager());
   params.AddValue<MapEditionTask*>(TaskConfigBuilder::kTaskParam, pMapTask_);
   LayerList* plist = GetViewcontextManager()->GetSelectedViewcontext()->GetLayerList();
   params.AddValue<LayerList*>(MapEditionTask::ELEMENTLIST, plist);
   pTaskConfig_ = GetTaskManager()->GetTaskConfig(MapEditionTask::TASKNAME, params);

   if (pTaskConfig_)
      pTaskConfig_->Configure();
   if (pMapTask_->IsActive())
      return false;
   bool started = pMapTask_->Start();
   if (started && pScaleCommand_) {
      pScaleCommand_->SetEnabled(SpatialReference::IsProjectedSpatialRef(sr));
   }
   return started;
}

/** Finaliza la tarea de edicion de mapa **/
bool MapEditionTool::EndTask() {
   if (!pMapTask_)
      return false;

   if (!pMapTask_->End())
      return false;

   pTaskConfig_->Clear();
   delete pTaskConfig_;
   pTaskConfig_ = NULL;
   GetDatasourceManager()->UnblockDatasource(blockedDatasource_);
   return true;
}

/** Metodo que se ejecuta cuando se apreta boton de mostrar leyenda */
void MapEditionTool::ExecuteShowLegend(bool Show) {
   if (!pMapTask_ || !pMapTask_->IsActive()) {
      SHOW_ERROR("Para poder ejecutar esta accion primero debe inicializar la tarea "\
            "haciendo click sobre el boton");
      return;
   }
   pMapTask_->ShowLegend(Show);
}

/** Metodo que se ejecuta cuando se apreta boton de mostrar norte */
void MapEditionTool::ExecuteShowNorth(bool Show) {
   if (!pMapTask_->IsActive()) {
      SHOW_ERROR("Para poder ejecutar esta accion primero debe inicializar la tarea "\
            "haciendo click sobre el boton");
      return;
   }
   pMapTask_->ShowNorth(Show);
}

/** Metodo que se ejecuta cuando se apreta boton de mostrar escala */
void MapEditionTool::ExecuteShowScale(bool Show) {
   if (!pMapTask_->IsActive()) {
      SHOW_ERROR("Para poder ejecutar esta accion primero debe inicializar la tarea "\
            "haciendo click sobre el boton");
      return;
   }
   pMapTask_->ShowScale(Show);
}

/** Metodo que se ejecuta cuando se apreta choice de grilla */
void MapEditionTool::ExecuteShowGrid(const std::string& GridValue) {
   if (!pMapTask_->IsActive()) {
      SHOW_ERROR("Para poder ejecutar esta accion primero debe inicializar la tarea "\
            "haciendo click sobre el boton");
      return;
   }
   pMapTask_->ShowGrid(GridValue);
}

/** Metodo que se ejecuta cuando se apreta boton de iniciar/finalizar tarea */
void MapEditionTool::ExecuteTaskMenu(bool Active) {
   if (Active)
      StartTask();
   else
      EndTask();
}

} /** namespace suri */
