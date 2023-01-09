/** \file CombinationInfoGenerator.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
