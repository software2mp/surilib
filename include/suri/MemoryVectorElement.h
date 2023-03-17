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

#ifndef MEMORYVECTORELEMENT_H_
#define MEMORYVECTORELEMENT_H_

// Include suri
#include "VectorElement.h"

/** namespace suri */
namespace suri {
// Forward declaration
class Vector;

/** VectorElement asociado a un vector en memoria */
/**
 *  Esta clase representa un vector en memoria.
 *  Debe interactuar con la clase suri::Vector en modo memoria y generar un
 * url de elemento que se pueda asociar a una instancia de dicha clase y que
 * en el metodo suri::Vector::Open(url) retorne esa instancia.
 */
class MemoryVectorElement : public VectorElement {
public:
   /** ctor */
   MemoryVectorElement();
   /** ctor de copia */
   MemoryVectorElement(const MemoryVectorElement &MemoryVectorElement);
   /** dtor */
   virtual ~MemoryVectorElement();
   /** Funcion estatica para crear un MemoryVectorElement */
   static MemoryVectorElement* Create(const std::string &Filename,
                                      const std::string &SpatialReference, int Layers,
                                      const Vector::VectorType Type);
   /** Funcion estatica para crear un MemoryVectorElement */
   static MemoryVectorElement* Create(const std::string &Filename,
                                      int Layers,
                                      const Vector::VectorType Type);

   /** Funcion que salva en un archivo el vector en memoria */
   virtual bool Save(const std::string &Filename);
   /** Funcion que salva en un archivo el vector en memoria */
   virtual std::string Save();
   /** obtiene el vector asociado */
   Vector *GetVector() const;
protected:
   /** Crea un vector en memoria */
   Vector *NewVector(const std::string &Filename, int Layers,
                     const Vector::VectorType Type);
   Vector *pMemVector_; /*! Instancia de vector en memoria */
   Vector *pSavedVector_; /*! Instancia de vector salvado */
private:
   /** Inicializa los nodos del elemento */
   virtual MemoryVectorElement* Initialize(const std::string &Filename);
};
}

#endif /*MEMORYVECTORELEMENT_H_*/
