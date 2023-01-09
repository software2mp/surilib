/*! \file WaveletModulusRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
