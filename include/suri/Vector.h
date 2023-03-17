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

#ifndef VECTOR_H_
#define VECTOR_H_

#include <string>
#include <set>

#include "Option.h"

#include "ogr_core.h"

class OGRDataSource;
class OGRLayer;
class OGRGeometry;

namespace suri {

class Subset;

/** Representa una fuente de datos vectorial */
/**
 *  Esta clase maneja los archivos de tipo vectorial
 *  Utiliza las clases del OGR/GDAL
 *  \todo: Manejo de multiples capas
 *  \todo: Soporte de estilos provenientes de archivo
 * Esta clase es la equivalente a Image pero para vectores. La interfaz
 * es similar si bien no heredan de la misma clase.
 * \note especializa Option para guardar informacion extra de vector
 *       como Format y Capabilities
 */
class Vector : public Option {
   /** Ctor. de Copia. */
   Vector(const Vector &Vector);

public:
   /** Tipos de vectores */
   typedef enum {
      Undefined = 0, Point = 1, Line = 2, Polygon = 3, GeometryCollection = 4
   } VectorType;
   
   /** Metodos de acceso */
   typedef enum {
      ReadOnly, WriteOnly, ReadWrite
   } VectorAccessType;

   /** Propiedades basicas */
   typedef enum {
      Format, FormatDescription, Description, StyleTable, Capabilities, Filename
   } VectorOptionIdType;
   
   /** Tipo para las propiedades basicas */
   typedef std::map<VectorOptionIdType, std::string> OptionsIdMapType;
   
   /** Constructor */
   Vector();
   /** Destructor */
   virtual ~Vector();

   /** Funcion que permite registrar una nueva clase vector */
   static void RegisterVector(std::string ClassId, Vector* (*CreateFunction)());
   
   /** Abre un archivo vectorial */
   static Vector* Open(const std::string &Filename, VectorAccessType VectorAccess = ReadOnly,
                       int Layers = 0, VectorType Type = Undefined);
   
   /** Cierra el archivo vectorial */
   static void Close(Vector*& pVector);
   
   /** Inicializa las clases */
   static bool Init();

   /** Crea una instancia de la clase Vector */
   static Vector* Create() {
      return new Vector;
   }

   /** Retorna el nombre de la clase */
   static std::string GetClassId() {
      return "OgrVector";
   }
   
   /** Salva el vector en un formato dado con las opciones dadas */
   virtual bool Save(const std::string &Filename, const std::string &Format,
                     const OptionsMapType &WriteOptions = OptionsMapType());
   
   /** Copia los datos al vector */
   virtual bool Copy(Vector *pTarget);
   
   /** Copia los datos al vector aplicando un filtro espacial dado por el subset que se pasa
    *  por parametro.Toda geometria que se interseca con el subset se copia al vector destino 
      (sin importar si el extent entero de la geometria entra en el extente a exportar). */
   virtual bool Copy(Vector *pTarget, const Subset& SpatialFilter,
                   const std::string& SpatialReference, int LayerIndex = 0);

   virtual OGRLayer* CopyLayer(int LayerIndex, const std::string& name);

   /** Acceso al DataSource de OGR */
   virtual OGRDataSource *GetVectorDataSource() const;
   
   /** Obtiene la cuenta de capas */
   virtual int GetLayerCount() const;
   
   /** Acceso a la capa por indice */
   virtual OGRLayer *GetLayer(int LayerIndex) const;
   
   /** Devuelve el indice a la capa por nombre (-1 si falla) */
   virtual int GetLayerIndex(const std::string &LayerName) const;
   
   /** Indica el tipo de capa */
   virtual VectorType GetLayerType(const int LayerIndex) const;
   
   /** Indica el Sistema de Referencia de la capa */
   virtual std::string GetLayerSR(const int LayerIndex) const;
   
   /** Crea una capa mediante comando SQL */
   virtual OGRLayer *CreateSqlLayer(const std::string &SQLStatement,
                                    OGRGeometry *pFilterGeometry = NULL,
                                    const std::string &SQLDialect = "");
   
   /** Destruye una capa mediante comando SQL */
   virtual bool DestroySqlLayer(OGRLayer * &pSQLLayer);
   
   /** Devuelve el tipo de Vector en String. */
   static std::string GetVectorTypeAsString(const int Type);
   
   /** Acceso a la capa por nombre */
   virtual OGRLayer *GetLayer(const std::string &LayerName) const;
   
   /** Crea capa con nombre, ref espacial y tipo de vector indicados */
   virtual OGRLayer *CreateLayer(const std::string &LayerName,
                                 const std::string &SpatialReference,
                                 const Vector::VectorType VecType);
   
   /** Elimina la capa en la posicion indicada */
   virtual bool DeleteLayer(int LayerIndex);
   
   /** Retorna el indice de la capa con el nombre pasado por parametro */
   virtual int GetLayerIndex(OGRLayer* pLayer);
   
   // Para Poder overloadear los metodos Get/SetOption
   using Option::SetOption;
   using Option::GetOption;
   
   /** Agrega o cambia opciones */
   void SetOption(const VectorOptionIdType Id, const std::string &Value);
   
   /** Retorna el valor de una opcion */
   std::string GetOption(const VectorOptionIdType Id) const;
   /**
    * Cuenta la cantidad de features en el layer
    * Si el filtro es vacio, cuenta todos los features.
    **/
   int GetFeatureCount(int LayerIndex, const std::string& filter = "") const;
   // ------------------------------------------------------------------------------
protected:
   /** Inspecciona el archivo, para saber si es del formato que maneja */
   virtual bool Inspect(const std::string &Filename, VectorAccessType VectorAccess = ReadOnly,
                        int Layers = 0, VectorType Type = Undefined);
   
   /** Carga el vector */
   virtual Vector* Load(const std::string &Filename, VectorAccessType VectorAccess = ReadOnly,
                        int Layers = 0, VectorType Type = Undefined);
   
   /** Asigna el datasource asociado */
   virtual void SetDataSource(OGRDataSource* pVectorDataSource);
   
   /** Inicializacion en las clases derivadas, se llama al registrarlas */
   virtual void InitializeClass();

   /**
    * Trata de obtener el tipo de geometria de la capa basandose
    * en las caracteristica disponibles.
    */
   OGRwkbGeometryType GuessGeomType(OGRLayer* pOgrLayer) const;

   OGRDataSource *pVectorDataSource_; /*! OGR Data Source */
   
private:
   std::set<OGRLayer *> createdSqlLayers_; /*! Capas SQL creadas */
   static OptionsIdMapType optionsIds_; /*! Identificador de Ids - Opciones */
   
   /**
    * Convierte el identificador de tipo de geometria de ORG al de SuriLib.
    */
   static VectorType TranslateType(OGRwkbGeometryType OrgType);
};

}

#endif /*VECTOR_H_*/
