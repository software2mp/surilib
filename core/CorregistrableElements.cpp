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
#include <vector>
#include <string>
// Includes Suri
#include "CorregistrableElements.h"
#include "GdalResizer.h"
#include "GdalTranslator.h"
#include "GdalWarperTool.h"
#include "GcpElementNotFoundException.h"
#include "suri/RasterElement.h"
#include "suri/RasterSpatialModel.h"
#include "suri/TextFileGcpLoader.h"
// Includes Wx
// Defines
// forwards

namespace suri{
   
CorregistrableElements::CorregistrableElements(
      RasterElement* pBase, RasterElement* pWarp, RasterElement* pCandidates,
      const std::string& seedGcpPath) :
      originalBase_(pBase), originalCandidates_(pCandidates),
      originalWarp_(pWarp){
   TextFileGcpLoader gcpLoader;
   gcpLoader.Load(seedGcpPath, this->gcpList);
   InitializeElements();
   for (int i = 0; i < pBase->GetBandCount(); ++i){
      this->baseBands.push_back(i);
   }
   for (int i = 0; i < pWarp->GetBandCount(); ++i){
      this->warpBands.push_back(i);
   }
   this->candidateBands.push_back(0);
}

CorregistrableElements::CorregistrableElements(
      RasterElement* pBase, RasterElement* pWarp, RasterElement* pCandidates,
      const std::string& seedGcpPath,
      const std::vector<int>& BaseBands, const std::vector<int>& WarpBands) :
      originalBase_(pBase), originalCandidates_(pCandidates), originalWarp_(pWarp),
      baseBands(BaseBands), warpBands(WarpBands){
   TextFileGcpLoader gcpLoader;
   gcpLoader.Load(seedGcpPath, this->gcpList);
   InitializeElements();
   this->candidateBands.push_back(0);
}

void CorregistrableElements::InitializeElements() {
   this->tempFilePath_.assign(wxFileName::CreateTempFileName(_("")).c_str());
   wxRemoveFile(this->tempFilePath_);
   this->corregistrableBasePath_.assign(this->tempFilePath_ + "BaseIntersect.tif");
   this->corregistrableWarpPath_.assign(this->tempFilePath_ + "WarpIntersect.tif");
   this->corregisteredWarpPath_.assign(this->tempFilePath_ + "CorregisteredWarp.tif");
   this->corrCandidatesPath_.assign(this->tempFilePath_ + "CandidatesIntersect.tif");
   this->corregistrableBase_ = this->originalBase_->Clone();
   this->corregistrableWarp_ = this->originalWarp_->Clone();
   this->corregistrableCandidates_ = this->originalCandidates_->Clone();
}

CorregistrableElements::~CorregistrableElements() {
   delete this->corregistrableBase_;
   delete this->corregistrableWarp_;
   delete this->corregistrableCandidates_;
}


void CorregistrableElements::CreateCorregistrableElements() {
   // Uso el wkt de la base para reproyectar el warp
   CreateCorregisteredWarp();
   RasterElement* pTransformedWarp = RasterElement::Create(corregisteredWarpPath_);
   Subset intersection, baseIntersect, warpIntersect;
   // Buscp la interseccion
   intersection = GetIntersection(originalBase_, pTransformedWarp);
   std::string baseSpatialModel(this->originalBase_->GetRasterModel());
   std::string warpSpatialModel(pTransformedWarp->GetRasterModel());
   RasterSpatialModel * pBaseRSM = RasterSpatialModel::Create(baseSpatialModel);
   RasterSpatialModel * pWarpRSM = RasterSpatialModel::Create(warpSpatialModel);
   baseIntersect = warpIntersect = intersection;
   Subset basePLIntersect = pBaseRSM->InverseTransform(baseIntersect);
   basePLIntersect = basePLIntersect.ToOrigin();
   Subset warpPLIntersect = pWarpRSM->InverseTransform(warpIntersect);
   warpPLIntersect = warpPLIntersect.ToOrigin();
   // Calculo el tamanio final de la imagen.
   Subset sizeSubset = Join(basePLIntersect, warpPLIntersect);
   if (sizeSubset.lr_.x_ == 0 || sizeSubset.lr_.y_ == 0){
      throw GcpElementNotFoundException();
   }
   std::vector<double> basePixelSize = pBaseRSM->GetModelParameters().pixelSizes_;
   std::vector<double> warpPixelSize = pWarpRSM->GetModelParameters().pixelSizes_;
   GdalResizer baseResizer(this->originalBase_->GetUrl().c_str(),
                           this->corregistrableBasePath_,
                           this->baseBands);
   // Creo imagen con interseccion
   baseResizer.Resize(intersection, sizeSubset.lr_.x_, sizeSubset.lr_.y_,
                      basePixelSize[0], basePixelSize[1]);
   // Escalo candidatos
   GdalResizer candidateResizer(this->originalCandidates_->GetUrl().c_str(),
                           this->corrCandidatesPath_,
                           this->candidateBands);
   // Creo imagen con interseccion
   candidateResizer.Resize(intersection, sizeSubset.lr_.x_, sizeSubset.lr_.y_,
                      basePixelSize[0], basePixelSize[1]);
   GdalResizer warpResizer(this->corregisteredWarpPath_.c_str(),
                           this->corregistrableWarpPath_,
                           this->warpBands);
   warpResizer.Resize(intersection, sizeSubset.lr_.x_, sizeSubset.lr_.y_,
                      warpPixelSize[0], warpPixelSize[1]);
   RefreshRasterElements();
   delete pTransformedWarp;
}

void CorregistrableElements::CreateCorregisteredWarp() {
   std::string baseWkt = this->gcpList.GetSpatialReferenceSource();
   std::string transformedWarpPath = corregistrableWarpPath_ + ".tmp";
   GdalTranslator translator(this->originalWarp_->GetUrl().c_str(),
                           transformedWarpPath,
                           this->warpBands);
   translator.Translate(this->gcpList);
   GdalWarperTool warper(transformedWarpPath, corregisteredWarpPath_);
   warper.Warp(baseWkt, true, 0);
}



Subset CorregistrableElements::GetIntersection(RasterElement* base, RasterElement* warp) {
   Subset baseExtent, warpExtent;
   base->GetElementExtent(baseExtent);
   warp->GetElementExtent(warpExtent);
   return baseExtent.Intersect(warpExtent);
}

void CorregistrableElements::RefreshRasterElements() {
   delete this->corregistrableBase_;
   delete this->corregistrableWarp_;
   delete this->corregistrableCandidates_;
   this->corregistrableBase_ = RasterElement::Create(this->corregistrableBasePath_);
   this->corregistrableWarp_ = RasterElement::Create(this->corregistrableWarpPath_);
   this->corregistrableCandidates_ = RasterElement::Create(this->corrCandidatesPath_);
}
   
RasterElement* CorregistrableElements::GetCorregistrableBase() {
   return this->corregistrableBase_;
}

RasterElement* CorregistrableElements::GetCorregistrableWarp() {
   return this->corregistrableWarp_;
}

RasterElement* CorregistrableElements::GetCorregistrableCandidates() {
   return this->corregistrableCandidates_;
}


GcpList CorregistrableElements::GetSeedGcps() {
   return this->gcpList;
}


} // namespace suri
