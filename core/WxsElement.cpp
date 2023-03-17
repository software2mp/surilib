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

#include "WxsElement.h"

// Includes standard
#include <map>
#include <sstream>
// Includes Suri
#include "suri/Image.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DataTypes.h"
#include "suri/Uri.h"
#include "WxsProperties.h"
#include "WxsVisualizationPart.h"
#include "RasterProperties.h"
#include "suri/DatasourceInterface.h"
#include "RasterDatasource.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xml/xml.h"
// Defines
#define MAX_PARAMETER_SIZE 256
#define DEFAULTRASTERSIZE 256

#define WXSRENDERER_NODE "wxs"
#define WXS_STYLE_PROPERTY "estilo"
#define WXS_FORMAT_PROPERTY "formato"
#define WXS_SERVICE_SRID_PROPERTY "idrefespacial"
#define WXS_FILENAME_SEPARATOR "|"

namespace suri {
/** Macro para registrar el tipo de elemento */
AUTO_REGISTER_CLASS(Element, WxsElement, 0)

/** Ctor */
WxsElement::WxsElement() {

}

/** Dtor */
WxsElement::~WxsElement() {

}

/**
 * Guarda el extent del raster en el nodo.
 * @param[in] WorldExtent Extent del elemento wms
 */
void WxsElement::SetElementExtent(const Subset &WorldExtent) {
   RasterElement::SetElementExtent(WorldExtent);
}

/**
 * Devuelve ventanas de propiedades de los elementos Raster.
 * @return vector con punteros a los Part generados.\n
 * NOTA: en caso de tener que agregar Part, RECORDAR que GetParts()devuelve 2
 * Part(1ro Comun a todos los elementos, 2do Para editar xml(solo en DEBUG))
 * ==> los Part a agregar deben agregarse entre medio de dichos Part's.
 * @return vector de parts con propiedades del Raster.
 */
std::vector<Part *> WxsElement::DoGetParts(DataViewManager *pDataViewManager,
                                            LayerInterface *pLayer,
                                            DatasourceInterface *pDatasource) {
   std::vector<Part*> partvector;
   if (!pDataViewManager || !pDatasource || !pLayer) {
      partvector.push_back(new RasterProperties(this));
   } else {
      RasterDatasource *prasterdatasource = dynamic_cast<RasterDatasource*>(pDatasource);
      if (prasterdatasource)
         partvector.push_back(new RasterProperties(prasterdatasource, pLayer));
   }
   return partvector;
}

/** Devuelve vector con Parts especificos de la visualizacion elemento. */
std::vector<suri::Part*> WxsElement::DoGetVisualizationParts(
      DataViewManager* pDataViewManager, LayerInterface *pLayer,
      DatasourceInterface *pDatasource) {
   std::vector<Part*> partvector;
   partvector.push_back(new WxsProperties(this));
   partvector.push_back(new WxsVisualizationPart(this));
   return partvector;
}

/**
 * Inicializa el raster con la fuente wxs
 * @param[in] FileName string con campos ingresados por usuario
 * @param[in] Metadata mapa de metadatos
 * @return this si se pudo configurar correctamente. NULL en otro caso
 */
RasterElement* WxsElement::Initialize(const std::string &FileName, const Option& Metadata) {
   std::map<std::string, std::string> keys;
   if (!ParseWxsFilename(FileName, keys))
      return NULL;

   SetUrl(keys["wxssource"]);

   Image *pimage = Image::Open(CreateWxsXml(FileName, Metadata));
   if (!pimage) {
      return NULL;
   }

   // nodos de georreferencia
   SetSpatialReference(pimage->GetOption(Image::Georreference));
   SetRasterModel(pimage->GetOption(Image::RasterMatrix));

   // nodos de dependencias
   std::vector<std::string> dependencies = tokenizer(pimage->GetOption("dependencies"), ",");
   for (size_t i = 0; i < dependencies.size(); i++)
      AddDependency(dependencies[i]);

   // nodos de renderizacion
   wxXmlNode *prnode = AddNode(GetNode(wxT("")), RENDERIZATION_NODE);

   wxXmlNode *pwxsnode = AddNode(prnode, WXSRENDERER_NODE);
   pwxsnode->AddProperty(WXS_STYLE_PROPERTY, Metadata.GetOption(WXS_STYLE_PROPERTY));
   pwxsnode->AddProperty(WXS_FORMAT_PROPERTY, Metadata.GetOption(WXS_FORMAT_PROPERTY));
   pwxsnode->AddProperty(WXS_SERVICE_SRID_PROPERTY, Metadata.GetOption(WXS_SERVICE_SRID_PROPERTY));

   AddNode(prnode, CANVAS_CACHE_NODE);

   // Cargo nodo data cast
   std::string datatype = pimage->GetDataType();
   if (datatype != DataInfo<unsigned char>::Name) {
      // casteo de datos a uchar
      AddNode(prnode, DATA_CAST_NODE, wxT(""), OUTPUT_PROPERTY,
              wxT(DataInfo<unsigned char>::Name));
   }

   // Como no encuentra el nodo RasterSize lo crea
   SetRasterSize(DEFAULTRASTERSIZE, DEFAULTRASTERSIZE);

   // Guarda el extent del elemento
   SetElementExtent(
         Subset(StringToNumber<double>(Metadata.GetOption("ulx")),
                StringToNumber<double>(Metadata.GetOption("uly")),
                StringToNumber<double>(Metadata.GetOption("lrx")),
                StringToNumber<double>(Metadata.GetOption("lry"))));

   // Nodo Archivo
   SetFileNode(pimage->GetOption(Image::Format), pimage->GetOption(Image::FormatDescription));
   RasterPropertiesType props;
   props.DataType = datatype;
   SetRasterProperties(props);

   // Configura el nombre del elemento al de la capa
   SetName(Metadata.GetOption("title"));

   // Agrego el nodo de Brillo/Contraste
   wxXmlNode *plutnode = AddNode(prnode, wxT(BRIGHTNESS_NODE), wxT(""),
                                 wxT(NAME_PROPERTY),
                                 wxT(NAME_PROPERTY_VALUE_LUT_BRIGHTNESS));
   AddNode(plutnode, wxT(LUT_BRIGHTNESS_NODE), wxT("0"));
   AddNode(plutnode, wxT(LUT_CONTRAST_NODE), wxT("1"));

   // Carga la propiedades de las bandas
   InitializeBandProperties(pimage);

   Image::Close(pimage);

   return this;

}

/**
 * Usa datos en FileName para generar Xml que se pasa a Image
 * @param[in] FileName string con campos ingresados por usuario
 * @return string con nodo xml
 */
std::string WxsElement::CreateWxsXml(const std::string &FileName, const Option& Metadata) {
   std::string retval = "";

   std::map<std::string, std::string> keys;
   ParseWxsFilename(FileName, keys);

   std::ostringstream ss;
   if (keys["service"] == "wms") {
      ss << "<GDAL_WMS> \
         <Service name=\"WMS\"> \
         <Version>1.1.1</Version> \
         <ServerUrl>";
      ss << keys["url"];
      ss << "</ServerUrl> \
         <SRS>";
      ss << Metadata.GetOption(WXS_SERVICE_SRID_PROPERTY);
      ss << "</SRS> \
         <ImageFormat>";
      ss << Metadata.GetOption(WXS_FORMAT_PROPERTY);
      ss << " </ImageFormat> \
         <Layers>";
      ss << keys["layers"];
      ss << "</Layers> \
            <Styles>";
      ss << Metadata.GetOption(WXS_STYLE_PROPERTY);
      ss << "</Styles> \
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
         <SizeX>";
      ss << DEFAULTRASTERSIZE;
      ss << "</SizeX> \
         <SizeY>";
      ss << DEFAULTRASTERSIZE;
      ss << "</SizeY> \
         </DataWindow> \
         <OverviewCount>12</OverviewCount> \
         <BlockSizeX>512</BlockSizeX> \
         <BlockSizeY>512</BlockSizeY> \
         <Projection>";
      ss << Metadata.GetOption(WXS_SERVICE_SRID_PROPERTY);
      ss << "</Projection> \
         <BandsCount>3</BandsCount> \
         <ClampRequests>false</ClampRequests> \
         </GDAL_WMS>";
      retval = ss.str();
   }

   return retval;
}

/**
 * Parsea url y retorna el host y los parametros en mapa(key=>value)
 * @param[in] Url: string con url que quiero parsear.
 * @param[out] Keys: mapa con pares key,value leidos.
 * @return bool que indica si el FileName tiene un formato correcto
 */
bool WxsElement::ParseWxsFilename(const std::string &Filename,
                                  std::map<std::string, std::string> &Keys) {
   size_t enduri = Filename.find(WXS_FILENAME_SEPARATOR);

   //Configuro uri y sus componentes
   Uri uri(Filename.substr(0, enduri));
   std::map<std::string, std::string> queryparameters = StringToMap(
         uri.GetQuery(), Uri::QUERY_SEPARATOR);

   if (queryparameters.count("service") == 0 || queryparameters.count("layers") == 0)
      return false;

   Keys["wxssource"] = uri.ToString();
   Keys["service"] = queryparameters["service"];
   std::string datasource;
   uri.GetDatasource(datasource);
   Keys["url"] = datasource;
   Keys["layers"] = queryparameters["layers"];

   if (enduri == std::string::npos)
      return true;

   return true;
}

}  //namespace suri
