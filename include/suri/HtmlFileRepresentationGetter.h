/*! \file HtmlFileRepresentationGetter.h */                                                     
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 
#ifndef __HTML_FILE_REPRESENTATION_H__
#define __HTML_FILE_REPRESENTATION_H__

// Include Estandar
#include <string>

// Includes Suri
#include "suri/ElementHtmlRepresentationGetter.h"

namespace suri {
// forward
class Element;
/** 
 *  que define el comportamiento esperado de toda clase encargada de obtener la
 * representacion html de una capa. 
**/
class HtmlFileRepresentationGetter : public ElementHtmlRepresentationGetter {
   /** ctor copia **/
   HtmlFileRepresentationGetter(const HtmlFileRepresentationGetter&);

public:
   /** ctor default **/
   HtmlFileRepresentationGetter();
   /** dtor */
   virtual ~HtmlFileRepresentationGetter();
   /** Metodo polimorfico encargado de obtener la representacion html de la capa que se
    *  encuentra por parametro.
    * @param[in] pElement elemento sobre el que se obtendra su representacion html
    * @param[out] Representation representacion en formato html del elemento
    * @return true en caso de poder obtener correctamente la interpretacion html del elemento
    * @return false en caso contrario 
   **/
   virtual bool DoGetRepresentation(const Element* pElement, std::string& Representation) const;
};
}  /** namespace suri **/
#endif   // __HTML_FILE_REPRESENTATION_H__
