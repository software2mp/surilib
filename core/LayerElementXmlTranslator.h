/*! \file LayerElementXmlTranslator.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LayerElementXmlTranslator_H_
#define LayerElementXmlTranslator_H_

// Includes Estandar

// Includes Suri
#include "suri/XmlTranslatorInterface.h"
#include "suri/Element.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 * Implementacion concreta de XmlTranslatorInterface. Es la encargada de realizar
 * la traduccion del contenido de los XML de LayerInterface y de Element
 */
class LayerElementXmlTranslator : public suri::XmlTranslatorInterface {
public:
   LayerElementXmlTranslator(LayerInterface* pLayer, Element* pElement);
   virtual ~LayerElementXmlTranslator();
   /**
    * Realiza la traduccion XML directa o inversa que es representada por la clase.
    * Devuelve el XML resultante o NULL en caso de no poder realizar la traduccion.
    */
   virtual wxXmlNode* GenerateXmlTranslation(bool Direct = true) const;
   /**
    * Aplica la traduccion XML directa o inversa que es representada por la clase,
    * actualizando el contenido, que proviene del obtjeto a traducir, del objeto que
    * recibe el contenido traducido.
   */
   virtual bool ApplyTranslation(bool Direct = true);

private:
   /** Metodo auxiliar que genera la traduccion Layer->Element **/
   wxXmlNode* GenerateDirectTranslation() const;
   /** Metodo auxiliar que genera la traduccion Element->Layer **/
   wxXmlNode* GenerateReverseTranslation() const;
   /** Metodo auxiliar que genera la traduccion Layer->Element **/
   bool ApplyDirectTranslation();
   /** Metodo auxiliar que genera la traduccion Element->Layer **/
   bool ApplyReverseTranslation();
   /** Determina que tipo de elemento se usa. El interno a la capa o el externo **/
   Element* GetElementInUse() const;
   /** Valida que se tenga los requerimientos necesarios para poder realizar
    * la traduccion. **/
   virtual bool IsPossibleToTranslate() const;
   /** Puntero a la capa **/
   LayerInterface* pLayer_;
   /** Puntero al element **/
   Element* pElement_;
};

} /** namespace suri */

#endif /* LayerElementXmlTranslator_H_ */
