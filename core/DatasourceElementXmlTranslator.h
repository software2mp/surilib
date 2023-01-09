/*! \file DatasourceElementXmlTranslator.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DATASOURCEELEMENTXMLTRANSLATOR_H_
#define DATASOURCEELEMENTXMLTRANSLATOR_H_

// Includes Estandar
// Includes Suri
#include "suri/XmlTranslatorInterface.h"
#include "suri/Element.h"
#include "suri/DatasourceInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Implementacion concreta de XmlTranslatorInterface. Es la encargada de realizar
 * la traduccion del contenido de los XML de DatasourceInterface y de Element
 */
class DatasourceElementXmlTranslator : public XmlTranslatorInterface {
public:
   DatasourceElementXmlTranslator(DatasourceInterface* pDatasource, Element* pElement);
   virtual ~DatasourceElementXmlTranslator();
   /**
    * Realiza la traduccion XML directa o inversa que es representada por la clase.
    * Devuelve el XML resultante o NULL en caso de no poder realizar la traduccion.
    */
   virtual wxXmlNode* GenerateXmlTranslation(bool Direct = true) const;
   /**
    * Aplica la traduccion XML directa o inversa que es representada por la clase,
    * actualizando el contenido, que proviene del obtjeto a traducir, del objeto que
    * recibe el contenido traducido.
    * (ej. con la informacion de una !LayerInterface concreta actualiza el XML de Element).
    */
   virtual bool ApplyTranslation(bool Direct = true);

private:
   /** Metodo auxiliar que genera la traduccion Datasource->Element **/
   wxXmlNode* GenerateDirectTranslation() const;
   /** Metodo auxiliar que genera la traduccion Element->Datasource **/
   wxXmlNode* GenerateReverseTranslation() const;
   /** Metodo auxiliar que genera la traduccion Datasource->Element **/
   bool  ApplyDirectTranslation();
   /** Metodo auxiliar que genera la traduccion Element->Datasource **/
   bool ApplyReverseTranslation();
   /** Valida que se tenga los requerimientos necesarios para poder realizar
    * la traduccion. */
   virtual bool IsPossibleToTranslate() const;
   /** Determina que tipo de elemento se usa. El interno a la capa o el externo **/
   Element *GetElementInUse()const;
   /** Cambia el contenido de el nodo renderizacion por un string vacio **/
   void ClearElementRenderizationContent(wxXmlNode* pNode)const;
   /** Puntero a la fuente de datos **/
   DatasourceInterface* pDatasource_;
   /** Puntero al element **/
   Element* pElement_;
};

} /** namespace suri */

#endif /* DATASOURCEELEMENTXMLTRANSLATOR_H_ */
