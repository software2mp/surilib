/*! \file WmtsXmlWriter.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <sstream>

// Includes Suri
#include "WmtsXmlWriter.h"
#include "suri/Option.h"
#include "SpatialReference.h"

// Includes Wx
// Defines
#define WXS_ULX_PROPERTY "ulx"
#define WXS_ULY_PROPERTY "uly"
#define WXS_LRX_PROPERTY "lrx"
#define WXS_LRY_PROPERTY "lry"
#define WXS_SERVICE_SRID_PROPERTY "idrefespacial"
#define WXS_BLOCK_SIZE_X "blocksizex"
#define WXS_BLOCK_SIZE_Y "blocksizey"
#define WXS_TILEMATRIX "tilematrix"
#define WXS_TILEMATRIXSET "tilematrixset"
#define WXS_MATRIX_WIDTH "matrixwidth"
#define WXS_MATRIX_HEIGTH "matrixheight"
#define WXS_URL "url"
#define URL_LAYERS_FIELD_NAME "layers"
#define WXS_STYLE_PROPERTY "estilo"
#define WXS_FORMAT_PROPERTY "formato"
#define EPSG_3857   "EPSG:3857"
#define EPSG_4326   "EPSG:4326"
#define EPSG_900913 "EPSG:900913"
#define GOOGLE_MAPS_COMPATIBLE "GoogleMapsCompatible"

// forwards

namespace suri {

/** Constructor */
WmtsXmlWriter::WmtsXmlWriter(TmsXmlWriter* pSuccessor) : TmsXmlWriter(pSuccessor) {
}

/** Destructor */
WmtsXmlWriter::~WmtsXmlWriter() {
}

/**
 * Genera el XML correspondiente al servicio y basado en los parametros suministrados.
 */
std::string WmtsXmlWriter::GetXml(const std::string& UrlStr,
                                   std::map<std::string, std::string>& Params,
                                   const Option& Metadata) {
   if (Params["service"] != "wmts")
      return GetSuccessor()->GetXml(UrlStr, Params, Metadata);

   std::string tilematrix = Metadata.GetOption(WXS_TILEMATRIX);
   std::string tilematrixset = Metadata.GetOption(WXS_TILEMATRIXSET);
   tilematrix = (tilematrix.length() == 1) ? "${z}" : tilematrixset.append(":${z}");

   std::ostringstream ss;
   ss << "<GDAL_WMS>";
   ss << "<Service name=\"TMS\">";
   ss << "<ServerUrl>" << Metadata.GetOption(WXS_URL);
   ss << "SERVICE=WMTS&amp;REQUEST=GetTile&amp;VERSION=1.0.0";
   ss << "&amp;LAYER=" << Metadata.GetOption(URL_LAYERS_FIELD_NAME);
   ss << "&amp;STYLE=" << Metadata.GetOption(WXS_STYLE_PROPERTY);
   ss << "&amp;FORMAT=" << Metadata.GetOption(WXS_FORMAT_PROPERTY);
   ss << "&amp;TILEMATRIXSET=" << Metadata.GetOption(WXS_SERVICE_SRID_PROPERTY);
   ss << "&amp;TILEMATRIX=" <<  tilematrix;
   ss << "&amp;TILECOL=${x}&amp;TILEROW=${y}</ServerUrl>";
   ss << "</Service>";
   ss << "<DataWindow>";
   ss << "<UpperLeftX>" << Metadata.GetOption(WXS_ULX_PROPERTY) << "</UpperLeftX>";
   ss << "<UpperLeftY>" << Metadata.GetOption(WXS_ULY_PROPERTY) << "</UpperLeftY>";
   ss << "<LowerRightX>" << Metadata.GetOption(WXS_LRX_PROPERTY) << "</LowerRightX>";
   ss << "<LowerRightY>" << Metadata.GetOption(WXS_LRX_PROPERTY) << "</LowerRightY>";
   ss << "<TileLevel>21</TileLevel>";
   ss << "<TileCountX>" << Metadata.GetOption(WXS_MATRIX_WIDTH) << "</TileCountX>";
   ss << "<TileCountY>" << Metadata.GetOption(WXS_MATRIX_HEIGTH) << "</TileCountY>";
   ss << "<YOrigin>top</YOrigin>";
   ss << "</DataWindow>";
   std::string projectionNode = Metadata.GetOption(WXS_SERVICE_SRID_PROPERTY);
   if(!validReferenceSystem(projectionNode)){
      projectionNode = correctReferenceSystem(projectionNode);
   }
   ss << "<Projection>" << projectionNode << "</Projection>";
   ss << "<BlockSizeX>" << Metadata.GetOption(WXS_BLOCK_SIZE_X) << "</BlockSizeX>";
   ss << "<BlockSizeY>" << Metadata.GetOption(WXS_BLOCK_SIZE_Y) << "</BlockSizeY>";
   ss << "<BandsCount>3</BandsCount>";
   ss << "<MaxConnections>5</MaxConnections>";
   ss << "<Cache />";
   ss << "</GDAL_WMS>";
   std::string retval = ss.str();

   return retval;
}

/** Verifica que el string pasado represente un sistema de referencia válido
 * @return true si el sistema de referencia tiene asociado un Wkt no vacío
 * @return false en caso contrario
*/
bool WmtsXmlWriter::validReferenceSystem(std::string referenceSystem){
   std::string Wkt = SpatialReference::GetWktWithAuthorityId(referenceSystem);
   return !Wkt.empty();
}

/** Devuelve el string con un sistema de referencia válido, en base a uno no
 *  soportado.
 */
std::string WmtsXmlWriter::correctReferenceSystem(std::string referenceSystem){
   std::string result = EPSG_4326;
   if(referenceSystem == EPSG_900913 || referenceSystem == GOOGLE_MAPS_COMPATIBLE){
      result = EPSG_3857;
   }
   return result;
}

} /** namespace suri */
