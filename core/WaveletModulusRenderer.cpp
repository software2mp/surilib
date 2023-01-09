/*! \file WaveletModulusRenderer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <map>
#include <string>
#include <vector>
// Includes Suri
#include "WaveletModulusRenderer.h"
// Includes Wx
// Defines
// forwards

namespace suri {

const std::string WaveletModulusRenderer::kEquation = "HL*HL + LH*LH";
const std::string WaveletModulusRenderer::kHLBandVarName = "HL";
const std::string WaveletModulusRenderer::kLHBandVarName = "LH";

WaveletModulusRenderer::~WaveletModulusRenderer() {}

wxXmlNode* WaveletModulusRenderer::GetXmlNode(
      const WaveletModulusRenderer::Parameters &RenderParameters) {
   BandMathRenderer::Parameters bandMathParams;
   std::vector<Parameters::BandPair>::const_iterator itPairs =
         RenderParameters.bandPairList_.begin();
   while (itPairs != RenderParameters.bandPairList_.end()) {
      std::map<std::string, int> bandMap;
      bandMap[kHLBandVarName] = itPairs->HLBand_;
      bandMap[kLHBandVarName] = itPairs->LHBand_;
      BandMathRenderer::Parameters::Equation eq(WaveletModulusRenderer::kEquation, bandMap);
      bandMathParams.equations_.push_back(eq);
      ++itPairs;
   }
   return BandMathRenderer::GetXmlNode(bandMathParams);
}

} /** namespace suri */
