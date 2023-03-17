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

// Include standard
#include <set>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

// Includes suri
#include "suri/AuxiliaryFunctions.h"

// defines
#define AE_EPSILON 0.000001

namespace suri {
// --------------------- FUNCIONES PARA MANEJO NUMERICO -----------------------

/** Compara dos numeros en punto flotante */
/**
 * @param[in] Number1 Primer numero a comparar
 * @param[in] Number2 Segundo numero a comparar
 * @return True si son iguales
 * \todo Verificar epsilon
 */
bool DoubleCompare(const double &Number1, const double &Number2) {
   return fabs(Number1 - Number2) <= EPSILON_ABS
         || fabs(Number1 - Number2) <= EPSILON_REL * fabs(Number1)
         || fabs(Number1 - Number2) <= EPSILON_REL * fabs(Number2);
}

/**
 * Compara dos strings ignorando mayusculas y minusculas
 * @param[in] String1 primer string a comparar
 * @param[in] String2 segundo string a comparar
 * @return bool que indica si son iguales
 */
bool CaseInsensitiveCompare(std::string String1, std::string String2) {
   std::transform(String1.begin(), String1.end(), String1.begin(), ::tolower);
   std::transform(String2.begin(), String2.end(), String2.begin(), ::tolower);
   return (String1.compare(String2) == 0);
}

/** Genera un string a partir de un numero double con la precision indicada */
/**
 * @param[in] Number Numero a convertir a string
 * @param[in] Precision Precision
 * @return String con el numero
 */
std::string DoubleToString(const double &Number, const size_t Precision) {
   std::stringstream ssnumber;
   ssnumber << std::setprecision(Precision) << std::fixed  << Number;
   std::string result=ssnumber.str();
   return  result;
}

/**
 * Genera un string a partir de un numero double con la precision indicada
 * agregando 0 al final si corresponde.
 *
 * @param[in] Number Numero a convertir a string
 * @param[in] Precision Precision
 * @return String con el numero
 */
std::string DoubleToStringEx(const double &Number, const size_t Precision) {
   std::stringstream ss;
   ss.precision(Precision);
   ss.setf(std::ios::fixed, std::ios::floatfield);
   ss << Number;
   std::string result=ss.str();
   return result;
}

/** Genera un string a partir de un numero long con la longitud indicada */
/**
 * @param[in] Number Numero a convertir a string
 * @param[in] Lenght Longitud
 * @return String con el numero
 */
std::string LongToString(const long &Number, const size_t Lenght) {
   std::stringstream ssnumber;
   ssnumber << std::setfill('0') << std::setw(Lenght) << Number;
   return ssnumber.str();
}

/**
 * Convierte un valor entero a un string.
 *
 * @param[in] ValueP Valor a convertir.
 * @return String que representa el valor dado.
 */
std::string IntToString(const int& ValueP) {
   std::stringstream ss;
   ss << ValueP;
   return ss.str();
}

/** Genera un string a partir de un numero long con la longitud indicada */
/**
 * @param[in] Number Numero a convertir a hexa
 * @param[in] Lenght Longitud
 * @return String con el numero convertido en hexa
 */
std::string LongToHexaString(const long &Number, const size_t Lenght) {
   std::stringstream ssnumber;
   ssnumber << std::setfill('0') << std::setw(Lenght) << std::setbase(16) << Number;
   return ssnumber.str();
}

/**
 * Genera a partir de un string con valor hexa un numero
 * @param[in] String con el numero convertido en hexa
 * @return long con el numero asociado al hexa
 */
long HexaStringToLong(const std::string& String) {
   std::stringstream ssnumber(String);
   ssnumber << std::setbase(16);
   long number;
   ssnumber >> number;
   return number;
}

/**
 * Divide a el string usando el Token y lo retorna en un vector de strings.
 * Si recibe un string vacio devuelve un vector sin elementos.
 * Si recibe un token vacio devuelve un vector sin elementos.
 * @param[in] String: cadena a dividir.
 * @param[in] Token: cadena de separacion.
 * @return vector con las cadenas divididas.
 */
std::vector<std::string> tokenizer(const std::string &String, const std::string &Token) {
   std::vector < std::string > vec;
   std::string::size_type lasttokenpos = 0;
   std::string::size_type tokenpos;

   if (!String.empty() && !Token.empty()) {
      do {
         tokenpos = String.find(Token, lasttokenpos);
         std::string::size_type size = tokenpos - lasttokenpos;
         vec.push_back(String.substr(lasttokenpos, size));
         // si cargo un string vacio, lo ignoro
         if (vec.back().empty()) {
            vec.pop_back();
         }
         lasttokenpos = tokenpos + Token.length();
      } while (tokenpos != std::string::npos);
   }
   return vec;
}

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
                                 std::string& InputString) {
   if (Unwanted.size() != Wanted.size() || Unwanted.empty() || Wanted.empty())
      return false;
   for (size_t i = 0; i < Unwanted.size(); ++i) {
      size_t pos = 0;
      std::string invalidstr = Unwanted[i];
      std::string newstr = Wanted[i];
      while((pos = InputString.find(invalidstr, pos)) != std::string::npos) {
        InputString.replace(pos, invalidstr.length(), newstr);
        pos += newstr.length();
      }
   }
   return true;
}

/**
 * Divide a el string usando el Token y lo retorna en un mapa key=>value
 * @param[in] String que quiero parsear. En el string los elementos del mapa
 * estan serparados por Token y el par clave=>valor esta separado por igual
 * Ej con token="|": "dataset=fs93|epsg=4326"
 * @param[in] Token caracter que separa cada elemento del mapa
 * @param[in] KeyValueSeparator caracter que separa cada par clave/valor
 * @param[in] ToLower si esta en true, convierte todos los valores a minusculas
 * @return mapa con los pares key=>value.
 */
std::map<std::string, std::string> StringToMap(const std::string &String,
                                               const std::string &Token,
                                               const std::string &KeyValueSeparator,
                                               bool ToLower) {
   std::map < std::string, std::string > returnmap;
   std::vector < std::string > tokens = tokenizer(String, Token);
   for (int i = 0; i < (int) tokens.size(); i++) {
      size_t firstequal = tokens[i].find(KeyValueSeparator);
      if (firstequal != std::string::npos && firstequal != 0) {
         std::string key = tokens[i].substr(0, firstequal);
         if (ToLower) {
            std::transform(key.begin(), key.end(), key.begin(), ::tolower);
         }
         returnmap[key] = tokens[i].substr(firstequal + 1);
      }
   }
   return returnmap;
}

/**
 * Elimina los caracteres deseados del principio y final del string
 * @param InputString String de entrada
 * @param pUnwantedChars Array con los caracteres indeseados
 * @return String sin los caracteres en pUnwantedChars al principio ni fin
 */
std::string trim(std::string const& InputString, char const* pUnwantedChars) {
   std::string::size_type const first = InputString.find_first_not_of(pUnwantedChars);
   return
         (first == std::string::npos) ? std::string() :
                                        InputString.substr(
                                              first,
                                              InputString.find_last_not_of(pUnwantedChars) - first
                                                    + 1);
}

/**
 * Dadas dos listas y un separador, elimina de la primera lista valores de la
 * segunda, si es que se encontraban en la primera.
 * @param[in] InputList : primera lista string.
 * @param[in] ListToRemove : segunda lista string.
 * @param[in] Token : separador, puede ser un char o un string.
 * @return resultlist : primera lista sin los elementos de la segunda lista.
 * \todo esta funcion podria ubicarse en otro lado.
 */
std::string RemoveFromList(const std::string &InputList, const std::string &ListToRemove,
                           const std::string &Token) {
   // Pasa las listas a vectores
   std::vector < std::string > vectorlisttoremove = tokenizer(ListToRemove, Token);
   std::vector < std::string > vectorinputlist = tokenizer(InputList, Token);

   // Con cada vector se crea un set
   std::set < std::string > setinputlist(vectorinputlist.begin(), vectorinputlist.end());
   std::set < std::string > setlisttoremove(vectorlisttoremove.begin(), vectorlisttoremove.end());

   // Iteradores para recorrer los sets y eliminar elementos
   std::set<std::string>::iterator iterase;
   std::set<std::string>::iterator itbegin;
   std::set<std::string>::iterator itend;

   // String de retorno
   std::string resultlist;

   // Inicializa los iteradores para recorrer la segunda lista
   itbegin = setlisttoremove.begin();
   itend = setlisttoremove.end();

   // Para cada elemento de la segunda lista
   while (itbegin != itend) {
      // Busca el elemento en la primera lista
      iterase = setinputlist.find(*itbegin);
      // Si encuentra el elemento en la segunda lista lo elimina
      if (iterase != setinputlist.end()) {
         setinputlist.erase(iterase);
      }
      itbegin++;
   }

   // Inicializa los iteradores para recorrer la primera lista
   itbegin = setinputlist.begin();
   itend = setinputlist.end();

   // La primera lista sin los elementos eliminados se vuelca a un string
   while (itbegin != itend) {
      resultlist += *itbegin;
      resultlist += Token;
      itbegin++;
   }

   // Elimina el ultimo token
   if (!resultlist.empty()) {
      resultlist = resultlist.substr(0, resultlist.length() - Token.length());
   }

   return resultlist;

#ifdef __UNUSED_CODE__

   /*
    * Intento de usar el metodo set::set_difference que directamente recibe
    * dos vactores y devuelve las diferencias. Se cuelga.
    */

   std::vector<std::string> vectorlisttoremove = tokenizer(ListToRemove, token);
   std::vector<std::string> vectorinputlist = tokenizer(InputList, token);

   sort(vectorinputlist.begin(), vectorinputlist.end() );
   sort(vectorlisttoremove.begin(), vectorlisttoremove.end() );

   std::set<std::string> setinputlist(vectorinputlist.begin(), vectorinputlist.end() );
   std::set<std::string> setlisttoremove(vectorlisttoremove.begin(), vectorlisttoremove.end() );

   std::vector<std::string>::iterator del;
   std::set<std::string>::iterator itbegin;
   std::set<std::string>::iterator itend;
   std::vector<std::string>::iterator it;

   std::string resultlist;

   // En este punto se cuelga.
   del = set_difference(vectorlisttoremove.begin(), vectorlisttoremove.end(), vectorinputlist.begin(),
         vectorinputlist.end(), it);

   for ( std::vector<std::string>::iterator tempit=it; tempit!=del; ++tempit )
   REPORT_DEBUG("D:%s", (*tempit).c_str() );

#endif
}

template<>
/**
 * Convierte un string a un valor numerico
 * \pre NumberStr debe estar entre [0, 255]
 * @param[in] NumberStr cadena que representa un numero.
 * @return unsigned char con el valor de la cadena convertido a valor numerico.
 */
unsigned char StringToNumber(const std::string &NumberStr) {
   int number;
   std::istringstream ssnumber(NumberStr);
   ssnumber >> std::dec >> number;
   return (unsigned char) number;
}

template<>
/**
 * Convierte un string a un valor numerico
 * \pre NumberStr debe estar entre [-128, 127]
 * @param[in] NumberStr cadena que representa un numero.
 * @return char con el valor de la cadena convertido a valor numerico.
 */
char StringToNumber(const std::string &NumberStr) {
   int number;
   std::istringstream ssnumber(NumberStr);
   ssnumber >> std::dec >> number;
   return (char) number;
}


/** Convierte un numero que esta en formato string al formato requerido */
/**
 * @param[in] NumberStr String con el numero
 * @return Numero del tipo requerido
 */
template<>
double StringToNumber(const std::string &NumberStr) {
   std::string numstrlower = ToLower(NumberStr);
   if (numstrlower.compare("nan") == 0)
      return std::numeric_limits<double>::quiet_NaN();

   if (numstrlower.compare("inf") == 0)
      return std::numeric_limits<double>::infinity();

   if (numstrlower.compare("-inf") == 0)
      return -std::numeric_limits<double>::infinity();

   double number = 0.0;
   std::istringstream ssnumber(NumberStr);
   ssnumber.precision(std::numeric_limits<double>::digits10);
   ssnumber >> number;
   return number;
}

/**
 * Especializacion para float.
 */
template<>
float StringToNumber(const std::string &NumberStr) {
   std::string numstrlower = ToLower(NumberStr);
   if (numstrlower.compare("nan") == 0)
      return std::numeric_limits<float>::quiet_NaN();

   if (numstrlower.compare("inf") == 0)
      return std::numeric_limits<float>::infinity();

   if (numstrlower.compare("-inf") == 0)
      return -std::numeric_limits<float>::infinity();

   float number = 0.0;
   std::istringstream ssnumber(NumberStr);
   ssnumber.precision(std::numeric_limits<float>::digits10);
   ssnumber >> number;
   return number;
}

/**
 * Compara strings por la cantidad de caracteres
 * @param[in] Lhs string 1
 * @param[in] Rhs string 2
 * @return true si la cantidad de caracteres de Lhs es menor que la cantidad de
 * caracteres de Rhs.
 * @return false si la cantidad de caracteres de Lhs es mayor o igual que la
 * cantidad de caracteres de Rhs.
 */
bool LessThanSize(const std::string &Lhs, const std::string &Rhs) {
   return Lhs.length() < Rhs.length();
}

/**
 * Convierte todos los caracteres de un std::string en minuscula
 * @param[in] String
 * @return string con caracteres en minuscula
 */
std::string ToLower(const std::string& String) {
   int size = String.size();
   std::string string = String;
   for(int i=0; i < size; ++i)
      string[i] = tolower(String[i]);
   return string;
}
/**
 * Convierte todos los caracteres de un std::string en mayuscula
 * @param[in] String
 * @return string con caracteres en minuscula
 */
std::string ToUpper(const std::string& String) {
   int size = String.size();
   std::string string = String;
   for(int i=0; i < size; ++i)
      string[i] = toupper(String[i]);
   return string;
}

/**
 * Devuelve true si el string es un  numero
 */

bool IsNumber(const std::string& s){
    unsigned int x=0;
    int dots=0;
    int minus=0;
    for (;x<s.length();x++){
       if (s[x]=='-'){
          minus++;
       }else if (s[x]=='.'){
          dots++;
       }
    }
    if ((dots>1)||(minus>1)) return false;
    return( strspn( s.c_str(), "-.0123456789" ) == s.size() );
}

} /*namespace suri*/

/**
 * Divide la cadena dada basandose en el token especificado.
 *
 * Tratar de usar esta funcion en lugar de split (que deberia ser deprecada)
 */
std::vector<std::string> SplitEx(const std::string& Str, const std::string& Token) {
    std::vector<std::string> retvec;

    std::size_t off = 0;
    std::size_t next = 0;

    // Agrego las partes que voy encontrando.
    while ((next = Str.find(Token, off)) != std::string::npos) {
        retvec.push_back(Str.substr(off, next - off));
        off = next + 1;
    }

    // Agrego el ultimo tramo.
    if (off > 0) {
        std::string lastone = Str.substr(off);
        retvec.push_back(lastone);
    } else if (off == 0) {
        retvec.push_back(Str);
    }

    return retvec;
}

/**
 * Junta los elementos de un vector de cadenas en una sola
 * usando el separador especificado.
 */
std::string JoinEx(const std::vector<std::string>& Strs, const std::string& Token) {
   std::string retstr;

   for (int ix = 0, lenix = Strs.size(); ix < lenix; ++ix) {
      if (retstr.length() > 0)
         retstr.append(Token);

      retstr.append(Strs[ix]);
   }

   return retstr;
}

/**
 * Compara los valores dados.
 * @param Num1 Primer valor.
 * @param Num2 Segundo valor.
 * @return true si los valores son iguales.
 */
bool AreEqual(double Num1, double Num2) {
   double max = std::max(1.0, std::max(std::fabs(Num1), std::fabs(Num2)));
   return std::fabs(Num1 - Num2) <= (AE_EPSILON * max);
}


