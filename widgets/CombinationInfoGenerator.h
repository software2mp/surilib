/** \file CombinationInfoGenerator.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COMBINATIONINFOGENERATOR_H_
#define COMBINATIONINFOGENERATOR_H_

#include "CombinationInfo.h"

#include "suri/Element.h"

#include "wx/string.h"
#include "wx/xml/xml.h"

namespace suri {

/**
 * Ofrece una interfaz para poder pedir al objeto que la implementa
 * informacion sobre la combinacion armada.
 */
class CombinationInfoGenerator {
public:
   /**
    * Metodo a implementar por las clases derivadas para devolver
    * infomacion sobre la combinacion.
    */
   virtual CombinationInfo* GetCombinationInfo() = 0;

   /**
    * Metodo estatico auxiliar para agregarle al nodo XML del elemento
    * la info de combinacion.
    */
   static bool Merge(CombinationInfo* pCombinationInfoP, Element* pElementP);
};

}  // namespace suri

#endif  // COMBINATIONINFOGENERATOR_H_
