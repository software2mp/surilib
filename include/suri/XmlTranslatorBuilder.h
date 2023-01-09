/*! \file XmlTranslatorBuilder.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef XMLTRANSLATORBUILDER_H_
#define XMLTRANSLATORBUILDER_H_

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class Element;
class LayerInterface;
class DatasourceInterface;
class LayerElementXmlTranslator;
class DatasourceElementXmlTranslator;

/*!
 * Clase encargada de la creacion y configuracion de diferentes traductores XML
 */
class XmlTranslatorBuilder {
public:
   XmlTranslatorBuilder();
   virtual ~XmlTranslatorBuilder();
   /**! Crea y configura un traductor Xml entre LayerInterface y Element.
    * Este metodo no se hace responsable de la destruccion del traductor generado
    **/
   static LayerElementXmlTranslator* GetLayerElementXmlTranslator(
         LayerInterface* pLayer, Element* pElement);
   /**! Crea y configura un traductor Xml entre DatasourceInterface y Element.
    * Este metodo no se hace responsable de la destruccion del traductor generado**/
   static DatasourceElementXmlTranslator* GetDatasourceElementXmlTranslator(
         DatasourceInterface* pDatasource, Element* pElement);
};

} /** namespace suri */

#endif /* XMLTRANSLATORBUILDER_H_ */
