/*! \file WxsGetCapabilities.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXSGETCAPABILITIES_H_
#define WXSGETCAPABILITIES_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
// Includes Wx
// Defines
// forwards
class wxXmlNode;

namespace suri {

/**
 * Clase que representa la informacion de las capas disponibles en el servicio
 *  WMS
 */
class WxsLayersInformation {

public:
	/** struct con informacion de estilo **/
	struct WxsStyleNode {
		std::string name_;
		std::string title_;
		/** struct con informacion acerca del formato y recurso **/
		struct WxsLengendUrl {
			std::string format_;
			std::string onlineResourceHref_;
			std::string onlineResourceHtype_;
		};
		struct WxsLengendUrl legendurl_;
	};
	struct BoundingBox {
		std::string srs_;
		std::string minX_;
		std::string maxX_;
		std::string maxY_;
		std::string minY_;
	};

	/** struct que contiene la informacion **/
	struct WxsLayerNode {
		std::string title_;
		std::string name_;
		std::string abstract_;
		bool queryable_;
		std::vector<WxsStyleNode> styles_;
		std::vector<std::string> srsList_;
		std::vector<BoundingBox> boundingBox_;
		std::vector<WxsLayerNode> childLayers_;
		std::vector<std::string> tileMatrixSetLinks_;
	};
	WxsLayerNode rootlayer_;
};
/**
 * Clase encarga de almacenar y encapsular la informacion que proviene de Getcapabilities.
 * Posee dos structs en donde almacena la respuesta:
 *  WxsServiceInformation: almacena la informacion correspondiente al nodo
 *  Service de la respuesta
 *  WxsLayersInformation: almacena la informacion de las capas del servicio WMS
 *  en forma de arbol dado que es la naturaleza de la estructura de la informacion de capas.
 */
class WxsCapabilitesInformation {
public:
	/**
	 * struct que representa la informacion asociada al servicio
	 *  que brinda el WMS segun estandard OGC
	 */
	struct WxsServiceInformation {
		std::string title_;
		std::string name_;
		std::string abstract_;
		std::string onlineResource_;
		std::string serviceType_;
		std::string version_;
	};
	/** struct que contiene informacion acerca de los conjuntos
	 *  de tiles que brinda un servicio WMTS segun OGC
	 */
	struct WxsTileMatrixSet {
		std::string identifier_;
		std::string srs_;
		struct WxsTileMatrix {
			std::string identifier_;
			std::string scaleDenominator_;
			WxsLayersInformation::BoundingBox bbox_;
			std::string upperx_;
			std::string uppery_;
			int tileWidth_;
			int tileHeight_;
			int matrixWidth_;
			int matrixHeight_;
		};
		std::vector<WxsTileMatrix> tileMatrixVector_;
	};
	WxsServiceInformation serviceInfo_;
	WxsLayersInformation layerInfo_;
	std::vector<WxsTileMatrixSet> tileMatrixSets_;
};

/**
 *  Clase encargada de procesar la solicitud de GetCapabilities y encapsular la
 *  respuesta en una estructura de datos mas versatil.
 */
class WxsGetCapabilities {
	/** ctor copia **/
	WxsGetCapabilities(const WxsGetCapabilities&);
public:
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
	 * @param[in] Version numero de version de GetCapabilities a consultar
	 * sobre la respuesta a la solicitud GetCapabilities
	 * @return true en caso de poder procesar con exito la solicitud
	 * @return false en otro caso
	 */
	static bool GetCapabilitiesRequest(const std::string& WxsUrl,
			WxsCapabilitesInformation& GetCapabilitesResponse,
			std::string& ServiceType,
			const std::string& Version = "1.1.1");
};


/**
 *  Clase base que representa un parser de respuesta de GetCapabilities de un
 *  servidor Wxs. Implementa un TemplateMethod para establecer el orden de parseo
 *  utilizando una cadena de responsabilidades entre los diferentes parsers
 *  deseados
 */
class WxsGetCapabilitiesParser {
public:
	WxsGetCapabilitiesParser() :
			pSucesor_(NULL) {
	}
	virtual ~WxsGetCapabilitiesParser() {
		delete pSucesor_;
	}
	/** Template Method para parseo y Chain of Responsabilties **/
	virtual bool ParseGetCapabilities(const std::string& WxsUrl,
			WxsCapabilitesInformation& GetCapabilitesResponse,
			std::string& ServiceType,
			const std::string& Version = "1.1.1") {
		if (!pSucesor_
				|| !pSucesor_->ParseGetCapabilities(WxsUrl,
						GetCapabilitesResponse, ServiceType, Version))
			return DoParseGetCapabilities(WxsUrl, GetCapabilitesResponse,
					ServiceType,
					Version);
		return true;
	}
	/** Metodo que configura el sucesor para la cadena de responsabilidades
	 *  en el parseo de GetCapabilities
	 */
	virtual void SetSucesor(WxsGetCapabilitiesParser* pSucesor) {
		pSucesor_ = pSucesor;
	}

protected:
	/** Metodo que realiza el parseo de GetCapabilities
	 *  @return true en caso de realizar el parseo con exito
	 *  @return false en caso de no poder realizar el parseo
	 */
	virtual bool DoParseGetCapabilities(const std::string& WxsUrl,
			WxsCapabilitesInformation& GetCapabilitesResponse,
			std::string& ServiceType,
			const std::string& Version = "1.1.1")=0;
private:
	WxsGetCapabilitiesParser* pSucesor_;
};

/**
 * Clase encargada de configurar la cadena de parseo para respuestas
 * GetCapabilities
 */
class WxsGetCapabilitiesParserBuilder {
	/** evita construccion copia**/
	WxsGetCapabilitiesParserBuilder(const WxsGetCapabilitiesParserBuilder&);

public:
	/** Metodo que genera la cadena de responsabilidades de parsers para
	 *  solicitudes GetCapabilities
	 */
	static WxsGetCapabilitiesParser* CreateParserChain();
};
} /** namespace suri */

#endif /* WXSGETCAPABILITIES_H_ */
