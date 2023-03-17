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

#ifndef WMSGETCAPABILITIESPARSER_H_
#define WMSGETCAPABILITIESPARSER_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "WxsGetCapabilities.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase encargada de realizar el parseo de solicitudes GetCapabilities
 *  de un servidor WMS
 */
class WmsGetCapabilitiesParser : public WxsGetCapabilitiesParser {

private:
	/** Metodo que realiza el parseo de GetCapabilities
	 *  @return true en caso de realizar el parseo con exito
	 *  @return false en caso de no poder realizar el parseo
	 */
	virtual bool DoParseGetCapabilities(const std::string& WxsUrl,
			WxsCapabilitesInformation& GetCapabilitesResponse,
			std::string& ServiceType,
			const std::string& Version = "1.1.1");
	/**
	 * Metodo encargado de parsear la respuesta de la solicitud de GetCapabilities
	 * @return true en caso de parsear correctamente la solicitud
	 * @return false en caso de falla
	 */
	bool ParseGetCapabilitiesResponse(wxXmlNode* pRootNode,
			WxsCapabilitesInformation& GetCapabilitesResponse);
	/**
	 *  Funcion encargada de parsear el xml correspondiente a la informacion de servicio
	 *  del WMS
	 */
	bool LoadServiceInfo(wxXmlNode* pServiceNode,
			WxsCapabilitesInformation& GetCapabilitesResponse);
	/**
	 * Funcion encargada de parsear el nodo Capability de un servicio WMS
	 */
	bool LoadCapabiltyInfo(wxXmlNode* pCapabilityNode,
			WxsCapabilitesInformation& GetCapabilitesResponse);
	/**
	 * Funcion encargada de parsear el nodo Layer de un servicio WMS
	 */
	bool LoadCapabilityLayersInfo(wxXmlNode* pLayerNode,
			WxsLayersInformation::WxsLayerNode& LayerInfo);
	/** Funcion que parsea el nodo estilo de una capa wms **/
	void LoadLayerStyleInfo(wxXmlNode* pStyleNode,
			WxsLayersInformation::WxsStyleNode& Style);
	/**
	 * Funcion encargada de parsear el nodo Request de una solicitud
	 * GetCapabilities a un servidor WMS
	 */
	bool LoadCapabilityRequestInfo(wxXmlNode* pLayerNode,
			WxsCapabilitesInformation& GetCapabilitesResponse);

	/** Metodo que parsea el nodo boundingbox de un servicio WMS **/
	void ParseBoundingBox(wxXmlNode* pBoundingBoxInfo,
			std::vector<WxsLayersInformation::BoundingBox>& BoundingBoxOut);
};

} /** namespace suri */

#endif /* WMSGETCAPABILITIESPARSER_H_ */
