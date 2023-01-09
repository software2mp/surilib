/*! \file WmtsGetCapabilitiesParser.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
