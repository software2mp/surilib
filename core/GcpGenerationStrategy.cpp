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
#include "suri/GcpGenerationStrategy.h"
#include "GcpBufferReader.h"
#include "suri/Image.h"
#include "suri/GcpList.h"
#include "GcpMatcher.h"
#include "suri/RasterElement.h"
#include "suri/TransformationFactoryBuilder.h"
// Includes Wx
// Defines
#define DAUBECHIES_OUTPUT 3
// forwards

namespace suri{

const int GcpGenerationStrategy::kGcpTransformOrder = 1;

Image* OpenImage(const RasterElement* pElement);

/**
 * Constructor
 */
GcpGenerationStrategy::GcpGenerationStrategy(RasterElement* pBaseElement,
                                             RasterElement* pBaseCandidates,
                                             RasterElement* pWarpElement,
                                             const std::string& seedGcpPath) :
                                             elements(pBaseElement, pWarpElement,
                                                      pBaseCandidates, seedGcpPath) {
   this->elements.CreateCorregistrableElements();
   this->pOriginalBase_ = pBaseElement;
   this->pOriginalWarp_ = pWarpElement;
   this->pCorregistrableBase_ = OpenImage(this->elements.GetCorregistrableBase());
   this->pCorregistrableWarp_ = OpenImage(this->elements.GetCorregistrableWarp());
   this->pBaseCandidates_ = OpenImage(this->elements.GetCorregistrableCandidates());
   InitializeGcpList(this->elements.GetCorregistrableBase(), pWarpElement);
   InitializeTransformer(this->elements.GetSeedGcps());
}

/**
 * Destructor
 */
GcpGenerationStrategy::~GcpGenerationStrategy(){
   Image::Close(this->pCorregistrableBase_);
   Image::Close(this->pCorregistrableWarp_);
   Image::Close(this->pBaseCandidates_);
   delete this->pCoordTransform;
}


/**
 * Devuelve una referencia a la lista de GCPs generados en el proceso.
 */
GcpList& GcpGenerationStrategy::RunProcess() {
   std::vector<int> gcpBand;
   gcpBand.push_back(0);
   GcpBufferReader gcpReader(this->pBaseCandidates_, gcpBand);
   int proximityRadius = GcpBufferReader::kProximityRadius * (11 - this->aggressionLevel_);
   gcpReader.SetProximityWindowRadius(proximityRadius);
   GcpMatcher gcpMatcher(this->pCorregistrableBase_, this->pCorregistrableWarp_,
                         this->baseBands_, this->warpBands_);

   int candidateX = 0, candidateY = 0;
   bool found = true;
   while (found) {
      found = gcpReader.GetNextCandidate(candidateX, candidateY);
      if (found) {
         REPORT_DEBUG("D: Encontrado candidato %d,%d", candidateX, candidateY);
         GroundControlPointExtended* pGcp = gcpMatcher.FindGcp(candidateX, candidateY);
         if (pGcp) {
            Coordinates gcpSrc = pGcp->GetSource();
            gcpReader.RemoveCandidates(gcpSrc.x_, gcpSrc.y_);
            Coordinates gcpDest = pGcp->GetDestination();
            // Ajusto a la imagen original
            this->pCoordTransform->Transform(gcpDest, true);
            this->gcpList_.Add(GroundControlPointExtended(gcpSrc, gcpDest));
         }
         delete pGcp;
      }
   }
   return this->gcpList_;
}

Image* OpenImage(const RasterElement* pElement){
   std::string imageUrl = pElement->GetUrl().c_str();
   Image *pImage = Image::Open(imageUrl);
   return pImage;
}

void GcpGenerationStrategy::InitializeTransformer(const GcpList& SeedGcps) {
   // Transformación de coordenadas
   suri::ParameterCollection params;
   params.AddValue<std::vector<GroundControlPoint> >(
         suri::TransformationFactory::kParamGcps, SeedGcps.CreateAdaptedGcps());
   params.AddValue<int>(suri::TransformationFactory::kParamOrder, kGcpTransformOrder);
   suri::TransformationFactory* pfactory = suri::TransformationFactoryBuilder::Build();

   this->pCoordTransform = pfactory->Create(
         suri::TransformationFactory::kPolynomial, params);
   suri::TransformationFactoryBuilder::Release(pfactory);
}


/**
 * Asigno modelo raster, sistema de referencia, etc.
 */
void GcpGenerationStrategy::InitializeGcpList(const RasterElement* pBaseElement,
                                              const RasterElement* pWarpElement) {
   this->gcpList_.SetRasterModelSource(pBaseElement->GetRasterModel());
   this->gcpList_.SetRasterModelDestination(pWarpElement->GetRasterModel());
   this->gcpList_.SetSpatialReferenceSource(pBaseElement->GetSpatialReference().c_str());
   this->gcpList_.SetSpatialReferenceDestination(pWarpElement->GetSpatialReference().c_str());
}

void GcpGenerationStrategy::SetAggressionLevel(int AgressionLevel){
   this->aggressionLevel_  =  AgressionLevel;
}

/**
 * Indica la banda de la imagen base con la que corregistro
 */
void GcpGenerationStrategy::SetBaseBands(const std::vector<int>& BaseBands){
   this->baseBands_ = BaseBands;
}

/**
 * Indica la banda de la imagen warp con la que corregistro
 */
void GcpGenerationStrategy::SetWarpBands(const std::vector<int>& WarpBands){
   this->warpBands_ = WarpBands;
}

}
