/*! \file RasterClassificationTool.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "resources.h"
#include "suri/ToolSupport.h"
#include "ClassifiedRasterDatasourceValidator.h"
#include "suri/messages.h"
#include "suri/ToolCommand.h"
#include "suri/RasterElement.h"
#include "suri/RasterProcess.h"
#include "suri/FilterProcess.h"
#include "suri/DatasourceInterface.h"
#include "suri/ClassificationProcess.h"
#include "suri/ProcessNotebookWidget.h"
#include "suri/ConfussionMatrixProcess.h"
#include "suri/RasterClassificationTool.h"
#include "suri/VectorEditionTask.h"
#include "VectorDatasource.h"
#include "EnclosureManipulator.h"
#include "EnclosureValidator.h"
#include "suri/ClassStatisticsProcess.h"
#include "suri/KMeansClassificationProcess.h"

// Includes Wx

// Defines
// todo provisorio mover a messages.h
#define tooltip_CREATE_CLASS "Crear clase"
#define tooltip_SELECT_CLASS "Seleccionar clase"
#define tooltip_SHOW_TABLE "Editar tabla de la clase"
#define tooltip_EDIT_CLASS_GEOMETRY "Editar area de entrenamiento"
#define tooltip_DELETE_CLASS_POINT "Eliminar punto de clase"
#define tooltip_REMOVE_TRAININGAREA "Eliminar area de entrenamiento"

// forwards

namespace suri {

/**
 * Constructor
 * @param[in]  pDataViewManager: Puntero al DataViewManager
 */
RasterClassificationTool::RasterClassificationTool(DataViewManager* pDataViewManager) :
                          VectorEditionTool(pDataViewManager, new EnclosureValidator()) {
   configurationVariableName_ = "classification";
   Initialize();
}

/**
 * Destructor
 */
RasterClassificationTool::~RasterClassificationTool() {
   Finalize();
}

/**
 *  Metodo auxiliar que devuelve el elemento de la fuente de datos
 *  asociado a la capa seleccionada
 * @return RasterElement* referencia al elemento de la fuente de datos
 * @return NULL en caso de no tener encontrar el elemento
 */
RasterElement* RasterClassificationTool::GetRasterElement() const {
   RasterElement* prasterfromlayer = ToolSupport::GetRaster(GetViewcontextManager());
   if (!prasterfromlayer)
      return NULL;
   LayerInterface* player = GetViewcontextManager()->GetAssociatedLayerByElementId(
         prasterfromlayer->GetUid());
   if (!player)
      return NULL;
   DatasourceInterface* pdatasource = GetDatasourceManager()->GetDatasource(
         player->GetAssociatedDatasourceId());
   if (!pdatasource)
      return NULL;
   RasterElement* praster = dynamic_cast<RasterElement*>(pdatasource->GetElement());
   return praster;
}
/**
 * Accion de "Ejecutar" de NonSupervisedClassification
 */
void RasterClassificationTool::ExecuteNonSupervisedClassification() const {
   RasterElement* prasterelement = GetRasterElement();
   Subset viewerwindow;
   if (prasterelement == NULL) {
      SHOW_WARNING(message_ONLY_ONE_RASTER_SELECTED);
      return;
   }
   std::string clasiftype = "KMeansPart";
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, prasterelement);
   KMeansClassificationProcess process(prasterelement, viewerwindow,
                                       GetDataViewManager());
   ProcessAdaptLayer* pal = process.GetProcessAdaptLayer();
   bool showmask = false;
   pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr,
                                             showmask);
   pal->AddAttribute<std::string>(ClassificationProcess::ClassificationType,
                                clasiftype);

   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(
         &process, caption_RASTER_UNSUPERVISED_CLASSIFICATION_PROCESS_NAME, GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
   }
}

/**
 * Accion de "Ejecutar" de SupervisedClassification
 */
void RasterClassificationTool::ExecuteSupervisedClassification() const {
   ExecuteClassificationProcess("ParametricClassificationPart",
                                caption_RASTER_CLASSIFICATION_PROCESS_NAME);
}

/**
 * Accion de "Ejecutar" de ClassFusion
 */
void RasterClassificationTool::ExecuteClassFusion() const {
   RasterElement* praster = GetRasterElement();
   if (!praster) {
      SHOW_WARNING(message_ONLY_ONE_RASTER_SELECTED);
      return;
   }
   ClassifiedRasterDatasourceValidator validator;
   DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource", praster);
   if (!pdatasource || !validator.IsValid(pdatasource)) {
      SHOW_WARNING(message_ONLY_CLASIFICATED_RASTER);
      return;
   }
   DatasourceInterface::Destroy(pdatasource);
   ExecuteClassificationProcess("ClassFussionParametersPart", caption_CLASS_FUSION, false, false);
}

/**
 * Accion de "Ejecutar" de SupervisedClassification
 */
void RasterClassificationTool::ExecuteConfusionMatrix() const {
   RasterElement* praster = GetRasterElement();
   if (praster == NULL) {
      SHOW_WARNING(message_ONLY_ONE_RASTER_SELECTED);
      return;
   }
   Subset viewerwindow;
   ClassifiedRasterDatasourceValidator validator;
   DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource", praster);
   if (pdatasource && validator.IsValid(pdatasource)
         && ToolSupport::ContainsPolygons(GetViewcontextManager())) {
      Widget* pstatisticsreport = NULL;
      StatisticsExporterProcess::OutputType output = ClassStatisticsProcess::HtmlPart;
      ConfussionMatrixProcess process(praster, viewerwindow,
                                      GetDataViewManager(), pstatisticsreport,
                                      output);
      process.OnProcessAdaptLayerSet();
      ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(
            &process, caption_CONFUSION_MATRIX_PROCESS_NAME, GetLibraryManager());
      if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK
            && pstatisticsreport) {
         // Creo ventana que despliega reporte de estadisticas
         pstatisticsreport->CreateTool();
      }

   } else {
      SHOW_WARNING(message_CLASIFICATED_RASTER_AND_POLYGON);
   }
   DatasourceInterface::Destroy(pdatasource);
}

/**
 * Accion de "Ejecutar" de ClassificationFilters
 */
void RasterClassificationTool::ExecuteClassificationFilters() const {
   RasterElement* praster = GetRasterElement();
   if (praster == NULL) {
      SHOW_WARNING(message_ONLY_ONE_RASTER_SELECTED);
      return;
   }
   ClassifiedRasterDatasourceValidator validator;
   int flags = POSTCLASSIFICATION_FILTER;
   bool showspectral = false;
   DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource", praster);
   if (pdatasource && validator.IsValid(pdatasource)) {
      Subset viewerwindow;
      ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);
      FilterProcess process(praster, viewerwindow, flags,
                            GetDataViewManager(),showspectral);
      ProcessAdaptLayer *pal = process.GetProcessAdaptLayer();
      pal->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, showspectral);
      bool includemask = false;
      pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr,
                                                 includemask);
      process.OnProcessAdaptLayerSet();
      ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(
            &process, _(caption_RASTER_FILTER_PROCESS_NAME), GetLibraryManager());
      pprocesswidget->CreateTool();
      pprocesswidget->ShowModal();
   } else {
      SHOW_WARNING(message_ONLY_CLASIFICATED_RASTER);
   }
   DatasourceInterface::Destroy(pdatasource);
}

/** Metodo que ejecuta el proceso de clasificacion por umbrales */
void RasterClassificationTool::ExecuteThresholdClassification() const {
   // debe seleccionar al menos un raster
   RasterElement* praster = ToolSupport::GetRaster(GetViewcontextManager());
   if (praster == NULL) {
      SHOW_WARNING(message_ONLY_ONE_RASTER_SELECTED);
      return;
   }
   bool showspectral = false;
   bool includemask = false;
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);
   LayerList* playerlist = ToolSupport::GetLayerList(GetViewcontextManager());
   ClassificationProcess process(playerlist, praster, viewerwindow,
                                 "ThresholdClassification",
                                 GetDataViewManager());
   ProcessAdaptLayer* pal = process.GetProcessAdaptLayer();
   pal->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, showspectral);
   pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr,
                                                includemask);
   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(
         &process, _(caption_THRESHOLD_CLASS), GetLibraryManager());
   pprocesswidget->CreateTool();
   pprocesswidget->ShowModal();
}

/**
 * Metodo que obtiene el elemento y genera el proceso de clasifiacion
 */
void RasterClassificationTool::ExecuteClassificationProcess(const std::string& ClassificationType,
                                                            const std::string& ClassificationName,
                                                            bool RequiredContainsPolygons,
                                                            bool ShowSpectralPart) const {
   RasterElement* praster = ToolSupport::GetRaster(GetViewcontextManager());

   if (praster == NULL) {
      SHOW_WARNING(message_ONLY_ONE_RASTER_SELECTED);
      return;
   }
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);
   LayerList* playerlist = ToolSupport::GetLayerList(GetViewcontextManager());
   ClassificationProcess process(playerlist, praster, viewerwindow,
                                 ClassificationType, GetDataViewManager(),
                                 ShowSpectralPart);
   ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(&process,
                                         ClassificationName, GetLibraryManager());
   ProcessAdaptLayer* pal=process.GetProcessAdaptLayer();
   pal->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, ShowSpectralPart);
   bool includemask = false;
   pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr,
                                              includemask);
   process.OnProcessAdaptLayerSet();
   pprocesswidget->CreateTool();
   pprocesswidget->ShowModal();
}

/** Crea un resinto e inicia su edicion */
void RasterClassificationTool::ExecuteCreateEnclosure() {
   EnclosureManipulator manipulator;
   StartTask(Vector::Polygon , &manipulator);
}

/**
 * Metodo que obtiene el elemento y genera el proceso para mapear angulos espectrales
 */
void RasterClassificationTool::ExecuteSpectralAngleMapper() const {
   // debe seleccionar al menos un raster
   RasterElement* praster = ToolSupport::GetRaster(GetViewcontextManager());
   if (praster == NULL) {
      SHOW_WARNING(message_ONLY_ONE_RASTER_SELECTED);
      return;
   }
   bool showspectral = true;
   bool includemask = false;
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);
   LayerList* playerlist = ToolSupport::GetLayerList(GetViewcontextManager());
   ClassificationProcess process(playerlist, praster, viewerwindow,
                                 "SpectralAngleMapper", GetDataViewManager());
   ProcessAdaptLayer *pal=process.GetProcessAdaptLayer();
   pal->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, showspectral);
   pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr,
                                                includemask);
   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(
         &process, _(caption_SPECTRAL_ANGLE_MAPPER), GetLibraryManager());
   pprocesswidget->CreateTool();
   pprocesswidget->ShowModal();
}

/**
 * Si el comando indicado, se encuentra entre la lista de ejecutables, se ejecutara
 * @param[in]  pToolCommand: Comando a ejecutar
 */
void RasterClassificationTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("NonSupervisedClassification")) {
      ExecuteNonSupervisedClassification();
   } else if (id == GetId("SupervisedClassification")) {
      ExecuteSupervisedClassification();
   } else if (id == GetId("ClassFusion")) {
      ExecuteClassFusion();
   } else if (id == GetId("ConfusionMatrix")) {
      ExecuteConfusionMatrix();
   } else if (id == GetId("ClassificationFilters")) {
      ExecuteClassificationFilters();
   } else if (id == GetId("CreateEnclosure")) {
      ExecuteCreateEnclosure();
   } else if (id == GetId("ThresholdClassification")) {
      ExecuteThresholdClassification();
   } else if (id == GetId("SpectralAngleMapper")) {
      ExecuteSpectralAngleMapper();
   } else {
      VectorEditionTool::Execute(pToolCommand);
   }
}

/**
 * Fabrica el comando si es uno de los predefinidos
 * @param[in]  CommandName: Nombre del comando a ejecutar
 * @return     Devuelve el comando creado si tuvo exito, sino NULL
 */
Command* RasterClassificationTool::CreateCommand(const std::string &CommandName) {
   Command *pcommand = NULL;
   if (CommandName == "NonSupervisedClassification") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_RASTER_UNSUPERVISED_CLASSIFICATION_BUTTON);
      pcommand->SetDecoration(tool_RASTER_UNSUPERVISED_CLASSIFICATION);
      pcommand->SetLocation("ID_NON_SUPERVISED_CLASSIFICATION_MENUTIEM");
   } else if (CommandName == "SupervisedClassification") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_RASTER_CLASSIFICATION_BUTTON);
      pcommand->SetDecoration(tool_RASTER_CLASSIFICATION);
      pcommand->SetLocation("ID_SUPERVISED_CLASSIFICATION_MENUTIEM");
   } else if (CommandName == "ClassFusion") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu|Button");
      pcommand->SetDescription(tooltip_CLASS_FUSSION);
      pcommand->SetDecoration(tool_CLASS_FUSSION);
      pcommand->SetLocation("ID_CLASS_FUSION_MENUTIEM|ID_CLASSIFICATION_TOOL_PANEL");
   } else if (CommandName == "ConfusionMatrix") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu|Button");
      pcommand->SetDescription(tooltip_CONFUSION_MATRIX);
      pcommand->SetDecoration(tool_CONFUSION_MATRIX);
      pcommand->SetLocation("ID_CONFUSION_MATRIX_MENUTIEM|ID_CLASSIFICATION_TOOL_PANEL");
   } else if (CommandName == "ClassificationFilters") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu|Button");
      pcommand->SetDescription(tooltip_RASTER_CLASSIFICATION_FILTER);
      pcommand->SetDecoration(tool_RASTER_FILTER);
      pcommand->SetLocation("ID_CLASSIFICATION_FILTERS_MENUTIEM|ID_CLASSIFICATION_TOOL_PANEL");
   } else if (CommandName == "CreateEnclosure") {
      // Configuro evento que inicializa creacion
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(_(tooltip_CREATE_CLASS));
      pcommand->SetDecoration(tool_DRAW_CLASS);
      pcommand->SetLocation("ID_CLASSIFICATION_TOOL_PANEL");
   } else if (CommandName == "ThresholdClassification") {
      // Configuro evento que inicializa creacion
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(_(tooltip_THRESHOLD_CLASSIFICATION));
      pcommand->SetLocation("ID_THRESHOLD_CLASSIFICATION_MENUTIEM");
   } else if (CommandName == "SpectralAngleMapper") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(caption_SPECTRAL_ANGLE_MAPPER);
      pcommand->SetLocation("ID_SPECTRAL_ANGLE_MAPPER_MENUTIEM");
   } else {
      pcommand = VectorEditionTool::CreateCommand(CommandName);
      if (pcommand && CommandName == "SelectFeatures")
         pcommand->SetDescription(_(tooltip_SELECT_CLASS));
      if (pcommand && CommandName == "ShowTable")
         pcommand->SetDescription(_(tooltip_SHOW_TABLE));
      if (pcommand && CommandName == "EditFeature")
         pcommand->SetDescription(_(tooltip_EDIT_CLASS_GEOMETRY));
      if (pcommand && CommandName == "DeletePoint")
         pcommand->SetDescription(_(tooltip_DELETE_CLASS_POINT));
      if (pcommand && CommandName == "DeleteFeature")
         pcommand->SetDescription(_(tooltip_REMOVE_TRAININGAREA));

      std::string location = "ID_CLASSIFICATION_TOOL_PANEL";
      if (CommandName == "TaskMenu") {
         location = location + "|" + "ID_EDIT_TRAINING_LAYER_MENUTIEM";
         if (pcommand) pcommand->SetAssociatedTaskId("ClassificationTask");
      }
      if (pcommand)
         pcommand->SetLocation(location);
   }
   return pcommand;
}

/**
 * Destruye un comando previamente creado con CreateCommand
 * @param[in]  pCommand: Puntero a un Comando creado no NULL a destruir
 */
void RasterClassificationTool::DestroyCommand(Command* &pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Creador del Widget
 */
Widget* RasterClassificationTool::CreateWidget(const std::string &Widget) {
   // TODO(Sebastian - TCK #878): Deteccion de codigo sin implementar
   return NULL;
}

} /** namespace suri */
