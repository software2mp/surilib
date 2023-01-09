/*! \file VectorEditor.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTOREDITOR_H_
#define VECTOREDITOR_H_

// Includes Standar
#include <vector>

// Includes Suri
#include "suri/Coordinates.h"
#include "suri/Vector.h"
#include "suri/DataTypes.h"

// Includes wx
#include "wx/wx.h"

// OGR
#include "ogr_feature.h"

/** namespace suri */
namespace suri {
class OgrGeometryEditor;

/** Permite editar vectores */
/**
 * Permite editar vectores. Antes de poder agregar puntos primero se debe
 * abrir un vector, luego una capa y finalmente un feature.
 * Las capas pueden contener campos que se crean con CreateField y se editan con
 * los Get/SetField. Para poder editar un campo debe estar abierto un feature.
 * Las funciones Add/Get/Delete y Count funcionan solo si el vector es de tipo
 * punto, linea o poligono. Si el vector es de otro tipo solo se podran editar
 * los campos.
 *
 * \attention Al guardar poligonos ESRI shapefile pide que los puntos se guarden
 * en sentido horario. Si los puntos que se piden guardar no respetan esto
 * se le agrega a la capa un campo clockwise y se setea a false.
 * Existe codigo deshabilitado que maneja esta situacion:
 * Cuando se abre el vector se busca este campo, si existe y vale false
 * significa que ya se habia editado el vector y cuando se guardo se invirtio
 * para respetar convension de poligonos en ESRI shapefile. Por lo tanto los
 * puntos se invierten para poder segir agregando puntos en mismo orden.
 */
class VectorEditor {
public:
   /** Ctor. */
   VectorEditor();
   /** Dtor. */
   virtual ~VectorEditor();

   // Manejo de vectores

   /** Abre un vector, si no existe lo crea */
   virtual bool OpenVector(const std::string &Filename);
   /** Cierra el vector abierto */
   virtual void CloseVector();
   /** Indica si se termino de editar el vector. */
   virtual bool IsVectorOpen() {
      return (pReadOnlyVector_ != NULL && pReadWriteVector_ != NULL);
   }
   /** Retorna el vector de solo lectura */
   virtual Vector* GetReadOnlyVector() {
      return pReadOnlyVector_;
   }
   /** Abre capa en posicion LayerIndex, si no existe la crea. */
   virtual bool OpenLayer(int LayerIndex);
   /** Abre capa de nombre LayerName, si no existe la crea. */
   virtual int OpenLayer(const std::string &LayerName,
                         const std::string &SpatialReference = "",
                         Vector::VectorType VecType = Vector::Undefined);
   /** Crea capa de nombre LayerName. */
   virtual OGRLayer* CreateLayer(const std::string &LayerName,
                                 const std::string &SpatialReference = "",
                                 Vector::VectorType Type = Vector::Undefined);
   /** Determina si se pueden crear capas */
   virtual bool CanCreateLayer() const;
   /** Elimina la capa en la posicion indicada */
   virtual void DeleteLayer(int LayerIndex);
   /** Elimina la capa de nombre indicado */
   virtual void DeleteLayer(const std::string &LayerName);
   /** Determina si se pueden eliminar capas */
   virtual bool CanDeleteLayer() const;
   /** Actualiza los cambios al disco */
   virtual void SyncToDisk();
   /** Crea una capa mediante comando SQL */
   virtual OGRLayer *CreateSqlLayer(const std::string &SQLStatement,
                                    OGRGeometry *pFilterGeometry = NULL,
                                    const std::string &SQLDialect = "");

   // Manejo de capas

   /** Cierra la capa abierta */
   virtual void CloseLayer();
   /** Verifica si se puede escribir en la tabla */
   virtual bool CanWrite() const;
   /** Verifica si se puede leer de la tabla */
   virtual bool CanRead() const;
   /** Indica cual es la capa abierta (-1 cuando no hay ninguna) */
   virtual int GetOpenLayer() {
      return currentLayerIndex_;
   }
   /** Devuelve el nombre de la capa abierta. si no hay capa abierta devuelve string vacio*/
   virtual std::string GetOpenLayerName() const;
   /** Crea campo de nombre y tipo indicados en capa abierta */
   virtual bool CreateField(const std::string &Name, const std::string &DataType, int Length = -1);
   /** Verifica si se pueden crear campos */
   virtual bool CanCreateField() const;
   /** Elimina un campo de la capa abierta */
   virtual bool DeleteField(const std::string &Name);
   /** Analiza si puede eliminar un campo en capa abierta */
   virtual bool CanDeleteField() const;
   /** Busca el id del feature en posicion indicada */
   virtual long GetFeatureId(int FeaturePosition);
   /** Retorna la referencia espacial de la capa */
   virtual std::string GetLayerSpatialReference() const;
   /** Abre Feature, si no se pasa parametro la crea */
   virtual long OpenFeature(long FeatureId = -1, bool LoadGeometry = true);
   /** Elimina el feature indicado, debe estar cerrado */
   virtual bool DeleteFeature(long FeatureId);
   /** Informa si se pueden eliminar features de una capa */
   virtual bool CanDeleteFeature() const;
   /** Informa si se pueden eliminar features de una capa */
   virtual bool CanCreateFeature() const;
   /** Retorna la cantidad de features abiertas */
   virtual int GetFeatureCount() const;
   /** Retorna la cantidad de campos en tabla de capa abierta */
   virtual int GetFieldCount() const;
   /** Retorna el nombre del campo en posicion indicada */
   virtual std::string GetFieldName(int Position) const;
   /** Verifica si un campo con este nombre existe y retorna la posicion */
   virtual int GetFieldPosition(const std::string& FieldName) const;
   /** Retorna el tipo de dato en el campo */
   virtual std::string GetFieldType(int Position) const;
   /** Retorna la longitud del campo para string o la precision en otro caso */
   virtual int GetFieldLength(int Position) const;

   // Manejo de features

   /** Cierra el Feature abierto */
   virtual void CloseFeature();
   /** Retorna identificador del Feature */
   virtual long GetOpenFeature();
   /** Guarda el valor en el campo indicado. El campo debe ser int */
   virtual bool SetField(const std::string &Name, int Value);
   /** Guarda el valor en el campo indicado. El campo debe ser double */
   virtual bool SetField(const std::string &Name, double Value);
   /** Guarda el valor en el campo indicado. El campo debe ser std::string */
   virtual bool SetField(const std::string &Name, const std::string &Value);
   /** Obtiene el valor del campo indicado. El campo debe ser int */
   virtual void GetField(const std::string &Name, int &Value) const;
   /** Obtiene el valor del campo indicado. El campo debe ser double */
   virtual void GetField(const std::string &Name, double &Value) const;
   /** Obtiene el valor del campo indicado. El campo debe ser std::string */
   virtual void GetField(const std::string &Name, std::string &Value) const;
   /** Agrega un punto a la geometria */
   virtual bool AddPoint(const Coordinates &Point, int Position = -1);
   /** Modifica el punto de la geometria */
   virtual bool SetPoint(const Coordinates &Point, int Position = -1);
   /** Obtiene las coordenadas de un punto de la linea */
   virtual bool GetPoint(Coordinates &Point, int Position);
   /** Elimina un punto de la linea */
   virtual bool DeletePoint(int Position);
   /** Retorna la cantidad de puntos en el feature abierto */
   virtual int GetPointCount() const;
   /** Indica al feature con el id que se pasa por parametro la geometria que le corresponde. **/
   virtual bool SetGeometryByWkt(long FeatureId, const std::string& GeomtryWkt);
   /** Obtiene el wkt de una geometria asociada a un feature **/
   virtual std::string GetGeometryWkt(long FeatureId) const;

protected:
   /** Abre el vector con filename obtenido en open */
   virtual bool OpenFilenameVector() {
      return false;
   }
   /** Cierra el vector abierto */
   virtual bool CloseFilenameVector() {
      return false;
   }
   /** Comparo dos FeatureDefinitions */
   virtual bool Compare(OGRFeatureDefn* pFeature1, OGRFeatureDefn* pFeature2);

private:
   Vector* pCurrentVector_; /*! Instancia de vector abierto que se usa */
   Vector* pReadOnlyVector_; /*! Instancia de vector abierto de solo lectura */
   Vector* pReadWriteVector_; /*! Instancia de vector abierto para lectura */
   /* escritura */
   int currentLayerIndex_; /*! Nro de capa abierta (-1 ninguna) */
   long currentFeatureId_; /*! ID de feature abierto (-1 ninguno) */
   OGRFeature* pCurrentFeature_; /*! Guarda el feature abierto */
   OGRFeatureDefn* pCurrentFeatureDefn_; /*! Guarda la definicion del feature */
   /* cuando se creo. */
   OGRLayer* pCurrentLayer_; /*! Guarda un puntero a la capa abierta */
   OgrGeometryEditor* pGeometryEditor_; /*! Geometry editor de feature. Solo */
   /* existe si hay feature abierto */
   std::string fileName_; /*! Nombre del archivo abierto */
   bool dirtyFeatureIds_; /*! Determina si hay que volver a cargar featuresIds_ */
   std::vector<int> featuresIds_; /*! Vector que asocia una posicion con su feature id. */
   /* se agrega un flag por cuestiones de performance en windows */
   bool dirty_; /*! Flag para determinar si un feature fue o no modificado*/

   /** Ejecuta la operacion REPACK en el vector */
   void SanitizeVector();
};
}

#endif /*VECTOREDITOR_H_*/
