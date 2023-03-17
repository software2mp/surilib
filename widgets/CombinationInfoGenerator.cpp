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

#include "CombinationInfoGenerator.h"

#include <vector>

namespace suri {

/**
 * Metodo estatico auxiliar para agregarle al nodo XML del elemento
 * la info de combinacion.
 *
 * @param[in] pCombinationInfoP Informacion de combinacion que hay que agregar
 *    al elemento.
 * @param[in|out] pXmlNodeP Nodo XML al que hay que agregarle la informacion.
 *    Se asume que es el nodo padre del XML.
 * @return true si se pudo realizar la operacion con exito.
 */
bool CombinationInfoGenerator::Merge(CombinationInfo* pCombinationInfo,
                                     Element* pElement) {
   wxXmlNode* pparent = pElement->GetNode(wxT(""));

   std::vector<wxXmlNode*> urls = pCombinationInfo->GetUrls();
   std::vector<wxXmlNode*> combinations = pCombinationInfo->GetCombinations();
   // Limpio el nodo URL si existe
   wxXmlNode* purl = pElement->GetNode(wxT(URL_NODE));
   if (purl != NULL) {
      pparent->RemoveChild(purl);
      delete purl;
      purl = NULL;
   }

   // Agrego los nodos URL
   std::vector<wxXmlNode*>::iterator iturls = urls.begin();
   for (; iturls != urls.end(); ++iturls) {
      pparent->AddChild(*iturls);
   }
   // Agrego los nodos con la info de las bandas de cada URL
   wxXmlNode* prender = pElement->GetNode(wxT(RENDERIZATION_NODE));
   // Si ya existe un nodo para apilamiento entonces lo saco para
   // poder poner uno nuevo con datos actualizados.
   wxXmlNode* pstacking = pElement->GetNode(wxT(BAND_STACKING_NODE));
   if (pstacking) {
      prender->RemoveChild(pstacking);
      delete pstacking;
      pstacking = NULL;
   }

   if (urls.size() > 1 && combinations.size() > 1) {
      // Creo el nodo de aplicamiento
      pstacking = new wxXmlNode(prender, wxXML_ELEMENT_NODE, wxT(BAND_STACKING_NODE));

      std::vector<wxXmlNode*>::iterator itcombs = combinations.begin();
      for (; itcombs != combinations.end(); ++itcombs) {
         pstacking->AddChild(*itcombs);
      }
   } else if (urls.size() == 1) {
      // Si posee un solo url se genera solo el nodo de combinacion para obtener
      // compatibilidad con procesos existentes que hacen uso del xml de combinacion
      pElement->AddNode(prender, combinations.at(0), true);
   } else {
      return false;
   }
   return true;
}

}  // namespace suri
