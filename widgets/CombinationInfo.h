/** \file CombinationInfo.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COMBINATIONINFO_H_
#define COMBINATIONINFO_H_

#include <vector>
#include <string>

#include "wx/xml/xml.h"

namespace suri {

/**
 * Clase que actua como contenedor con la informacion para una
 * combinacion de bandas.
 */
class CombinationInfo {
public:
   /**
    * Ctor
    */
   CombinationInfo(std::vector<wxXmlNode*>& UrlsP, std::vector<wxXmlNode*>& CombinationsP);

   /**
    * Dtor
    */
   virtual ~CombinationInfo();

   /**
    * Devuelve las URLs desde las cuales se van a tomar las bandas
    * para la combinacion.
    */
   std::vector<wxXmlNode*> GetUrls();

   /**
    * Devuelve las combinaciones que se quieren aplicar desde cada
    * una de las Urls.
    */
   std::vector<wxXmlNode*> GetCombinations();
   /** Realiza una copia de los nodos xml de urls y de combinacion que posee **/
   static CombinationInfo* Clone(CombinationInfo* pCombination);

private:
   std::vector<wxXmlNode*> urls_;  // Urls de las fuentes de datos.
   std::vector<wxXmlNode*> combinations_;  // Bandas de cada fuente de datos.
};

}

#endif  // COMBINATIONINFO_H_
