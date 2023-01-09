/*! \file ResizeProcess.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
// Includes Estandar
#include <string>
#include <vector>
// Includes Suri
#include "ResizeProcess.h"
#include "BandInfo.h"
#include "suri/DatasourceInterface.h"
#include "GdalResizer.h"
// Includes Wx
// Defines
// forwards
namespace suri {

ResizeProcess::ResizeProcess(Element* pInputElement, const Subset& ViewportSubset,
                        const SuriObject::UuidType& LibraryId,
                        DataViewManager* pDataViewManager,
                        bool AskGcpFile, bool Reproject):
                        ReprojectionProcess(pInputElement, ViewportSubset,
                                            LibraryId, pDataViewManager,
                                              AskGcpFile, Reproject ) {}


bool ResizeProcess::RunProcess() {
   LayerList *plist = GetElementList();
   std::vector<Element*> elements = plist->GetRenderizationOrderList();
   std::vector<Element*>::const_iterator it = elements.begin();
   Element* pelement = GetInputElement();
   World * pWorld = GetWorld();
   if (!pWorld) {
       message_.assign(message_SAVE_ERROR wxT(" ") message_GET_WORLD_ERROR);
       return false;
   }
   std::string filename;
   Option option;
   GetSelectedOutput(filename, option);
   wxFileName file(filename);
   std::string fileIn;

   Resize(std::string(pelement->GetUrl()), filename, pWorld);
   RasterElement* raster = new RasterElement();
   Option opt;
   raster = raster->Create(filename.c_str(), opt);
   raster->SetName(file.GetName());
   ProcessAdaptLayer* pPal = GetProcessAdaptLayer();
   pPal->AddAttribute(ProcessAdaptLayer::OutputElementKeyAttr, raster);
   DatasourceInterface* pdatasource =
        DatasourceInterface::Create("RasterDatasource", raster);
   ExportMetadataToFile(pdatasource, filename);
   return true;
}

/**
 * Escala el raster especificado.
 */
void ResizeProcess::Resize(const std::string& SrcFilename,
                            const std::string& DestFilename,
                            World* pWorld) {
   std::vector<int> selectedBands = GetSelectedRasterBands();
   GdalResizer resizer(SrcFilename, DestFilename, selectedBands);
   resizer.Resize(pWorld);
}

}
