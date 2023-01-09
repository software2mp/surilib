/*! \file LayerToolBuilder.h */                                                                          
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
 */    

// Includes Suri
#include "suri/LayerToolBuilder.h"
#include "suri/LayerAdministrationCommandCreator.h"
#include "suri/GenericTool.h"
#include "suri/AddCsvLayerCommandCreator.h"
#include "suri/CreateGroupCommandCreator.h"
#include "suri/AddLayerCommandExecutionHandler.h"
#include "suri/AddTerrainCommandExecutionHandler.h"
#include "suri/AddCsvLayerCommandExecutionHandler.h"
#include "suri/AddRemoteLayerCommandExecutionHandler.h"
#include "suri/RemoveLayerCommandExecutionHandler.h"
#include "suri/ClassConverterCommandCreator.h"
#include "suri/CreateGroupCommandCreator.h"
#include "suri/DisplayLayerCommandCreator.h"
#include "suri/HideLayerCommandCreator.h"
#include "suri/ExportBufferCommandCreator.h"
#include "suri/ExportLayerCommandCreator.h"
#include "suri/RasterGenericImporterCommandCreator.h"
#include "suri/StackBandCommandCreator.h"
#include "suri/ClassConverterCommandExecutionHandler.h"
#include "suri/CreateGroupCommandExecutionHandler.h"
#include "suri/StackBandCommandExecutionHandler.h"
#include "suri/RasterGenericImporterCommandExecutionHandler.h"
#include "suri/ExportLayerCommandExecutionHandler.h"
#include "suri/ExportBufferCommandExecutionHandler.h"
#include "suri/LayerVisualizationCommandExecutionHandler.h"
#include "suri/ExportVectorLayerSelectionCommandExecutionHandler.h"
#include "suri/ExportVectorLayerCommandExecutionHandler.h"
#include "suri/VectorOperationCommandCreator.h"
#include "suri/VectorOperationCommandExecutionHandler.h"
#include "suri/MergeLayersCommandCreator.h"
#include "suri/MergeLayersCommandExecutionHandler.h"
#include "suri/ManualEnhancementCommandCreator.h"
#include "suri/ManualEnhancementCommandExecutionHandler.h"
#include "suri/VectorReprojectionCommandCreator.h"
#include "suri/VectorReprojectionCommandExecutionHandler.h"
#include "suri/BatchProcessCommandCreator.h"
#include "suri/BatchProcessCommandExecutionHandler.h"
#include "suri/VisualizationExporterCommandExecutionHandler.h"
#include "suri/VisualizationExporterCommandCreator.h"
#include "suri/MosaicCommandExecutionHandler.h"
#include "suri/RasterizationCommandCreator.h"
#include "suri/MosaicCommandCreator.h"
#include "suri/HistogramMatchingCommandCreator.h"
#include "suri/HistogramMatchingCommandExecutionHandler.h"
#include "suri/RasterizationCommandExecutionHandler.h"
namespace suri {

namespace core {

/** Constructor **/
LayerToolBuilder::LayerToolBuilder() {
}

/** Destructor **/
LayerToolBuilder::~LayerToolBuilder() {
}

/** Metodo que crea y configura una herramienta con los hanlders para la ejecucion
 * (CommandExecutionHandlers)
 * y las acciones correspondientes (Commands). **/
ToolInterface* LayerToolBuilder::BuildTool(DataViewManager* pDataViewManager) {
   GenericTool* ptool = new GenericTool(pDataViewManager);
   /** Creadores de commands **/
   ptool->AddCommandCreator(new LayerAdministrationCommandCreator);
   ptool->AddCommandCreator(new AddCsvLayerCommandCreator);
   ptool->AddCommandCreator(new CreateGroupCommandCreator);
#ifdef __UNUSED_CODE__
   // Con el requerimiento del Ticket #7796 la clase queda obsoleta
   ptool->AddCommandCreator(new AddTerrainCommandCreator);
#endif
   ptool->AddCommandCreator(new ClassConverterCommandCreator);
   ptool->AddCommandCreator(new CreateGroupCommandCreator);
   ptool->AddCommandCreator(new DisplayLayerCommandCreator);
   ptool->AddCommandCreator(new ExportBufferCommandCreator);
   ptool->AddCommandCreator(new ExportLayerCommandCreator);
   ptool->AddCommandCreator(new HideLayerCommandCreator);
   ptool->AddCommandCreator(new RasterGenericImporterCommandCreator);
   ptool->AddCommandCreator(new StackBandCommandCreator);
   ptool->AddCommandCreator(new VectorOperationCommandCreator);
   ptool->AddCommandCreator(new MergeLayersCommandCreator);
   ptool->AddCommandCreator(new ManualEnhancementCommandCreator);
   ptool->AddCommandCreator(new VectorReprojectionCommandCreator);
   ptool->AddCommandCreator(new VisualizationExporterCommandCreator);
   ptool->AddCommandCreator(new MosaicCommandCreator);
   ptool->AddCommandCreator(new HistogramMatchingCommandCreator);
   ptool->AddCommandCreator(new RasterizationCommandCreator);
#ifdef __DEBUG__
   ptool->AddCommandCreator(new BatchProcessCommandCreator);
   ptool->AddCommandExecutionHandler(new BatchProcessCommandExecutionHandler(pDataViewManager));
#endif
   /** Administradores de ejecucion */
   ptool->AddCommandExecutionHandler(new AddLayerCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new AddTerrainCommandExecutionHandler(pDataViewManager));
     ptool->AddCommandExecutionHandler(new AddCsvLayerCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new AddRemoteLayerCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new RemoveLayerCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new ClassConverterCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new CreateGroupCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler
                                  (new LayerVisualizationCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new ExportBufferCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new ExportLayerCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler
                               (new RasterGenericImporterCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new StackBandCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler
                          (new ExportVectorLayerSelectionCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler
                                   (new ExportVectorLayerCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new VectorOperationCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new MergeLayersCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler
                                   (new ManualEnhancementCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler
                                  (new VectorReprojectionCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler
                               (new VisualizationExporterCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler
                                    (new MosaicCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler
                                   (new HistogramMatchingCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler
                                     (new RasterizationCommandExecutionHandler(pDataViewManager));
   ptool->SetConfigurationVariableName("layer");
   ptool->Initialize();
   return ptool;
}

}  /** namespace core **/
}  /** namespace suri **/
