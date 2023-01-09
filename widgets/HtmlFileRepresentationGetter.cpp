/*! \file HtmlFileRepresentationGetter.cpp */
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 

// Includes Suri
#include "suri/HtmlFileRepresentationGetter.h"
#include "suri/Element.h"
#include "HtmlElement.h"

namespace suri {

/** ctor default **/
HtmlFileRepresentationGetter::HtmlFileRepresentationGetter() {
}
/** dtor */
HtmlFileRepresentationGetter::~HtmlFileRepresentationGetter() {
}

/** Metodo polimorfico encargado de obtener la representacion html de la capa que se
 *  encuentra por parametro.
 * @param[in] pElement elemento sobre el que se obtendra su representacion html
 * @param[out] Representation representacion en formato html del elemento
 * @return true en caso de poder obtener correctamente la interpretacion html del elemento
 * @return false en caso contrario 
 **/
bool HtmlFileRepresentationGetter::DoGetRepresentation(const Element* pElement,
                                          std::string& Representation) const {
   if (!dynamic_cast<const HtmlElement*>(pElement))
      return false;
   //No se traduce, elemento de XML
   Representation = pElement->GetNode(URL_NODE)->GetNodeContent().c_str();
   return  true;
}

}  /** namespace suri */
