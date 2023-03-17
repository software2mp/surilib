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

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/ToolSupport.h"
#include "suri/ToolCommand.h"
#include "suri/ReprojectionTool.h"
#include "suri/ProcessNotebookWidget.h"
#include "suri/LibraryManager.h"
#include "suri/LibraryClient.h"
#include "SpatialReference.h"
#include "GeoreferenceProcess.h"
#include "ReprojectionProcess.h"
#include "ReprojectionProcess2.h"

// Includes Wx

// Defines
#define SRS_LIBRARY_CODE "srs"

// forwards

namespace suri {

/**
 * Constructor
 * @param[in]  pDataViewManager: Puntero al DataViewManager
 */
ReprojectionTool::ReprojectionTool(DataViewManager* pDataViewManager) :
      GenericTool(pDataViewManager) {
   configurationVariableName_ = "reprojection";
   Initialize();
}

/**
 * Destructor
 */
ReprojectionTool::~ReprojectionTool() {
   Finalize();
}

/**
 * Accion de "Ejecutar" de Coregister
 */
void ReprojectionTool::ExecuteCoregister() const {
   RasterElement* praster = ToolSupport::GetRaster(GetViewcontextManager());
   if (praster == NULL){
	  SHOW_WARNING(message_PROCESS_START_ERROR);
      return;
   }
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);
   // TODO(Sebastian - TCK #842): Agregar codigo cuando se implemente

   Library* plibrary = GetLibraryManager()->GetLibraryByCode(SRS_LIBRARY_CODE,
                                                LibraryManager::AccessKey(true));
   Element* pelement = NULL;

   RasterElement *pnosrraster = RasterElement::Create(praster->GetUrl().c_str(),
                                                      praster->GetMetadataMap());
#ifdef __UNUSED_CODE__
   // Fuerza que el raster de entrada no tenga SR ya que el proceso falla
   // al utilizar los GCPs.
   wxXmlNode *pnode = pnosrraster->GetNode(
         GEORREFERENCE_NODE NODE_SEPARATION_TOKEN SPATIAL_REFERENCE_NODE);
   if (pnode && pnode->GetChildren()) {
      pnode->GetChildren()->SetContent(wxT("[Raster]"));
   }
   pnode = pnosrraster->GetNode(
         GEORREFERENCE_NODE NODE_SEPARATION_TOKEN RASTER_SPATIAL_MODEL_NODE);
   if (pnode && pnode->GetChildren()) {
      pnode->GetChildren()->SetContent(
            "PARAM_MT[\"Affine\",PARAMETER[\"num_row\",2],PARAMETER[\""       \
            "num_col\",3],PARAMETER[\"elt_0_0\",0],PARAMETER[\"elt_0_1\",1]," \
            "PARAMETER[\"elt_0_2\",0],PARAMETER[\"elt_1_0\",0],PARAMETER[\""  \
            "elt_1_1\",0],PARAMETER[\"elt_1_2\",1]]");
   }
   pnosrraster->GetNode()->RemoveChild(pnosrraster->GetNode(DIMENSION_NODE));
#endif
   GeoreferenceProcess process(pnosrraster, viewerwindow, pelement,
                               plibrary->GetId(), GetDataViewManager());
   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(
         &process, caption_COREGISTER_TOOL, GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource", pelement);
      if (pdatasource)
         ExportMetadataToFile(pdatasource, pdatasource->GetElement()->GetUrl().c_str());
      GetDatasourceManager()->AddDatasource(pdatasource);
   }
   delete pnosrraster;
}

/**
 * Accion de "Ejecutar" de Georeference
 */
void ReprojectionTool::ExecuteGeoreference() const {
   RasterElement* praster = ToolSupport::GetRaster(GetViewcontextManager());
   if (praster == NULL){
	  SHOW_WARNING(message_PROCESS_START_ERROR);
      return;
   }
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);
   Library* plibrary = GetLibraryManager()->GetLibraryByCode(SRS_LIBRARY_CODE,
                                                LibraryManager::AccessKey(true));
   Element* pelement = NULL;

   GeoreferenceProcess process(praster, viewerwindow, pelement,
                               plibrary->GetId(), GetDataViewManager());

   ProcessAdaptLayer* pal = process.GetProcessAdaptLayer();
   std::vector<Element*> inputelem;
   inputelem.push_back(praster);
   SuriObject::UuidType libid = plibrary->GetId();
   pal->AddAttribute<SuriObject::UuidType>(ProcessAdaptLayer::LibraryIdKeyAttr,
                                          libid);
   pal->AddAttribute<std::vector<Element*> >(ProcessAdaptLayer::InputElementsKeyAttr,
                                            inputelem);
   pal->AddAttribute<Subset>(ProcessAdaptLayer::InputSubsetWindowKeyAttr, viewerwindow);
   DataViewManager* pdvmgr = const_cast<DataViewManager*>(GetDataViewManager());
   pal->AddAttribute<DataViewManager*>(ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   pal->AddAttribute<Element*>(ProcessAdaptLayer::OutputElementKeyAttr, pelement);
   bool reproj = true, askfile = true;
   pal->AddAttribute<bool>(ReprojectionProcess::ReprojectionKeyAtttr, reproj);
   pal->AddAttribute<bool>(ReprojectionProcess::AskGcpFileAttr, askfile);
   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(
         &process, caption_GEOREFERENCE_TOOL, GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      DatasourceInterface* pdatasource =
      DatasourceInterface::Create("RasterDatasource", pelement);
      GetDatasourceManager()->AddDatasource(pdatasource);
      if (pdatasource)
         ExportMetadataToFile(pdatasource, pdatasource->GetElement()->GetUrl().c_str());
   }
}

/**
 * Accion de "Ejecutar" de Reproject
 */
void ReprojectionTool::ExecuteReproject() const {
   RasterElement* praster = ToolSupport::GetRaster(GetViewcontextManager());
   if (praster == NULL ||
         SpatialReference::IsPixelLineSpatialRef(praster->GetSpatialReference().c_str())) {
      SHOW_WARNING(_(message_INVALID_REPROJECTION_RASTER_ERROR));
      return;
   }
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);

   Library* plibrary = GetLibraryManager()->GetLibraryByCode(SRS_LIBRARY_CODE,
                                                LibraryManager::AccessKey(true));

   Element* pelement = NULL;
   ReprojectionProcess2 process(praster, viewerwindow, pelement,
                                plibrary->GetId(), GetDataViewManager());
   ProcessAdaptLayer *pal = process.GetProcessAdaptLayer();
   std::vector<Element*> inputelem;
   inputelem.push_back(praster);
   SuriObject::UuidType libid = plibrary->GetId();
   pal->AddAttribute<SuriObject::UuidType>(ProcessAdaptLayer::LibraryIdKeyAttr,
                                          libid);
   pal->AddAttribute<std::vector<Element*> >(ProcessAdaptLayer::InputElementsKeyAttr,
                                            inputelem);
   pal->AddAttribute<Subset>(ProcessAdaptLayer::InputSubsetWindowKeyAttr, viewerwindow);
   DataViewManager* pdvmgr = const_cast<DataViewManager*>(GetDataViewManager());
   pal->AddAttribute<DataViewManager*>(ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   pal->AddAttribute<Element*>(ProcessAdaptLayer::OutputElementKeyAttr, pelement);
   bool reproj = true, askfile = false;
   pal->AddAttribute<bool>(ReprojectionProcess::ReprojectionKeyAtttr, reproj);
   pal->AddAttribute<bool>(ReprojectionProcess::AskGcpFileAttr, askfile);
   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(
                        &process, caption_REPROJECTION_TOOL, GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource", pelement);
      bool exportmetadata = false;
      pal->GetAttribute<bool>(ProcessAdaptLayer::ExportMetadataKeyAttr, exportmetadata);
      if (exportmetadata && pdatasource) {
         ExportMetadataToFile(pdatasource, pdatasource->GetElement()->GetUrl().c_str());
      }
      GetDatasourceManager()->AddDatasource(pdatasource);
   }
}

/**
 * Si el comando indicado, se encuentra entre la lista de ejecutables, se ejecutara
 * @param[in]  pToolCommand: Comando a ejecutar
 */
void ReprojectionTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("Coregister")) {
      ExecuteCoregister();
   } else if (id == GetId("Georeference")) {
      ExecuteGeoreference();
   } else if (id == GetId("Reproject")) {
      ExecuteReproject();
   }
}

/**
 * Fabrica el comando si es uno de los predefinidos
 * @param[in]  CommandName: Nombre del comando a ejecutar
 * @return     Devuelve el comando creado si tuvo exito, sino NULL
 */
Command* ReprojectionTool::CreateCommand(const std::string &CommandName) {
   Command *pcommand = NULL;
   if (CommandName == "Coregister") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetLocation("ID_COREGISTER_MENUTIEM");
   } else if (CommandName == "Georeference") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetLocation("ID_GEOREFERENCE_MENUTIEM");
   } else if (CommandName == "Reproject") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetLocation("ID_REPROJECT_MENUTIEM");
   }
   return pcommand;
}

/**
 * Destruye un comando previamente creado con CreateCommand
 * @param[in]  pCommand: Puntero a un Comando creado no NULL a destruir
 *
 */
void ReprojectionTool::DestroyCommand(Command* &pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Creador del Widget
 */
Widget* ReprojectionTool::CreateWidget(const std::string &Widget) {
   // TODO(Sebastian - TCK #878): Deteccion de codigo sin implementar
   return NULL;
}

} /** namespace suri */
