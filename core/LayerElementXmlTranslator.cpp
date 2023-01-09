/*! \file LayerElementXmlTranslator.cpp */
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
#include "LayerElementXmlTranslator.h"
#include "suri/Element.h"
#include "suri/LayerInterface.h"
#include "suri/xmlnames.h"

// Includes Wx
#include "wx/xml/xml.h"

// Defines
// forwards

namespace suri {

/**
 * Constructor. Inicializa la instancia del traductor
 * @param pLayer puntero a la capa a traducir o que recibe la traduccion
 * @param pElement puntero al elemento a traducir o recibir traduccion
 */
LayerElementXmlTranslator::LayerElementXmlTranslator(LayerInterface* pLayer,
                                                     Element* pElement) :
      pLayer_(pLayer), pElement_(pElement) {
}

/** Destructor **/
LayerElementXmlTranslator::~LayerElementXmlTranslator() {
}

/**
 * Realiza la traduccion XML directa o inversa que es representada por la clase.
 * Devuelve el XML resultante o NULL en caso de no poder realizar la traduccion.
 * @param[in] Direct bool que indica si la transformacion es directa (true)
 * o inversa (false)
 * @return referencia a xml creado, NULL en caso de no poder generar la traduccion.
 */
wxXmlNode* LayerElementXmlTranslator::GenerateXmlTranslation(bool Direct) const {
   if (!IsPossibleToTranslate())
      return NULL;
   if (Direct)
      return GenerateDirectTranslation();
   else
      return GenerateReverseTranslation();
}
/**
 * Aplica la traduccion XML directa o inversa que es representada por la clase,
 * actualizando el contenido, que proviene del obtjeto a traducir, del objeto que
 * recibe el contenido traducido.
 * @param[in] Direct bool que indica si la transformacion es directa (true)
 * o inversa (false)
 * @return si pudo aplicarse la traduccion, false en caso contrario
 */
bool LayerElementXmlTranslator::ApplyTranslation(bool Direct) {
   if (!IsPossibleToTranslate())
      return false;
   if (Direct)
      return ApplyDirectTranslation();
   else
      return ApplyReverseTranslation();
}

/**
 * Metodo auxiliar que genera la traduccion Layer->Element.
 * La traduccion directa utiliza el elemento interno a la capa
 * Precondicion: es posible realizar la traduccion
 * @return puntero a nodo XML correspondiente a un xml de elemento con la informacion
 * correspondiente a la traduccion de Layer a Element.
 * @return NULL en caso contrario
 */
wxXmlNode* LayerElementXmlTranslator::GenerateDirectTranslation() const {
   return new wxXmlNode(*pLayer_->GetElement()->GetNode());
}

/**
 * Metodo auxiliar que genera la traduccion inversa (Element->Layer)
 * Genera un xml del estilo:
 * \verbatim
 *   <Layer>
 *       <Server>
 *          <OnlineResource />
 *       </Server>
 *       <Extension>
 *          <renderizacion>
 *               ...
 *          </renderization>
 *       </Extension>
 *   </Layer>
 * \endverbatim
 * Precondicion: es posible realizar la traduccion
 * @return puntero a nodo XML correspondiente a un xml de elemento con la informacion
 * correspondiente a la traduccion inversa.
 * @return NULL en caso contrario
 */
wxXmlNode* LayerElementXmlTranslator::GenerateReverseTranslation() const {
   Element* pelement = GetElementInUse();
   wxXmlNode* prenderization = pelement->GetNode(RENDERIZATION_NODE);
   if (!prenderization)
      return NULL;
   wxXmlNode* playernode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, LAYER_NODE2);
   wxXmlNode* playerservernode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                               LAYER_SERVER_NODE2);
   playerservernode->AddProperty(LAYER_SERVER_SERVICE_PROPERTY,
                                 LAYER_SERVER_SURI_SERVICE_VALUE);
   wxXmlNode* ponlineresource = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                              LAYER_SERVER_ONLINE_RESOURCE_NODE2);
   ponlineresource->AddProperty(ONLINE_RESOURCE_TYPE_PROPERTY,
                                ONLINE_RESOURCE_TYPE_PROPERTY_VALUE);
   ponlineresource->AddProperty(ONLINE_RESOURCE_HREF_PROPERTY,
                                pLayer_->GetAssociatedDatasourceId());
   playerservernode->AddChild(ponlineresource);
   playernode->AddChild(playerservernode);
   wxXmlNode* pextension = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                         LAYER_EXTENSION_NODE2);
   pextension->AddChild(new wxXmlNode(*prenderization));
   playernode->AddChild(pextension);
   return playernode;
}

/** Metodo auxiliar que genera la traduccion Layer->Element
 *  La traduccion actualiza la informacion del elemento interno a la capa
 *  En caso de que el elemento externo sea NULL, la traduccion no se aplica ya que no
 *  tiene sentido.
 *  La traduccion actualiza el elemento interno de la fuente de datos con el elemento
 externo que se pasa en el constructor
 *  Precondicion: es posible realizar la traduccion
 *  @return true en caso de aplicar la traduccion correctamente
 *  @return false en caso contrario
 **/
bool LayerElementXmlTranslator::ApplyDirectTranslation() {
   if (!pElement_)
      return false;
   wxXmlNode* pelementnewnode = new wxXmlNode(*pLayer_->GetElement()->GetNode());
   pElement_->AddNode(NULL, pelementnewnode, true);
   return true;
}

/** Metodo auxiliar que genera la traduccion Element->Layer
 * A partir del nodo xml de elemento genera la estructura
 * Precondicion: es posible realizar la traduccion
 * @return true en caso de aplicar la traduccion inversa correctamente
 * @return false en caso contrario
 * **/
bool LayerElementXmlTranslator::ApplyReverseTranslation() {
   if (!pElement_)
      return false;
   wxXmlNode* pelementnewnode = new wxXmlNode(*pElement_->GetNode());
   pLayer_->GetElement()->AddNode(NULL, pelementnewnode, true);
   return true;
}

/** Determina que tipo de elemento se usa. El interno a la capa o el externo
 *  @return Element* puntero al elemento a usar
 *  @return NULL en caso de que no exista elemento para usar
 **/
Element* LayerElementXmlTranslator::GetElementInUse() const {
   return (pElement_ == NULL) ? pLayer_->GetElement() : pElement_;
}

/**
 * Valida que se tenga los requerimientos necesarios para poder realizar
 * la traduccion.
 * @return true en caso de poder realizar la traduccion
 * @return false en caso contrario
 */
bool LayerElementXmlTranslator::IsPossibleToTranslate() const {
   return (pLayer_ && GetElementInUse());
}
} /** namespace suri */
