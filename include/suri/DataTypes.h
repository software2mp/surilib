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

#ifndef DATATYPES_H_
#define DATATYPES_H_

// Include standard
#include <string>
#include <map>
#include <limits>

/** Namespace que contiene las clases suri */
/**
 *  Namespace que contiene las clases de imagenes y procesamiento SURI.
 */
namespace suri {
/** Template generico para informacion de tipo */
template<typename T>
struct DataInfo {
};

/** Template especializado para informacion de tipo */
template<>
struct DataInfo<void> {
   static const char* Name; /*! Nombre del tipo */
   static const int Size = 0; /*! Tamanio del tipo */
   static const long Id = 0x00000000; /*! Identificador del tipo */
};

/** Template especializado para informacion de tipo uchar */
template<>
struct DataInfo<unsigned char> {
   static const char* Name; /*! Nombre del tipo */
   static const int Size = sizeof(unsigned char); /*! Tamanio del tipo */
   static const long Id = 0x00000001; /*! Identificador del tipo */
};

/** Template especializado para informacion de tipo char */
template<>
struct DataInfo<char> {
   static const char* Name; /*! Nombre del tipo */
   static const int Size = sizeof(char); /*! Tamanio del tipo */
   static const long Id = DataInfo<unsigned char>::Id << 1; /*! Identificador del tipo */
};

/** Template especializado para informacion de tipo short */
template<>
struct DataInfo<short> {
   static const char* Name; /*! Nombre del tipo */
   static const int Size = sizeof(short); /*! Tamanio del tipo */
   static const long Id = DataInfo<char>::Id << 1; /*! Identificador del tipo */
};

/** Template especializado para informacion de tipo ushort */
template<>
struct DataInfo<unsigned short> {
   static const char* Name; /*! Nombre del tipo */
   static const int Size = sizeof(unsigned short); /*! Tamanio del tipo */
   static const long Id = DataInfo<short>::Id << 1; /*! Identificador del tipo */
};

/** Template especializado para informacion de tipo int */
template<>
struct DataInfo<int> {
   static const char* Name; /*! Nombre del tipo */
   static const int Size = sizeof(int); /*! Tamanio del tipo */
   static const long Id = DataInfo<unsigned short>::Id << 1; /*! Identificador del tipo */
};

/** Template especializado para informacion de tipo uint */
template<>
struct DataInfo<unsigned int> {
   static const char* Name; /*! Nombre del tipo */
   static const int Size = sizeof(unsigned int); /*! Tamanio del tipo */
   static const long Id = DataInfo<int>::Id << 1; /*! Identificador del tipo */
};

/** Template especializado para informacion de tipo float */
template<>
struct DataInfo<float> {
   static const char* Name; /*! Nombre del tipo */
   static const int Size = sizeof(float); /*! Tamanio del tipo */
   static const long Id = DataInfo<unsigned int>::Id << 1; /*! Identificador del tipo */
};

/** Template especializado para informacion de tipo double */
template<>
struct DataInfo<double> {
   static const char* Name; /*! Nombre del tipo */
   static const int Size = sizeof(double); /*! Tamanio del tipo */
   static const long Id = DataInfo<float>::Id << 1; /*! Identificador del tipo */
};

/** Devuelve el tamanio de un tipo de dato */
int SizeOf(const std::string &DataName);

/** funcion que convierte un nombre de tipo a un entero para usarlo en switch */
long Type2Id(const std::string &DataName);

/** Valida si se trata de un tipo entero */
bool IsIntegerType(const std::string &DataName);

/** Valida si se trata de un tipo string */
bool IsCharType(const std::string& Dataname);

/**
 * Valida si se trata de un tipo de dato de punto flotante.
 */
bool IsFloatingPoint(const std::string& Dataname);

/** Obtiene los limites de tipos nativo enteros **/
void GetIntigersLimits(long &min, long &max, const std::string &DataType);

/**
 * Obtiene el nombre del tipo de dato completo.
 * (ej: uchar -> unsigned char)
 */
std::string GetFullDataTypeName(const std::string& DataType);

/**
 * Obtiene el nombre del tipo de dato basico.
 * (ej: unsigned char -> uchar)
 */
std::string GetShortDataTypeName(const std::string& DataType);

/**
 * Verifica si el tipo de dato dado admite el valor NaN.
 */
bool HasNaN(const std::string& Datatype);

/**
 * Verifica si el tipo de dato dado admite el valor INF.
 */
bool HasINF(const std::string& Datatype);

/**
 * Obtiene los valores minimo y maximo para el tipo de dato dado.
 */
void GetDatatypeLimits(const std::string& Datatype, double& Min, double& Max);

/** Calcula el tamano de un array cte */
#define ARRAY_SIZE(Array) (sizeof(Array)/sizeof(Array[0]))

/** Declara mapa (<MappedInstance>TypeMap) de tipo de dato a funcion correspondiente */
#define INITIALIZE_DATATYPE_MAP(MappedType, MappedInstance)                    \
   std::pair<std::string, MappedType> MappedInstance ## _dummy[] =   {        \
      std::pair<std::string, MappedType>                                      \
         (DataInfo<unsigned char>::Name, MappedInstance<unsigned char>),    \
      std::pair<std::string, MappedType>                                      \
         (DataInfo<char>::Name, MappedInstance<char>),    \
      std::pair<std::string, MappedType>                                      \
         (DataInfo<short>::Name, MappedInstance<short>),                    \
      std::pair<std::string, MappedType>                                      \
         (DataInfo<unsigned short>::Name, MappedInstance<unsigned short>),  \
      std::pair<std::string, MappedType>                                      \
         (DataInfo<int>::Name, MappedInstance<int>),                        \
      std::pair<std::string, MappedType>                                      \
         (DataInfo<unsigned int>::Name, MappedInstance<unsigned int>),      \
      std::pair<std::string, MappedType>                                      \
         (DataInfo<float>::Name, MappedInstance<float>),                    \
      std::pair<std::string, MappedType>                                      \
         (DataInfo<double>::Name, MappedInstance<double>) };                \
   std::map<std::string, MappedType>                                          \
   MappedInstance ## TypeMap(MappedInstance ## _dummy,               \
                             MappedInstance ## _dummy+ARRAY_SIZE(MappedInstance ## _dummy) )

/** Tipos de datos soportados */
#define ALLTYPES(TYPED_MACRO, ...)                                             \
   TYPED_MACRO(unsigned char, ## __VA_ARGS__);                                  \
   TYPED_MACRO(char, ## __VA_ARGS__);                                  \
   TYPED_MACRO(unsigned short, ## __VA_ARGS__);                                 \
   TYPED_MACRO(short, ## __VA_ARGS__);                                          \
   TYPED_MACRO(unsigned int, ## __VA_ARGS__);                                   \
   TYPED_MACRO(int, ## __VA_ARGS__);                                            \
   TYPED_MACRO(float, ## __VA_ARGS__);                                          \
   TYPED_MACRO(double, ## __VA_ARGS__)

/** inserta funcion en mapa. */
#define MAPPED_INSERT(Type, FunctionName, T2)                                   \
   singleMappedCallbacks_.insert(                                             \
      std::make_pair(suri::DataInfo<Type>::Name, FunctionName<T2, Type>) )

/** inserta funcion en mapa doble. */
#define DUAL_MAPPED_INSERT(Type, FunctionName)                                 \
   ALLTYPES(MAPPED_INSERT, FunctionName, Type);                                 \
   dualMappedCallbacks_->insert(                                              \
      std::make_pair(suri::DataInfo<Type>::Name, singleMappedCallbacks_) );  \
   singleMappedCallbacks_.clear()

/** Genera un mapa doble (matriz) indexado por tipos a la funcion */
/**
 * \post Se crea una instancia de mapa doble llamada MappedFunctionTypeMatrix
 *       que se puede usar para acceder a los punteros de las realizaciones
 *       de la funcion MappedFunction<T1,T2> en forma dinamica.
 * \todo Implementar CreateMap usando ALLTYPES(DUAL_MAPPED_INSERT,MappedFunction)
 */
#define INITIALIZE_DUAL_DATATYPE_MAP(MappedType, MappedFunction)               \
   template <typename T>                                                      \
   class dualmapped_ ## MappedFunction {                                      \
public:                                                                    \
   typedef typename std::map<std::string, T> SingleMapType;                \
   typedef typename std::map<std::string, SingleMapType> DualMapType;      \
   dualmapped_ ## MappedFunction() {CreateMap(); }                           \
   ~dualmapped_ ## MappedFunction() {}                                      \
   static void CreateMap() {                                               \
      SingleMapType singleMappedCallbacks_;                                \
      dualMappedCallbacks_ = new DualMapType;                              \
      DUAL_MAPPED_INSERT(unsigned char, MappedFunction);                    \
      DUAL_MAPPED_INSERT(char, MappedFunction);                         \
      DUAL_MAPPED_INSERT(unsigned short, MappedFunction);                   \
      DUAL_MAPPED_INSERT(short, MappedFunction);                            \
      DUAL_MAPPED_INSERT(unsigned int, MappedFunction);                     \
      DUAL_MAPPED_INSERT(int, MappedFunction);                              \
      DUAL_MAPPED_INSERT(float, MappedFunction);                            \
      DUAL_MAPPED_INSERT(double, MappedFunction);   }                       \
   static DualMapType *dualMappedCallbacks_; };                            \
                                                                              \
   template <typename T>                                                      \
   typename dualmapped_ ## MappedFunction<T>::DualMapType *                   \
   dualmapped_ ## MappedFunction<T>::dualMappedCallbacks_ = NULL;          \
                                                                              \
   dualmapped_ ## MappedFunction<MappedType> mapped ## MappedFunction;        \
                                                                              \
   dualmapped_ ## MappedFunction<MappedType>::DualMapType                     \
   MappedFunction ## TypeMatrix( (*mapped ## MappedFunction.dualMappedCallbacks_) )
}  // namespace suri

#endif /*DATATYPES_H_*/

