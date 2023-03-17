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
#include <curl/curl.h>
#include <curl/easy.h>
#include <string>
#include <vector>

// Includes Suri
#include "suri/WmsGetCapabilitiesParser.h"
#include "logmacros.h"

// Includes Wx
#include "wx/filename.h"
#include "wx/xml/xml.h"
// Defines
// forwards

namespace suri {
/**
 * Metodo encargado de parsear la respuesta de la solicitud de GetCapabilities
 * @return true en caso de parsear correctamente la solicitud
 * @return false en caso de falla
 */
bool WmsGetCapabilitiesParser::ParseGetCapabilitiesResponse(wxXmlNode* pRootNode,
		WxsCapabilitesInformation& GetCapabilitesResponse) {
	wxXmlNode* pcurrent = pRootNode ? pRootNode->GetChildren() : NULL;
	if (!pcurrent)
		return false;
	bool ok = true;
	while (ok && pcurrent) {
		if (pcurrent->GetName().CompareTo("Service") == 0) {
			ok = LoadServiceInfo(pcurrent, GetCapabilitesResponse);
		} else if (pcurrent->GetName().CompareTo("Capability") == 0) {
			ok = LoadCapabiltyInfo(pcurrent, GetCapabilitesResponse);
		}
		pcurrent = pcurrent->GetNext();
	}
	return ok;
}


/**
 *  Funcion encargada de parsear el xml correspondiente a la informacion de servicio
 *  del WMS
 */
bool WmsGetCapabilitiesParser::LoadServiceInfo(wxXmlNode* pServiceNode,
		WxsCapabilitesInformation& GetCapabilitesResponse) {
	wxXmlNode* pcurrent = pServiceNode ? pServiceNode->GetChildren() : NULL;
	if (!pcurrent)
		return false;
	while (pcurrent) {
		if (pcurrent->GetName().CompareTo("Name") == 0) {
			GetCapabilitesResponse.serviceInfo_.name_ =
					pcurrent->GetNodeContent().c_str();
		} else if (pcurrent->GetName().CompareTo("Title") == 0) {
			GetCapabilitesResponse.serviceInfo_.title_ =
					pcurrent->GetNodeContent().c_str();
		} else if (pcurrent->GetName().CompareTo("Abstract") == 0) {
			GetCapabilitesResponse.serviceInfo_.abstract_ =
					pcurrent->GetNodeContent().c_str();
 		} else if (pcurrent->GetName().CompareTo("OnlineResource") == 0) {
			GetCapabilitesResponse.serviceInfo_.onlineResource_ =
					pcurrent->GetPropVal("xlink:href", "").c_str();
 		}
		pcurrent = pcurrent->GetNext();
	}
	return true;
}

/**
 * Funcion encargada de parsear el nodo Capability de un servicio WMS
 */
bool WmsGetCapabilitiesParser::LoadCapabiltyInfo(wxXmlNode* pCapabilityNode,
		WxsCapabilitesInformation& GetCapabilitesResponse) {
	wxXmlNode* pcurrent = pCapabilityNode ? pCapabilityNode->GetChildren() : NULL;
	if (!pcurrent)
		return false;
	bool ok = true;
	while (ok && pcurrent) {
		if (pcurrent->GetName().CompareTo("Layer") == 0) {
			ok = LoadCapabilityLayersInfo(pcurrent,
					GetCapabilitesResponse.layerInfo_.rootlayer_);
		} else if (pcurrent->GetName().CompareTo("Request") == 0) {
			ok = LoadCapabilityRequestInfo(pcurrent, GetCapabilitesResponse);
		}
		pcurrent = pcurrent->GetNext();
	}
	return ok;
}

/**
 * Funcion encargada de parsear el nodo Layer de un servicio WMS
 */
bool WmsGetCapabilitiesParser::LoadCapabilityLayersInfo(wxXmlNode* pLayerNode,
		WxsLayersInformation::WxsLayerNode& LayerInfoNode) {
	wxXmlNode* pcurrent = pLayerNode ? pLayerNode->GetChildren() : NULL;
	wxString queryablevalue = pLayerNode->GetPropVal("queryable", "");
	LayerInfoNode.queryable_ = queryablevalue.CompareTo("1") == 0;
	if (!pcurrent)
		return false;
	bool ok = true;
	while (ok && pcurrent) {
		if (pcurrent->GetName().CompareTo("Name") == 0) {
			LayerInfoNode.name_ = pcurrent->GetNodeContent().c_str();
		} else if (pcurrent->GetName().CompareTo("Title") == 0) {
			LayerInfoNode.title_ = pcurrent->GetNodeContent().c_str();
		} else if (pcurrent->GetName().CompareTo("Abstract") == 0) {
			LayerInfoNode.abstract_ = pcurrent->GetNodeContent().c_str();
		} else if (pcurrent->GetName().CompareTo("BoundingBox") == 0) {
			ParseBoundingBox(pcurrent, LayerInfoNode.boundingBox_);
		} else if (pcurrent->GetName().CompareTo("Layer") == 0) {
			// se carga el proximo nivel de la capa
			WxsLayersInformation::WxsLayerNode child;
			if (LoadCapabilityLayersInfo(pcurrent, child)) {
				LayerInfoNode.childLayers_.push_back(child);
			}
		} else if (pcurrent->GetName().CompareTo("SRS") == 0
				|| pcurrent->GetName().CompareTo("CRS") == 0) {
			LayerInfoNode.srsList_.push_back(
					pcurrent->GetNodeContent().c_str());
		} else if (pcurrent->GetName().CompareTo("Style") == 0) {
			WxsLayersInformation::WxsStyleNode style;
			LoadLayerStyleInfo(pcurrent, style);
			LayerInfoNode.styles_.push_back(style);
		}
		pcurrent = pcurrent->GetNext();
	}
	return ok;
}

/** Funcion que parsea el nodo estilo de una capa wms **/
void WmsGetCapabilitiesParser::LoadLayerStyleInfo(wxXmlNode* pStyleNode,
		WxsLayersInformation::WxsStyleNode& Style) {
	wxXmlNode* pcurrent = pStyleNode ? pStyleNode->GetChildren() : NULL;
	if (!pcurrent)
		return;
	while (pcurrent) {
		if (pcurrent->GetName().CompareTo("Name") == 0) {
			Style.name_ = pcurrent->GetNodeContent().c_str();
		} else if (pcurrent->GetName().CompareTo("Title") == 0) {
			Style.title_ = pcurrent->GetNodeContent().c_str();
		} else if (pcurrent->GetName().CompareTo("LegendURL") == 0) {
			wxXmlNode* pchild = pcurrent->GetChildren();
			while (pchild) {
				if (pchild->GetName().CompareTo("Format") == 0) {
					Style.legendurl_.format_ = pchild->GetNodeContent().c_str();
				} else if (pchild->GetName().CompareTo("OnlineResource") == 0) {
					Style.legendurl_.onlineResourceHref_ = pchild->GetPropVal(
							"xlink:href", "").c_str();
					Style.legendurl_.onlineResourceHtype_ =
							pchild->GetPropVal("xlink:type", "").c_str();
				}
				pchild = pchild->GetNext();
			}
		}
		pcurrent = pcurrent->GetNext();
	}
}
/**
 * Funcion encargada de parsear el nodo Request de una solicitud
 * GetCapabilities a un servidor WMS
 */
bool WmsGetCapabilitiesParser::LoadCapabilityRequestInfo(wxXmlNode* pLayerNode,
		WxsCapabilitesInformation& GetCapabilitesResponse) {
	// TODO(Gabriel - #): Implementar
	return true;
}

/** Metodo que parsea el nodo boundingbox de un servicio WMS **/
void WmsGetCapabilitiesParser::ParseBoundingBox(wxXmlNode* pBoundingBoxInfo,
		std::vector<WxsLayersInformation::BoundingBox>& BoundingBoxOut) {
	WxsLayersInformation::BoundingBox boundingbox;
	boundingbox.srs_ = pBoundingBoxInfo->GetPropVal("SRS", "").c_str();
	boundingbox.maxX_ = pBoundingBoxInfo->GetPropVal("maxx", "").c_str();
	boundingbox.maxY_ = pBoundingBoxInfo->GetPropVal("maxy", "").c_str();
	boundingbox.minX_ = pBoundingBoxInfo->GetPropVal("minx", "").c_str();
	boundingbox.minY_ = pBoundingBoxInfo->GetPropVal("miny", "").c_str();
	BoundingBoxOut.push_back(boundingbox);
}

/** Metodo que realiza el parseo de GetCapabilities
 *  @return true en caso de realizar el parseo con exito
 *  @return false en caso de no poder realizar el parseo
 */
bool WmsGetCapabilitiesParser::DoParseGetCapabilities(const std::string& WxsUrl,
		WxsCapabilitesInformation& GetCapabilitesResponse,
		std::string& ServiceType,
		const std::string& Version) {
	if (WxsUrl.find("WMTS") != std::string::npos ||
			WxsUrl.find("wmts") != std::string::npos)
		return false;
	wxString tmpfilename = wxFileName::CreateTempFileName("").c_str();
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if (curl) {
		wxString wxsurl = WxsUrl.c_str();
		if (WxsUrl[WxsUrl.length() - 1] != '?') {
			wxsurl += "?";
		}
		if (wxsurl.Find("GetCapabilities") == wxNOT_FOUND) {
			wxsurl += "VERSION=" + Version
					+ "&REQUEST=GetCapabilities&SERVICE=WMS&";
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
	ServiceType = "wms";
	return ParseGetCapabilitiesResponse(proot, GetCapabilitesResponse);
}
} /** namespace suri */
