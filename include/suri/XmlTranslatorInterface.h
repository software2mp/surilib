/*! \file XmlTranslatorInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef XMLTRANSLATORINTERFACE_H_
#define XMLTRANSLATORINTERFACE_H_


// Includes Estandar

// Includes Suri

// Includes Wx
#include "wx/xml/xml.h"

// Defines

// forwards

namespace suri {
/**
 * Interfaz que representa el comportamiento en comun para todos los traductores XML
 * (podria ser analogo a una XSLT)
 */
class XmlTranslatorInterface {
public:
   XmlTranslatorInterface() {}
   virtual ~XmlTranslatorInterface() {}
   /**
    * Realiza la traduccion XML directa o inversa que es representada por la clase.
    * Devuelve el XML resultante o NULL en caso de no poder realizar la traduccion.
    * @param[in] Direct bool que indica si la transformacion es directa (true)
    * o inversa (false)
    * @return referencia a xml creado, NULL en caso de no poder generar la traduccion.
    */
   virtual wxXmlNode* GenerateXmlTranslation(bool Direct = true)const=0;
   /**
    * Aplica la traduccion XML directa o inversa que es representada por la clase,
    * actualizando el contenido, que proviene del obtjeto a traducir, del objeto que
    * recibe el contenido traducido.
    * (ej. con la informacion de una !LayerInterface concreta actualiza el XML de Element).
    * En caso de que el elemento externo sea NULL, la traduccion no se aplica ya que no
    * tiene sentido.
    * @param Direct bool que indica si la transformacion es directa (true) o inversa (false)
    * @return si pudo aplicarse la traduccion, false en caso contrario
    */
   virtual bool ApplyTranslation(bool Direct = true)=0;

protected:
   /**
    * Valida que se tenga los requerimientos necesarios para poder realizar
    * la traduccion.
    * @return true en caso de poder realizar la traduccion
    * @return false en caso contrario
    */
   virtual bool IsPossibleToTranslate()const=0;
};
}  // namespace suri



#endif /* XMLTRANSLATORINTERFACE_H_ */
