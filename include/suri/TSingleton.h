/*! \file suri/TSingleton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TSINGLETON_H_
#define TSINGLETON_H_

/** Template usado para crear singletons */
/**
 *  Imlementa un singleton con persistencia de una clase dada.
 *  Debe declararse como friend de la clase que se desea hacer singleton,
 * ademas deben hacerse privados los constructores y destructor de dicha clase.
 */
template<typename T>
class TSingleton {
public:
   /** Retorna la instancia del objeto singleton */
   static T& Instance() {
      static T object;
      return object;
   }
   /** Destructor */
   ~TSingleton() {
   }
private:
   /** Constructor */
   TSingleton() {
   }
   /** Constructor de copia */
   TSingleton(const TSingleton&);
   /** Operador de asignacion */
   TSingleton& operator=(const TSingleton&);
};

#endif   // TSINGLETON_H_
