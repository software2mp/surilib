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

#ifndef WAVELETMODULUSRENDERER_H_
#define WAVELETMODULUSRENDERER_H_

// Includes Estandar
#include <string>
#include <vector>
// Includes Suri
#include "BandMathRenderer.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class WaveletModulusRenderer : public BandMathRenderer {
public:
   virtual ~WaveletModulusRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** parametros de BandMathRenderer */
   class Parameters {
   public:
      /** Funcion que se aplica en el algebra de bandas. */
      class BandPair {
      public:
         /** Ctor. */
         BandPair() :
            HLBand_(0), LHBand_(0) {
         }
         /** Ctor. */
         BandPair(int HLBand, int LHBand) :
               HLBand_(HLBand), LHBand_(LHBand) {
         }
         // Numero de la banda HL
         int HLBand_;
         // Numero de la banda LH
         int LHBand_;
      };
      std::vector<BandPair> bandPairList_; /*! vector con ecuaciones */
      void insertPair(int HLBand, int LHBand){
         BandPair pair(HLBand, LHBand);
         bandPairList_.push_back(pair);
      }
   };
// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(const WaveletModulusRenderer::Parameters &RenderParameters);
private:
   static const std::string kEquation;
   static const std::string kHLBandVarName;
   static const std::string kLHBandVarName;
};

} /** namespace suri */

#endif /* WAVELETMODULUSRENDERER_H_ */
