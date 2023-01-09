/*! \file VectorEditorDriver.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <vector>
#include <string>
#include <limits>
#include <map>
#include <utility>
#include <fenv.h>

// Includes Suri
#include "suri/AuxiliaryFunctions.h"
#include "PermissionList.h"
#include "suri/VectorEditorDriver.h"
#include "suri/DatasourceInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

const short MaxValue = std::numeric_limits<short>::max();
const std::string VectorEditorDriver::GeometryColumnName = "the_geom";
const std::string VectorEditorDriver::FeatureIdColumnName = "ID";

/*
 * Constructor
 */
VectorEditorDriver::VectorEditorDriver(DatasourceInterface* pDatasource) :
                                 permissionKey_(0), geometryColumnIndex_(-1) {
   if (!pDatasource)
      return;
   id_ = pDatasource->GetId();
   permissions_ = new PermissionList();
   std::string file = pDatasource->GetUrl();
   vectorEditor_.OpenVector(file);
   // TODO(Gabriel - TCK #3988): se tendria que abrir la capa activa no
   // la 0. Esto trae problemas por ej. para gpx
   if (!vectorEditor_.OpenLayer(0))
      return;
   if (vectorEditor_.CanRead()) {
      permissions_->InsertOperation(permissionKey_, PermissionList::GETVAL);
   }
   if (vectorEditor_.CanWrite() && !pDatasource->IsReadOnly()) {
      permissions_->InsertOperation(permissionKey_, PermissionList::SETVAL);
   }
   if (vectorEditor_.CanCreateLayer()) {
      if (vectorEditor_.CanCreateField()) {
         // VectorEditor soporta append, pero no insert
         permissions_->InsertOperation(permissionKey_, PermissionList::APPCOL);
      }
      if (vectorEditor_.CanDeleteField()) {
         permissions_->InsertOperation(permissionKey_, PermissionList::DELCOL);
      }
#ifdef __PROBLEM_APPENDING_ROWS_WITH_VECTOR_EDITOR_SOLVED__
      if (vectorEditor_.CanWrite()) {
         // No hay soporte directo con un metodo "CanCreateFeature"
         // VectorEditor soporta append, pero no insert
         permissions_->InsertOperation(permissionKey_, PermissionList::APPROW);
      }
#else
      if (vectorEditor_.CanCreateFeature()) {
         // VectorEditor soporta append, pero no insert
         permissions_->InsertOperation(permissionKey_, PermissionList::APPROW);
      }
#endif
      if (vectorEditor_.CanDeleteFeature()) {
         permissions_->InsertOperation(permissionKey_, PermissionList::DELROW);
      }
   }
   if (vectorEditor_.GetFieldPosition(FeatureIdColumnName) < 0) {
      // Si no posee la columna fid la agrega al iniciar la tabla
      vectorEditor_.CreateField(FeatureIdColumnName,DataInfo<int>::Name);
   }
   geometryColumnIndex_ = 0;
   int rows = vectorEditor_.GetFeatureCount();
   for (int r = 0; r < rows; ++r) {
      // TODO(Gabriel - TCK #2741): Ver valor por defecto en la columna.
      // Optimizar: por ahora levanta toda la columna en memoria.
      geometryColumn_.insert(std::make_pair(r, ""));
      short featureid = vectorEditor_.GetFeatureId(r);
      rows_.push_back(featureid);
   }
}

/**
 * Destructor
 */
VectorEditorDriver::~VectorEditorDriver() {
   delete permissions_;
   vectorEditor_.CloseLayer();
}

/**
 * Devuelve el FeatureId en funcion del Row
 * @param Row
 */
short VectorEditorDriver::GetFeatureIdAt(int Row) const {
   int rows = GetRows();
   if (0 > Row || Row > rows)
      return MaxValue;
   std::vector<short>::const_iterator it = rows_.begin();
   for (int i = 0; i < Row && it != rows_.end(); i++) {
      it++;
   }
   return it == rows_.end() ? MaxValue : (*it);
}

/**
 * Elimina el FeatureId en funcion del Row
 * @param Row
 */
void VectorEditorDriver::EraseFeatureIdAt(int Row) {
   if (Row < static_cast<int>(rows_.size())) {
      rows_.erase(rows_.begin() + Row);
   }
}

/**
 * Leer un entero del driver
 * @param[in] Column indice de la columna de la cual se leera el dato
 * @param[in] Row indice de la fila de la cual se leera el dato
 * @param[in] Data dato a ser leido
 * @return true si pudo leer exitosamente el dato, sino devuelve falso
 */
bool VectorEditorDriver::ReadInt(int Column, int Row, int& Data) const {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::GETVAL))
      return false;

   if (!IsValidCell(Column, Row) || !IsIntColumn(Column)) {
      return false;
   }

   // No se puede agregar const a GetFeatureId, OpenFeature y CloseFeature y se hace esto
   VectorEditor& ve = const_cast<VectorEditor&>(vectorEditor_);

   // Busca indice de feature en posicion Row y lo abre
   long featureid = ve.GetFeatureId(Row);
   if (featureid < 0 || ve.OpenFeature(featureid, false) < 0) {
      return false;
   }

   int fieldvalue = 0;
   ve.GetField(ve.GetFieldName(Column - 1), fieldvalue);
   Data = fieldvalue;

   ve.CloseFeature();
   return true;
}

/**
 * Escribir un entero en el driver
 * @param[in] Column indice de la columna en la cual se escribira el dato
 * @param[in] Row indice de la fila en la cual se escribira el dato
 * @param[in] Data dato a ser escrito
 * @return true si pudo escribir exitosamente el dato, sino devuelve falso
 */
bool VectorEditorDriver::WriteInt(int Column, int Row, int Data) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::SETVAL))
      return false;
   if (!IsValidCell(Column, Row) || !IsIntColumn(Column)) {
      return false;
   }

   // Busca indice de feature en posicion Row y lo abre
   long featureid = GetFeatureIdAt(Row);
   if (featureid == MaxValue || featureid < 0
         || vectorEditor_.OpenFeature(featureid, false) < 0) {
      return false;
   }

   std::string fieldname = vectorEditor_.GetFieldName(Column - 1);
   bool returnvalue = vectorEditor_.SetField(fieldname, Data);

   vectorEditor_.CloseFeature();
   vectorEditor_.SyncToDisk();
   return returnvalue;
}

/**
 * Leer un string del driver
 * @param[in] Column indice de la columna de la cual se leera el dato
 * @param[in] Row indice de la fila de la cual se leera el dato
 * @param[in] Data dato a ser leido
 * @return true si pudo leer exitosamente el dato, sino devuelve falso
 */
bool VectorEditorDriver::ReadVarchar(int Column, int Row, std::string& Data) const {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::GETVAL))
      return false;

   if (!IsValidCell(Column, Row) && Column != geometryColumnIndex_) {
      return false;
   }

   // No se puede agregar const a GetFeatureId, OpenFeature y CloseFeature y se hace esto
   VectorEditor& ve = const_cast<VectorEditor&>(vectorEditor_);

   // Busca indice de feature en posicion Row y lo abre
   long featureid = ve.GetFeatureId(Row);
   if (featureid < 0 || ve.OpenFeature(featureid, false) < 0) {
      return false;
   }

   if (Column != geometryColumnIndex_) {
      std::string fieldcontent = "";
      std::string name = ve.GetFieldName(Column - 1);
      ve.GetField(name, fieldcontent);
      Data = fieldcontent;

      ve.CloseFeature();
   } else {
      GeometryColumnMap::const_iterator findit = geometryColumn_.find(Row);
      if (findit != geometryColumn_.end() && ve.OpenFeature(featureid, false) >= 0) {
         Data =
               (!(findit->second).empty()) ? findit->second :
                                             FetchGeometryWkt(featureid);
         ve.CloseFeature();
      } else {
         return false;
      }
   }
   return true;
}

/**
 * Escribir un string en el driver
 * @param[in] Column indice de la columna en la cual se escribira el dato
 * @param[in] Row indice de la fila en la cual se escribira el dato
 * @param[in] Data dato a ser escrito
 * @return true si pudo escribir exitosamente el dato, sino devuelve falso
 *
 * \todo (danieltesta-#3189): En todos lados se esta usando Column - 1 ya
 * que la posicion 0 esta reservada para la columna the_geom. Habria que
 * verificar tambien si un vector sigue siendo necesario y no haria falta
 * cambiar por un mapa (que pasa cuando tengo 2 columnas virtuales antes)
 */
bool VectorEditorDriver::WriteVarchar(int Column, int Row, const std::string& Data) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::SETVAL))
      return false;
   if (!IsValidCell(Column, Row) && Column != geometryColumnIndex_) {
      return false;
   }
   // Busca indice de feature en posicion Row y lo abre
   long featureid = GetFeatureIdAt(Row);
   if (featureid == MaxValue)
      return false;
   if (Column != geometryColumnIndex_) {
      if (featureid == MaxValue || featureid < 0
            || vectorEditor_.OpenFeature(featureid, false) < 0) {
         return false;
      }
      std::string fieldname = vectorEditor_.GetFieldName(Column - 1);
      bool returnvalue = vectorEditor_.SetField(fieldname, Data);
      vectorEditor_.CloseFeature();
      vectorEditor_.SyncToDisk();
      return returnvalue;
   } else {
      // Primero intenta configurar la geometria y luego borra el valor anterior
      // de la columna
      if (!Data.empty()) {
         if (vectorEditor_.SetGeometryByWkt(featureid, Data)
               && geometryColumn_.erase(Row) > 0) {
            geometryColumn_.insert(std::make_pair(Row, Data));
            vectorEditor_.CloseFeature();
            vectorEditor_.SyncToDisk();
         } else {
            return false;
         }
      }
   }

   return true;
}

/**
 * Leer un punto flotante del driver (VER COMENTARIO EN ReadDouble)
 *
 * @param[in] Column indice de la columna de la cual se leera el dato
 * @param[in] Row indice de la fila de la cual se leera el dato
 * @param[in] Data dato a ser leido
 * @return true si pudo leer exitosamente el dato, sino devuelve falso
 */
bool VectorEditorDriver::ReadFloat(int Column, int Row, float& Data) const {
   fesetround(FE_DOWNWARD);
   double fieldvalue = 0.0;
   bool success=ReadDouble(Column,Row,fieldvalue);
   if (!success) return false;
   Data = static_cast<float>(fieldvalue);
   fesetround(FE_UPWARD);
   return true;
}

/**
 * Leer un double del driver
 * @param[in] Column indice de la columna de la cual se leera el dato
 * @param[in] Row indice de la fila de la cual se leera el dato
 * @param[in] Data dato a ser leido
 * @return true si pudo leer exitosamente el dato, sino devuelve falso
 */
bool VectorEditorDriver::ReadDouble(int Column, int Row, double& Data) const {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::GETVAL))
      return false;

   // Esto sirve para leer double y float ya que OGRFeature puede leer double solamente.
   if (!IsValidCell(Column, Row) || !(IsFloatColumn(Column) || IsDoubleColumn(Column))) {
      return false;
   }

   // No se puede agregar const a GetFeatureId, OpenFeature y CloseFeature y se hace esto
   VectorEditor& ve = const_cast<VectorEditor&>(vectorEditor_);

   // Busca indice de feature en posicion Row y lo abre
   long featureid = ve.GetFeatureId(Row);
   if (featureid < 0 || ve.OpenFeature(featureid, false) < 0) {
      return false;
   }
   ve.GetField(ve.GetFieldName(Column - 1), Data);
   ve.CloseFeature();
   return true;
}


bool VectorEditorDriver::WriteDouble(int Column,int Row, double Data){
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::SETVAL))
        return false;
     if (!IsValidCell(Column, Row) || !IsDoubleColumn(Column)) {
        return false;
     }

     // Busca indice de feature en posicion Row y lo abre
     long featureid = GetFeatureIdAt(Row);
     if (featureid == MaxValue || featureid < 0
           || vectorEditor_.OpenFeature(featureid, false) < 0) {
        return false;
     }

     std::string fieldname = vectorEditor_.GetFieldName(Column - 1);
     std::string data=DoubleToStringEx(Data,6);
     bool returnvalue = vectorEditor_.SetField(fieldname, data);

     vectorEditor_.CloseFeature();
     vectorEditor_.SyncToDisk();
     return returnvalue;
}
/**
 * Escribir un punto flotante en el driver
 * @param[in] Column indice de la columna en la cual se escribira el dato
 * @param[in] Row indice de la fila en la cual se escribira el dato
 * @param[in] Data dato a ser escrito
 * @return true si pudo escribir exitosamente el dato, sino devuelve falso
 */
bool VectorEditorDriver::WriteFloat(int Column, int Row, float Data) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::SETVAL))
      return false;
   if (!IsValidCell(Column, Row) || !IsFloatColumn(Column)) {
      return false;
   }

   // Busca indice de feature en posicion Row y lo abre
   long featureid = GetFeatureIdAt(Row);
   if (featureid == MaxValue || featureid < 0
         || vectorEditor_.OpenFeature(featureid, false) < 0) {
      return false;
   }

   std::string fieldname = vectorEditor_.GetFieldName(Column - 1);
   bool returnvalue = vectorEditor_.SetField(fieldname, Data);

   vectorEditor_.CloseFeature();
   vectorEditor_.SyncToDisk();
   return returnvalue;
}

/**
 * Devuelve la cantidad de columnas
 * @return la cantidad de columnas del driver
 */
int VectorEditorDriver::GetColumns() const {
   // si el indice de la columna de geometria es mayor a 0 suma 1 a la cantida de
   // columnas (?)

//   if (geometryColumnIndex_ > 0)
//      return vectorEditor_.GetFieldCount();

   return vectorEditor_.GetFieldCount() + 1;
}

/**
 * Devuelve la cantidad de filas
 * @return la cantidad de filas del driver
 */
int VectorEditorDriver::GetRows() const {
#ifndef __UNUSED_CODE__
   return rows_.size();
#else
   // TODO(Gabriel - TCK #2741): Ver porque surge diferencia entre getfeaturecount
   // y la cantidad de filas luego de borrar una.
   return vectorEditor_.GetFeatureCount();
#endif
}

/**
 * Agrega una fila al final de la tabla
 * @return devuelve true si la operacion tuvo exito, sino devuelve false
 */
bool VectorEditorDriver::AppendRow() {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::APPROW))
      return false;

   long featureid = vectorEditor_.OpenFeature();
   rows_.push_back(featureid);
   geometryColumn_.insert(std::make_pair(rows_.size() - 1, ""));
   vectorEditor_.CloseFeature();
   vectorEditor_.SyncToDisk();
#ifdef __UNUSED_CODE__
   permissions_->EraseOperation(permissionKey_, PermissionList::APPROW);
#endif
   return true;
}

/**
 * Agrega una fila en la posicion especificada de la tabla
 * @param Row
 * @return
 */
bool VectorEditorDriver::InsertRow(int Row) {
   // VectorEditor soporta append, pero no insert
   return false;
}

/**
 * Elimina una fila de la tabla (debe existir, sino devuelve false)
 * @param[in] Row indice de la fila a ser eliminada
 * @return devuelve true si la operacion tuvo exito, sino devuelve false
 */
bool VectorEditorDriver::DeleteRow(int Row) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::DELROW))
      return false;

   vectorEditor_.CloseFeature();
   long featureid = GetFeatureIdAt(Row);
   if (featureid == MaxValue) {
      return false;
   }
   if (vectorEditor_.DeleteFeature(featureid)) {
      // Si se borra la ultima fila es la que estaba en edicion
      if (Row == static_cast<int>(rows_.size() - 1)) {
         permissions_->InsertOperation(permissionKey_, PermissionList::APPROW);
      }
      EraseFeatureIdAt(Row);
      vectorEditor_.SyncToDisk();
      geometryColumn_.erase(Row);
   } else {
      return false;
   }

   return true;
}

/**
 * Agrega una columna al final de la tabla
 * @param[in] ColumnType tipo de dato para la nueva columna
 * @param[in] ColumnName nombre de la columna
 * @param[in] ColumnFlag flag que indica el tipo de columna
 * @return devuelve true si la operacion tuvo exito, sino devuelve false
 */
bool VectorEditorDriver::AppendColumn(const std::string& ColumnType,
                                      const std::string& ColumnName,  int ColumnFlag) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::APPCOL))
      return false;

   return vectorEditor_.CreateField(ColumnName, ColumnType);
}

/**
 * Agrega una columna en la posicion especificada de la tabla
 * @param Column
 * @param ColumnType
 * @param ColumnName
 * @return
 */
bool VectorEditorDriver::InsertColumn(int Column, const std::string& ColumnType,
                                      const std::string& ColumnName, int ColumnFlag) {
   return false;
}

/**
 * Elimina una columna de la tabla (debe existir, sino devuelve false)
 * @param[in] Column indice de la columna a ser eliminada
 * @return devuelve true si la operacion tuvo exito, sino devuelve false
 */
bool VectorEditorDriver::DeleteColumn(int Column) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::DELCOL))
      return false;

   vectorEditor_.CloseFeature();
   std::string name = vectorEditor_.GetFieldName(Column - 1);
   if (vectorEditor_.DeleteField(name)) {
      vectorEditor_.SyncToDisk();
      return true;
   }

   return false;
}

/**
 * Elimina todas las filas y su contenido de la tabla (no elimina columnas)
 * @return devuelve true si la operacion tuvo exito, sino devuelve false
 */
bool VectorEditorDriver::Clear() {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::DELROW))
      return false;
   for (int row = GetRows(); row >= 0; row--) {
      DeleteRow(row);
   }
   return true;
}

/**
 * Obtiene el nombre de la columna
 * @param Column
 * @return
 */
std::string VectorEditorDriver::GetColumnName(int Column) const {
   if (Column != geometryColumnIndex_)
      return vectorEditor_.GetFieldName(Column - 1);
   else
      return GeometryColumnName;
}

/**
 * Obtiene el tipo de la columna
 * @param Column
 * @return
 */
std::string VectorEditorDriver::GetColumnType(int Column) const {
   if (Column != geometryColumnIndex_)
      return vectorEditor_.GetFieldType(Column - 1);
   else
      return DataInfo<unsigned char>::Name;
}

/**
 * Obtiene los atributos de la columna.
 *
 * @param[in] Column Columna de la que se quieren obtener los atributos.
 * @return Flags de la columna.
 */
int VectorEditorDriver::GetColumnFlags(int Column) const {
   if (Column != geometryColumnIndex_)
      return DriverInterface::RegularFlag;
   else
      return DriverInterface::VirtualFlag;
}

/**
 * Getea la lista de permisos configurada para la tabla
 * @return
 */
const PermissionList* VectorEditorDriver::GetCapabilities() const {
   return permissions_;
}

/**
 * Getea el permiso utilizado en GetCapabilities
 * @return
 */
int VectorEditorDriver::GetPermission() const {
   return permissionKey_;
}

/**
 * Getea el Id del Datasource asociado
 * @return
 */
SuriObject::UuidType VectorEditorDriver::GetAssociatedDatasourceId() const {
   return id_;
}

/**
 * Determina si una celda es valida
 * @param[in] Column indice de la columna a ser eliminada
 * @return devuelve true si la operacion tuvo exito, sino devuelve false
 */
bool VectorEditorDriver::IsValidCell(int Column, int Row) const {
   return (!(Column >= GetColumns() || Row >= GetRows() || Column < 0 || Row < 0));
}

/**
 * Determina si la columna es entera
 * @param[in] Column indice de la columna
 * @return devuelve true si la columna es entera
 */
bool VectorEditorDriver::IsIntColumn(int Column) const {
   return Type2Id(GetColumnType(Column)) == DataInfo<int>::Id;
}

/**
 * Determina si la columna es de cadena de caracteres
 * @param[in] Column indice de la columna
 * @return devuelve true si la columna es varchar
 */
bool VectorEditorDriver::IsVarcharColumn(int Column) const {
   // El tipo unsigned char se usa para representar del string porque DataInfo no lo contempla
   if (Column != geometryColumnIndex_)
      return Type2Id(GetColumnType(Column)) == DataInfo<unsigned char>::Id;
   else
      return true;
}

/**
 * Determina si la columna es de punto flotante
 * @param[in] Column indice de la columna
 * @return devuelve true si la columna es flotante
 */
bool VectorEditorDriver::IsFloatColumn(int Column) const {
   return Type2Id(GetColumnType(Column)) == DataInfo<float>::Id;
}

/** Determina si la columna es de punto flotante (double) */
bool VectorEditorDriver::IsDoubleColumn(int Column) const {
   return Type2Id(GetColumnType(Column)) == DataInfo<double>::Id;
}

/**
 * Determina si se soporta el tipo
 * @param ColumnType Tipo de dato
 * @return devuelve true si se soporta el tipo
 */
bool VectorEditorDriver::IsSupportedType(const std::string& ColumnType) {
   bool idint = Type2Id(ColumnType) == DataInfo<int>::Id;
   bool idstring = Type2Id(ColumnType) == DataInfo<unsigned char>::Id;
   bool idfloat = Type2Id(ColumnType) == DataInfo<double>::Id;
   return idint || idstring || idfloat;
}

/** Metodo que se llama cuando se agrega una fila. Se permite
 *  solamente una edicion de fila a la vez para dar soporte al registro fantasma
 *  @return true en caso de poder agregar una fila
 *  @return false en caso contrario
 */
bool VectorEditorDriver::CanAppendRow() {
   return permissions_->IsOperationPermitted(permissionKey_, PermissionList::APPROW);
}

/** Obtiene el wkt asociado a un feature con el id pasado por parametro*/
std::string VectorEditorDriver::FetchGeometryWkt(long FeatureId) const {
   return vectorEditor_.GetGeometryWkt(FeatureId);
}

/** Devuelve el id asociado a la columna, -1 en caso contrario
 *  @param[in] Row fila a consultar
 *  @return id de la fila
 *  @return -1 en caso de que la fila no se encuentre en la tabla
 *  */
long VectorEditorDriver::GetRowId(int Row) const {
   return IsValidCell(0, Row) ? GetFeatureIdAt(Row) : -1;
}

/** obtiene la fila asociada al id que se pasa por parametro
*  @param[in] Id id de la fila a consultar
*  @return id de la fila
*  @return -1 en caso de que la fila no se encuentre en la tabla*/
int VectorEditorDriver::GetRowById(long RowId) const {
   std::vector<short>::const_iterator it = rows_.begin();
   int row = -1;
   int k = 0;
   for (; it != rows_.end(); it++, ++k) {
      if (*it == RowId) {
         row = k;
         break;
      }
   }
   return row;
}

/** Obtiene un nuevo id */
long VectorEditorDriver::GetNewRowId() {
   return 0;
}

/** Obtiene los ids de las filas que contiene la tabla **/
std::vector<long> VectorEditorDriver::GetRowIds() const {
   std::vector<long> ids;
   std::vector<short>::const_iterator it = rows_.begin();
   for (; it != rows_.end(); it++) {
      ids.push_back(*it);
   }
   return ids;
}
} /** namespace suri */
