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

#ifndef WMTSGETCAPABILITIESPARSER_H_
#define WMTSGETCAPABILITIESPARSER_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "WxsGetCapabilities.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/**
 *  Clase encargada de realizar el parseo de solicitudes GetCapabilities
 *  de un servidor WMTS
 */
class WmtsGetCapabilitiesParser : public WxsGetCapabilitiesParser {
private:
	/** Metodo que realiza el parseo de GetCapabilities
	 *  @return true en caso de realizar el parseo con exito
	 *  @return false en caso de no poder realizar el parseo
	 */
	virtual bool DoParseGetCapabilities(const std::string& WxsUrl,
			WxsCapabilitesInformation& GetCapabilitesResponse,
			std::string& ServiceType,
			const std::string& Version = "1.0.0");
	/**
	 * Metodo encargado de parsear la respuesta de la solicitud de GetCapabilities
	 * @return true en caso de parsear correctamente la solicitud
	 * @return false en caso de falla
	 */
	bool ParseGetCapabilitiesResponse(wxXmlNode* pRootNode,
			WxsCapabilitesInformation& GetCapabilitesResponse);
	/**
	 * Metodo que parsea el nodo Service de la respuesta
	 * a la consulta GetCapabilities sobre un servidor WMTS. Este es el
	 * nodo que contiene informacion acerca de las prestaciones
	 * un servicio WMTS
	 * @param[in] pServiceNode nodo XML que contiene la informacion a parsear
	 * @param[out] GetCapabilitesResponse estructura que almacenara la informacion del nodo
	 * @return true en caso de parsear con exito el xml
	 * @return false en caso contrario
	 */
	bool ParseServiceIdNode(wxXmlNode* pServiceNode,
                    WxsCapabilitesInformation& GetCapabilitesResponse);
	bool ParseOperationsNode(wxXmlNode* pOperationNode,
                    WxsCapabilitesInformation& GetCapabilitesResponse);
	/**
	 * Metodo que parsea el nodo Content de la respuesta
	 * a la consulta GetCapabilities sobre un servidor WMTS. Este es el
	 * nodo que contiene informacion acerca de las capas y los tiles que ofrece
	 * un servicio WMTS
	 * @param[in] pContentNode nodo XML que contiene la informacion a parsear
	 * @param[out] GetCapabilitesResponse estructura que almacenara la informacion del nodo
	 * @return true en caso de parsear con exito el xml
	 * @return false en caso contrario
	 */
	bool ParseContentsNode(wxXmlNode* pContentNode,
                     WxsCapabilitesInformation& GetCapabilitesResponse);
	/**
	 * Metodo que parsea el nodo Layer de la respuesta
	 * a la consulta GetCapabilities sobre un servidor WMTS
	 * @param[in] pLayerNode nodo XML que contiene la informacion a parsear
	 * @param[out] GetCapabilitesResponse estructura que almacenara la informacion del nodo
	 * @return true en caso de parsear con exito el xml
	 * @return false en caso contrario
	 */
	bool ParseLayerNode(wxXmlNode* pLayerNode,
			WxsCapabilitesInformation& GetCapabilitesResponse);
	/**
	 * Metodo que parsea el nodo WGS84BoundingBox de la respuesta
	 * a la consulta GetCapabilities sobre un servidor WMTS
	 * @param[in] pBboxNode nodo XML que contiene la informacion a parsear
	 * @param[out] Layer estructura que almacenara la informacion del nodo
	 * @return true en caso de parsear con exito el xml
	 * @return false en caso contrario
	 */
	bool ParseWgs84BBox(wxXmlNode* pBboxNode,
			WxsLayersInformation::WxsLayerNode& Layer);
	/**
	 * Metodo que parsea el nodo Style de la respuesta
	 * a la consulta GetCapabilities sobre un servidor WMTS
	 * @param[in] pStyleNode nodo XML que contiene la informacion a parsear
	 * @param[out] GetCapabilitesResponse estructura que almacenara la informacion del nodo
	 * @return true en caso de parsear con exito el xml
	 * @return false en caso contrario
	 */
	bool ParseLayerStyleNode(wxXmlNode* pStyleNode,
			WxsLayersInformation::WxsStyleNode& Style);
	/**
	 * Metodo que parsea el nodo TileMatrixSet de la respuesta
	 * a la consulta GetCapabilities sobre un servidor WMTS
	 * @param[in] pTileMatrixNode nodo XML que contiene la informacion a parsear
	 * @param[out] GetCapabilitesResponse estructura que almacenara la informacion del nodo
	 * @return true en caso de parsear con exito el xml
	 * @return false en caso contrario
	 */
	bool ParseTileMatrixSetNode(wxXmlNode* pTileMatrixNode,
			WxsCapabilitesInformation& GetCapabilitesResponse);
	/**
	 * Metodo que parsea el nodo TileMatrix de la respuesta
	 * a la consulta GetCapabilities sobre un servidor WMTS
	 * @param[in] pTileMatrixNode nodo XML que contiene la informacion a parsear
	 * @param[out] TimeMatrixSet estructura que almacenara la informacion del nodo
	 * @return true en caso de parsear con exito el xml
	 * @return false en caso contrario
	 */
	bool ParseTileMatrixNode(wxXmlNode* pTileMatrix,
			WxsCapabilitesInformation::WxsTileMatrixSet& TileMatrixSet);
	/**
	 * Metodo que parsea el nodo TileMatrixSetLink de la respuesta
	 * a la consulta GetCapabilities sobre un servidor WMTS
	 * @param[in] pTileMatrixSetLinkNode nodo XML que contiene la informacion a parsear
	 * @param[out] Layer estructura que almacenara la informacion del nodo
	 * @return true en caso de parsear con exito el xml
	 * @return false en caso contrario
	 */
	bool ParseTileMatrixSetLinkNode(wxXmlNode* pTileMatrixSetLinkNode,
			WxsLayersInformation::WxsLayerNode& Layer);
};

} /** namespace suri */

#endif /* WMTSGETCAPABILITIESPARSER_H_ */
