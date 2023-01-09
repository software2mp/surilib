/*! \file MemoryVectorElement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
