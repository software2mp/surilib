/*! \file DataTypes.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Include standard
#include <string>
#include <limits>


#include "suri/DataTypes.h"

/** namespace suri */
namespace suri {
const char* DataInfo<void>::Name = "void"; /*! Nombre de tipo de dato void */
const char* DataInfo<unsigned char>::Name = "uchar"; /*! Nombre de tipo de dato uchar */
const char* DataInfo<char>::Name = "char"; /*! Nombre de tipo de dato uchar */
const char* DataInfo<short>::Name = "short"; /*! Nombre de tipo de dato short */
const char* DataInfo<unsigned short>::Name = "ushort"; /*! Nombre de tipo de dato ushort */
const char* DataInfo<int>::Name = "int"; /*! Nombre de tipo de dato int */
const char* DataInfo<unsigned int>::Name = "uint"; /*! Nombre de tipo de dato uint */
const char* DataInfo<float>::Name = "float"; /*! Nombre de tipo de dato float */
const char* DataInfo<double>::Name = "double"; /*! Nombre de tipo de dato double */
/** puntero a funcion que devuelve int. Utilizado para devolver el tamano de */
/** un tipo de dato. */
typedef int (*sizeofdata)();

/** Template que devuelve el tamanio de un tipo de dato */
template<typename T>
int SizeOf() {
   return DataInfo<T>::Size;
}
/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(sizeofdata, SizeOf);

/** puntero a funcion que devuelve int. Utilizado para devolver el tamano de */
/** un tipo de dato. */
typedef long (*idofdata)();

/** funcion que convierte un nombre de tipo a un entero para usarlo en switch */
template<typename T>
long Type2Id() {
   return DataInfo<T>::Id;
}

/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(idofdata, Type2Id);

/**
 * Metodo que devuelve el tamanio de un tipo de dato
 * @param[in] DataName nombre del tipo de dato.
 */
int SizeOf(const std::string &DataName) {
   if (DataName == DataInfo<void>::Name)
      return DataInfo<void>::Size;

   return SizeOfTypeMap[DataName]();
}

/**
 * Metodo que devuelve el identificador entero de un tipo de dato
 * @param[in] DataName Nombre del tipo de dato como lo da DataInfo::Name
 * @return Valor identificador de un tipo
 */
long Type2Id(const std::string &DataName) {
   if (DataName == DataInfo<void>::Name)
      return DataInfo<void>::Id;
   return Type2IdTypeMap[DataName]();
}

void GetIntigersLimits(long &min, long &max, const std::string &DataType) {
   switch (Type2Id(DataType)) {
      case DataInfo<unsigned char>::Id: {
         min = std::numeric_limits<unsigned char>::min();
         max = std::numeric_limits<unsigned char>::max();
         break;
      }
      case DataInfo<char>::Id: {
               min = std::numeric_limits<char>::min();
               max = std::numeric_limits<char>::max();
               break;
      }
      case DataInfo<unsigned short>::Id: {
         min = std::numeric_limits<unsigned short>::min();
         max = std::numeric_limits<unsigned short>::max();
         break;
      }
      case DataInfo<short>::Id: {
         min = std::numeric_limits<short>::min();
         max = std::numeric_limits<short>::max();
         break;
      }
      case DataInfo<unsigned int>::Id: {
         min = 0;
         max = std::numeric_limits<int>::max();
         break;
      }
      case DataInfo<int>::Id: {
         min = std::numeric_limits<int>::min();
         max = std::numeric_limits<int>::max();
         break;
      }
   }
}

/**
 * Obtiene el nombre del tipo de dato completo.
 * (ej: uchar -> unsigned char)
 */
std::string GetFullDataTypeName(const std::string& DataType) {
   std::string retstr = DataType;

   if (DataType.compare(DataInfo<unsigned char>::Name) == 0) {
      retstr = "unsigned char";
   } else if (DataType.compare(DataInfo<unsigned short>::Name) == 0) {
      retstr = "unsigned short";
   } else if (DataType.compare(DataInfo<unsigned int>::Name) == 0) {
      retstr = "unsigned int";
   }

   return retstr;
}

/**
 * Obtiene el nombre del tipo de dato basico.
 * (ej: unsigned char -> uchar)
 */
std::string GetShortDataTypeName(const std::string& DataType) {
   std::string retstr = DataType;

   if (DataType.compare("unsigned char") == 0) {
      retstr = DataInfo<unsigned char>::Name;
   } else if (DataType.compare("unsigned short") == 0) {
      retstr = DataInfo<unsigned short>::Name;
   } else if (DataType.compare("unsigned int") == 0) {
      retstr = DataInfo<unsigned int>::Name;
   }

   return retstr;
}

/** Valida si se trata de un tipo entero */
bool IsIntegerType(const std::string &DataName) {
   bool result = false;
   switch (Type2Id(DataName)) {
      case DataInfo<unsigned char>::Id:
      case DataInfo<char>::Id:
      case DataInfo<unsigned short>::Id:
      case DataInfo<short>::Id:
      case DataInfo<unsigned int>::Id:
      case DataInfo<int>::Id:
         result = true;
         break;
      default:
         break;
   }
   return result;
}

/** Valida si se trata de un tipo string */
bool IsCharType(const std::string& Dataname) {
   bool result = false;
   switch (Type2Id(Dataname)) {
      case DataInfo<unsigned char>::Id :
      case DataInfo<char>::Id :
         result = true;
         break;
      default:
         break;
   }
   return result;
}

/**
 * Valida si se trata de un tipo de dato de punto flotante.
 */
bool IsFloatingPoint(const std::string& Dataname) {
   bool result = false;
   switch (Type2Id(Dataname)) {
      case DataInfo<float>::Id:
      case DataInfo<double>::Id:
         result = true;
         break;
      default:
         break;
   }
   return result;
}

/**
 * Verifica si el tipo de dato dado admite el valor NaN.
 */
bool HasNaN(const std::string& Datatype) {
   bool result = false;
   switch (Type2Id(Datatype)) {
      case DataInfo<unsigned char>::Id:
         result = std::numeric_limits<unsigned char>::has_quiet_NaN;
         break;
      case DataInfo<char>::Id:
         result = std::numeric_limits<char>::has_quiet_NaN;
         break;
      case DataInfo<unsigned short>::Id:
         result = std::numeric_limits<unsigned short>::has_quiet_NaN;
         break;
      case DataInfo<short>::Id:
         result = std::numeric_limits<short>::has_quiet_NaN;
         break;
      case DataInfo<unsigned int>::Id:
         result = std::numeric_limits<unsigned int>::has_quiet_NaN;
         break;
      case DataInfo<int>::Id:
         result = std::numeric_limits<int>::has_quiet_NaN;
         break;
      case DataInfo<float>::Id:
         result = std::numeric_limits<float>::has_quiet_NaN;
         break;
      case DataInfo<double>::Id:
         result = std::numeric_limits<double>::has_quiet_NaN;
         break;
      default:
         break;
   }
   return result;
}

/**
 * Verifica si el tipo de dato dado admite el valor INF.
 */
bool HasINF(const std::string& Datatype) {
   bool result = false;
   switch (Type2Id(Datatype)) {
      case DataInfo<unsigned char>::Id:
         result = std::numeric_limits<unsigned char>::has_infinity;
         break;
      case DataInfo<char>::Id:
         result = std::numeric_limits<char>::has_infinity;
         break;
      case DataInfo<unsigned short>::Id:
         result = std::numeric_limits<unsigned short>::has_infinity;
         break;
      case DataInfo<short>::Id:
         result = std::numeric_limits<short>::has_infinity;
         break;
      case DataInfo<unsigned int>::Id:
         result = std::numeric_limits<unsigned int>::has_infinity;
         break;
      case DataInfo<int>::Id:
         result = std::numeric_limits<int>::has_infinity;
         break;
      case DataInfo<float>::Id:
         result = std::numeric_limits<float>::has_infinity;
         break;
      case DataInfo<double>::Id:
         result = std::numeric_limits<double>::has_infinity;
         break;
      default:
         break;
   }
   return result;
}

/**
 * Obtiene los valores minimo y maximo para el tipo de dato dado.
 */
void GetDatatypeLimits(const std::string& Datatype, double& Min, double& Max) {
   switch (Type2Id(Datatype)) {
      case DataInfo<unsigned char>::Id:
         Min = std::numeric_limits<unsigned char>::min();
         Max = std::numeric_limits<unsigned char>::max();
         break;
      case DataInfo<char>::Id:
         Min = std::numeric_limits<char>::min();
         Max = std::numeric_limits<char>::max();
         break;
      case DataInfo<unsigned short>::Id:
         Min = std::numeric_limits<unsigned short>::min();
         Max = std::numeric_limits<unsigned short>::max();
         break;
      case DataInfo<short>::Id:
         Min = std::numeric_limits<short>::min();
         Max = std::numeric_limits<short>::max();
         break;
      case DataInfo<unsigned int>::Id:
         Min = std::numeric_limits<unsigned int>::min();
         Max = std::numeric_limits<unsigned int>::max();
         break;
      case DataInfo<int>::Id:
         Min = std::numeric_limits<int>::min();
         Max = std::numeric_limits<int>::max();
         break;
      case DataInfo<float>::Id:
         Min = -std::numeric_limits<float>::max();
         Max = std::numeric_limits<float>::max();
         break;
      case DataInfo<double>::Id:
         Min = -std::numeric_limits<double>::max();
         Max = std::numeric_limits<double>::max();
         break;
      default:
         break;
   }
}

}  // namespace suri

