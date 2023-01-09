/*! \file WmsGetCapabilitiesParser.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
