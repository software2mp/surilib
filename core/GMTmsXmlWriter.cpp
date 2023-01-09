/*! \file GMTmsXmlWriter.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <map>
#include <sstream>
#include <string>

#include "GMTmsXmlWriter.h"
#include "suri/Option.h"

#define GM_URL_ANCHOR      "mt.google.com"

namespace suri {

/**
 * Ctor.
 */
GMTmsXmlWriter::GMTmsXmlWriter(TmsXmlWriter* pSuccessor) : TmsXmlWriter(pSuccessor) {

}

/**
 * Dtor.
 */
GMTmsXmlWriter::~GMTmsXmlWriter() {

}

/**
 * Genera el XML correspondiente al servicio y basado en los parametros suministrados.
 */
std::string GMTmsXmlWriter::GetXml(const std::string& UrlStr,
                                   std::map<std::string, std::string>& Params,
                                   const Option& Metadata) {
   std::string retval = "";

   if (UrlStr.find(GM_URL_ANCHOR) != std::string::npos) {
      std::string compurl = Params["url"] + "lyrs=" + GetLayerId(Params["layers"])
            + "&amp;x=${x}&amp;y=${y}&amp;z=${z}";

      std::ostringstream ss;
      if (Params["service"] == "tms") {
         ss << "<GDAL_WMS> \
           <Service name=\"TMS\"> \
               <ServerUrl>";
         ss << compurl;
         ss << "</ServerUrl> \
           </Service> \
           <DataWindow> \
           <UpperLeftX>";
         ss << Metadata.GetOption("ulx");
         ss << "</UpperLeftX> \
               <UpperLeftY>";
         ss << Metadata.GetOption("uly");
         ss << "</UpperLeftY> \
               <LowerRightX>";
         ss << Metadata.GetOption("lrx");
         ss << "</LowerRightX> \
               <LowerRightY>";
         ss << Metadata.GetOption("lry");
         ss << "</LowerRightY> \
               <TileLevel>20</TileLevel> \
               <TileCountX>1</TileCountX> \
               <TileCountY>1</TileCountY> \
               <YOrigin>top</YOrigin> \
            </DataWindow> \
            <Projection>EPSG:3857</Projection> \
            <BlockSizeX>256</BlockSizeX> \
            <BlockSizeY>256</BlockSizeY> \
            <BandsCount>3</BandsCount> \
            <MaxConnections>5</MaxConnections> \
            <Cache /> \
         </GDAL_WMS>";
         retval = ss.str();
         Params["expdesc"] = GetLayerDescription(Params["layers"]);
      }
   } else if (GetSuccessor() != NULL) {
      return GetSuccessor()->GetXml(UrlStr, Params, Metadata);
   }

   return retval;
}

/**
 * Obtiene el ID de la capa en base al nombre de la misma.
 * map = m
 * satellite = s
 * hybrid = y
 * terrain = t
 * terrain, streets and water = p
 */
std::string GMTmsXmlWriter::GetLayerId(const std::string& LayerName) {
   std::string retstr;

   if (LayerName.compare("map") == 0) {
      retstr = "m";
   } else if (LayerName.compare("satellite") == 0) {
      retstr = "s";
   } else if (LayerName.compare("hybrid") == 0) {
      retstr = "y";
   } else if (LayerName.compare("terrain") == 0) {
      retstr = "t";
   } else if (LayerName.compare("terrainstreetswater") == 0) {
      retstr = "p";
   }

   return retstr;
}

/**
 * Obtiene la descripcion de la capa en base al nombre de la misma.
 */
std::string GMTmsXmlWriter::GetLayerDescription(const std::string& LayerName) {
   std::string retstr;

   if (LayerName.compare("map") == 0) {
      retstr = "Google Maps (Mapa)";
   } else if (LayerName.compare("satellite") == 0) {
      retstr = "Google Maps (Satelite)";
   } else if (LayerName.compare("hybrid") == 0) {
      retstr = "Google Maps (Hibrido)";
   } else if (LayerName.compare("terrain") == 0) {
      retstr = "Google Maps (Terreno)";
   } else if (LayerName.compare("terrainstreetswater") == 0) {
      retstr = "Google Maps (Terreno, calles y agua)";
   }

   return retstr;
}

}  // namespace suri
