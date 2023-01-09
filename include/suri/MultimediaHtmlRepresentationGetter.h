/*! \file MultimediaHtmlRepresentationGetter.h */                                                     
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 
#ifndef __MULTIMEDIA_HTML_FILE_REPRESENTATION_H__
#define __MULTIMEDIA_HTML_FILE_REPRESENTATION_H__

// Include Estandar
#include <string>

// Includes Suri
#include "suri/ElementHtmlRepresentationGetter.h"

class wxString;

namespace suri {
// forward
class Element;
/** 
 *  que define el comportamiento esperado de toda clase encargada de obtener la
 * representacion html de una capa. 
**/
class MultimediaHtmlRepresentationGetter : public ElementHtmlRepresentationGetter {
   /** ctor copia **/
   MultimediaHtmlRepresentationGetter(const MultimediaHtmlRepresentationGetter&);

public:
   /** ctor default **/
   MultimediaHtmlRepresentationGetter();
   /** dtor */
   virtual ~MultimediaHtmlRepresentationGetter();
   /** Metodo polimorfico encargado de obtener la representacion html de la capa que se
    *  encuentra por parametro.
    * @param[in] pElement elemento sobre el que se obtendra su representacion html
    * @param[out] Representation representacion en formato html del elemento
    * @return true en caso de poder obtener correctamente la interpretacion html del elemento
    * @return false en caso contrario 
   **/
   virtual bool DoGetRepresentation(const Element* pElement, std::string& Representation) const;

private:
   /**
    * Objetivo: Verifica que exista el directorio donde se encuentran los templates
    * html para visualizar la informacion del PAT.
    * En caso de que el directorio no exista lo crea y extrae los templates.
    * @param[in]	Path: path del directorio a verificar.
    * @return si existe o si puede crearlo retorna true; otro caso false.
    */
   bool CheckDataDirectory(const wxString &Path) const;
};
}  /** namespace suri **/
#endif   // __MULTIMEDIAMULTIMEDIA_HTML_FILE_REPRESENTATION_H__
