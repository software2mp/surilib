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
