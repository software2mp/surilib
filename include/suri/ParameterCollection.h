/** \file ParameterCollection.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PARAMETERCOLLECTION_H_
#define PARAMETERCOLLECTION_H_

#include <string>
#include <map>

#include "logmacros.h"

namespace suri {

/**
 * Clase base para parametros genericos.
 */
class BaseParameter {
public:
   virtual ~BaseParameter() { }
};

/**
 * Contenedor generico para valores de cualquier tipo.
 */
template<class C>
class GenericParameter : public BaseParameter {
public:
   GenericParameter(const C& ValueP) : value_(ValueP) { }

   virtual ~GenericParameter() { }

   C GetValue() {
      return value_;
   }

private:
   C value_;
};

/**
 * Contendor generico (map) que permite guardar en el mismo contenedor valores
 * de cualquier tipo asociandolo con una clave.
 */
class ParameterCollection {
public:
   virtual ~ParameterCollection() {
      REPORT_DEBUG("D: ParameterCollection DESTRUCTOR");
      std::map<std::string, BaseParameter*>::iterator it;
      for (it = params_.begin(); it != params_.end(); ++it) {
         delete (*it).second;
         (*it).second = NULL;
      }
      params_.clear();
   }

   /**
    * Verifica si existe algun elemento con la clave dada.
    *
    * @param[in] KeyP Clave que se quiere verificar.
    * @return true si existe algun elemento con esa clave.
    */
   bool Contains(std::string KeyP) {
      if (params_.find(KeyP) == params_.end())
         return false;

      return true;
   }

   /**
    * Agrega un elemento de cualquier tipo al contenedor asociado a una clave.
    *
    * @param[in] KeyP Clave del elemento.
    * @param[in] ValueP Elemento que se desea guardar.
    */
   template<class V>
   void AddValue(const std::string& KeyP, const V& ValueP) {
      std::map<std::string, BaseParameter*>::iterator it = params_.find(KeyP);
      if (it != params_.end()) {
         GenericParameter<V>* pgeneric =
               static_cast<GenericParameter<V>*>(params_[KeyP]);
         params_.erase(it);
         delete pgeneric;
      }
      params_[KeyP] = new GenericParameter<V>(ValueP);
   }

   /**
    * Obtiene un elemento del contendor basado en la clave dada.
    *
    * @param[in] KeyP Clave del elemento.
    * @param[in] ValueP Refencia en donde se va a guardar el elemento.
    * @return true si el elemento fue encontrado.
    */
   template<class V>
   bool GetValue(const std::string& KeyP, V& ValueP) {
      GenericParameter<V>* pvalue = NULL;
      if (Contains(KeyP)) {
         pvalue = static_cast<GenericParameter<V>*>(params_[KeyP]);
         ValueP = pvalue->GetValue();
         return true;
      }
      return false;
   }

private:
   std::map<std::string, BaseParameter*> params_;
};

}  // namespace suri

#endif  // PARAMETERCOLLECTION_H_
