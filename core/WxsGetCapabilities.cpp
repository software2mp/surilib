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
#include "suri/WxsGetCapabilities.h"
#include "logmacros.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/WmsGetCapabilitiesParser.h"
#include "suri/WmtsGetCapabilitiesParser.h"
#include "suri/GMGetCapabilitiesParser.h"

// Includes Wx
#include "wx/string.h"
#include "wx/filename.h"
#include "wx/xml/xml.h"

// Defines
// forwards

namespace suri {

/**
 * Metodo estatico que procesa la solicitud de GetCapabilities
 * hacia un servidor wms utilizando libcurl.
 * El resultado de del procesamiento de la solicitud es una estructura
 * de datos que brinda la informacion de getcapabilities segun estandard
 * OGC
 * La solicitud de GetCapabilities es del estilo:
 * http://<hostname>/<deploy_name>/com.esri.wms.Esrimap?
 * VERSION=1.1.1&REQUEST=GetCapabilities&SERVICE=WMS&
 * @param[in] WxsUrl url del servidor wms a consultar
 * @param[out] GetCapabilitiesResponse estructura con la informacion
 * sobre la respuesta a la solicitud GetCapabilities
 * @param[out] ServiceType tipo del servicio reconocido por el parseo exitoso
 * de la solicitud
 * @param[in] Version numero de version de GetCapabilities a consultar
 * sobre la respuesta a la solicitud GetCapabilities
 * @return true en caso de poder procesar con exito la solicitud
 * @return false en otro caso
 */
bool WxsGetCapabilities::GetCapabilitiesRequest(const std::string& WxsUrl,
		WxsCapabilitesInformation& GetCapabilitesResponse,
		std::string& ServiceType, const std::string& Version) {
	// Se procesa la solicitud en un archivo temporal
	WxsGetCapabilitiesParser* pparser = WxsGetCapabilitiesParserBuilder::CreateParserChain();
	if (!pparser)
		return false;
	bool returnvalue = pparser->ParseGetCapabilities(WxsUrl,
			GetCapabilitesResponse, ServiceType, Version);
	delete pparser;
	return returnvalue;
}

/** Metodo que genera la cadena de responsabilidades de parsers para
 *  solicitudes GetCapabilities
 */
WxsGetCapabilitiesParser* WxsGetCapabilitiesParserBuilder::CreateParserChain() {
   GMGetCapabilitiesParser* pgm = new GMGetCapabilitiesParser;

   WmtsGetCapabilitiesParser* pwmts = new WmtsGetCapabilitiesParser;
   pwmts->SetSucesor(pgm);

   WxsGetCapabilitiesParser* pwxs = new WmsGetCapabilitiesParser;
   pwxs->SetSucesor(pwmts);

	return pwxs;
}
} /** namespace suri */
