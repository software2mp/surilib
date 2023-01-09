/** \file SuriMuParser.h */
/*!
*  Biblioteca de procesamiento GeoEspacial SuriLib.
*  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
*
*  Este producto contiene software desarrollado por
*  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
*
*/

// Includes standard
#include <map>
#include <string>

// Includes suri
#include "SuriMuParser.h"

namespace suri {

#ifdef __MU_PARSER__

#ifdef __WXMSW__
// --------------------- INTEFAZ PARA WINDOWS -------------------------

/** Ctor */
muParser::muParser() {
   pMuHandler_ = mupCreate(muBASETYPE_FLOAT);
}
/** Dtor */
muParser::~muParser() {
   mupRelease(pMuHandler_);
}
/** Configura la factoria que al parsear, asigna la memoria a las variables */
void muParser::SetVarFactory(muFactoryFunction pFunction, void* pUserData) {
   mupSetVarFactory(pMuHandler_, pFunction, pUserData);
}
/** Agrega operador binario */
void muParser::DefineOperator(const char *Name, muFun2 pFunction2, int Precendence) {
   mupDefineOprt(pMuHandler_, Name, pFunction2, Precendence, 0, true);
}
/** Agrega operador unario */
void muParser::DefineInfixOperator(const char *pName, muFun1 pFunction1) {
   mupDefineInfixOprt(pMuHandler_, pName, pFunction1, true);
}
/** Configura la funcion a evaluar. No fuerza el parseo */
void muParser::SetExpr(const char *pExpresion) {
   mupError(pMuHandler_);  // Limpio el error
   mupSetExpr(pMuHandler_, pExpresion);
}
/**
 * Retorna las variables generadas al parsear la funcion. Fuerza el parseo
 * @pre {Invocar el metodo SetExpr}
 */
const std::map<std::string, muDataType*> muParser::GetVarMap() {
   std::map<std::string, muDataType*> varmap;
   int varcount = mupGetExprVarNum(pMuHandler_);
   const char* pname;
   muDataType* value;
   for (int i = 0; i < varcount; i++) {
      mupGetVar(pMuHandler_, i, &pname, &value);
      varmap[pname] = value;
   }
   return varmap;
}
/**
 * Indica si ocurrio un error.
 * @pre {Invocar el metodo GetVarMap}
 * @post {El flag interno de error se setea a false}
 */
bool muParser::Error() {
   return mupError(pMuHandler_);
}
/**
 * Retorna el codigo de error
 * @pre {Invocar el metodo GetVarMap}
 */
int muParser::GetErrorCode() {
   return mupGetErrorCode(pMuHandler_);
}
/**
 * Retorna la posicion del error
 * @pre {Invocar el metodo GetVarMap}
 */
int muParser::GetErrorPosition() {
   return mupGetErrorPos(pMuHandler_);
}
/** Elimina las variables configuradas */
void muParser::ClearVar() {
   mupClearVar(pMuHandler_);
}





#else  // __WXMSW__

// --------------------- INTERFAZ PARA LINUX ---------------------------
/** Ctor */
muParser::muParser() {
   error_ = false;
}
/** Dtor */
muParser::~muParser() {}
/** Configura la factoria que al parsear, asigna la memoria a las variables */
void muParser::SetVarFactory(muFactoryFunction pFunction, void* pUserData) {
   muParser_.SetVarFactory(pFunction, pUserData);
}
/** Agrega operador binario */
void muParser::DefineOperator(const char *Name, muFun2 pFunction2, int Precendence) {
   muParser_.DefineOprt(Name, pFunction2, Precendence);
}
/** Agrega operador unario */
void muParser::DefineInfixOperator(const char *pName, muFun1 pFunction1) {
   muParser_.DefineInfixOprt(pName, pFunction1);
}
/** Configura la funcion a evaluar. No fuerza el parseo */
void muParser::SetExpr(const char *Expresion) {
   error_ = false;
   muParser_.SetExpr(Expresion);
}
/**
 * Retorna las variables generadas al parsear la funcion. Fuerza el parseo
 * @pre {Invocar el metodo SetExpr}
 */
const std::map<std::string, muDataType*> muParser::GetVarMap() {
   std::map<std::string, muDataType*> varmap;
   try {
      varmap = muParser_.GetUsedVar();
   } catch(mu::Parser::exception_type &e) {
      error_ = true;
      position_ = e.GetPos();
      errorCode_ = e.GetCode();
   }
   return varmap;
}
/**
 * Indica si ocurrio un error.
 * @pre {Invocar el metodo GetVarMap}
 * @post {El flag interno de error se setea a false}
 */
bool muParser::Error() {
   bool error = error_;
   error_ = false;
   return error;
}
/**
 * Retorna el codigo de error
 * @pre {Invocar el metodo GetVarMap}
 */
int muParser::GetErrorCode() {
   return errorCode_;
}
/**
 * Retorna la posicion del error
 * @pre {Invocar el metodo GetVarMap}
 */
int muParser::GetErrorPosition() {
   return position_;
}
/** Elimina las variables configuradas */
void muParser::ClearVar() {
   muParser_.ClearVar();
}

#endif  // __WXMSW__





#else  // __MU_PARSER__
// --------------------- INTEFAZ SIN MUPARSER -------------------------
#define NO_MUPARSER_LIBRARY -3

/** Ctor */
muParser::muParser() {}
/** Dtor */
muParser::~muParser() {}
/** Configura la factoria que al parsear, asigna la memoria a las variables */
void muParser::SetVarFactory(muFactoryFunction pFunction, void* pUserData) {}
/** Agrega operador binario */
void muParser::DefineOperator(const char *Name, muFun2 pFunction2, int Precendence) {}
/** Agrega operador unario */
void muParser::DefineInfixOperator(const char *pName, muFun1 pFunction1) {}
/** Configura la funcion a evaluar. No fuerza el parseo */
void muParser::SetExpr(const char *Expresion) {}
/**
 * Retorna las variables generadas al parsear la funcion. Fuerza el parseo
 * @pre {Invocar el metodo SetExpr}
 */
const std::map<std::string, muDataType*> muParser::GetVarMap() {
   std::map<std::string, muDataType*> emptyvars;
   return emptyvars;
}
/**
 * Indica si ocurrio un error.
 * @pre {Invocar el metodo GetVarMap}
 * @post {El flag interno de error se setea a false}
 */
bool muParser::Error() {
   return true;
}
/**
 * Retorna el codigo de error
 * @pre {Invocar el metodo GetVarMap}
 */
int muParser::GetErrorCode() {
   return NO_MUPARSER_LIBRARY;
}
/**
 * Retorna la posicion del error
 * @pre {Invocar el metodo GetVarMap}
 */
int muParser::GetErrorPosition() {
   return -1;
}
/** Elimina las variables configuradas */
void muParser::ClearVar() {}

#endif  // __MU_PARSER__

}  // namespace

