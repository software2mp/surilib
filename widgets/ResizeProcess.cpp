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
