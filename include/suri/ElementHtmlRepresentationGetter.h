/*! \file ElementHtmlRepresentationGetter.h */ 
/**                                             
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
*/
#ifndef __ELEMENT_HTML_FILE_REPRESENTATION_GETTER_H__
#define __ELEMENT_HTML_FILE_REPRESENTATION_GETTER_H__

// Include Estandar
#include <string>

namespace suri {
// forward
class Element;
/** 
 *  que define el comportamiento esperado de toda clase encargada de obtener la
 * representacion html de una capa. 
**/
class ElementHtmlRepresentationGetter {
   /** ctor copia **/
   ElementHtmlRepresentationGetter(const ElementHtmlRepresentationGetter&);

public:
   /** ctor default **/
   ElementHtmlRepresentationGetter() : pSuccessor_(NULL) {}
   /** dtor */
   virtual ~ElementHtmlRepresentationGetter() {
      if (pSuccessor_)
         delete pSuccessor_;
   }
   /**
    * Metodo template encargado de obtener la representacion html de la capa que se pasa por parametro
    * recorriendo la cadena de responsabilidades
    * @param[in] pElement elemento sobre el que se obtendra su representacion html
    * @param[out] Representation representacion en formato html del elemento 
    * @return true en caso de poder obtener correctamente la interpretacion html del elemento
    * @return false en caso contrario
   **/
   bool GetRepresentation(const Element* pElement, std::string& Representation) const {
      if (DoGetRepresentation(pElement, Representation))
         return true;
      return pSuccessor_->DoGetRepresentation(pElement, Representation);
   }

   /** Metodo polimorfico encargado de obtener la representacion html de la capa que se
    *  encuentra por parametro.
    * @param[in] pElement elemento sobre el que se obtendra su representacion html
    * @param[out] Representation representacion en formato html del elemento
    * @return true en caso de poder obtener correctamente la interpretacion html del elemento
    * @return false en caso contrario 
   **/
   virtual bool DoGetRepresentation(const Element* pElement, std::string& Representation) const=0;
   /** Configura el sucesor en la cadena de responsabilidades **/
   void SetSuccessor(ElementHtmlRepresentationGetter* pSuccessor) {
      if (pSuccessor_)
         delete pSuccessor_;
      pSuccessor_ = pSuccessor;
   }

protected:
   ElementHtmlRepresentationGetter* pSuccessor_;
};
}  /** namespace suri **/
#endif   // __ELEMENT_HTML_FILE_REPRESENTATION_GETTER_H__
