/*! \file TextFileGcpLoader.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TEXTFILEGCPLOADER_H_
#define TEXTFILEGCPLOADER_H_

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "suri/GcpLoaderInterface.h"
#include "suri/ParameterCollection.h"

// Includes Wx
// Defines

namespace suri {

// Forwards
class GroundControlPoint;

/*!
 *
 */
class TextFileGcpLoader : public GcpLoaderInterface {
   /** Constructor copia **/
   TextFileGcpLoader(const TextFileGcpLoader&);

public:
   /** constructor por defecto **/
   explicit TextFileGcpLoader(const std::string& PointToken = "\t");
   virtual ~TextFileGcpLoader();
   /** A partir del filename que se indica por parametro,
    * crea una lista de gcp correspondiente.
    * @param[in] FileName nombre del archivo con el path
    * @param[in] pList lista de puntos de control en la cual se cargaran los datos
    * @return true en caso de poder cargar correctamente los puntos de control
    * @return false en caso contrario (ej. pList == NULL)
    */
   virtual ParserResult Load(const std::string& FileName, GcpList& List);
   /** Guarda la lista de gcps en el path indicado por parametro.
    * @param[in] FileName nombre del archivo con el path en donde se guardara la lista
    * @param[in] pList lista de puntos de control que contiene la informacion a guardar
    * @return true en caso de poder guardar correctamente los puntos de control
    * @return false en caso contrario
    */
   virtual bool Save(const std::string& FileName, const GcpList& List);

private:
   /** Definicion de puntero a metodo para carga de valores de atributos*/
   typedef bool (TextFileGcpLoader::*AttributeLoader)(const std::string& Value, GcpList &pList);
   /** struct para el manejo de la carga del valor del atributo **/
   typedef struct Attribute {
      std::string definition_;
      AttributeLoader loaderFunc_;
   } Attribute;

   /** Metodo encargada de cargar el atributo de sistema de referencia
    *  del origen
    */
   bool LoadSourceSpatialReference(const std::string& Value, GcpList &pList);

   /** Metodo encargada de cargar el atributo de sistema de referencia
    *  del destino
    */
   bool LoadDestinationSpatialReference(const std::string& Value, GcpList &pList);
   /** Metodo encargada de cargar el atributo de modelo raster
    *  del origen
    */
   bool LoadSourceRasterModel(const std::string& Value, GcpList &pList);
   /** Metodo encargada de cargar el atributo de modelo raster
    *  del destino
    */
   bool LoadDestinationRasterModel(const std::string& Value, GcpList &pList);
   /** Metodo encargada de cargar el atributo de informacion sobre la fuente
    *  de datos del origen
    */
   bool LoadSourceDatasourceInfo(const std::string& Value, GcpList &pList);
   /** Metodo encargada de cargar el atributo de informacion sobre la fuente
    *  de datos del origen
    */
   bool LoadDestinationDatasourceInfo(const std::string& Value, GcpList &pList);
   /** Metodo encargada de cargar el atributo de tipo de transformacion */
   bool LoadTransformationType(const std::string& Value, GcpList &pList);
   /** Metodo encargada de cargar el atributo de parametros de la transformacion  */
   bool LoadTransformationParameters(const std::string& Value, GcpList &pList);
   /**
    * Metodo auxiliar que carga las palabras usadas para la definicion de atributos
    */
   void LoadAttributesDefinition(std::vector<Attribute>& Attributes);
   /** Metodo que parsea una linea en un punto de control **/
   GroundControlPointExtended* CreateGcpFromLine(const std::string& GcpLine);
   /** Carga la informacion que se encuentra en la linea comentario a la lista*/
   bool LoadParameterFromComment(const std::string& CommentLine, GcpList &pList);
   /** Metodo auxiliar que crea el string asociado a un punto de control **/
   std::string CreateLineFromGcp(const GroundControlPointExtended& pGcp);
   /** Indica cual es la columna correspondiente a la coordenada x del origen */
   void SetSourceCoordinateXColumn(int Index);
   /** Indica cual es la columna correspondiente a la coordenada y del origen */
   void SetSourceCoordinateYColumn(int Index);
   /** Indica cual es la columna correspondiente a la coordenada x del destino */
   void SetDestCoordinateXColumn(int Index);
   /** Indica cual es la columna correspondiente a la coordenada y del destino */
   void SetDestCoordinateYColumn(int Index);
   /** Indica cual es la columna que indica si el punto se encuentra activo */
   void SetEnableColumn(int Index);
   /** Metodo auxiliar que obtiene el url y el id de una linea que tiene informacion
     * sobre la fuente de datos en formato {url="..",id="..."}
     */
   void LoadUrlAndIdFromLine(const std::string &Line, std::string& Url, std::string &Id);
   /** Obtiene de lal lista de puntos de control las lineas correspondientes al
    *  header del archivo de gcp*/
   void  GetInfoHeader(const GcpList& List, std::vector<std::string>& Header) const;
   /** token para las columnas de la tabla de puntos **/
   std::string pointToken_;
   /** numeros de columnas para coordenadas **/
   int sourceX_, sourceY_, destX_, destY_, enableIndex_;
};

} /** namespace suri */

#endif /* TEXTFILEGCPLOADER_H_ */
