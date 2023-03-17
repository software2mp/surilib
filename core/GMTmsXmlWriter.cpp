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
