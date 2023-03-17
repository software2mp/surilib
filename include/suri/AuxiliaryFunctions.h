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

#ifndef AUXILIARYFUNCTIONS_H_
#define AUXILIARYFUNCTIONS_H_

// Includes standard
#include <cmath>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <limits>
#include <string>
#include <string.h>

// Defines
/** Epsilon relativo al numero que se compara */
#define EPSILON_REL 0.000001
/** Epsilon absoluto */
#define EPSILON_ABS 0.000000001
/** Precision de operacion join */
#define JOIN_DOUBLE_PRECISION 8
/** Compara dos float con epsilon relativo y absolutos indicados */
#define FLOAT_COMPARE_WITH_PRECISION(Number1, Number2, RelativeEpsilon,       \
                                     AbsoluteEpsilon)  \
   (fabs(Number1-Number2) <= AbsoluteEpsilon ||             \
    fabs(Number1-Number2) <= RelativeEpsilon*fabs(Number1) || \
    fabs(Number1-Number2) <= RelativeEpsilon*fabs(Number2) )
/** Compara dos float con epsilon relativo y absoluto defaults */
#define FLOAT_COMPARE(Number1, Number2) FLOAT_COMPARE_WITH_PRECISION(Number1, \
                                                                     Number2, EPSILON_REL, \
                                                                     EPSILON_ABS)

/** Macro para determinar si un numero es NaN */
#define SURI_ISNAN(Number) ( (Number) != (Number) )
/** Macro para castear un float a entero con redondeo */
#define SURI_ROUND(Type, Number) (Type)( (Number)+0.5)
/** Macro para castear un tipo a otro truncando */
#define SURI_TRUNC(Type, Number) (Type)(Number)
/** Macro para techo de un float */
#define SURI_CEIL(Number) ceil(Number)
/** Define el string de formato para size_t segun sea x86 o x84_64 */
#ifdef __x86_64__
/** Define el string de formato para size_t para x86_64 */
#  define PERCENT_U "%zu"
#else   // __x86_64__
/** Define el string de formato para size_t para x86 */
#  define PERCENT_U "%u"
#endif   // __x86_64__
/** Separador de valores de claves en StringToMap */
#define VALUE_SEPARATOR "="
/** Separador de valores de tokens en StringToMap */
#define TOKEN_SEPARATOR "|"

namespace suri {
// --------------------- FUNCIONES PARA MANEJO NUMERICO -----------------------

/** 
 * Indica si el numero es finito 
 * @param[in] Number numero a analizar
 * @return bool que indica si el numero es finito
 */
template<typename T>
inline bool IsFinite(T Number) {
   return !std::numeric_limits<T>::has_infinity
            || (Number >= std::numeric_limits<T>::min() && Number <=
                                    std::numeric_limits<T>::max());
}

/** Compara dos numeros en punto flotante */
/**
 * @param[in] Number1 Primer numero a comparar
 * @param[in] Number2 Segundo numero a comparar
 * @return True si son iguales
 * \todo Verificar EPSILON_REL y EPSILON_ABS
 */
bool DoubleCompare(const double &Number1, const double &Number2);

/**
 * Compara dos strings ignorando mayusculas y minusculas
 * @param[in] String1 primer string a comparar
 * @param[in] String2 segundo string a comparar
 * @return bool que indica si son iguales
 */
bool CaseInsensitiveCompare(std::string String1, std::string String2);

/** Genera un string a partir de un numero double con la precision indicada */
/**
 * @param[in] Number Numero a convertir a string
 * @param[in] Precision Precision
 * @return String con el numero
 */
std::string DoubleToString(const double &Number, const size_t Precision = 6);

/**
 * Genera un string a partir de un numero double con la precision indicada
 * agregando 0 al final si corresponde.
 *
 * @param[in] Number Numero a convertir a string
 * @param[in] Precision Precision
 * @return String con el numero
 */
std::string DoubleToStringEx(const double &Number, const size_t Precision = 6);

/** Genera un string a partir de un numero long con la longitud indicada */
/**
 * @param[in] Number Numero a convertir a string
 * @param[in] Lenght Longitud
 * @return String con el numero
 */
std::string LongToString(const long &Number, const size_t Lenght = 0);

/**
 * Convierte un valor entero a un string.
 *
 * @param[in] ValueP Valor a convertir.
 * @return String que representa el valor dado.
 */
std::string IntToString(const int& ValueP);

/** Genera un string a partir de un numero long con la longitud indicada */
/**
 * @param[in] Number Numero a convertir a hexa
 * @param[in] Lenght Longitud
 * @return String con el numero convertido en hexa
 */
std::string LongToHexaString(const long &Number, const size_t Lenght = 0);

/**
 * Genera a partir de un string con valor hexa un numero
 * @param[in] String con el numero convertido en hexa
 * @return long con el numero asociado al hexa
 */
long HexaStringToLong(const std::string& String);

/**
 * Devuelve true si el string es un  numero
 */
bool IsNumber(const std::string&s);

/** Divide a el string usando el Token y lo retorna en un vector de strings */
std::vector<std::string> tokenizer(const std::string &String, const std::string &Token);

/** Divide a el string usando el Token y lo retorna en un mapa key=>value */
/**
 * @param[in] String que quiero parsear. En el string los elementos del mapa
 * estan serparados por Token y el par clave=>valor esta separado por igual
 * Ej con token="|": "dataset=fs93|epsg=4326"
 * @param[in] Token caracter que separa cada elemento del mapa
 */
std::map<std::string, std::string> StringToMap(const std::string &String,
                                               const std::string &Token = TOKEN_SEPARATOR,
                                               const std::string &KeyValueSeparator =
                                                     VALUE_SEPARATOR,
                                               bool ToLower = true);

/** Divide el string usando espacio y convierte los separados al tipo T */
template<class T>
std::vector<T> split(const std::string &String, const std::string& Separator = " ") {
   std::vector<T> vec;

   std::vector < std::string > splitted = tokenizer(String, Separator);
   for (size_t i = 0; i < splitted.size(); i++) {
      std::stringstream ss(splitted.at(i));
      T value;
      ss >> value;
      vec.push_back(value);
   }
   return vec;
}

/** Une los datos de tipo T en un string separado por Token */
template<class T>
std::string join(const T &Begin, const T &End, const std::string &Token = " ") {
   std::string value;
   T it = Begin;
   while (it != End) {
      std::stringstream ss;
      ss << std::setprecision(JOIN_DOUBLE_PRECISION) << *it;

      if (value.empty()) {
         value = ss.str();
      } else {
         value += Token + ss.str();
      }

      it++;
   }
   return value;
}

/** Elimina caracteres del principio y final de un string */
std::string trim(std::string const &InputString, char const* pUnwantedChars);

/** Funcion que compara dos listas y devuelve la diferencia */
std::string RemoveFromList(const std::string &InputList, const std::string &list2,
                           const std::string &token);

/** Remplaza una serie de strings por otro 
 * @param[in] Unwanted lista de strings que seran reemplazados (no desaedos)
 * @param[in] Wanted lista de strings a reemplazar (corresponden 1 a 1 con la lista de no desaeados)
 * @param[in] InputString string en el cual se reemplazaran los caracteres deciados
 * @param[out] InputString string con los caracteres reemplazados
 * @return true en caso de que el reemplazo se realice con exito
 * @return false en caso contrario
 **/
bool ReplaceCharactersFromString(const std::vector<std::string>& Unwanted,
                                 const std::vector<std::string>& Wanted,
                                 std::string& InputString);

/** Convierte un numero que esta en formato string al formato requerido */
/**
 * @param[in] NumberStr String con el numero
 * @return Numero del tipo requerido
 */
template<class T>
T StringToNumber(const std::string &NumberStr) {
   T number = 0;
   std::istringstream ssnumber(NumberStr);
   ssnumber >> std::dec >> number;
   return number;
}



/** Convierte un numero que esta en formato string en un uchar */
template<>
unsigned char StringToNumber(const std::string &NumberStr);

template<>
/** Convierte un numero que esta en formato string en un char */
char StringToNumber(const std::string &NumberStr);


/** Convierte un numero que esta en formato string al formato requerido */
/**
 * @param[in] NumberStr String con el numero
 * @return Numero del tipo requerido
 */
template<>
double StringToNumber(const std::string &NumberStr);

/**
 * Especializacion para float.
 */
template<>
float StringToNumber(const std::string &NumberStr);

template<class T>
std::string NumberToString(T Number, int Precision=6) {
   std::stringstream sstring;
   sstring << Number;
   if (sstring.str().find(".")==std::string::npos) return sstring.str();
   else{
      sstring.str(std::string());
      sstring.precision(Precision);
      sstring << std::fixed << Number;
      return sstring.str();
     }
}

/** Compara strings por la cantidad de caracteres */
bool LessThanSize(const std::string &lhs, const std::string &rhs);

/** Convierte una cadena de caracteres en minuscula */
std::string ToLower(const std::string& String);
/**
 * Convierte todos los caracteres de un std::string en mayusculo
 * @param[in] String
 * @return string con caracteres en minuscula
 */
std::string ToUpper(const std::string& String);

} /*namespace suri*/

/** Struct que posee la funcion de borrado **/
struct delete_object {
   template<typename Value>
   void operator()(Value *ptr) {
      delete ptr;
   }
};
/**
 * Funcion que elimina todos los objetos de un contenedor stl (a excepcion de los maps)
 * @param Collection contenedor sobre el cual se desea borrar los objetos
 */
template<typename T>
void delete_all(T const& Collection) {
   std::for_each(Collection.begin(), Collection.end(), delete_object());
}


/** Struct que borra posee una funcion que borra solamente el primer elemento de
 * un iterador de par
 */
struct delete_first_object {
   template<typename T>
   void operator()(const T& pX) const {
      delete pX.first;
   }
};

/**
 * Funcion auxiliar que para un mapa dado borra todos los first de cada
 * elemento del mapa
 * @param Map Mapa sobre el cual se desea borrar los elementos
 */
template<typename T>
void delete_first_all(T const& Map) {
   std::for_each(Map.begin(), Map.end(), delete_first_object());
}

struct delete_second_object {
   template<typename T>
   void operator()(const T& pX) const {
      delete pX.second;
   }
};

/**
 * Funcion auxiliar que para un mapa dado borra todos los second de cada
 * elemento del mapa
 * @param Map Mapa sobre el cual se desea borrar los elementos
 */
template<typename T>
void delete_second_all(T const& Map) {
   std::for_each(Map.begin(), Map.end(), delete_second_object());
}

/** Struct para el borrado de first y second de un iterador con par **/
struct delete_first_and_second_object {
   template<typename T>
   void operator()(const T& pX) const {
      delete pX.first;
      delete pX.second;
   }
};

/**
 * Funcion auxiliar que para un mapa dado borra todos los second de cada
 * elemento del mapa
 * @param Map Mapa sobre el cual se desea borrar los elementos
 */
template<typename T>
void delete_first_and_second_all(T const& Map) {
   std::for_each(Map.begin(), Map.end(), delete_first_and_second_object());
}

/**
 * Divide la cadena dada basandose en el token especificado.
 *
 * Tratar de usar esta funcion en lugar de split (que deberia ser deprecada)
 */
std::vector<std::string> SplitEx(const std::string& Str, const std::string& Token);

/**
 * Junta los elementos de un vector de cadenas en una sola
 * usando el separador especificado.
 */
std::string JoinEx(const std::vector<std::string>& Strs, const std::string& Token);

/**
 * Compara los valores dados.
 * @param Num1 Primer valor.
 * @param Num2 Segundo valor.
 * @return true si los valores son iguales.
 */
bool AreEqual(double Num1, double Num2); 

#endif /*AUXILIARYFUNCTIONS_H_*/

