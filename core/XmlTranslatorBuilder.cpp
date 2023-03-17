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
