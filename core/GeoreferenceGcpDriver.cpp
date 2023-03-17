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

#include <vector>
#include <string>
#include <limits>

#include "suri/GeoreferenceGcpDriver.h"
#include "suri/TextFileGcpLoader.h"
#include "suri/DataTypes.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/PolynomialCoordinatesTransformation.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "NewRowNotification.h"
#include "RmseChangeNotification.h"
#include "SourceSrsChangeNotification.h"

// Defines
#define GEOREF_ENABLE_COLUMN_NAME      "Activado"
#define GEOREF_X_COLUMN_NAME           "E"
#define GEOREF_Y_COLUMN_NAME           "N"
#define GEOREF_CALCULATION_ERROR_MSG   "#ERROR: No se puede calcular el valor"

namespace suri {

/** Definicion de nombres de columas **/
const std::string GeoreferenceGcpDriver::kGeometryColumn = "the_geom";

/** flagas para columnas calculables**/
const int GeoreferenceGcpDriver::kX = 0x01;
const int GeoreferenceGcpDriver::kY = 0x02;
const int GeoreferenceGcpDriver::kPixel = 0x04;
const int GeoreferenceGcpDriver::kLine = 0x08;
const int GeoreferenceGcpDriver::kGeometry = 0x10;

/**
 * Ctor.
 */
GeoreferenceGcpDriver::GeoreferenceGcpDriver() {}

/**
 * Ctor.
 */
GeoreferenceGcpDriver::GeoreferenceGcpDriver(const std::string& GcpFileName,
                                             const std::string& SourceSrs,
                                             const std::string& DestinationSrs,
                                             const std::string& SourceRasterModel,
                                             const std::string& DestinationRasterModel) :
      GcpDriver() {
   this->gcpFileName_.assign(GcpFileName); 
   gcpList_.SetSpatialReferenceSource(SourceSrs);
   gcpList_.SetSpatialReferenceDestination(DestinationSrs);
   gcpList_.SetRasterModelSource(SourceRasterModel);
   gcpList_.SetRasterModelDestination(DestinationRasterModel);
}

/**
 * Dtor.
 */
GeoreferenceGcpDriver::~GeoreferenceGcpDriver() {}

/**
 * Verifica si se configuraron valores en las columnas para permitir el
 * agregado de una nueva fila en funcion de la celda en el cual se configuro
 * el valor.
 */
void GeoreferenceGcpDriver::UpdateAppendRowAvailability(int Column, int Row) {
   // si se puede agregar fila verifico como se van configurando los valores de las columnas
   if (!rowAvailable_ && Row == GetRows() - 1) {
      if (Column == xIndex_) {
         currentValuesSetFlag_ = currentValuesSetFlag_ | kX;
      } else if (Column == yIndex_) {
         currentValuesSetFlag_ = currentValuesSetFlag_ | kY;
      } else if (Column == pixelIndex_) {
         currentValuesSetFlag_ = currentValuesSetFlag_ | kPixel;
      } else if (Column == lineIndex_) {
         currentValuesSetFlag_ = currentValuesSetFlag_ | kLine;
      } else if (Column == geometryIndex_) {
         currentValuesSetFlag_ = currentValuesSetFlag_ | kGeometry;
      }
   }
   bool x = (currentValuesSetFlag_ & kX);
   bool y = (currentValuesSetFlag_ & kY);
   bool pixel = (currentValuesSetFlag_ & kPixel);
   bool line = (currentValuesSetFlag_ & kLine);
   bool geometry = (currentValuesSetFlag_ & kGeometry);

   // si se configuraron todas las columnas necesarias entonces se puede agregar
   // una nueva fila
   rowAvailable_ = x & y & pixel & line & geometry;
}

/**
 * Actualiza las celdas de geometrias en funcion de la actualizacion de celdas
 * correspondientes a alguna coordenada del punto de control.
 */
void GeoreferenceGcpDriver::UpdateGeometriesColumns(int Column, int Row, float Data) {
   int columnindex = -1;
   Coordinates coord;
   columnindex = geometryIndex_;
   if (Column == pixelIndex_ || Column == lineIndex_) {
      if (Column == pixelIndex_) {
         coord.x_ = Data;
         float y;
         memDriver_.ReadFloat(lineIndex_, Row, y);
         coord.y_ = y;
      } else {
         float x;
         memDriver_.ReadFloat(pixelIndex_, Row, x);
         coord.x_ = x;
         coord.y_ = Data;
      }

      if (columnindex >= 0
            && memDriver_.WriteVarchar(columnindex, Row,
                                         GetWktRepresentation(coord))) {
         UpdateAppendRowAvailability(columnindex, Row);
      }
   }
}

/**
 * Inicializa la tabla (columnas y atributos necesarios)
 */
void GeoreferenceGcpDriver::InitializeColumns() {
   if (includeEnableColumn_) {
      enableIndex_ = GetColumns();
      AppendColumn(DataInfo<int>::Name, _(GEOREF_ENABLE_COLUMN_NAME));
   }

   xIndex_ = GetColumns();
   AppendColumn(DataInfo<float>::Name, _(GEOREF_X_COLUMN_NAME));

   yIndex_ = GetColumns();
   AppendColumn(DataInfo<float>::Name, _(GEOREF_Y_COLUMN_NAME));

   pixelIndex_ = GetColumns();
   AppendColumn(DataInfo<float>::Name, _(XmodelColumnName.c_str()));

   lineIndex_ = GetColumns();
   AppendColumn(DataInfo<float>::Name, _(YmodelColumnName.c_str()));

   std::string equationex = "c_";
   equationex.append(NumberToString<int>(xIndex_));
   equationex.append(" - c_");
   equationex.append(NumberToString<int>(pixelIndex_));

   std::string equationey = "c_";
   equationey.append(NumberToString<int>(yIndex_));
   equationey.append(" - c_");
   equationey.append(NumberToString<int>(lineIndex_));

   std::string equation2 = "sqrt( (";
   equation2.append(equationex);
   equation2.append(")^2 + (");
   equation2.append(equationey);
   equation2.append(")^2 )");

   errorxIndex_ = GetColumns();
   AppendCalculatedColumn(DataInfo<unsigned char>::Name, _(XErrorColumnName.c_str()),
                          equationex, VirtualFlag);

   erroryIndex_ = GetColumns();
   AppendCalculatedColumn(DataInfo<unsigned char>::Name, _(YErrorColumnName.c_str()),
                          equationey, VirtualFlag);

   geometryIndex_ = GetColumns();
   AppendColumn(DataInfo<unsigned char>::Name, _(kGeometryColumn.c_str()),
                VirtualFlag);
}

/**
 * Metodo auxiliar que dado un punto de control se agrega en la tabla.
 */
void GeoreferenceGcpDriver::AddGcpToTable(const GroundControlPointExtended& pGcp, int Row) {
   if (AppendRow()) {
      WriteInt(enableIndex_, Row, pGcp.IsEnabled());
      Coordinates psource = pGcp.GetSource();
      WriteDouble(xIndex_, Row, psource.x_);
      WriteDouble(yIndex_, Row, psource.y_);
      Coordinates pdestination = pGcp.GetDestination();
      WriteDouble(pixelIndex_, Row, pdestination.x_);
      WriteDouble(lineIndex_, Row, pdestination.y_);
      WriteVarchar(geometryIndex_, Row, GetWktRepresentation(pdestination));
   }
}

/** Metodo auxiliar que actualiza el valor de las celdas de las columnas autocalculables
 *  en funcion de los valores actualizados de las demas columnas
 * @param Row indice de fila a actualizar
 * @return true en caso de actualizar correctamente los valores de las celdas
 * correspondientes de las columnas autocalculables
 * @return false en caso contrario
 */
bool GeoreferenceGcpDriver::UpdateCalculatedData(int Row) {
   AutoColumnsMap::iterator it = autoColumns_.begin();
   EquationParser parser;
   bool error = false;
   for (; it != autoColumns_.end(); ++it) {
      error = (!parser.SetEquation(it->second)
            || !UpdateCalculatedCellData(it->first, Row, parser));
   }
   return !error;
}

/**
 * Lee la informacion que posee la celda en funcion del tipo de dato de la columna
 * @param[in] Column indice de columbna
 * @param[in] Row indice de fila
 * @param[out] Data informacion que posee la celda
 * @return
 */
bool GeoreferenceGcpDriver::ReadData(int Column, int Row, std::string& Data) {
   if (!IsValidCell(Column, Row))
      return false;
   bool result = false;
   if (IsFloatColumn(Column)) {
      float data = 0;
      result = ReadFloat(Column, Row, data);
      Data = NumberToString<float>(data);
   } else if (IsIntColumn(Column)) {
      int data = 0;
      result = ReadInt(Column, Row, data);
      Data = NumberToString<int>(data);
   } else if (IsVarcharColumn(Column)) {
      result = ReadVarchar(Column, Row, Data);
   }
   return result;
}

/**
 * Obtiene el indice de la columna que corresponde a una variable de una ecuacion
 * @param Variable variable sobre la cual se consulta
 * @return indice de la columna
 * @return -1 en caso de no encontrar un indice valido de la columna
 */
int GeoreferenceGcpDriver::GetVariableColumnIndex(const std::string& Variable) const {
   size_t found = Variable.find_last_of("_");
   int columnindex = -1;
   if (found != std::string::npos) {
      std::string columnindexstr = Variable.substr(found + 1);
      columnindex = StringToNumber<int>(columnindexstr);
   }
   return columnindex;
}

/**
 * Actualiza la celda de la columna calculada en funcion del parser que se pasa por parametro
 * @param[in] Column indice de columna autocalculable
 * @param[in] Row indice de fila que se actualiza
 * @param[in] Parser referencia al parser que tiene cargada la ecuacion
 * @return true en caso de actualizar con exito la celda
 * @return false en caso contrario
 */
bool GeoreferenceGcpDriver::UpdateCalculatedCellData(int Column, int Row, EquationParser& Parser) {
   std::vector < std::string > variablenames;
   Parser.GetVariableNames(variablenames);
   std::vector<std::string>::iterator eqit = variablenames.begin();
   bool error = pCoordinateTransform_ == NULL || !HasValidCalculationData(Column, Row);
   for (; eqit != variablenames.end() && !error; ++eqit) {
      int columnindex = GetVariableColumnIndex(*eqit);
      if (columnindex < 0) {
         error = true;
      } else {
         std::string data;
         ReadData(columnindex, Row, data);
         double value = StringToNumber<double>(data);
         // Si la columna es alguna coordenada base debo aplicar la transformacion
         if (columnindex == pixelIndex_) {
            std::string ydata;
            ReadData(yIndex_, Row, ydata);
            double yvalue = StringToNumber<double>(ydata);
            Coordinates coord(value, yvalue);
            if (!pCoordinateTransform_->Transform(coord)) {
               error = true;
               break;
            }
            value = coord.x_;
         } else if (columnindex == lineIndex_) {
            std::string xdata;
            ReadData(xIndex_, Row, xdata);
            double xvalue = StringToNumber<double>(xdata);
            Coordinates coord(xvalue, value);
            if (!pCoordinateTransform_->Transform(coord)) {
               error = true;
               break;
            }
            value = coord.y_;
         }
         double* pvalue = NULL;
         Parser.GetVariableValuePointer(*eqit, pvalue);
         if (pvalue) {
            *pvalue = value;
         }
      }
   }
   if (!error) {
      error = !memDriver_.WriteVarchar(
            Column, Row, NumberToString<float>(Parser.EvaluateEquation()));
   } else {
      memDriver_.WriteVarchar(Column, Row, GEOREF_CALCULATION_ERROR_MSG);
   }
   return !error;
}

/**
 * Convierte las filas de la tabla
 * en una lista valida de puntos de control
 * @param List lista de puntos de control sobre la cual guardar el contenido
 */
void GeoreferenceGcpDriver::UpdateGcpList() {
   this->gcpList_.Clear();
   for (int row = 0; row < GetRows(); ++row) {
      bool enable = true;
      Coordinates source;
      Coordinates destination;
      double data;
      ReadDouble(xIndex_, row, data);
      source.x_ = data;
      ReadDouble(yIndex_, row, data);
      source.y_ = data;
      ReadDouble(pixelIndex_, row, data);
      destination.x_ = data;
      ReadDouble(lineIndex_, row, data);
      destination.y_ = data;
      if (includeEnableColumn_) {
         int enableint = 0;
         ReadInt(enableIndex_, row, enableint);
         enable = enableint;
      }
      GroundControlPointExtended gcp(source, destination, enable);
      this->gcpList_.Add(gcp);
   }
}

/** Actualiza las celdas de puntos de control en funcion de la
 *  actualizacion de celdas correspondientes a la descripcion de la geometria del punto
 * @param[in] Column Indice de columna
 * @param[in] Row Indice de fila
 * @param[in] Data informacion proveniente de alguna celda de geometria
 */
void GeoreferenceGcpDriver::UpdateGcpColumns(int Column, int Row, const std::string& Data) {
   Coordinates coord;
   GetCoordinateFromWkt(Data, coord);
   int columnxindex = -1;
   int columnyindex = -1;

   if (Column == geometryIndex_) {
      columnxindex = pixelIndex_;
      columnyindex = lineIndex_;
   }

   if (columnxindex >= 0) {
      float x = coord.x_;
      float y = coord.y_;
      memDriver_.WriteFloat(columnxindex, Row, x);
      UpdateAppendRowAvailability(columnxindex, Row);
      memDriver_.WriteFloat(columnyindex, Row, y);
      UpdateAppendRowAvailability(columnyindex, Row);
      UpdateGcpList();
      UpdateModel();
      UpdateCalculatedData(Row);
   }
}

/**
 * Obtiene la coordenada correspondiente al punto definido en formato wkt
 * @param[in] Data string que contiene el wkt
 * @param[out] Coordinate coordenada correspondiente al punto wkt
 * @return coordenada
 */
void GeoreferenceGcpDriver::GetCoordinateFromWkt(const std::string& Data,
                                     Coordinates& Coordinate) const {
   size_t firstpost = Data.find("(");
   size_t lastpost = Data.find(")");
   if (firstpost != std::string::npos && lastpost != std::string::npos) {
      std::string poitnstr = Data.substr(firstpost + 1, lastpost - 1);
      std::vector<double> coords = split<double>(poitnstr);
      if (coords.size() == 2)
         Coordinate.x_ = coords.at(0);
      Coordinate.y_ = coords.at(1);
   }
}

/** Actualiza el modelo para la conversion de puntos */
void GeoreferenceGcpDriver::UpdateModel(bool UpdateValues) {
   suri::ParameterCollection params;
   params.AddValue<std::vector<GroundControlPoint> >(
         suri::TransformationFactory::kParamGcps, gcpList_.CreateAdaptedGcps());
   params.AddValue<int>(suri::TransformationFactory::kParamOrder, order_);
   suri::TransformationFactory* pfactory = suri::TransformationFactoryBuilder::Build();

   suri::CoordinatesTransformation* ptransform = pfactory->Create(
         suri::TransformationFactory::kPolynomial, params);
   if (pCoordinateTransform_ != NULL)
      delete pCoordinateTransform_;
   pCoordinateTransform_ = ptransform;
   if (UpdateValues)
      UpdateCalculatedColumnsValue();
   RmseChangeNotification notif(GetRmse());
   Notify(&notif);
   suri::TransformationFactoryBuilder::Release(pfactory);
}

/**
 * Metodo auxiliar que verifica que para la columna autocalculada
 *  se encuentren configurados los valores necesarios para realizar
 *  el calculo de su valor
 * @param ColumnCalculatedIndex indice de la columna autocalculada
 * @param Row indice de la fila sobre la cual se realizara el calculo
 * @return true en caso de que esten configurados los valores
 * @return false en caso contrario
 */
bool GeoreferenceGcpDriver::HasValidCalculationData(int ColumnCalculatedIndex, int Row) {
   bool valid = false;
   // Si la fila es anterior a la ultima, ya se encuentran configurados los valores
   if (Row < GetRows() - 1)
      return true;
   bool x = (currentValuesSetFlag_ & kX);
   bool y = (currentValuesSetFlag_ & kY);
   bool pixel = (currentValuesSetFlag_ & kPixel);
   bool line = (currentValuesSetFlag_ & kLine);
   if (ColumnCalculatedIndex == errorxIndex_) {
      valid = x & pixel;
   } else if (ColumnCalculatedIndex == erroryIndex_) {
      valid = y & line;
   } else if (ColumnCalculatedIndex == error2Index_) {
      valid = x & pixel & y & line;
   }
   return valid;
}

/**
 * Ejecuta tareas extra de inicializacion.
 * Se usa para terminar algunas tareas despues de haber attachado un observer.
 */
void GeoreferenceGcpDriver::LazyInit() {
   SourceSrsChangeNotification notif(gcpList_.GetSpatialReferenceSource());
   Notify(&notif);
   // UpdateModel(false);
}

/**
 * Configura gcps con el srs de destino
 * @param[in] DestinationSrs sistema de referencia a configurar
 */
void GeoreferenceGcpDriver::SetGcpSourceSrs(const std::string &SourceSrs) {
   gcpList_.SetSpatialReferenceSource(SourceSrs);
}

}  // namespace suri
