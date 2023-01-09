/*! \file XmlTranslatorBuilder.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "suri/XmlTranslatorBuilder.h"
#include "LayerElementXmlTranslator.h"
#include "DatasourceElementXmlTranslator.h"
// Includes Wx
// Defines
// forwards

namespace suri {

XmlTranslatorBuilder::XmlTranslatorBuilder() {
}

XmlTranslatorBuilder::~XmlTranslatorBuilder() {
}

/** Crea y configura un traductor Xml entre LayerInterface y Element.
 *  Este metodo no se hace responsable de la destruccion del traductor generado
 *  @param[in] pLayer capa que contiene la informacion a ser traducida.
 *  @param[in] pElement puntero al !Element que recibe la traduccion.
 *  @return Traductor LayerElement creado y configurado correctamente,
 *  NULL en caso contrario
 **/
LayerElementXmlTranslator* XmlTranslatorBuilder::GetLayerElementXmlTranslator(
      LayerInterface* pLayer, Element* pElement) {
   return new LayerElementXmlTranslator(pLayer, pElement);
}

/** Crea y configura un traductor Xml entre DatasourceInterface y Element.
 *  Este metodo no se hace responsable de la destruccion del traductor generado
 *  @param[in] pDatasource fuente de datos que contiene la informacion
 *  a ser traducida.
 *  @param[in] pElement puntero al !Element que recibe la traduccion.
 *  @return Traductor DatasourceElement creado y configurado correctamente,
 *  NULL en caso contrario
 **/
DatasourceElementXmlTranslator* XmlTranslatorBuilder::GetDatasourceElementXmlTranslator(
      DatasourceInterface* pDatasource, Element* pElement) {
   return new DatasourceElementXmlTranslator(pDatasource, pElement);
}
} /** namespace suri */
