/*! \file WxsGetCapabilities.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
