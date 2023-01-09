/*! \file WmtsGetCapabilitiesParser.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */



// Includes Estandar
#include <curl/curl.h>
#include <curl/easy.h>
#include <string>
#include <vector>

// Includes Suri
#include "suri/WmtsGetCapabilitiesParser.h"
#include "suri/AuxiliaryFunctions.h"
#include "logmacros.h"

// Includes Wx
#include "wx/filename.h"
#include "wx/xml/xml.h"

// Defines
// forwards

namespace suri {

/** Metodo que realiza el parseo de GetCapabilities
 *  @return true en caso de realizar el parseo con exito
 *  @return false en caso de no poder realizar el parseo
 */
bool WmtsGetCapabilitiesParser::DoParseGetCapabilities(const std::string& WxsUrl,
		WxsCapabilitesInformation& GetCapabilitesResponse,
		std::string& ServiceType,
		const std::string& Version) {
   // busca que exista la palabra wmts
   std::string lowerurl = WxsUrl;
   std::transform(lowerurl.begin(), lowerurl.end(), lowerurl.begin(), ::tolower);
	if (lowerurl.find("wmts") == std::string::npos)
	return false;
   std::string tmpfilename = wxFileName::CreateTempFileName("").c_str();
   CURL *curl;
   CURLcode res;
   curl = curl_easy_init();
   if (curl) {
      std::string wxsurl = WxsUrl.c_str();
      if (lowerurl.find("/wmtscapabilities.xml") == std::string::npos &&
            lowerurl.find("getcapabilities") == std::string::npos) {
         wxsurl += "?version=1.0.0&service=WMTS&request=GetCapabilities";
      }
      curl_easy_setopt(curl, CURLOPT_URL, wxsurl.c_str());
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

      FILE* pf = fopen(tmpfilename.c_str(), "w+");
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, pf);

      /* Procesa la solicitud dejando en res el codigo de retorno */
      res = curl_easy_perform(curl);
      if (res != CURLE_OK) {
         REPORT_DEBUG("Error al obtener informacion sobre servicio %s %s",
               WxsUrl.c_str(), curl_easy_strerror(res));
         return false;
      }
      /* s cleanup */
      curl_easy_cleanup(curl);

      fclose(pf);
   }

   wxXmlDocument doc;
   doc.Load(tmpfilename, wxLocale::GetSystemEncodingName(),
         wxXMLDOC_KEEP_WHITESPACE_NODES);
   wxXmlNode* proot = doc.GetRoot();
   ServiceType = "wmts";
   return ParseGetCapabilitiesResponse(proot, GetCapabilitesResponse);
}

/**
 * Metodo encargado de parsear la respuesta de la solicitud de GetCapabilities
 * @return true en caso de parsear correctamente la solicitud
 * @return false en caso de falla
 */
bool WmtsGetCapabilitiesParser::ParseGetCapabilitiesResponse(
						wxXmlNode* pRootNode,
						WxsCapabilitesInformation& GetCapabilitesResponse) {
	wxXmlNode* pcurrent = pRootNode ? pRootNode->GetChildren() : NULL;
	if (!pcurrent)
		return false;
	bool ok = true;
	while (ok && pcurrent) {
		if (pcurrent->GetName().CompareTo("ows:ServiceIdentification") == 0) {
			ok = ParseServiceIdNode(pcurrent, GetCapabilitesResponse);
		} else if (pcurrent->GetName().CompareTo("ows:OperationsMetadata") == 0) {
			ok = ParseOperationsNode(pcurrent, GetCapabilitesResponse);
		} else if ((pcurrent->GetName().CompareTo("Contents") == 0) ||
				(pcurrent->GetName().CompareTo("ows:Contents") == 0)) {
			ok = ParseContentsNode(pcurrent, GetCapabilitesResponse);
		}
		pcurrent = pcurrent->GetNext();
	}
	return ok;
}

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
bool WmtsGetCapabilitiesParser::ParseServiceIdNode(wxXmlNode* pServiceNode,
                   WxsCapabilitesInformation& GetCapabilitesResponse) {
	wxXmlNode* pcurrent = pServiceNode ? pServiceNode->GetChildren() : NULL;
	if (!pcurrent)
		return false;
	bool ok = true;
	WxsCapabilitesInformation::WxsServiceInformation service =
							GetCapabilitesResponse.serviceInfo_;
	while (ok && pcurrent) {
		if (pcurrent->GetName().CompareTo("ows:Title") == 0) {
			service.title_ = pcurrent->GetNodeContent().c_str();
		} else if (pcurrent->GetName().CompareTo("ows:Abstract") == 0) {
			service.abstract_ = pcurrent->GetNodeContent().c_str();
		} else if (pcurrent->GetName().CompareTo("ows:ServiceType") == 0) {
			service.serviceType_ = pcurrent->GetNodeContent().c_str();
		} else if (pcurrent->GetName().CompareTo("ows:ServiceTypeVersion") == 0) {
			service.version_ = pcurrent->GetNodeContent().c_str();
		}
		pcurrent = pcurrent->GetNext();
	}
	return true;
}

bool WmtsGetCapabilitiesParser::ParseOperationsNode(wxXmlNode* pOperationNode,
					WxsCapabilitesInformation& GetCapabilitesResponse) {
	wxXmlNode* pcurrent = pOperationNode ? pOperationNode->GetChildren() : NULL;
	if (!pcurrent)
		return false;
   // TODO(Gabriel - #5019): Implementar
	return true;
}

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
bool WmtsGetCapabilitiesParser::ParseContentsNode(wxXmlNode* pContentNode,
                    WxsCapabilitesInformation& GetCapabilitesResponse) {
	wxXmlNode* pcurrent = pContentNode ? pContentNode->GetChildren() : NULL;
	if (!pcurrent)
		return false;
	bool ok = true;
	GetCapabilitesResponse.layerInfo_.rootlayer_.title_ = "Capas";
	while (ok && pcurrent) {
		if (pcurrent->GetName().CompareTo("Layer") == 0) {
			ok = ParseLayerNode(pcurrent, GetCapabilitesResponse);
		} else if (pcurrent->GetName().CompareTo("TileMatrixSet") == 0) {
			ok = ParseTileMatrixSetNode(pcurrent, GetCapabilitesResponse);
		}
		pcurrent = pcurrent->GetNext();
	}
	return ok;
}

/**
 * Metodo que parsea el nodo Layer de la respuesta
 * a la consulta GetCapabilities sobre un servidor WMTS
 * @param[in] pLayerNode nodo XML que contiene la informacion a parsear
 * @param[out] GetCapabilitesResponse estructura que almacenara la informacion del nodo
 * @return true en caso de parsear con exito el xml
 * @return false en caso contrario
 */
bool WmtsGetCapabilitiesParser::ParseLayerNode(wxXmlNode* pLayerNode,
		WxsCapabilitesInformation& GetCapabilitesResponse) {
	wxXmlNode* pchild = pLayerNode? pLayerNode->GetChildren() : NULL;
	if (!pchild)
		return false;
	WxsLayersInformation::WxsLayerNode layer;
	WxsLayersInformation::WxsStyleNode currstyle;
	while (pchild) {
		if (pchild->GetName().CompareTo("ows:Title") == 0) {
			layer.title_ = pchild->GetNodeContent().c_str();
		} else if (pchild->GetName().CompareTo("ows:Abstract") == 0) {
			layer.abstract_ = pchild->GetNodeContent().c_str();
		} else if (pchild->GetName().CompareTo("ows:WGS84BoundingBox") == 0) {
			ParseWgs84BBox(pchild, layer);
		} else if (pchild->GetName().CompareTo("ows:Identifier") == 0) {
			layer.name_ = pchild->GetNodeContent().c_str();
		} else if (pchild->GetName().CompareTo("Style") == 0) {
			ParseLayerStyleNode(pchild, currstyle);
		} else if (pchild->GetName().CompareTo("Format") == 0) {
			currstyle.legendurl_.format_ = pchild->GetNodeContent().c_str();
			layer.styles_.push_back(currstyle);
		} else if (pchild->GetName().CompareTo("TileMatrixSetLink") == 0) {
			ParseTileMatrixSetLinkNode(pchild, layer);
		}
		pchild = pchild->GetNext();
	}
	if (layer.srsList_.empty())
		layer.srsList_.push_back("EPSG:4326");
	GetCapabilitesResponse.layerInfo_.rootlayer_.childLayers_.push_back(layer);
	return true;
}

/**
 * Metodo que parsea el nodo WGS84BoundingBox de la respuesta
 * a la consulta GetCapabilities sobre un servidor WMTS
 * @param[in] pBboxNode nodo XML que contiene la informacion a parsear
 * @param[out] Layer estructura que almacenara la informacion del nodo
 * @return true en caso de parsear con exito el xml
 * @return false en caso contrario
 */
bool WmtsGetCapabilitiesParser::ParseWgs84BBox(wxXmlNode* pBboxNode,
		WxsLayersInformation::WxsLayerNode& Layer) {
	wxXmlNode* pchild = pBboxNode? pBboxNode->GetChildren() : NULL;
	if (!pchild)
		return false;
	WxsLayersInformation::BoundingBox bbox;
	while (pchild) {
		if (pchild->GetName().CompareTo("ows:LowerCorner") == 0) {
			std::vector<std::string> coords = tokenizer(
					pchild->GetNodeContent().c_str(), " ");
			if (coords.size() != 2)
				return false;
			// lrx
			bbox.maxX_ = coords.at(0);
			// lry
			bbox.minY_ = coords.at(1);
		} else if (pchild->GetName().CompareTo("ows:UpperCorner") == 0) {
			std::vector<std::string> coords = tokenizer(
					pchild->GetNodeContent().c_str(), " ");
			if (coords.size() != 2)
				return false;
			// ulx
			bbox.minX_ = coords.at(0);
			// uly
			bbox.maxY_ = coords.at(1);
		}
		pchild = pchild->GetNext();
	}
	Layer.boundingBox_.push_back(bbox);
	return true;
}

/**
 * Metodo que parsea el nodo Style de la respuesta
 * a la consulta GetCapabilities sobre un servidor WMTS
 * @param[in] pStyleNode nodo XML que contiene la informacion a parsear
 * @param[out] GetCapabilitesResponse estructura que almacenara la informacion del nodo
 * @return true en caso de parsear con exito el xml
 * @return false en caso contrario
 */
bool WmtsGetCapabilitiesParser::ParseLayerStyleNode(wxXmlNode* pStyleNode,
		WxsLayersInformation::WxsStyleNode& Style) {
	wxXmlNode* pchild = pStyleNode ? pStyleNode->GetChildren() : NULL;
	if (!pchild)
		return false;
	while (pchild) {
		if (pchild->GetName().CompareTo("ows:Identifier") == 0) {
			Style.title_ = pchild->GetNodeContent().c_str();
		}
		pchild = pchild->GetNext();
	}
	return true;
}

/**
 * Metodo que parsea el nodo TileMatrixSet de la respuesta
 * a la consulta GetCapabilities sobre un servidor WMTS
 * @param[in] pTileMatrixNode nodo XML que contiene la informacion a parsear
 * @param[out] GetCapabilitesResponse estructura que almacenara la informacion del nodo
 * @return true en caso de parsear con exito el xml
 * @return false en caso contrario
 */
bool WmtsGetCapabilitiesParser::ParseTileMatrixSetNode(
		wxXmlNode* pTileMatrixNode,
		WxsCapabilitesInformation& GetCapabilitesResponse) {
	wxXmlNode* pchild = pTileMatrixNode ? pTileMatrixNode->GetChildren() : NULL;
	if (!pchild)
		return false;
	WxsCapabilitesInformation::WxsTileMatrixSet tilematrixset;
	while (pchild) {
		if (pchild->GetName().CompareTo("ows:Identifier") == 0) {
			tilematrixset.identifier_ = pchild->GetNodeContent().c_str();
		} else if (pchild->GetName().CompareTo("ows:SupportedCRS") == 0) {
			std::vector<std::string> tokens = tokenizer(
					pchild->GetNodeContent().c_str(), ":");
			std::vector<std::string>::iterator it = tokens.begin();
			for (; it != tokens.end(); ++it) {
				if (it->compare("OGC") == 0 || it->compare("EPSG:") == 0) {
					tilematrixset.srs_ = *it;
					tilematrixset.srs_.append(":");
					tilematrixset.srs_.append(*(it+2));
					break;
				}
			}
		} else if (pchild->GetName().CompareTo("WellKnownScaleSet") == 0) {
			// TODO: Implementar
		} else if (pchild->GetName().CompareTo("TileMatrix") == 0) {
			ParseTileMatrixNode(pchild, tilematrixset);
		}
		pchild = pchild->GetNext();
	}
	GetCapabilitesResponse.tileMatrixSets_.push_back(tilematrixset);
	return true;
}

/**
 * Metodo que parsea el nodo TileMatrix de la respuesta
 * a la consulta GetCapabilities sobre un servidor WMTS
 * @param[in] pTileMatrixNode nodo XML que contiene la informacion a parsear
 * @param[out] TimeMatrixSet estructura que almacenara la informacion del nodo
 * @return true en caso de parsear con exito el xml
 * @return false en caso contrario
 */
bool WmtsGetCapabilitiesParser::ParseTileMatrixNode(
		wxXmlNode* pTileMatrixNode,
		WxsCapabilitesInformation::WxsTileMatrixSet& TileMatrixSet) {
	wxXmlNode* pchild = pTileMatrixNode ? pTileMatrixNode->GetChildren() : NULL;
	if (!pchild)
		return false;
	WxsCapabilitesInformation::WxsTileMatrixSet::WxsTileMatrix tilematrix;
	while (pchild) {
		if (pchild->GetName().CompareTo("ows:Identifier") == 0) {
			tilematrix.identifier_ = pchild->GetNodeContent().c_str();
		} else if (pchild->GetName().CompareTo("ScaleDenominator") == 0) {
			tilematrix.scaleDenominator_ = pchild->GetNodeContent().c_str();
		} else if (pchild->GetName().CompareTo("TopLeftCorner") == 0) {
			std::vector<std::string> coords = tokenizer(
					pchild->GetNodeContent().c_str(), " ");
			if (coords.size() == 2) {
				tilematrix.upperx_ = coords[0];
				tilematrix.uppery_ = coords[1];
			}
		} else if (pchild->GetName().CompareTo("TileWidth") == 0) {
			tilematrix.tileWidth_ = StringToNumber<int>(
					pchild->GetNodeContent().c_str());
		} else if (pchild->GetName().CompareTo("TileHeight") == 0) {
			tilematrix.tileHeight_ = StringToNumber<int>(
					pchild->GetNodeContent().c_str());
		} else if (pchild->GetName().CompareTo("MatrixWidth") == 0) {
			tilematrix.matrixWidth_ = StringToNumber<int>(
					pchild->GetNodeContent().c_str());
		} else if (pchild->GetName().CompareTo("MatrixHeight") == 0) {
			tilematrix.matrixHeight_ = StringToNumber<int>(
					pchild->GetNodeContent().c_str());
		}
		pchild = pchild->GetNext();
	}
	TileMatrixSet.tileMatrixVector_.push_back(tilematrix);
	return true;
}

/**
 * Metodo que parsea el nodo TileMatrixSetLink de la respuesta
 * a la consulta GetCapabilities sobre un servidor WMTS
 * @param[in] pTileMatrixSetLinkNode nodo XML que contiene la informacion a parsear
 * @param[out] Layer estructura que almacenara la informacion del nodo
 * @return true en caso de parsear con exito el xml
 * @return false en caso contrario
 */
bool WmtsGetCapabilitiesParser::ParseTileMatrixSetLinkNode(
		wxXmlNode* pTileMatrixSetLinkNode,
		WxsLayersInformation::WxsLayerNode& Layer) {
	wxXmlNode* pchild =
			pTileMatrixSetLinkNode ?
					pTileMatrixSetLinkNode->GetChildren() : NULL;
	if (!pchild)
		return false;
	while (pchild) {
		if (pchild->GetName().CompareTo("TileMatrixSet") == 0) {
			Layer.tileMatrixSetLinks_.push_back(
					pchild->GetNodeContent().c_str());
			Layer.srsList_.push_back(pchild->GetNodeContent().c_str());
		}

		pchild = pchild->GetNext();
	}
	return true;
}
} /** namespace suri */
