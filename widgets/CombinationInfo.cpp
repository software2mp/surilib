/** \file CombinationInfo.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "CombinationInfo.h"

#include <vector>

namespace suri {

/**
 * Ctor
 */
CombinationInfo::CombinationInfo(std::vector<wxXmlNode*>& UrlsP,
                                 std::vector<wxXmlNode*>& CombinationsP) {
   urls_ = UrlsP;
   combinations_ = CombinationsP;
}

/**
 * Dtor
 */
CombinationInfo::~CombinationInfo() {
}

/**
 * Devuelve las URLs desde las cuales se van a tomar las bandas
 * para la combinacion.
 */
std::vector<wxXmlNode*> CombinationInfo::GetUrls() {
   return urls_;
}

/**
 * Devuelve las combinaciones que se quieren aplicar desde cada
 * una de las Urls.
 */
std::vector<wxXmlNode*> CombinationInfo::GetCombinations() {
   return combinations_;
}

/** Realiza una copia de los nodos xml de urls y de combinacion que posee **/
CombinationInfo* CombinationInfo::Clone(CombinationInfo* pCombination) {
   std::vector<wxXmlNode*> copyurls;
   std::vector<wxXmlNode*> urls = pCombination->GetUrls();
   for (unsigned int i = 0; i < urls.size(); ++i) {
      wxXmlNode* url = new wxXmlNode(*urls[i]);
      copyurls.push_back(url);
   }
   std::vector<wxXmlNode*> copycombs;
   std::vector<wxXmlNode*> combs = pCombination->GetCombinations();
   for (unsigned int i = 0; i < combs.size(); ++i) {
      wxXmlNode* comb = new wxXmlNode(*combs[i]);
      copycombs.push_back(comb);
   }
   return new CombinationInfo(copyurls, copycombs);
}

}  // namespace suri
