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

// Includes standard
#include <string>

// Includes suri
#include "suri/messages.h"
#include "suri/VectorEditionTool.h"
#include "suri/ToggleButton.h"
#include "VectorDatasourceSelectionSource.h"
#include "VectorEditionTaskEvent.h"
#include "suri/FeatureSelection.h"
#include "GeometrySelectionEvent.h"
#include "GeometryCreationEvent.h"
#include "GeometryEditionEvent.h"
#include "suri/ToolCommand.h"
#include "suri/EventCommand.h"
#include "CompositeCommand.h"
#include "resources.h"
#include "suri/ToolSupport.h"
#include "suri/VectorEditionTask.h"
#include "suri/ParameterCollection.h"
#include "PointDeleteEvent.h"
#include "GeometryDeleteEvent.h"
#include "suri/VectorEditionTable.h"
#include "suri/BufferedDriver.h"
#include "SpatialReference.h"
#include "suri/DatasourceManipulatorInterface.h"
#include "suri/DatasourceValidatorInterface.h"
#include "EnclosureManipulator.h"
#include "suri/Configuration.h"
#include "suri/DynCommandButton.h"
#include "suri/AuxiliaryFunctions.h"
#include "SnapPart.h"
#include "suri/SnapTool.h"

// Includes OGR
#include "ogrsf_frmts.h"

// defines
#define message_DATASOURCE_FAIL_VALIDATION "La fuente de datos no paso la validacion."
#define message_TASK_INICIALIZATION_FAIL "No se pudo inicializar la tarea correctamente"

namespace suri {

#define tooltip_VECTOREDITION_TOOLTIP_DESC "Iniciar / Finalizar edicion vectorial"

/** Ctor */
VectorEditionTool::VectorEditionTool(DataViewManager* pDataViewManager,
                                     DatasourceValidatorInterface* pValidator) :
      GenericTool(pDataViewManager), pTaskConfig_(NULL), pSnapTool_(NULL) {
   pVectorEditionTask_ = new VectorEditionTask();
   pVectorEditionTask_->SetFeatureSelector(GetFeatureSelection());
   pVectorEditionTask_->SetViewportManager(pDataViewManager->GetViewportManager());
   pMemoryVector_ = NULL;
   pTaskCommand_ = NULL;
   pValidator_ = pValidator;
}

/** Ctor */
VectorEditionTool::VectorEditionTool(DataViewManager* pDataViewManager) :
      GenericTool(pDataViewManager), pTaskConfig_(NULL), pSnapTool_(NULL) {
   configurationVariableName_ = "vectoredition";
   pVectorEditionTask_ = new VectorEditionTask();
   pVectorEditionTask_->SetFeatureSelector(GetFeatureSelection());
   pVectorEditionTask_->SetViewportManager(pDataViewManager->GetViewportManager());
   pMemoryVector_ = NULL;
   pTaskCommand_ = NULL;
   pMemoryDatasource_ = NULL;
   pValidator_ = NULL;
   Initialize();
}

/** Dtor */
VectorEditionTool::~VectorEditionTool() {
   Finalize();

   delete pMemoryDatasource_;
   Vector::Close(pMemoryVector_);
}

/**
 * Si el comando indicado, se encuentra entre la lista de ejecutables, se ejecutara
 * @param[in]  pToolCommand: Comando a ejecutar
 */
void VectorEditionTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("SelectFeatures") && pToolCommand->IsActive()) {
      ExecuteStartSelectVectorFeatures();
   } else if (id == GetId("SelectFeatures") && !pToolCommand->IsActive()) {
      ExecuteEndSelection();
   } else if (id == GetId("CreateVector")) {
      ExecuteCreateVectorDatasource();
   } else if (id == GetId("CreatePolygon")) {
      ExecuteCreatePolygonDatasource();
   } else if (id == GetId("CreateLine")) {
      ExecuteCreateLineDatasource();
   } else if (id == GetId("CreatePoint")) {
      ExecuteCreatePointDatasource();
   } else if (id == GetId("TaskMenu") && pToolCommand->IsActive()) {
      ExecuteStartTask();
   } else if (id == GetId("TaskMenu") && !pToolCommand->IsActive()) {
      ExecuteEndTask();
   } else if (id == GetId("EditFeature") && pToolCommand->IsActive()) {
      Configuration::SetParameterEx(VETYPE_PARAM_MODE_NAME,
      VETYPE_PARAM_MODE_EDIT);
   } else if (id == GetId("EditFeature") && !pToolCommand->IsActive()) {
      ExecuteEndEditFeatureDatasource();
      std::string m = Configuration::GetParameterEx(VETYPE_PARAM_MODE_NAME,
      VETYPE_PARAM_MODE_UNDEFINED);
      if (m.compare(VETYPE_PARAM_MODE_EDIT) == 0)
         Configuration::RemoveParameterEx(VETYPE_PARAM_MODE_NAME);
   } else if (id == GetId("DeletePoint") && pToolCommand->IsActive()){
      Configuration::SetParameterEx(VETYPE_PARAM_MODE_NAME,
            VETYPE_PARAM_MODE_DELETE_NODE);
   } else if (id == GetId("DeletePoint") && !pToolCommand->IsActive()) {
      Configuration::RemoveParameterEx(VETYPE_PARAM_MODE_NAME);
      ExecuteEndDeletePointDatasource();
   } else if (id == GetId("CreateFeature") && pToolCommand->IsActive()) {
      if (EndEditFeatureIsNecessary()){
         ExecuteEndEditFeatureDatasource();
      }
      Configuration::SetParameterEx(VETYPE_PARAM_MODE_NAME,
      VETYPE_PARAM_MODE_CREATE);
   } else if (id == GetId("CreateFeature") && !pToolCommand->IsActive()) {
      ExecuteEndCreateFeatureDatasource();
      std::string m = Configuration::GetParameterEx(VETYPE_PARAM_MODE_NAME,
      VETYPE_PARAM_MODE_UNDEFINED);
      if (m.compare(VETYPE_PARAM_MODE_CREATE) == 0)
         Configuration::RemoveParameterEx(VETYPE_PARAM_MODE_NAME);
   } else if (id == GetId("SnapProperties") && pToolCommand->IsActive()) {
      ExecuteStartSnapTool();
   } else if (id == GetId("SnapProperties") && !pToolCommand->IsActive()) {
      ExecuteEndSnapTool();
   }
}

/** 
 * Creacion del Comando
 * @param[in] CommandName nombre del comando
 * @return referencia a comando creado
 * @return NULL si el nombre es invalido
 */
Command* VectorEditionTool::CreateCommand(const std::string &CommandName) {
   Command* pcommand = NULL;
   if (CommandName == "SelectFeatures") {
      // Configuro evento que maneja seleccion
      GeometrySelectionEvent* pevent = new GeometrySelectionEvent(
            GetViewportManager(), GetViewcontextManager(), GetFeatureSelection());
      SetMouseEvent(pevent);
      SetPaintEvent(pevent);
      Command* peventcommand = new EventCommand(pevent, pevent);

      // Configuro evento que inicializa seleccion
      Command* ptoolcommand = new ToolCommand(this);

      // Configuro CompositeCommand
      CompositeCommand* pcomposite = new CompositeCommand();
      pcomposite->AppendCommand(ptoolcommand);
      pcomposite->AppendCommand(peventcommand);
      pcomposite->SetType("ToggleButton");
      pcomposite->SetDescription(_(tooltip_SELECT_GEOMETRIES));
      pcomposite->SetDecoration(tool_SELECT_VECTOR);
      pcomposite->SetLocation("ID_VECTOR_TOOL_PANEL");

      pcommand = pcomposite;

   } else if (CommandName == "CreateVector") {
      // Configuro evento que inicializa creacion
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(_(tooltip_CREATE_POLYGON_VECTOR));
      pcommand->SetDecoration(tool_DRAW_POLYGON);
      pcommand->SetLocation("ID_VECTOR_TOOL_PANEL");

   } else if (CommandName == "CreatePolygon") {
      // Configuro evento que inicializa creacion
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(_(tooltip_CREATE_POLYGON_VECTOR));
      pcommand->SetDecoration(tool_DRAW_POLYGON);
      pcommand->SetLocation("ID_VECTOR_TOOL_PANEL");

   } else if (CommandName == "CreateLine") {
      // Configuro evento que inicializa creacion
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(_(tooltip_CREATE_LINE_VECTOR));
      pcommand->SetDecoration(tool_DRAW_LINE);
      pcommand->SetLocation("ID_VECTOR_TOOL_PANEL");

   } else if (CommandName == "CreatePoint") {
      // Configuro evento que inicializa creacion
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(_(tooltip_CREATE_POINT_VECTOR));
      pcommand->SetDecoration(tool_DRAW_POINT);
      pcommand->SetLocation("ID_VECTOR_TOOL_PANEL");

   } else if (CommandName == "TaskMenu") {
      // Configuro evento que maneja seleccion
      VectorEditionTaskEvent* pevent = new VectorEditionTaskEvent(
            GetViewportManager(), GetViewcontextManager(), pVectorEditionTask_);
      GetFeatureSelection()->Attach(pevent);
      SetPaintEvent(pevent);
      SetMouseEvent(pevent);
      Command* peventcommand = new EventCommand(pevent, pevent);

      // Configuro evento que inicializa seleccion
      Command* ptoolcommand = new ToolCommand(this);

      // Configuro CompositeCommand
      CompositeCommand* pcomposite = new CompositeCommand();
      pcomposite->AppendCommand(ptoolcommand);
      pcomposite->AppendCommand(peventcommand);
      pcommand = pcomposite;

      pcommand->SetType("DynButton|Menu");
      pcommand->SetDescription(_(tooltip_VECTOREDITION_TOOLTIP_DESC));
      pcommand->SetGroupId(-64);
      pcommand->SetDecoration(button_START_ANIMATION_SRC);
      pcommand->SetLocation("ID_VECTOR_TOOL_PANEL|ID_EDIT_VECTOR_MENUTIEM");
      pcommand->SetAssociatedTaskId("VectorEditionTask");

      ParameterCollection* pparams = new ParameterCollection();
      pparams->AddValue<std::string>(DynCommandButton::kActivatedBitmap,
      button_STOP_ANIMATION);
      pparams->AddValue<std::string>(DynCommandButton::kDeactivatedBitmap,
      button_START_ANIMATION_SRC);
      pparams->AddValue<std::string>(DynCommandButton::kActivatedTooltip,
      tooltip_TASK_MENU);
      pparams->AddValue<std::string>(DynCommandButton::kDeactivatedTooltip,
      tooltip_TASK_MENU);
      pcommand->SetInputParameters(pparams);

      pTaskCommand_ = pcommand;

   } else if (CommandName == "EditFeature") {
      GeometryEditionEvent* pevent = new GeometryEditionEvent(GetViewportManager(),
                                                              GetViewcontextManager(),
                                                              pVectorEditionTask_);
      SetMouseEvent(pevent);
      SetPaintEvent(pevent);
      Command* peventcommand = new EventCommand(pevent, pevent);

      // Configuro evento que inicializa seleccion
      Command* ptoolcommand = new ToolCommand(this);

      // Configuro CompositeCommand
      CompositeCommand* pcomposite = new CompositeCommand();
      pcomposite->AppendCommand(ptoolcommand);
      pcomposite->AppendCommand(peventcommand);
      pcomposite->SetType("ToggleButton");
      pcomposite->SetDescription(_(tooltip_EDIT_VECTOR));
      pcomposite->SetDecoration(tool_EDIT_VECTOR);
      pcomposite->SetLocation("ID_VECTOR_TOOL_PANEL");

      pcommand = pcomposite;

   } else if (CommandName == "CreateFeature") {
      // Configuro evento que maneja seleccion
      GeometryCreationEvent* pevent = new GeometryCreationEvent(GetViewportManager(),
                                                                GetViewcontextManager(),
                                                                pVectorEditionTask_);
      SetMouseEvent(pevent);
      SetPaintEvent(pevent);
      Command* peventcommand = new EventCommand(pevent, pevent);
      Command* ptoolcommand = new ToolCommand(this);

      // Configuro CompositeCommand
      CompositeCommand* pcomposite = new CompositeCommand();
      pcomposite->AppendCommand(ptoolcommand);
      pcomposite->AppendCommand(peventcommand);
      pcomposite->SetType("ToggleButton");
      pcomposite->SetDescription(_(tooltip_CREATE_FEATURE));
      pcomposite->SetDecoration(tool_CREATE_FEATURE);
      pcomposite->SetLocation("ID_VECTOR_TOOL_PANEL");

      pcommand = pcomposite;

   } else if (CommandName == "DeleteFeature") {
      // Configuro evento que maneja seleccion
      GeometryDeleteEvent* pevent = new GeometryDeleteEvent(GetViewportManager(),
                                                            GetViewcontextManager(),
                                                            pVectorEditionTask_);
      SetMouseEvent(pevent);
      SetPaintEvent(pevent);
      pcommand = new EventCommand(pevent, pevent);
      pcommand->SetType("ToggleButton");
      pcommand->SetDescription(_(tooltip_REMOVE_GEOMETRY));
      pcommand->SetDecoration(button_DELETE_GEOMETRY);
      pcommand->SetLocation("ID_VECTOR_TOOL_PANEL");

   } else if (CommandName == "DeletePoint") {
      // Configuro evento que maneja seleccion
      PointDeleteEvent* pevent = new PointDeleteEvent(GetViewportManager(),
                                                      GetViewcontextManager(),
                                                      pVectorEditionTask_);
      SetMouseEvent(pevent);
      SetPaintEvent(pevent);
      Command* peventcommand = new EventCommand(pevent, pevent);
      Command* ptoolcommand = new ToolCommand(this);

      // Configuro CompositeCommand
      CompositeCommand* pcomposite = new CompositeCommand();
      pcomposite->AppendCommand(ptoolcommand);
      pcomposite->AppendCommand(peventcommand);
      pcommand = pcomposite;
      pcommand->SetType("ToggleButton");
      pcommand->SetDescription(_(tooltip_DELETE_POINT));
      pcommand->SetDecoration(tool_DELETE_POINT);
      pcommand->SetLocation("ID_VECTOR_TOOL_PANEL");

   } else if (CommandName == "ShowTable") {
      // Configuro evento que inicializa creacion
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(_(tooltip_EDIT_TABLE));
      pcommand->SetDecoration(tool_SHOW_TABLE);
      pcommand->SetLocation("ID_VECTOR_TOOL_PANEL");
   } else if (CommandName == "SnapProperties") {
      Command* ptoolcommand = new ToolCommand(this);
      CompositeCommand* pcomposite = new CompositeCommand();
      pcomposite->AppendCommand(ptoolcommand);
      pcommand = pcomposite;
      pcommand->SetType("ToggleButton");
      pcommand->SetDescription(_(tooltip_SNAP_PROPERTIES));
      pcommand->SetDecoration(tool_SNAP_PROPERTIES);
      pcommand->SetLocation("ID_VECTOR_TOOL_PANEL");
   }
   return pcommand;
}

/** 
 * Destruccion del Comando 
 * @param[out] pCommand comando a destruir
 */
void VectorEditionTool::DestroyCommand(Command* &pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/** 
 * Creacion del Widget 
 * @return Siempre NULL.
 */
Widget* VectorEditionTool::CreateWidget(const std::string &Widget) {
   return NULL;
}

/**
 * Inicia la tarea de edicion vectorial
 * @param[in] pDatasource datasource de trabajo
 * @return bool que indica si tuvo exito.
 */
bool VectorEditionTool::StartTask(VectorDatasource* pDatasource) {
   if (pVectorEditionTask_->IsActive()) {
      SHOW_WARNING(_(message_ELEMENT_IN_EDITION));
      return false;
   }

   if (pValidator_ && !pValidator_->IsValid(pDatasource)) {
      SHOW_ERROR(_(message_DATASOURCE_FAIL_VALIDATION));
      pTaskCommand_->SetActive(false);
      return false;
   }

   RasterElement* praster = ToolSupport::GetActiveRaster(
         GetViewcontextManager(), GetViewcontextManager()->GetSelectedViewcontextId());
   if (!praster
         || SpatialReference::IsPixelLineSpatialRef(
               praster->GetSpatialReference().c_str())) {
      pTaskCommand_->SetActive(false);
      SHOW_WARNING(message_CANT_EDIT_ELEMENT_WITHOUT_SR);
      return false;
   }

   ParameterCollection params;
   params.AddValue<DataViewManager*>("DataView", GetDataViewManager());
   params.AddValue<VectorEditionTask*>("Task", pVectorEditionTask_);
   params.AddValue<VectorDatasource*>("VectorDatasource", pDatasource);
   params.AddValue<DatasourceInterface*>(
         "RasterDatasource",
         GetDatasourceManager()->GetDatasourceByElementId(praster->GetUid()));
   params.AddValue<std::string>("ToolName", configurationVariableName_);
   pTaskConfig_ = GetTaskManager()->GetTaskConfig(VectorEditionTask::TASKNAME, params);
   if (pTaskConfig_) 
		pTaskConfig_->Configure();
	

   if (!pVectorEditionTask_->Start()) {
      pTaskCommand_->SetActive(false);
      return false;
   }
	/** TODO(Gabriel - #6672): a partir de la seleccion almancenada en el xml configurar
		la seleccion en el featureselection  **/
   ViewcontextInterface* pactivevc = GetViewcontextManager()->GetSelectedViewcontext();
  	LayerInterface* player = pDatasource ? pactivevc->GetAssociatedLayer(pDatasource->GetId()) : NULL; 
	wxXmlNode* pfnode = player ? 	player->GetElement()->
											GetNode(VECTOR_SELECTION_NODE NODE_SEPARATION_TOKEN FID_SELECTION) :
											NULL;
	if (player && pfnode) {
 		std::string fidselection = pfnode->GetNodeContent().c_str();
		std::vector<std::string> fids = tokenizer(fidselection, ",");
		for (std::vector<std::string>::iterator fid = fids.begin(); fid != fids.end(); 
		++fid) {
			GetFeatureSelection()->Select(StringToNumber<long>(*fid), pDatasource->GetId());
		}
	}

   pTaskCommand_->SetActive(true);
   return true;
}

/**
 * Inicia la tarea de edicion vectorial con un vector nuevo
 * @param[in] NewVectorType tipo de vector a crear
 * @param[in] pManipulator manipulador que hay que aplicar sobre el datasource
 * creado
 * @return bool que indica si tuvo exito.
 */
bool VectorEditionTool::StartTask(Vector::VectorType NewVectorType,
                                  DatasourceManipulatorInterface* pManipulator) {
   if (pVectorEditionTask_->IsActive())
      return false;

   std::string filename;
   pMemoryVector_ = CreateMemoryVector(NewVectorType, filename);
   if (!pMemoryVector_) {
      pTaskCommand_->SetActive(false);
      return false;
   }

   pMemoryDatasource_ = VectorDatasource::Create(filename);
   if (pMemoryDatasource_ == NULL
         || (pManipulator && !pManipulator->AddNature(pMemoryDatasource_))
         || !StartTask(pMemoryDatasource_)) {
      delete pMemoryDatasource_;
      Vector::Close(pMemoryVector_);
      pTaskCommand_->SetActive(false);
      return false;
   }

   return true;
}

/** 
 * Crea un vector en memoria del tipo indicado.
 * @param[in] NewVectorType tipo de vector a crear
 * @param[out] MemUrl url temporal en donde se genera el vector
 * @return puntero a vector en memoria
 */
Vector* VectorEditionTool::CreateMemoryVector(Vector::VectorType NewVectorType,
                                              std::string& MemUrl) const {
   Vector* pmemoryvector = NULL;
   static int vectornumber = 0;
   ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   std::string spatialReference = pviewer->GetWorld()->GetSpatialReference();
   MemUrl = "shpmemory:NewVector" + NumberToString(vectornumber);
   std::string memoryvectorurl = MemUrl + ":" + spatialReference;
   ++vectornumber;
   pmemoryvector = Vector::Open(memoryvectorurl, Vector::ReadWrite, 0, NewVectorType);
   if (pmemoryvector)
      pmemoryvector->CreateLayer("NewLayer", spatialReference,
                                 NewVectorType);

   return pmemoryvector;
}

/** Inicia la seleccion de features */

void VectorEditionTool::ExecuteStartSelectVectorFeatures() const {
   // Si se esta editando, no se pueden seleccionar features de otro vector
   if (pVectorEditionTask_->IsActive()) return;

   VectorDatasource* pdatasource = ToolSupport::GetVectorDatasource(
         GetDatasourceManager(), GetViewcontextManager());

   if (pValidator_ && !pValidator_->IsValid(pdatasource)) {
      SHOW_ERROR(_(message_DATASOURCE_FAIL_VALIDATION));
      return;
   }

   if (pdatasource) {
      GetFeatureSelection()->Configure(
            new VectorDatasourceSelectionSource(pdatasource, NULL));
	   GetDatasourceManager()->BlockDatasource(pdatasource->GetId());
   }
}

/** Metodo auxiliar que persiste la seleccion dentro de la capa **/
void VectorEditionTool::SaveVectorSelectionToLayer(LayerInterface* pLayer) const {   
   std::vector<FeatureIdType> fidselected = GetFeatureSelection()->GetSelectedFeatures();
   if (pLayer && !fidselected.empty())  {
		// Si se selecciono alguna geometria 
	   wxXmlNode* pselnode = pLayer->GetElement()->GetNode(VECTOR_SELECTION_NODE);
      wxXmlNode* pnewselnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, VECTOR_SELECTION_NODE);
		wxXmlNode* pquerynode = new wxXmlNode(pnewselnode, wxXML_ELEMENT_NODE, FID_SELECTION);
		std::string fidselectionstr;
		for (std::vector<FeatureIdType>::iterator fid = fidselected.begin(); 
		                                                         fid != fidselected.end(); ++fid) {
			fidselectionstr += NumberToString<long>(*fid);
			if (fid + 1 != fidselected.end())
				fidselectionstr +=",";
		}
		new wxXmlNode(pquerynode, wxXML_TEXT_NODE, wxEmptyString, fidselectionstr);	
      if (!pselnode)
         pLayer->GetElement()->AddNode(pLayer->GetElement()->GetNode(""), pnewselnode);
      else
         pLayer->GetElement()->ReplaceNode(pLayer->GetElement()->GetNode(""), pnewselnode, pselnode);
		
		// Dejo en la fuente de datos la ultima seleccion
		DatasourceInterface* pdatasource =  
									GetDatasourceManager()->GetDatasource(pLayer->GetAssociatedDatasourceId());
		
		pselnode = pdatasource->GetElement()->GetNode(VECTOR_SELECTION_NODE);
		new wxXmlNode(pquerynode, wxXML_TEXT_NODE, wxEmptyString, fidselectionstr);	
		pnewselnode = new wxXmlNode(*pnewselnode);

		if (!pselnode)
         pdatasource->GetElement()->AddNode(pdatasource->GetElement()->GetNode(""), pnewselnode);
      else
         pdatasource->GetElement()->ReplaceNode(pdatasource->GetElement()->GetNode(""), pnewselnode, pselnode);

   }
}

/** Finaliza la seleccion de features **/
void VectorEditionTool::ExecuteEndSelection() const {
   if (pVectorEditionTask_->IsActive()) return;
	DatasourceInterface* pdatasource =  GetDatasourceManager()->GetDatasource(
	                                                GetFeatureSelection()->GetWorkingDatasourceId());
   ViewcontextInterface* pactivevc = GetViewcontextManager()->GetSelectedViewcontext();
   LayerInterface* player =
                        pdatasource ? pactivevc->GetAssociatedLayer(pdatasource->GetId()) : NULL;
   SaveVectorSelectionToLayer(player);
	GetDatasourceManager()->UnblockDatasource(GetFeatureSelection()->GetWorkingDatasourceId());
	GetFeatureSelection()->Configure(NULL);
   RefreshViewer();
}

/** Crea una capa vectorial en memoria de tipo correcto */
void VectorEditionTool::ExecuteCreateVectorDatasource() {
}

/** Crea una capa vectorial en memoria de tipo correcto */
void VectorEditionTool::ExecuteCreatePolygonDatasource() {
   StartTask(Vector::Polygon);
}

/** Crea una capa vectorial en memoria de tipo correcto */
void VectorEditionTool::ExecuteCreateLineDatasource() {
   StartTask(Vector::Line);
}

/** Crea una capa vectorial en memoria de tipo correcto */
void VectorEditionTool::ExecuteCreatePointDatasource() {
   StartTask(Vector::Point);
}

/** Inicializa la herramienta de snapping */
void VectorEditionTool::ExecuteStartSnapTool() {
   RasterElement* praster = ToolSupport::GetActiveRaster(
         GetViewcontextManager(), GetViewcontextManager()->GetSelectedViewcontextId());
   pSnapTool_ = new SnapTool(pVectorEditionTask_->GetTable(), pVectorEditionTask_->GetWorld(),
                             praster->GetRasterModel());
   ui::SnapPart* psnappart = new ui::SnapPart();
   psnappart->SetTool(pSnapTool_);
   if (psnappart->CreateTool() && psnappart->ShowModal() == wxID_OK)
      pVectorEditionTask_->SetSnapTool(pSnapTool_);
   else {
      delete pSnapTool_;
      pSnapTool_ = NULL;
      pVectorEditionTask_->SetSnapTool(pSnapTool_);
   }
}

/** Finaliza la herramienta de snapping */
void VectorEditionTool::ExecuteEndSnapTool() {
   if (pSnapTool_) {
      delete pSnapTool_;
      pSnapTool_ = NULL;
      pVectorEditionTask_->SetSnapTool(pSnapTool_);
   }
}

/**
 * Inicia la tarea de edicion vectorial
 */

void VectorEditionTool::ExecuteStartTask() {
   VectorDatasource* pdatasource = ToolSupport::GetVectorDatasource(
         GetDatasourceManager(), GetViewcontextManager());
   if (!pdatasource) {
      SHOW_WARNING(message_REQUIRED_VECTOR_ELEMENT);
      pTaskCommand_->SetActive(false);
      return;
   }
   StartTask(pdatasource);
}

/** Finaliza la tarea de edicion vectorial */
void VectorEditionTool::ExecuteEndTask() {
   DatasourceInterface* pdatasource =  GetDatasourceManager()->GetDatasource(GetFeatureSelection()->GetWorkingDatasourceId());
   ViewcontextInterface* pactivevc = GetViewcontextManager()->GetSelectedViewcontext();
   LayerInterface* player = pdatasource ? pactivevc->GetAssociatedLayer(pdatasource->GetId()) : NULL;
	SaveVectorSelectionToLayer(player);
  	if (!pVectorEditionTask_->End())
   	SHOW_ERROR(_(message_CANT_FINISH_TASK));

   if (pTaskConfig_) 
		pTaskConfig_->Clear();

   if (pMemoryVector_) {
      // Trata de guardarlo a disco
      std::string filename;
      if (GetOutputFileName(filename)) {
         SaveMemoryDatasourceToDisk(filename);
      }
      delete pMemoryDatasource_;
      Vector::Close(pMemoryVector_);
   }

   delete pTaskConfig_;
   pTaskConfig_ = NULL;
//   GetDatasourceManager()->UnblockAllDatasource();
   RefreshViewer();
}

/** Finaliza la edicion grafica de features, no finaliza la tarea. */
void VectorEditionTool::ExecuteEndEditFeatureDatasource() const {
   pVectorEditionTask_->EndFeatureEdition();
   RefreshViewer();
}

/** Finaliza DeletePoint, no finaliza la tarea. */
void VectorEditionTool::ExecuteEndDeletePointDatasource() const {
   pVectorEditionTask_->EndFeatureEdition();
   RefreshViewer();
}

/** Finaliza la creacion grafica de features, no finaliza la tarea. */
void VectorEditionTool::ExecuteEndCreateFeatureDatasource() const {
   pVectorEditionTask_->EndFeatureEdition();
   RefreshViewer();
}

/** Refresca el visualizador en edicion */
void VectorEditionTool::RefreshViewer() const {
   ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   pviewer->GetWindow()->Refresh();
}

/**
 * Abre una ventana de dialogo para seleccionar la ruta y nombre para guardar
 * un vector.
 * @param[out] Filename ruta y nombre del archivo a salvar.
 * @return false si el usuario cancelo
 */
bool VectorEditionTool::GetOutputFileName(std::string &Filename) {
   wxFileDialog dlg(NULL, _(caption_SAVE_VECTOR), wxT(""), wxT(""),
   filter_SAVE_VECTOR,
                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

   if (dlg.ShowModal() == wxID_OK) {
      Filename = dlg.GetPath();
		if (Filename.find(".shp") == std::string::npos)
			Filename += ".shp";
      return true;
   }
   return false;
}

/**
 * Copia el datasource en memoria a disco
 * @param[in] Filename nombre del shapefile en disco.
 */
bool VectorEditionTool::SaveMemoryDatasourceToDisk(const std::string &Filename) {
   bool result = true;
   if (wxFileName::FileExists(Filename)) {
      if (!CanDeleteVectorDatasource(Filename)) {
         SHOW_ERROR(message_CANT_DELETE_VECTOR);
         return false;
      }

      // Prueba eliminarlo de la lista, por si ademas de existir esta abierto
      // Crea un elemento para obtener el UID
      Element *pelement = VectorElement::Create(Filename);
      if (pelement) {
         Element::UidType uid = pelement->GetUid();
         Element::Destroy(pelement);
         GetDatasourceManager()->RemoveDatasourceByElementId(uid);
      }
      if (!DeleteVectorDatasource(Filename)) {
         SHOW_ERROR(message_VECTOR_DELETION_ERROR);
         return false;
      }
   }

   if (!Filename.empty() && !wxFileName::FileExists(Filename)
         && pMemoryVector_->Save(Filename, "ESRI Shapefile")) {
      DatasourceInterface* pdatasource = suri::DatasourceInterface::Create(
            "VectorDatasource", Filename);
      if (pdatasource) {
         // TODO(Ramiro - TCK #3353) En el futuro remplazar por metodo
         // generico que permita copiar todos los manipuladores aplicados
         // a la fuente de datos original. Para mas informacion ver TCK #3353.
         EnclosureManipulator manipulator;
         manipulator.CopyNature(pMemoryDatasource_, pdatasource);

         result = GetDatasourceManager()->AddDatasource(pdatasource);
         if (result) {
            ViewcontextInterface* pactivevc =
                  GetViewcontextManager()->GetSelectedViewcontext();
            LayerInterface* player = pactivevc->GetAssociatedLayer(
                  pdatasource->GetId());
            if (player) {
               player->GetElement()->Activate(true);
            }
         }
      }
   } else {
      SHOW_ERROR("Fallo al guardar el vector");
      return false;
   }
   return result;
}

/**
 * Analiza si se puede eliminar la fuente de datos vectorial y sus archivos asociados
 * Trata de abrir el datasource con el filename recibido, si no lo puede abrir
 * retorna false.
 * Recorre las capas, si en alguna no tiene permiso de escritura retorna false
 * @param[in] Filename nombre del archivo que se desea eliminar
 * @return informa si se puede eliminar archivo
 */
bool VectorEditionTool::CanDeleteVectorDatasource(const std::string &Filename) {
   // Verifico que sea un vector valido
   OGRDataSource *pdatasource = OGRSFDriverRegistrar::Open(Filename.c_str());
   if (!pdatasource) {
      return false;
   }

   OGRSFDriver *pdriver = pdatasource->GetDriver();
   if (!pdriver || !pdriver->TestCapability(ODrCDeleteDataSource)) {
      return false;
   }

   OGRDataSource::DestroyDataSource(pdatasource);

   // Verifico que el elemento no este abierto
   VectorEditor vectoreditor;
   vectoreditor.OpenVector(Filename);
   int layerindex = 0;
   while (vectoreditor.OpenLayer(layerindex)) {
      if (!vectoreditor.CanWrite()) {
         vectoreditor.CloseVector();
         return false;
      }
      layerindex++;
   }
   vectoreditor.CloseVector();

   return true;
}

/**
 * Elimina un datasource vectorial y todos los archivos asociados.
 * @param[in] Filename url del shapefile que quiero eliminar
 * @return true si pudo borrar el datasource (todos los archivos asociados).
 * \todo este metodo deberia estar en vector.cpp
 */
bool VectorEditionTool::DeleteVectorDatasource(const std::string &Filename) {
   /** Elimino shapefile */
   if (!CanDeleteVectorDatasource(Filename)) {
      return false;
   }

   OGRDataSource *pdatasource = OGRSFDriverRegistrar::Open(Filename.c_str());
   if (!pdatasource) {
      return false;
   }
   OGRSFDriver *pdriver = pdatasource->GetDriver();
   OGRDataSource::DestroyDataSource(pdatasource);

   /** Elimino archivo */
   if (pdriver->DeleteDataSource(Filename.c_str()) != OGRERR_NONE) {
      return false;
   }

   return true;
}

/** Determina si, al volver de una herramienta a la edicion vectorial, es necesario
 *  terminar la edicion del elemento vectorial o no.
 *  @return true si la herramienta previa era "Eliminar nodo" o "Modificar punto".
 *  @return false en caso contrario.
 */
bool VectorEditionTool::EndEditFeatureIsNecessary(){
   std::string previousTool = Configuration::GetParameterEx(VETYPE_PARAM_MODE_NAME,
                                                VETYPE_PARAM_MODE_UNDEFINED);
   bool result = (previousTool == VETYPE_PARAM_MODE_EDIT);
   result = result || (previousTool == VETYPE_PARAM_MODE_DELETE_NODE);
   return result;
}

} /* namespace suri */
