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
