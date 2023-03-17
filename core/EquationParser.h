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

#ifndef EQUATIONPARSER_H_
#define EQUATIONPARSER_H_

// Includes standard
#include <string>
#include <map>
#include <vector>

#include "SuriMuParser.h"


namespace suri {

/**
 * Clase que permite parsear una ecuacion, asignarle valores a las variables
 * que contiene y evaluar el resultado.
 */
class EquationParser {
public:
   /** Codigos de error */
   enum ErrorCode {
     // Errores de sintaxis
     ecUNEXPECTED_OPERATOR    = 0,  /*! Unexpected binary operator found */
     ecUNASSIGNABLE_TOKEN     = 1,  /*! Token cant be identified. */
     ecUNEXPECTED_EOF         = 2,  /*! Unexpected end of formula. (Example: "2+sin(") */
     ecUNEXPECTED_ARG_SEP     = 3,  /*! An unexpected comma has been found. (Example: "1,23") */
     ecUNEXPECTED_ARG         = 4,  /*! An unexpected argument has been found */
     ecUNEXPECTED_VAL         = 5,  /*! An unexpected value token has been found */
     ecUNEXPECTED_VAR         = 6,  /*! An unexpected variable token has been found */
     ecUNEXPECTED_PARENS      = 7,  /*! Unexpected Parenthesis, opening or closing */
     ecUNEXPECTED_STR         = 8,  /*! A string has been found at an inapropriate position */
     ecSTRING_EXPECTED        = 9,  /*! A string function has been called */
                                    /* with a different type of argument */
     ecVAL_EXPECTED           = 10, /*! A numerical function has been called */
                                    /* with a non value type of argument */
     ecMISSING_PARENS         = 11, /*! Missing parens. (Example: "3*sin(3") */
     ecUNEXPECTED_FUN         = 12, /*! Unexpected function found. */
                                    /* (Example: "sin(8)cos(9)") */
     ecUNTERMINATED_STRING    = 13, /*! unterminated string constant. */
                                    /* (Example: "3*valueof("hello)") */
     ecTOO_MANY_PARAMS        = 14, /*! Too many function parameters */
     ecTOO_FEW_PARAMS         = 15, /*! Too few function parameters. */
                                    /* (Example: "ite(1<2,2)") */
     ecOPRT_TYPE_CONFLICT     = 16, /*! binary operators may only be applied */
                                    /* to value items of the same type */
     ecSTR_RESULT             = 17, /*! result is a string */

#ifdef __UNUSED_CODE__
     // Parametros de entrada invalidos
     ecINVALID_NAME           = 18, /*! Invalid function, variable or constant name. */
     ecINVALID_BINOP_IDENT    = 19, /*! Invalid binary operator identifier */
     ecINVALID_INFIX_IDENT    = 20, /*! Invalid function, variable or constant name. */
     ecINVALID_POSTFIX_IDENT  = 21, /*! Invalid function, variable or constant name. */

     ecBUILTIN_OVERLOAD       = 22, /*! Trying to overload builtin operator */
     ecINVALID_FUN_PTR        = 23, /*! Invalid callback function pointer */
     ecINVALID_VAR_PTR        = 24, /*! Invalid variable pointer */
     ecEMPTY_EXPRESSION       = 25, /*! The Expression is empty */
     ecNAME_CONFLICT          = 26, /*! Name conflict */
     ecOPT_PRI                = 27, /*! Invalid operator priority */
     //
     ecDOMAIN_ERROR           = 28, /*! catch division by zero, sqrt(-1), log(0) */
                                    /* (currently unused) */
     ecDIV_BY_ZERO            = 29, /*! Division by zero (currently unused) */
#endif

     ecGENERIC                = 30, /*! Generic error */
     ecLOCALE                 = 31, /*! Conflict with current locale */

     ecUNEXPECTED_CONDITIONAL = 32,
     ecMISSING_ELSE_CLAUSE    = 33,
     ecMISPLACED_COLON        = 34,

     // Errores internos
     ecINTERNAL_ERROR         = 35, /*! Internal error of any kind. */

     // Codigos especiales
     ecUNDEFINED              = -1,  /*! Error no identificado */
     ecNO_ERROR                = -2 /*! No se encontro un error al parsear la ecuacion */
   };

	/**
	 * Estructura que informa el codigo y posicion del 
	 * error encontrado en ecuacion
	 */
   struct EquationError {
      ErrorCode code_;  /*! Codigo que indica el tipo de error encontrado al */
                        /* parsear la ecuacion */
      int position_;  /*! Posicion del error cometido */
   };

   /** Ctor */
   EquationParser();
   /** Dtor */
   ~EquationParser();

   /** Configura la ecuacion que se va a evaluar. */
   bool SetEquation(const std::string &Equation);
   /** Retorna informacion sobre errores al parsear la ecuacion. */
   EquationError GetError();

   /** Retorna los nombres de las variables */
   bool GetVariableNames(std::vector<std::string> &VariableNames);
   /** Asigna el valor a una variable. Si no existe falla. */
   bool GetVariableValuePointer(const std::string &VarName, double* &pValue);

   /** Ejecuta la ecuacion configurada. Esta en .h para que sea inline */
   double EvaluateEquation() {return pMuParser_->Eval();}

private:
   /** Valida que la expresion este bien formada. Configura equationError_. */
   bool ValidateExpresion();
   /** Limpia las variables encontradas en la ecuacion. */
   void CleanVariables();
   /** Retorna handler configurado con las operaciones soportadas por suri. */
   muParser* ConfigMuParser();
   /** Cierra el handler de muParser */
   void ReleaseMuParser(muParser* &pMuParser);
   /** Remplaza operador ingresado por el usuario por otro cargado en muparser */
   bool ReplaceOperator(const std::string OldOperator,
                     const std::string &NewOperator, std::string &Equation);

   muParser* pMuParser_; /*! Handler de la ecuacion */
   EquationError equationError_; /*! Contiene error generado al parsear la ecuacion */

   static const std::string SOPIEQUAL;
   static const std::string SOPINOTEQUAL;
   static const std::string MUPARSEREQUAL;
   static const std::string MUPARSERNOTEQUAL;
   static const std::string SOPIAND;
   static const std::string SOPIOR;
   static const std::string SOPIXOR;
   static const std::string SOPINOT;
   static const std::string SOPIMOD;
};

}  // namespace

#endif /* EQUATIONPARSER_H_ */
