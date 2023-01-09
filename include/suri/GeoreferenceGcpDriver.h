/** \file GeoreferenceGcpDriver.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOREFERENCEGCPDRIVER_H_
#define GEOREFERENCEGCPDRIVER_H_

// Includes Estandar
#include <string>
#include <map>
#include <set>
#include <vector>

// Includes Suri
#include "GcpDriver.h"
#include "GcpLoaderInterface.h"
#include "MemoryDriver.h"
#include "EquationParser.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/Subject.h"
#include "ObserverInterface.h"
#include "suri/PolynomialCoordinatesTransformation.h"

namespace suri {

// Forwards
class GroundControlPointExtended;

/** Clase que representa un driver para puntos de control provenientes de la toma de
 *  puntos durante el georreferencia de una imagen */
class GeoreferenceGcpDriver : public suri::GcpDriver {
public:
   /** Definicion de nombres de columas **/
   static const std::string kGeometryColumn;
   /**
    * Ctor.
    */
   GeoreferenceGcpDriver();

   /**
    * Ctor.
    */
   explicit GeoreferenceGcpDriver(const std::string& GcpFileName,
                                 const std::string& SourceSrs  = "",
                                 const std::string& DestinationSrs  = "",
                                 const std::string& SourceRasterModel  = "",
                                 const std::string& DestinationRasterModel  = "");

   /**
    * Dtor.
    */
   virtual ~GeoreferenceGcpDriver();

   /** Configura gcps con el srs de destino */
   void SetGcpSourceSrs(const std::string &SourceSrs);

   /**
    * Ejecuta tareas extra de inicializacion.
    * Se usa para terminar algunas tareas despues de haber attachado un observer.
    */
   virtual void LazyInit();

protected:
   /** Flags para indicar la configuracion de las columnas de puntos de control **/
   static const int kX;
   static const int kY;
   static const int kPixel;
   static const int kLine;
   static const int kGeometry;

   /** indices de columnas **/
   int xIndex_, yIndex_;
   int pixelIndex_, lineIndex_;
   int errorxIndex_, erroryIndex_, error2Index_;
   int geometryIndex_;

   /**
    * Verifica si se configuraron valores en las columnas para permitir el
    * agregado de una nueva fila en funcion de la celda en el cual se configuro
    * el valor.
    */
   virtual void UpdateAppendRowAvailability(int Column, int Row);

   /**
    * Actualiza las celdas de geometrias en funcion de la actualizacion de celdas
    * correspondientes a alguna coordenada del punto de control.
    */
   virtual void UpdateGeometriesColumns(int Column, int Row, float Data);

   /**
    * Inicializa la tabla (columnas y atributos necesarios)
    */
   virtual void InitializeColumns();

   /**
    * Metodo auxiliar que dado un punto de control se agrega en la tabla.
    */
   virtual void AddGcpToTable(const GroundControlPointExtended& pGcp, int Row);


private:
   /**
    * Metodo auxiliar que verifica que para la columna autocalculada
    *  se encuentren configurados los valores necesarios para realizar
    *  el calculo de su valor
    */
   bool HasValidCalculationData(int ColumnIndex, int Row);

   /** Actualiza el modelo para la conversion de puntos */
   void UpdateModel(bool UpdateValues = true);

   /**  Metodo auxiliar que actualiza el valor de las celdas de las columnas autocalculables
    *  en funcion de los valores actualizados de las demas columnas*/
   bool UpdateCalculatedData(int Row);
   /** Lee la informacion que posee la celda en funcion del tipo de dato de la columna*/
   bool ReadData(int Column, int Row, std::string& Data);
   /** Obtiene el indice de la columna que corresponde a una variable de una ecuacion */
   int GetVariableColumnIndex(const std::string& Variable) const;
   /** Actualiza la celda de la columna calculada en funcion del parser que se pasa por parametro*/
   bool UpdateCalculatedCellData(int Column, int Row, EquationParser& Parser);

   /** Convierte las filas de la tabla en una lista valida de puntos de control */
   void UpdateGcpList();

   /** Actualiza las celdas de puntos de control en funcion de la
    *  actualizacion de celdas correspondientes a la descripcion de la geometria del punto*/
   void UpdateGcpColumns(int Column, int Row, const std::string& Data);
   /** Obtiene la coordenada correspondiente al punto definido en formato wkt **/
   void GetCoordinateFromWkt(const std::string& Data, Coordinates& Coordinate) const;

};

}  // namespace suri

#endif  // GEOREFERENCEGCPDRIVER_H_
