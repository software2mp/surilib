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
#include <vector>
// Includes suri
#include "suri/RasterizationProcess.h"
#include "suri/DataViewManager.h"
#include "suri/Dimension.h"
#include "GdalRasterizer.h"
#include "suri/Progress.h"
// Includes Wx
#include "wx/wx.h"
#include "wx/xml/xml.h"



#include "cpl_conv.h"
#include "cpl_multiproc.h"
#include "cpl_string.h"
#include "cpl_vsi.h"
#include "gdal_priv.h"
#include "gdal.h"
#include "gdalwarper.h"
#include "ogr_spatialref.h"
#include "vrtdataset.h"
#include "ogrsf_frmts.h"

/** namespace suri */
namespace suri {
/** Cantidad de ciclos de la herramienta para la barra de progreso */
#define EXPORT_OPERATION_COUNT 3
#define PROCESS_DESCRIPTION "Proceso"
/** Nombre del proceso **/
const std::string RasterizationProcess::kProcessName = "RasterizationProcess";
/**
 * Constructor
 * Asigna el tipo de dato del elemento creado por la herramienta.
 * @param[in] pInputElement: puntero al elemento sobre el que se aplica la herramienta
 * @param[in] WindowSubset subset de la imagen desplegada cuando se selecciono la herramienta
 * @param[out] pOutputElement referencia al elemento creado por la herramienta
 */
RasterizationProcess::RasterizationProcess(std::vector<Element*> InputElements,
                                           const Subset &WindowSubset,
                                           DataViewManager* pDataViewManager,
                                           bool ShowSpectralSelectionPart) :
      FileExporterProcess(InputElements, WindowSubset, pDataViewManager){
   // Oculta/Despliega part seleccion espectral
   showSpectralSelectionPart_ = ShowSpectralSelectionPart;
   SetProcessName(kProcessName);
}

/**
 * Destructor
 */
RasterizationProcess::~RasterizationProcess() {
}



bool RasterizationProcess::RunProcess() {
   VectorDatasource* pVectorDatasource = NULL;
   pAdaptLayer_->GetAttribute<VectorDatasource*>(
                 ProcessAdaptLayer::RasterizationVectorDatasourceAttr, pVectorDatasource);
   // Despliego barra de progreso
   Progress progression(EXPORT_OPERATION_COUNT, PROCESS_DESCRIPTION);
   progression.Update();
   VectorElement* pVectorElement = dynamic_cast<VectorElement*>(pVectorDatasource->GetElement());
   Subset sub;
   pVectorElement->GetElementExtent(sub);
   Dimension dim(sub);
   std::vector<int> bands;
   bands.push_back(0);
   std::string filename;
   Option option;
   GetSelectedOutput(filename, option);
   wxFileName file(filename);
   std:: string typeSelection;
   progression.Update();
   pAdaptLayer_->GetAttribute<std::string>(
               ProcessAdaptLayer::RasterizationTypeSelectionAttr, typeSelection);
   GdalRasterizer rasterizer(pVectorDatasource, bands, filename, typeSelection);
   std::string attr;
   pAdaptLayer_->GetAttribute<std::string>(
           ProcessAdaptLayer::RasterizationAttrSelectionAttr, attr);
   int pixels = 0;
   int lines = 0;
   pAdaptLayer_->GetAttribute<int>(ProcessAdaptLayer::OutputSizePixelsKeyAttr, pixels);
   pAdaptLayer_->GetAttribute<int>(ProcessAdaptLayer::OutputSizeLinesKeyAttr, lines);
   rasterizer.Rasterize(attr, pixels, lines);
   progression.Update();
   RasterElement* raster = new RasterElement();
   Option opt;
   raster = raster->Create(filename.c_str(), opt);
   raster->SetName(file.GetName());
   ProcessAdaptLayer* pPal = GetProcessAdaptLayer();
   pPal->AddAttribute(ProcessAdaptLayer::OutputElementKeyAttr, raster);
   DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource",
                                                                  raster);
   ExportMetadataToFile(pdatasource, filename);
   return true;
}




/** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso**/
void RasterizationProcess::DoOnProcessAdaptLayerSet() {
   FileExporterProcess::DoOnProcessAdaptLayerSet();
}






bool RasterizationProcess::ConfigureProcess() {
   if (!pRasterModelOut_) return false;
   return FileExporterProcess::ConfigureProcess();
}








World * RasterizationProcess::GetOutputWorld(){
   return RasterProcess::GetOutputWorld();
}


}  // namespace
