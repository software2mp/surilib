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
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

// Includes Suri
#include "suri/TextFileGcpLoader.h"
#include "suri/ParserResult.h"
#include "suri/GroundControlPoint.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
// Defines
#define COMMENT ';'
#define ATTRIBUTE_TOKEN "="
#define COORDINATE_TOKEN '\t'
#define BASE_DATASOURCE_DEFINITION "base_datasource"
#define WARP_DATASOURCE_DEFINITION "warp_datasource"
#define SRC_SR_WKT "source_spatialreference_wkt"
#define DEST_SR_WKT "destination_spatialreference_wkt"
#define SRC_RASTERMODEL_WKT "source_rastermodel_wkt"
#define DEST_RASTERMODEL_WKT "destination_rastermodel_wkt"
#define TRANSFORMATION_NAME_DEF "transformation_name"
#define TRANSFORMATION_PARAMS_DEF "transformation_parameters"
#define POINT_ENABLE "ON"
#define VALUES_TOKEN ","
#define URL_ATTRIBUTE "url"
#define ID_ATTRIBUTE "id"
#define ATTRIBUTE_SET_INIT_TOKEN '{'
#define ATTRIBUTE_SET_FINISH_TOKEN '}'

// forwards

namespace suri {

/** Constructor **/
TextFileGcpLoader::TextFileGcpLoader(const std::string& PointToken) :
      pointToken_(PointToken), sourceX_(0), sourceY_(1), destX_(2), destY_(3),
      enableIndex_(4) {
}

/** Destructor **/
TextFileGcpLoader::~TextFileGcpLoader() {
}

/** A partir del filename que se indica por parametro,
 * crea una lista de gcp correspondiente.
 * @param[in] FileName nombre del archivo con el path
 * @param[in] pList lista de puntos de control en la cual se cargaran los datos
 * @return true en caso de poder cargar correctamente los puntos de control
 * @return false en caso contrario (ej. pList == NULL)
 */
ParserResult TextFileGcpLoader::Load(const std::string& FileName, GcpList& List) {
   std::string line;
   std::ifstream file(FileName.c_str());
   int gcpcount = 0;
   int lineNumber = 0;
   if (file.is_open()) {
      while (file.good()) {
         getline(file, line);
         lineNumber++;
         if (line.size() == 0)
            break;
         if (line.at(0) != COMMENT) {
            GroundControlPointExtended* pcgp = CreateGcpFromLine(line);
            if (pcgp) {
               List.Add(*pcgp);
               gcpcount++;
               delete pcgp;
            } else {
               // Fallo al parsear el gcp
               ParserResult result(false, lineNumber, line, message_INVALID_GCP);
               file.close();
               return result;
            }
			} else {
			   // Se cargo un comentario invalido.
			   if (!LoadParameterFromComment(line, List)) {
			      ParserResult result(false, lineNumber, line, message_INVALID_COMMENT);
			      file.close();
			      return result;
			   }
			}
      }
   }
   file.close();
   // Devuelvo success si levante al menos un GCP
   ParserResult result(gcpcount > 0, lineNumber, "", message_NO_GCPS_AVAILABLE);
   return result;
}

/** Guarda la lista de gcps en el path indicado por parametro.
 * @param[in] FileName nombre del archivo con el path en donde se guardara la lista
 * @param[in] pList lista de puntos de control que contiene la informacion a guardar
 * @return true en caso de poder guardar correctamente los puntos de control
 * @return false en caso contrario
 */
bool TextFileGcpLoader::Save(const std::string& FileName, const GcpList& List) {
   std::vector < std::string > header;
   GetInfoHeader(List, header);
   std::ofstream file;
   file.open(FileName.c_str());
   for (std::vector<std::string>::iterator it = header.begin(); it != header.end();
         ++it) {
      if ((*it).length() > 0)
         file << (*it) << std::endl;
   }
   std::vector<GroundControlPointExtended> list = List.GetAsExtendedGcps();
   std::vector<GroundControlPointExtended>::const_iterator it = list.begin();
   for (; it != list.end(); ++it) {
      std::string line = CreateLineFromGcp(*it);
      if (line.length() > 0)
         file << line << std::endl;
   }
   file.close();
   return true;
}

/** Metodo que parsea una linea en un punto de control
 *  @param[in] GcpLine linea que representa un punto de control
 *  @return GroundControlPoint* nueva instancia de punto de control que representa la linea
 *  @return NULL en caso de no poder parsear la linea de forma correcta
 *  (ej. linea comentario)
 **/
GroundControlPointExtended* TextFileGcpLoader::CreateGcpFromLine(
      const std::string& GcpLine) {
   if (GcpLine.size() == 0)
      return NULL;
   std::vector < std::string > gcpline = tokenizer(GcpLine, pointToken_);
   int size = gcpline.size();
   /** Si algun indice no se puede obtener no se parsea correctamente la linea **/
   if (size < 4 || size <= sourceX_ || size <= sourceY_ || size <= destX_
         || size <= destY_)
      return NULL;

   double xsrc = StringToNumber<double>(gcpline.at(sourceX_));
   double ysrc = StringToNumber<double>(gcpline.at(sourceY_));
   double xdest = StringToNumber<double>(gcpline.at(destX_));
   double ydest = StringToNumber<double>(gcpline.at(destY_));
   // si no esta definida la columna de activacion de puntos se toma como activo el punto
   bool enable =
         (enableIndex_ < size) ? trim(gcpline.at(enableIndex_), "\r").compare(
                                       POINT_ENABLE) == 0 :
                                 true;
   GroundControlPointExtended* pgcp = new GroundControlPointExtended(xsrc, ysrc, 0,
                                                                     xdest, ydest, 0,
                                                                     enable);
   return pgcp;
}

/** Carga la informacion que se encuentra en la linea comentario a la lista
 *  Precondicion: se debe haber reservado memoria previamente para la lista.
 *  En caso de que pList sea NULL la  carga de los parametros falla
 *  @param[in] CommentLine linea que representa un comentario en el archivo
 *  @return true en caso de que el comentario tenga informacion valida para cargar
 *  en la lista
 *  @return false en caso contrario
 **/
bool TextFileGcpLoader::LoadParameterFromComment(const std::string& CommentLine,
                                                 GcpList& List) {
   size_t found = CommentLine.find_first_of(ATTRIBUTE_TOKEN);
   if (found == std::string::npos)
      return true;

   std::vector<Attribute> attributes;
   LoadAttributesDefinition(attributes);

   std::string key = CommentLine.substr(0, found);
   std::string value = CommentLine.substr(found + 1);
   std::vector<Attribute>::iterator it;
   for (it = attributes.begin(); it != attributes.end(); ++it) {
      Attribute attr = *it;
      if (key.find(attr.definition_) != std::string::npos) {
         // Esto esta para el caso de la carga de puntos para
         // georreferenciacion. El modelo raster puede no venir.
         if (value.length() > 0)
            return (this->*attr.loaderFunc_)(value, List);
      }
   }
   return true;
}

/**
 * Metodo auxiliar que carga las palabras usadas para la definicion de atributos
 * en el header
 * @param[out] Attributes vector en donde se cargaran los loaders de atributos
 */
void TextFileGcpLoader::LoadAttributesDefinition(std::vector<Attribute>& Attributes) {
   Attribute attr;
   attr.definition_ = BASE_DATASOURCE_DEFINITION;
   attr.loaderFunc_ = &TextFileGcpLoader::LoadSourceDatasourceInfo;
   Attributes.push_back(attr);
   attr.definition_ = WARP_DATASOURCE_DEFINITION;
   attr.loaderFunc_ = &TextFileGcpLoader::LoadDestinationDatasourceInfo;
   Attributes.push_back(attr);
   attr.definition_ = SRC_SR_WKT;
   attr.loaderFunc_ = &TextFileGcpLoader::LoadSourceSpatialReference;
   Attributes.push_back(attr);
   attr.definition_ = SRC_RASTERMODEL_WKT;
   attr.loaderFunc_ = &TextFileGcpLoader::LoadSourceRasterModel;
   Attributes.push_back(attr);
   attr.definition_ = DEST_SR_WKT;
   attr.loaderFunc_ = &TextFileGcpLoader::LoadDestinationSpatialReference;
   Attributes.push_back(attr);
   attr.definition_ = DEST_RASTERMODEL_WKT;
   attr.loaderFunc_ = &TextFileGcpLoader::LoadDestinationRasterModel;
   Attributes.push_back(attr);
   attr.definition_ = TRANSFORMATION_NAME_DEF;
   attr.loaderFunc_ = &TextFileGcpLoader::LoadTransformationType;
   Attributes.push_back(attr);
   attr.definition_ = TRANSFORMATION_PARAMS_DEF;
   attr.loaderFunc_ = &TextFileGcpLoader::LoadTransformationParameters;
   Attributes.push_back(attr);
}

/** Metodo auxiliar que crea el string asociado a un punto de control
 *  @param[in] pGcp referencia al punto de control
 *  @return linea correspondiente al punto de control
 *  @return string vacio en caso de no poder parsear el punto **/
std::string TextFileGcpLoader::CreateLineFromGcp(const GroundControlPointExtended& pGcp) {
   std::string line;
   Coordinates psource = pGcp.GetSource();
   line.append(NumberToString<double>(psource.x_,8));
   line.append(pointToken_);
   line.append(NumberToString<double>(psource.y_,8));
   line.append(pointToken_);
   Coordinates pdest = pGcp.GetDestination();
   line.append(NumberToString<double>(pdest.x_,8));
   line.append(pointToken_);
   line.append(NumberToString<double>(pdest.y_,8));
   line.append(pointToken_);
   line.append(pGcp.IsEnabled() ? "ON" : "OFF");
   return line;
}

/** Metodo encargada de cargar el atributo de sistema de referencia
 *  del origen
 *  @param[in] Value valor correspondiente a la definicion de atributo de sist. ref
 *  @param[out] pList lista donde se cargara el atributo
 *  @return true en caso de configurar correctamente el valor en la lista
 */
bool TextFileGcpLoader::LoadSourceSpatialReference(const std::string& Value,
                                                   GcpList &List) {
   return List.SetSpatialReferenceSource(Value);
}

/** Metodo encargada de cargar el atributo de sistema de referencia
 *  del destino
 *  @param[in] Value valor correspondiente a la definicion de atributo de sist. ref
 *  @param[out] pList lista donde se cargara el atributo
 *  @return true en caso de configurar correctamente el valor en la lista
 */
bool TextFileGcpLoader::LoadDestinationSpatialReference(const std::string& Value,
                                                        GcpList &List) {
   return List.SetSpatialReferenceDestination(Value);
}

/** Metodo encargada de cargar el atributo de modelo raster
 *  del origen
 *  @param[in] Value valor correspondiente a la definicion de atributo de modelo raster
 *  @param[out] pList lista donde se cargara el atributo
 *  @return true en caso de configurar correctamente el valor en la lista
 */
bool TextFileGcpLoader::LoadSourceRasterModel(const std::string& Value,
                                              GcpList &List) {
   return List.SetRasterModelSource(Value);
}

/** Metodo encargada de cargar el atributo de modelo raster
 *  del destino
 *  @param[in] Value valor correspondiente a la definicion de atributo de modelo raster
 *  @param[out] pList lista donde se cargara el atributo
 *  @return true en caso de configurar correctamente el valor en la lista
 */
bool TextFileGcpLoader::LoadDestinationRasterModel(const std::string& Value,
                                                   GcpList &List) {
   return List.SetRasterModelDestination(Value);
}

/** Metodo encargada de cargar el atributo de informacion sobre la fuente
 *  de datos del origen
 *  @param[in] Value valor correspondiente a la definicion de atributo de informacion
 *  sobre la fuente de datos origen
 *  @param[out] pList lista donde se cargara el atributo
 *  @return true en caso de configurar correctamente el valor en la lista
 */
bool TextFileGcpLoader::LoadSourceDatasourceInfo(const std::string& Value,
                                                 GcpList &List) {
   std::string url;
   std::string id;
   LoadUrlAndIdFromLine(Value, url, id);
   List.SetSourceDatasource(url, id);
   return true;
}

/** Metodo encargada de cargar el atributo de informacion sobre la fuente
 *  de datos del origen
 *  @param[in] Value valor correspondiente a la definicion de atributo de informacion
 *  sobre la fuente de datos destino
 *  @param[out] pList lista donde se cargara el atributo
 *  @return true en caso de configurar correctamente el valor en la lista
 */
bool TextFileGcpLoader::LoadDestinationDatasourceInfo(const std::string& Value,
                                                      GcpList &List) {
   std::string url;
   std::string id;
   LoadUrlAndIdFromLine(Value, url, id);
   List.SetDestinationDatasource(url, id);
   return true;
}

/** Metodo encargada de cargar el atributo de tipo de transformacion
 *  @param[in] Value valor correspondiente a la definicion de atributo de informacion
 *  de tipo de transformacion
 *  @param[out] pList lista donde se cargara el atributo
 *  @return true en caso de configurar correctamente el valor en la lista
 */
bool TextFileGcpLoader::LoadTransformationType(const std::string& Value,
                                               GcpList &List) {
   List.SetTransformacionType(Value);
   return true;
}

/** Metodo encargada de cargar el atributo de parametros de la transformacion
 *  @param[in] Value valor correspondiente a la definicion de atributo de informacion
 *  parametros de la transformacion
 *  @param[out] pList lista donde se cargara el atributo
 *  @return true en caso de configurar correctamente el valor en la lista
 */
bool TextFileGcpLoader::LoadTransformationParameters(const std::string& Value,
                                                     GcpList &List) {
   // TODO(Gabriel - TCK #2853): Definir los parametros a tomar
   return true;
}

/** Indica cual es la columna correspondiente a la coordenada x del origen
 *  @param[in] Index indice de la columna a la que corresponde la coordenada
 */
void TextFileGcpLoader::SetSourceCoordinateXColumn(int Index) {
   sourceX_ = Index;
}

/** Indica cual es la columna correspondiente a la coordenada y del origen
 *  @param[in] Index indice de la columna a la que corresponde la coordenada
 */
void TextFileGcpLoader::SetSourceCoordinateYColumn(int Index) {
   sourceY_ = Index;
}

/** Indica cual es la columna correspondiente a la coordenada x del destino
 *  @param[in] Index indice de la columna a la que corresponde la coordenada
 */
void TextFileGcpLoader::SetDestCoordinateXColumn(int Index) {
   destX_ = Index;
}

/** Indica cual es la columna correspondiente a la coordenada y del destino
 *  @param[in] Index indice de la columna a la que corresponde la coordenada
 */
void TextFileGcpLoader::SetDestCoordinateYColumn(int Index) {
   destY_ = Index;
}

/** Indica cual es la columna que indica si el punto se encuentra activo
 *  @param[in] Index indice de la columna a la que corresponde a la columna
 */
void TextFileGcpLoader::SetEnableColumn(int Index) {
   enableIndex_ = Index;
}

/** Metodo auxiliar que obtiene el url y el id de una linea que tiene informacion
 * sobre la fuente de datos en formato {url="..",id="..."}
 */
void TextFileGcpLoader::LoadUrlAndIdFromLine(const std::string &Line, std::string& Url,
                                             std::string &Id) {
   // TODO(Gabriel - TCK #2853): Mejorar
   if (Line.size() > 0) {
      // realizo un split para obtener todos los valores de los atributos
      std::vector < std::string > values = tokenizer(Line, VALUES_TOKEN);
      std::vector<std::string>::iterator it = values.begin();
      for (; it != values.end(); ++it) {
         // split sobre key value por =
         std::vector < std::string > keyvalue = tokenizer(*it, ATTRIBUTE_TOKEN);
         if (keyvalue.size() == 2) {
            std::string key = keyvalue.at(0);
            std::string value = keyvalue.at(1);
            // si la clave termina con { se lo saco
            if (key.at(0) == ATTRIBUTE_SET_INIT_TOKEN) {
               key.erase(0, 1);
            }
            // si el valor termina con } se lo saco
            if (value.at(value.size() - 1) == ATTRIBUTE_SET_FINISH_TOKEN) {
               value.erase(value.size() - 1);
            }
            if (key.compare(URL_ATTRIBUTE) == 0) {
               Url = value;
            } else if (key.compare(ID_ATTRIBUTE) == 0) {
               Id = value;
            }
         }
      }
   }
}

/** Obtiene de lal lista de puntos de control las lineas correspondientes al
 *  header del archivo de gcp
 * @param[in] List lista de puntos de control
 * @param[out] Header vector con informacion del encabezado
 */
void TextFileGcpLoader::GetInfoHeader(const GcpList& List,
                                      std::vector<std::string>& Header) const {
   // TODO(Gabriel - TCK #2886): Mejorar y poner info de las fuente de datos
   // y la transformacion realizada
   Header.push_back(";SURILIB GCP File");
   std::string srsource = ";";
   srsource.append(SRC_SR_WKT);
   srsource.append(ATTRIBUTE_TOKEN);
   srsource.append(List.GetSpatialReferenceSource());
   Header.push_back(srsource);
   std::string mrsource = ";";
   mrsource.append(SRC_RASTERMODEL_WKT);
   mrsource.append(ATTRIBUTE_TOKEN);
   mrsource.append(List.GetRasterModelSource());
   Header.push_back(mrsource);
   std::string srdest = ";";
   srdest.append(DEST_SR_WKT);
   srdest.append(ATTRIBUTE_TOKEN);
   srdest.append(List.GetSpatialReferenceDestination());
   Header.push_back(srdest);
   std::string mrdest = ";";
   mrdest.append(DEST_RASTERMODEL_WKT);
   mrdest.append(ATTRIBUTE_TOKEN);
   mrdest.append(List.GetRasterModelDestination());
   Header.push_back(mrdest);
}
} /** namespace suri */
