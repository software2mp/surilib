/** \file SuriMuParser.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


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
