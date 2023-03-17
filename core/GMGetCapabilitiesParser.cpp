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

#include "suri/GMGetCapabilitiesParser.h"

#define GM_URL_ANCHOR         "mt.google.com"
#define GM_SRS                "EPSG:3857"
#define GM_BOUNDINGBOX_MAXX   "-20037508.34"
#define GM_BOUNDINGBOX_MAXY   "20037508.34"
#define GM_BOUNDINGBOX_MINX   "20037508.34"
#define GM_BOUNDINGBOX_MINY   "-20037508.34";
#define GM_FORMAT             "image/jpeg"

namespace suri {

/**
 * Ctor.
 */
GMGetCapabilitiesParser::GMGetCapabilitiesParser() {

}

/**
 * Dtor.
 */
GMGetCapabilitiesParser::~GMGetCapabilitiesParser() {

}

/**
 * Metodo que realiza el parseo de GetCapabilities.
 *
 * @return true en caso de realizar el parseo con exito.
 * @return false en caso de no poder realizar el parseo.
 */
bool GMGetCapabilitiesParser::DoParseGetCapabilities(
      const std::string& WxsUrl, WxsCapabilitesInformation& GetCapabilitesResponse,
      std::string& ServiceType, const std::string& Version) {

   if (WxsUrl.find(GM_URL_ANCHOR) == std::string::npos)
      return false;

   GetCapabilitesResponse.serviceInfo_.title_ = "Google Maps (TMS)";
   GetCapabilitesResponse.serviceInfo_.serviceType_ = ServiceType = "tms";

   GetCapabilitesResponse.layerInfo_.rootlayer_.name_ = "Google Maps (TMS) ";
   GetCapabilitesResponse.layerInfo_.rootlayer_.title_ = "Google Maps (TMS) ";

   WxsLayersInformation::WxsLayerNode lyrsmap;
   CreateLayer("map", "Vista mapa ", lyrsmap);
   GetCapabilitesResponse.layerInfo_.rootlayer_.childLayers_.push_back(lyrsmap);

   WxsLayersInformation::WxsLayerNode lyrssatellite;
   CreateLayer("satellite", "Vista satelite ", lyrssatellite);
   GetCapabilitesResponse.layerInfo_.rootlayer_.childLayers_.push_back(lyrssatellite);

   WxsLayersInformation::WxsLayerNode lyrshybrid;
   CreateLayer("hybrid", "Vista hibrida ", lyrshybrid);
   GetCapabilitesResponse.layerInfo_.rootlayer_.childLayers_.push_back(lyrshybrid);

   WxsLayersInformation::WxsLayerNode lyrsterrain;
   CreateLayer("terrain", "Vista terreno ", lyrsterrain);
   GetCapabilitesResponse.layerInfo_.rootlayer_.childLayers_.push_back(lyrsterrain);

   WxsLayersInformation::WxsLayerNode lyrstsw;
   CreateLayer("terrainstreetswater", "Vista terreno, calles y agua ", lyrstsw);
   GetCapabilitesResponse.layerInfo_.rootlayer_.childLayers_.push_back(lyrstsw);

   return true;
}

/**
 * Crea una capa para el servicio.
 */
void GMGetCapabilitiesParser::CreateLayer(
      const std::string& Name, const std::string& Title,
      WxsLayersInformation::WxsLayerNode& TmsLayer) {
   // Datos generales de la capa.
   TmsLayer.name_ = Name;
   TmsLayer.title_ = Title;

   // Dimensiones.
   WxsLayersInformation::BoundingBox boundingbox;
   boundingbox.srs_ = GM_SRS;
   boundingbox.maxX_ = GM_BOUNDINGBOX_MAXX;
   boundingbox.maxY_ = GM_BOUNDINGBOX_MAXY;
   boundingbox.minX_ = GM_BOUNDINGBOX_MINX;
   boundingbox.minY_ = GM_BOUNDINGBOX_MINY;
   TmsLayer.boundingBox_.push_back(boundingbox);

   // Formato.
   WxsLayersInformation::WxsStyleNode stylestallite;
   stylestallite.name_ = "default";
   stylestallite.title_ = "default";
   stylestallite.legendurl_.format_ = GM_FORMAT;
   TmsLayer.styles_.push_back(stylestallite);

   // SRS.
   TmsLayer.srsList_.push_back(GM_SRS);
}

}  // namespace suri
