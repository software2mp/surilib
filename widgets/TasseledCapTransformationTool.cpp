/*! \file TasseledCapTransformationTool.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/TasseledCapTransformationTool.h"

#include "TasseledCapProcess.h"
#include "suri/DataViewManager.h"
#include "suri/Element.h"
#include "suri/ProcessAdaptLayer.h"
#include "suri/ProcessNotebookWidget.h"
#include "suri/ToolCommand.h"
#include "suri/ToolSupport.h"
#include "suri/World.h"

namespace suri {

/**
 * Ctor.
 */
TasseledCapTransformationTool::TasseledCapTransformationTool(
      DataViewManager* pDataViewManager) :
      GenericTool(pDataViewManager) {
   configurationVariableName_ = "tasseledcaptransformation";
   Initialize();
}

/**
 * Dtor.
 */
TasseledCapTransformationTool::~TasseledCapTransformationTool() {

}

/**
 * Ejecuta un comando (ToolInterface::Execute)
 */
void TasseledCapTransformationTool::Execute(const Command *pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("TasseledCapTransformation")) {
      ExecuteTCTransformation();
   }
}

/**
 * Metodo para la creacion de comandos.
 */
Command* TasseledCapTransformationTool::CreateCommand(const std::string& CommandName) {
   Command *pcommand = NULL;
   if (CommandName == "TasseledCapTransformation") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription("Transformacion Tasseled-Cap");
      pcommand->SetLocation("ID_TCTRANSFORMATION_MENUITEM");
   }
   return pcommand;
}

/**
 * Metodo para la destruccion de comandos.
 */
void TasseledCapTransformationTool::DestroyCommand(Command*& pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Metodo para la creacion de widgets de la herramienta.
 */
Widget* TasseledCapTransformationTool::CreateWidget(const std::string& Widget) {
   return NULL;
}

/**
 * Ejecuta el proceso de transformacion tasseled-cap.
 */
void TasseledCapTransformationTool::ExecuteTCTransformation() {

//   SHOW_WARNING(_("La herramienta seleccionada no se encuentra disponible en este momento."));
//   return;

   Element* pinputelement = ToolSupport::GetRaster(
         GetDataViewManager()->GetViewcontextManager());

   // Validar seleccion de 2 elementos (uno para baja reslocion y otro para alta resolucion)
   if (pinputelement == NULL) {
      SHOW_WARNING(_("Debe seleccionar un elemento raster."));
      return;
   }



   // Data view manager.
   DataViewManager* pdvm = GetDataViewManager();
   // Crear el mundo con las propiedades del elemento recibido.
   World* pinputworld = new World;
   pinputworld->SetSpatialReference(pinputelement->GetSpatialReference().c_str());
   Subset elementsubset;
   pinputelement->GetElementExtent(elementsubset);
   pinputworld->SetWorld(elementsubset);
   pinputworld->SetWindow(elementsubset);
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, dynamic_cast<RasterElement*>(pinputelement));
   TasseledCapProcess process(pinputelement, viewerwindow, GetDataViewManager());
   ProcessAdaptLayer* pPal = process.GetProcessAdaptLayer();
   pPal->AddAttribute<DataViewManager*>(ProcessAdaptLayer::DataViewManagerKeyAttr, pdvm);//
   pPal->AddAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr, pinputworld);//
   // La cosa parece estar en orden. Dale para adelante.
   pPal->AddAttribute<Element*>(ProcessAdaptLayer::TasseledCapInputKeyAttr,
                                     pinputelement);//
   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(
         &process, caption_TCTRANSFORMATION_TOOL, GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      Element* pNewElement = NULL;
      pPal->GetAttribute<Element*>(
                  ProcessAdaptLayer::OutputElementKeyAttr, pNewElement);
      DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource",
                                                                     pNewElement);
      GetDatasourceManager()->AddDatasource(pdatasource);
   }
}

}  // namespace suri
