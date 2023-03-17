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

#ifndef SURIMUPARSER_H_
#define SURIMUPARSER_H_

// Includes standard
#include <map>
#include <string>


#ifdef __MU_PARSER__
# ifdef __WXMSW__
#  include "muParserDLL.h"
#  define muDataType muFloat_t
# else
#  include "muParser.h"
#  define muDataType mu::value_type
# endif
#else
#  define muDataType double
#endif


namespace suri {

typedef muDataType* (*muFactoryFunction)(const char*, void*);
typedef muDataType (*muFun1 )(muDataType);
typedef muDataType (*muFun2 )(muDataType, muDataType);

/**
 * Encapsula interfaz de biblioteca muParser.
 */
class muParser {
public:
   /** Ctor */
   muParser();
   /** Dtor */
   ~muParser();
   /** Configura la factoria que al parsear, asigna la memoria a las variables */
   void SetVarFactory(muFactoryFunction pFunction, void* pUserData);
   /** Agrega operador binario */
   void DefineOperator(const char *Name, muFun2 pFunction2, int Precendence);
   /** Agrega operador unario */
   void DefineInfixOperator(const char *pName, muFun1 pFunction1);
   /** Evalua la funcion. Esta en .h para que sea inline  */
   double Eval() {
#ifdef __MU_PARSER__
# ifdef __WXMSW__
      return mupEval(pMuHandler_);
# else  // __WXMSW__
      double returnvalue = 0;
      try {
         returnvalue = muParser_.Eval();
      } catch(mu::Parser::exception_type &e) {
         error_ = true;
         errorCode_ = e.GetPos();
         position_ = e.GetCode();
      }
      return returnvalue;
# endif  // __WXMSW__
#else  // __MU_PARSER__
      return 0;
#endif  // __MU_PARSER__
   }

   /** Configura la funcion a evaluar. No fuerza el parseo */
   void SetExpr(const char *pExpresion);
   /**
    * Retorna las variables generadas al parsear la funcion. Fuerza el parseo
    * @pre {Invocar el metodo SetExpr}
    */
   const std::map<std::string, muDataType*> GetVarMap();
   /**
    * Indica si ocurrio un error.
    * @pre {Invocar el metodo GetVarMap}
    * @post {El flag interno de error se setea a false}
    */
   bool Error();
   /**
    * Retorna el codigo de error
    * @pre {Invocar el metodo GetVarMap}
    */
   int GetErrorCode();
   /**
    * Retorna la posicion del error
    * @pre {Invocar el metodo GetVarMap}
    */
   int GetErrorPosition();
   /** Elimina las variables configuradas */
   void ClearVar();

#ifdef __MU_PARSER__

private:
# ifdef __WXMSW__
   muParserHandle_t pMuHandler_; /*! Handler de la ecuacion */
# else
   mu::Parser muParser_;  /*! Handler de la ecuacion */
   bool error_;  /*! Indica si ocurrio un error en el ultimo parseo de la ecuacion */
   int errorCode_;  /*! Codigo de error generado por muparser */
   int position_;  /*! Posicion del errro en la ecuacion */
# endif

#endif
};

}  // namespace

#endif /* SURIMUPARSER_H_ */
