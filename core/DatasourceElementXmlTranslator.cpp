/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

// Includes Estandar
// Includes Suri
#include "DatasourceElementXmlTranslator.h"

// Includes Wx
#include "wx/xml/xml.h"

// Defines
// forwards

namespace suri {

DatasourceElementXmlTranslator::DatasourceElementXmlTranslator(
      DatasourceInterface* pDatasource, Element* pElement) :
      pDatasource_(pDatasource), pElement_(pElement) {
}

DatasourceElementXmlTranslator::~DatasourceElementXmlTranslator() {
}

/**
 * Realiza la traduccion XML directa o inversa que es representada por la clase.
 * Devuelve el XML resultante o NULL en caso de no poder realizar la traduccion.
 * @param[in] Direct bool que indica si la transformacion es directa (true)
 * o inversa (false)
 * @return referencia a xml creado, NULL en caso de no poder generar la traduccion.
 */
wxXmlNode* DatasourceElementXmlTranslator::GenerateXmlTranslation(bool Direct) const {
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
 * @param Direct bool que indica si la transformacion es directa (true) o inversa (false)
 * @return true si pudo aplicarse la traduccion, false en caso contrario
 */
bool DatasourceElementXmlTranslator::ApplyTranslation(bool Direct) {
   if (!IsPossibleToTranslate())
      return false;
   if (Direct)
      return ApplyDirectTranslation();
   else
      return ApplyReverseTranslation();
}

/**
 * Metodo auxiliar que genera la traduccion Datasource->Element
 * Precondicion: es posible realizar la traduccion
 * @return puntero a nodo XML correspondiente a un xml de elemento con la informacion
 * correspondiente a la traduccion de Datasource a Element.
 * @return NULL en caso contrario
 */
wxXmlNode* DatasourceElementXmlTranslator::GenerateDirectTranslation() const {
   wxXmlNode* pelementnode = new wxXmlNode(*pDatasource_->GetElement()->GetNode());
   ClearElementRenderizationContent(pelementnode);
   return pelementnode;
}

/**
 * Metodo auxiliar que genera la traduccion inversa (Element->Datasource)
 * Genera un nodo del estilo
 * \verbatim
 *    <Datasource>
 *       <elemento>
 *          ...
 *       </elemento>
 *    </Datasource>
 * \endverbatim
 * Precondicion: es posible realizar la traduccion
 * @return puntero a nodo XML correspondiente a un xml de elemento con la informacion
 * correspondiente a la traduccion inversa.
 * @return NULL en caso contrario
 */
wxXmlNode* DatasourceElementXmlTranslator::GenerateReverseTranslation() const {
   wxXmlNode* pdatasourcenode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                              DATASOURCE_NODE2);
   pdatasourcenode->AddProperty(DATASOURCE_ID_PROPERTY, pDatasource_->GetId());
   pdatasourcenode->AddChild(new wxXmlNode(*GetElementInUse()->GetNode()));
   ClearElementRenderizationContent(pdatasourcenode->GetChildren());
   return pdatasourcenode;
}

/** Metodo auxiliar que genera la traduccion Datasource->Element
 * En caso de que el elemento externo sea NULL, la traduccion no se aplica ya que no
 tiene sentido.
 La traduccion actualiza el elemento interno de la fuente de datos con el elemento
 externo que se pasa en el constructor
 * Precondicion: es posible realizar la traduccion
 * @return true en caso de poder aplicar la traduccion directa
 * @return false en caso contrario
 **/
bool DatasourceElementXmlTranslator::ApplyDirectTranslation() {
   if (!pElement_)
      return false;
   wxXmlNode* pelementnewnode = new wxXmlNode(*pDatasource_->GetElement()->GetNode());
   pElement_->AddNode(NULL, pelementnewnode, true);
   return true;
}

/** Metodo auxiliar que genera la traduccion Element->Datasource
 *  Precondicion: es posible realizar la traduccion
 *  @return true en caso de poder aplicar la traduccion inversa
 *  @return false en caso contrario
 **/
bool DatasourceElementXmlTranslator::ApplyReverseTranslation() {
   if (!pElement_)
      return false;
   wxXmlNode* pelementnewnode = new wxXmlNode(*pElement_->GetNode());
   pDatasource_->GetElement()->AddNode(NULL, pelementnewnode, true);
   return true;
}

/** Determina que tipo de elemento se usa. El interno a la capa o el externo
 *  @return Element* puntero al elemento a usar
 *  @return NULL en caso de que no exista elemento para usar
 **/
Element* DatasourceElementXmlTranslator::GetElementInUse() const {
   return (!pElement_) ? pDatasource_->GetElement() : pElement_;
}

/**
 * Valida que se tenga los requerimientos necesarios para poder realizar
 * la traduccion.
 * @return true en caso de poder realizar la traduccion
 * @return false en caso contrario
 */
bool DatasourceElementXmlTranslator::IsPossibleToTranslate() const {
   return (pDatasource_ && GetElementInUse());
}

/** Cambia el contenido de el nodo renderizacion por un string vacio
 *  Prencondicion: El nodo renderizacion es hijo directo del nodo pasado
 *  por parametro
 *  @param[in] pNode nodo al que se desea limpiar el subnodo "renderizacion"
 **/
void DatasourceElementXmlTranslator::ClearElementRenderizationContent(
      wxXmlNode* pNode) const {
   wxXmlNode* psearchednode = pNode->GetChildren();
   // busco el nodo renderization
   while (psearchednode && (psearchednode->GetName().CompareTo(RENDERIZATION_NODE) != 0)) {
      psearchednode = psearchednode->GetNext();
   }
   if (!psearchednode)
      return;
   wxXmlNode* pParentNode = psearchednode->GetParent();
   wxXmlNode* pclearrenderization = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                                  RENDERIZATION_NODE);
   pParentNode->InsertChild(pclearrenderization, psearchednode->GetNext());
   pParentNode->RemoveChild(psearchednode);
   delete psearchednode;
}

} /** namespace suri */
