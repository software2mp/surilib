/*! \file GcpGenerationStrategy.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
