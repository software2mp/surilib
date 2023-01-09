/*! \file CoregisterGcpDriver.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COREGISTERGCPDRIVEDR_H_
#define COREGISTERGCPDRIVEDR_H_

// Includes Estandar
// Includes Suri
#include "GcpDriver.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase que representa un driver para puntos de control
 * provenientes de la toma de puntos durante el corregistro de una imagen
 */
class CoregisterGcpDriver : public GcpDriver {
public:
   /** constantes para la definicion de los nombres de las columnas geometrias **/
   static const std::string SourceGeometryColumn;
   static const std::string DestinationGeometryColumn;

   /** Constructor para abrir/inicializar un nuevo archivo gcps */
   explicit CoregisterGcpDriver(const std::string& GcpFileName,
                                 const std::string& SourceSrs = "",
                                 const std::string& DestinationSrs = "",
                                 const std::string& SourceRasterModel = "",
                                 const std::string& DestinationRasterModel = "");
   /** Constructor default */
   CoregisterGcpDriver();
   /** destructor */
   virtual ~CoregisterGcpDriver();

private:
   /** Flags para indicar la configuracion de las columnas de puntos de control **/
   static const int PixelBaseSet_;
   static const int LineBaseSet_;
   static const int PixelWarpSet_;
   static const int LineWarpSet_;
   static const int GeometryBaseSet_;
   static const int GeometryWarpSet_;
   /**
    * Indices de las columnas
    */
   int basePixelIndex_, baseLineIndex_;
   int warpPixelIndex_, warpLineIndex_;
   int modelX_, modelY_;
   int errorxIndex_, erroryIndex_, error2Index_;
   int geom1ndex_, geom2Index_;
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

   /**
    * Metodo auxiliar que verifica que para la columna autocalculada
    *  se encuentren configurados los valores necesarios para realizar
    *  el calculo de su valor
    */
   virtual bool HasValidCalculationData(int ColumnIndex, int Row);

   /** Actualiza el modelo para la conversion de puntos */
   virtual void UpdateModel(bool UpdateValues = true);

   /**  Metodo auxiliar que actualiza el valor de las celdas de las columnas autocalculables
    *  en funcion de los valores actualizados de las demas columnas*/
   virtual bool UpdateCalculatedData(int Row);
   /** Lee la informacion que posee la celda en funcion del tipo de dato de la columna*/
   virtual bool ReadData(int Column, int Row, std::string& Data);
   /** Obtiene el indice de la columna que corresponde a una variable de una ecuacion */
   virtual int GetVariableColumnIndex(const std::string& Variable) const;
   /** Actualiza la celda de la columna calculada en funcion del parser que se pasa por parametro*/
   virtual bool UpdateCalculatedCellData(int Column, int Row, EquationParser& Parser);
   /** Convierte las filas de la tabla en una lista valida de puntos de control */
   virtual void UpdateGcpList();
   /** Actualiza las celdas de puntos de control en funcion de la
    *  actualizacion de celdas correspondientes a la descripcion de la geometria del punto*/
   virtual void UpdateGcpColumns(int Column, int Row, const std::string& Data);
   /** Obtiene la coordenada correspondiente al punto definido en formato wkt **/
   virtual void GetCoordinateFromWkt(const std::string& Data,
                                     Coordinates& Coordinate) const;

};

} /** namespace suri */

#endif /* COREGISTERGCPDRIVEDR_H_ */
