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

// Includes standard
#include <string>
#include <vector>
#include <map>

// Includes suri
#include "EquationParser.h"
#include "suri/AuxiliaryFunctions.h"
#include "logmacros.h"

namespace suri {

/**
 * Funcion que se registra en muparser. Al parsear la ecuacion, cada vez que se
 * detecta una variable nueva, le asigna una posicion en el mapa.
 * @param[in] pName Nombre de la variable
 * @param[in] pUserData puntero a datos de usuario. No se usa, NULL.
 * @return puntero double con valor de la variable.
 */
double* MuAddVariable(const char *pName, void *pUserData) {
   return new double(0);
}

/**
 * And logico entre valores.
 * @param[in] Val1 valor de la primer variable
 * @param[in] Val2 valor de la segunda variable
 * @return resultado de and.
 */
muDataType MuAnd(muDataType Val1, muDataType Val2) {
   return Val1 && Val2;
}

/**
 * Or logico entre valores.
 * @param[in] Val1 valor de la primer variable
 * @param[in] Val2 valor de la segunda variable
 * @return resultado de or.
 */
muDataType MuOr(muDataType Val1, muDataType Val2) {
   return Val1 || Val2;
}

/**
 * Not logico.
 * @param[in] Val valor que quiero negar
 * @return resultado de not.
 */
muDataType MuNot(muDataType Val) {
   return !Val;
}

/**
 * Xor logico entre valores.
 * @param[in] Val1 valor de la primer variable
 * @param[in] Val2 valor de la segunda variable
 * @return resultado de xor.
 */
muDataType MuXor(muDataType Val1, muDataType Val2) {
   return (Val1 || Val2) && !(Val1 && Val2);
}

/**
 * Retorna resto de division entre valores. Antes
 * redondea los valores al entero mas cercano.
 * @param[in] Val1 valor de la primer variable
 * @param[in] Val2 valor de la segunda variable
 * @return resultado de modulos.
 */
muDataType MuMod(muDataType Val1, muDataType Val2) {
   int val1 = SURI_ROUND(int, Val1);
   int val2 = SURI_ROUND(int, Val2);
   return val1 % val2;
}

/**
 * Compara los valores usando epsilon.
 * @param[in] Val1 valor de la primer variable.
 * @param[in] Val2 valor de la segunda variable.
 * @return resultado de igual.
 */
muDataType MuEqual(muDataType Val1, muDataType Val2) {
   return FLOAT_COMPARE(Val1, Val2);
}

/**
 * Compara los valores usando epsilon.
 * @param[in] Val1 valor de la primer variable.
 * @param[in] Val2 valor de la segunda variable.
 * @return resultado de diferentes.
 */
muDataType MuNotEqual(muDataType Val1, muDataType Val2) {
   return !FLOAT_COMPARE(Val1, Val2);
}

/** Strings constantes */
const std::string EquationParser::SOPIEQUAL("==");
const std::string EquationParser::SOPINOTEQUAL("!=");
// NOTA: MUPARSEREQUAL y MUPARSERNOTEQUAL deben tener la misma longitud
// que los operadores originales para que no se modifique la posicion
// al ocurrir un error. Por esta razon se uso #= y #!.
// Se eligio # porque no se usa en ningun operador. Como la eleccion es
// transparente al resto de la aplicacion se puede modificar en el futuro.
const std::string EquationParser::MUPARSEREQUAL("#=");
const std::string EquationParser::MUPARSERNOTEQUAL("#!");
const std::string EquationParser::SOPIAND("and");
const std::string EquationParser::SOPIOR("or");
const std::string EquationParser::SOPIXOR("xor");
const std::string EquationParser::SOPINOT("!");
const std::string EquationParser::SOPIMOD("%");

/** Ctor */
EquationParser::EquationParser() {
   pMuParser_ = ConfigMuParser();
   equationError_.code_ = ecUNDEFINED;
}

/** Dtor */
EquationParser::~EquationParser() {
   CleanVariables();
   ReleaseMuParser(pMuParser_);
}

/**
 * Configura la ecuacion que se va a evaluar
 * @param[in] Equation string con la ecuacion que se desea parsear
 * @return bool que indica si pudo parsear la ecuacion.
 */
bool EquationParser::SetEquation(const std::string &Equation) {
   CleanVariables();
   std::string equation = Equation;

   if (!ReplaceOperator(SOPIEQUAL, MUPARSEREQUAL, equation) ||
        !ReplaceOperator(SOPINOTEQUAL, MUPARSERNOTEQUAL, equation))
      return false;



   pMuParser_->SetExpr(equation.c_str());

   return ValidateExpresion();
}

/**
 * Retorna informacion sobre errores al parsear la ecuacion.
 * @return estructura con error y posicion en ecuacion
 */
EquationParser::EquationError EquationParser::GetError() {
   return equationError_;
}

/**
 * Retorna los nombres de las variables
 * @param[out] VarNames vector donde se guardaran los nombres de las variables
 * @return bool que indica si la ecuacion estaba configurada correctamente
 */
bool EquationParser::GetVariableNames(std::vector<std::string> &VarNames) {
   if (equationError_.code_ != ecNO_ERROR)
      return false;

   std::map<std::string, muDataType*> variables = pMuParser_->GetVarMap();
   std::map<std::string, muDataType*>::iterator it;
   for (it = variables.begin(); it != variables.end(); it++)
      VarNames.push_back(it->first);
   return true;
}

/**
 * Retorna el puntero a la variable que contiene el valor que se usa para
 * evaluar una expresion.
 * @param[in] VarName nombre de la variable
 * @param[out] pValue puntero al valor.
 * @return bool que indica si la ecuacion estaba configurada correctamente.
 */
bool EquationParser::GetVariableValuePointer(const std::string &VarName,
                                                            double* &pValue) {
   if (equationError_.code_ != ecNO_ERROR)
      return false;

   bool found = false;
   std::map<std::string, muDataType*> variables = pMuParser_->GetVarMap();
   std::map<std::string, muDataType*>::iterator it = variables.find(VarName);
   if (it != variables.end()) {
      found = true;
      pValue = it->second;
   }
   return found;
}

/**
 * Valida que la expresion este bien formada. Configura equationError_.
 * @return bool que indica si la ecuacion es valida
 */
bool EquationParser::ValidateExpresion() {
   // Con GetVarMap fuerzo a muParser a parsear la ecuacion.
   pMuParser_->GetVarMap();
   if (pMuParser_->Error()) {
      // Los codigos de ErrorCode son iguales a los de muParser
      equationError_.code_ = (ErrorCode)pMuParser_->GetErrorCode();
      equationError_.position_ = pMuParser_->GetErrorPosition();
      return false;
   }
   equationError_.code_ = ecNO_ERROR;
   return true;
}

/**
 * Limpia las variables encontradas en la ecuacion.
 */
void EquationParser::CleanVariables() {
   std::map<std::string, muDataType*> variables = pMuParser_->GetVarMap();
   std::map<std::string, muDataType*>::iterator it;
   for (it = variables.begin(); it != variables.end(); it++)
      delete it->second;
   pMuParser_->ClearVar();
}

/**
 * Retorna muParser configurado con las operaciones soportadas por suri.
 * @return puntero a handler con operaciones configuradas.
 */
muParser* EquationParser::ConfigMuParser() {
   muParser* pmuparser = new muParser();
   pmuparser->SetVarFactory(&MuAddVariable, this);
   pmuparser->DefineOperator(MUPARSEREQUAL.c_str(), &MuEqual, 5);
   pmuparser->DefineOperator(MUPARSERNOTEQUAL.c_str(), &MuNotEqual, 5);
   pmuparser->DefineOperator(SOPIAND.c_str(), &MuAnd, 1);
   pmuparser->DefineOperator(SOPIOR.c_str(), &MuOr, 2);
   pmuparser->DefineInfixOperator(SOPINOT.c_str(), &MuNot);
   pmuparser->DefineOperator(SOPIXOR.c_str(), &MuXor, 3);
   pmuparser->DefineOperator(SOPIMOD.c_str(), &MuMod, 7);
   return pmuparser;
}

/**
 * Cierra el handler de muParser
 * @param[in] puntero a handler que quiero cerrar
 */
void EquationParser::ReleaseMuParser(muParser* &pMuParser) {
   delete pMuParser;
   pMuParser = NULL;
}

/**
 * Remplaza operador ingresado por el usuario por otro cargado en muparser.
 * Permite redefinir el comportamiento de los operadores hardcodeados en
 * muParser.
 * @param[in] OldOperator operador que quiero remplazar
 * @param[in] NewOperator nuevo operador
 * @param[in] Equation ecuacion que quiero modificar
 * @param[out] Equation ecuacion con operador modificado
 * @return true en caso de exito
 * @return false si en la ecuacion existia el operador NewOperator
 */
bool EquationParser::ReplaceOperator(const std::string OldOperator,
                  const std::string &NewOperator, std::string &Equation) {
   // Si encuentra el operdor NewOperator falla.
   if (Equation.find(NewOperator) != std::string::npos) {
      equationError_.code_ = ecUNEXPECTED_OPERATOR;
      equationError_.position_ = Equation.find(NewOperator);
      return false;
   }

   // Remplaza los operadores OldOperator por NewOperator en Equation
   size_t found = Equation.find(OldOperator);
   while (found != std::string::npos) {
      Equation.replace(found, OldOperator.size(), NewOperator);
      found = Equation.find(OldOperator, found + NewOperator.size());
   }

   return true;
}

}  // namespace

