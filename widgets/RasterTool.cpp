/*! \file RasterTool.cpp */
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
#include "suri/ToolSupport.h"
#include "ResizeProcess.h"
#include "suri/RasterTool.h"
#include "suri/ToolCommand.h"
#include "ReprojectionProcess.h"
#include "suri/ProcessNotebookWidget.h"
#include "suri/LibraryManager.h"
#include "suri/LibraryClient.h"
// Includes Wx

// Defines
#define SRS_LIBARARY_CODE "srs"

// forwards

namespace suri {

/**
 * Constructor
 * @param[in]  pDataViewManager: Puntero al DataViewManager
 */
RasterTool::RasterTool(DataViewManager* pDataViewManager) :
      GenericTool(pDataViewManager) {
   configurationVariableName_ = "raster";
   Initialize();
}

/**
 * Destructor
 */
RasterTool::~RasterTool() {
   Finalize();
}

/**
 * Accion de "Ejecutar" de ScaleImage
 */
void RasterTool::ExecuteScaleImage() const {
   RasterElement* praster = ToolSupport::GetRaster(GetViewcontextManager());
   if (praster == NULL) {
      SHOW_WARNING(message_PROCESS_START_ERROR);
      return;
   }
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);

   Library* plibrary = GetLibraryManager()->GetLibraryByCode(SRS_LIBARARY_CODE,
                                                                LibraryManager::ALL);
   ResizeProcess process(praster, viewerwindow,
                               plibrary->GetId(), GetDataViewManager(),
                               false, false);
   process.OnProcessAdaptLayerSet();
   suri::ProcessNotebookWidget* pprocesswidget = new suri::ProcessNotebookWidget(
                       &process, caption_SCALE_TOOL, GetLibraryManager());
   ProcessAdaptLayer* pPal = process.GetProcessAdaptLayer();
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      Element* pelement = NULL;
      pPal->GetAttribute<Element*>(ProcessAdaptLayer::OutputElementKeyAttr, pelement);
      DatasourceInterface* pdatasource =
                                       DatasourceInterface::Create("RasterDatasource", pelement);
      GetDatasourceManager()->AddDatasource(pdatasource);
   }
}

/**
 * Accion de "Ejecutar" de BandStacking
 */
void RasterTool::ExecuteBandStacking() const {
   RasterElement* praster = ToolSupport::GetRaster(GetViewcontextManager());
   if (praster == NULL) {
      SHOW_WARNING(message_PROCESS_START_ERROR);
      return;
   }
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);
   // TODO(Sebastian - TCK #842): Agregar codigo cuando se implemente
}

/**
 * Si el comando indicado, se encuentra entre la lista de ejecutables, se ejecutara
 * @param[in]  pToolCommand: Comando a ejecutar
 */
void RasterTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("ScaleImage")) {
      ExecuteScaleImage();
   } else if (id == GetId("BandStacking")) {
      ExecuteBandStacking();
   }
}

/**
 * Fabrica el comando si es uno de los predefinidos
 * @param[in]  CommandName: Nombre del comando a ejecutar
 * @return     Devuelve el comando creado si tuvo exito, sino NULL
 */
Command* RasterTool::CreateCommand(const std::string &CommandName) {
   Command *pcommand = NULL;
   if (CommandName == "ScaleImage") {
      // TODO(Sebastian - TCK #842): Agregar codigo cuando se implemente (ID_SCALE_IMAGE_MENUTIEM)
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
//          pcommand->SetDescription(tooltip_r);
      pcommand->SetLocation("ID_SCALE_IMAGE_MENUTIEM");
   } else if (CommandName == "BandStacking") {
      // TODO(Sebastian - TCK #842): Agregar codigo cuando se implemente (ID_BAND_STACKING_MENUTIEM)
   }
   return pcommand;
}

/**
 * Destruye un comando previamente creado con CreateCommand
 * @param[in]  pCommand: Puntero a un Comando creado no NULL a destruir
 *
 */
void RasterTool::DestroyCommand(Command* &pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Creador del Widget
 */
Widget* RasterTool::CreateWidget(const std::string &Widget) {
   // TODO(Sebastian - TCK #878): Deteccion de codigo sin implementar
   return NULL;
}

} /** namespace suri */
