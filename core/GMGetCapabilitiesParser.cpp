/*! \file GMGetCapabilitiesParser.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
