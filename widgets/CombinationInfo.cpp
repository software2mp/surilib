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
