/*! \file TFactory.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TFACTORY_H_
#define TFACTORY_H_

// Includes standard
#include <string>
#include <map>
#include <stdexcept>
#include <vector>

// Includes suri
#include "suri/messages.h"
#include "../include/suri/TSingleton.h"

/** Politica creadora de objetos */
/**
 *  Es una politica para ser usada en la factoria que permite decidir la forma
 * en que se crearan los objetos
 */
template<class T>
class CreateObjectPolicy {
public:
   /** Constructor */
   CreateObjectPolicy() {
   }
   ;
   /** Destructor */
   ~CreateObjectPolicy() {
   }
   ;
   /** Tipo creador de ojbetos */
   typedef T* (*CreatorType)();
   /** Forma de obtener un objeto. */
   /**
    * @param[in] ObjectCreator funcion que crea el objeto
    * @return objeto creado
    */
   static T* GetObject(CreatorType ObjectCreator) {
      return ObjectCreator();
   }
   ;
   /** Forma de descartar (destruir) un objeto */
   static void DisposeObject(CreatorType ObjectCreator) {
   }
   ;
protected:
private:
};

/** Template Factoria de objetos */
template<class T, template<class > class FactoryPolicy = CreateObjectPolicy>
class TFactory {
public:
   /** Friend el Singleton */
   friend class TSingleton<TFactory<T, FactoryPolicy> > ;
   /** El tipo por el que se identifican las clases */
   typedef std::string IdentifierType;
   /** El vector donde se guardan los identificadores */
   typedef std::vector<IdentifierType> IdentifierVector;

// CONSTRUCTORES/DESTRUCTOR
   /** Rompe con el patron singleton. */
// OPERADORES
// CONSULTA
// OPERACIONES
   /** Regresa true si se pudo realizar la registracion. */
   /**
    * @param[in] ClassId id de la clase
    * @param[in] SecondManipulator
    * @return
    */
   bool RegisterClass(IdentifierType ClassId,
                      typename FactoryPolicy<T>::CreatorType SecondManipulator)
#ifndef __GNUGCC__
                      {
      bool result = createCallbacks_.insert(
            typename CallbackMapType::value_type(ClassId, SecondManipulator)).second;
      if (result) {
         idVector_.push_back(ClassId);
      }
      return result;
   }
#endif
   /** Regresa true si ClassId habia sido registrado. */
   bool UnRegisterClass(IdentifierType ClassId);
   /** Regresa la clase que se le pasa por ClassId */
   T *CreateClass(IdentifierType ClassId);
// ACCESO
   /** Regresa un vector de los ClassId registrados en la Factoria */
   IdentifierVector& GetClassIds();
protected:
private:
   /** Tipo donde se guardan los pares, id/objeto_creador */
   typedef std::map<IdentifierType, typename FactoryPolicy<T>::CreatorType> CallbackMapType;
// CONSTRUCTORES/DESTRUCTOR
   /** Ctor */
   TFactory() {
   }
   ;
   /** Dtor */
   virtual ~TFactory() {
   }
   ;
   /** Ctor de copia */
   TFactory(const TFactory &);
   CallbackMapType createCallbacks_; /*! gurada los pares */
   IdentifierVector idVector_; /*! guarda los identificadores en el orden que se agregan */
};

/* //////////////////////////// PUBLIC ////////////////////////////////// */
// ============================ OPERADORES ===============================
// =========================== OPERACIONES ===============================
// ============================= ACCESO ==================================
template<class T, template<class > class FactoryPolicy>
typename TFactory<T, FactoryPolicy>::IdentifierVector&
TFactory<T, FactoryPolicy>::GetClassIds() {
   return idVector_;
}
// ============================ CONSULTA =================================
// =========================== OPERACIONES ===============================

#ifdef __GNUGCC__
/**
 * Regresa true si se pudo realizar la registracion.
 * @param[in] ClassId id de la clase a registrar
 * @param[in] SecondManipulator funcion que crea las clases
 * @return resultado de la operacion
 */
template <class T, template <class> class FactoryPolicy>
bool TFactory<T, FactoryPolicy>::RegisterClass(
      IdentifierType ClassId,
      typename FactoryPolicy<T>::CreatorType
      SecondManipulator)
{
   bool result = createCallbacks_.
   insert(typename CallbackMapType::value_type(ClassId, SecondManipulator) ).
   second;
   if ( result ) {
      idVector_.push_back(ClassId);
   }
   return result;
}
#endif

/**
 * Regresa true si ClassId habia sido registrado.
 * @param[in] ClassId id de la clase que quiero desregistrar
 * @return resultado de la operacion
 */
template<class T, template<class > class FactoryPolicy>
bool TFactory<T, FactoryPolicy>::UnRegisterClass(IdentifierType ClassId) {
   typename CallbackMapType::const_iterator it = createCallbacks_.find(ClassId);

   return createCallbacks_.erase(ClassId) == 1;
}

/**
 * Crea una clase del tipo indicado
 * @param[in] ClassId id de la clase que quiero crear
 * @return clase creada
 */
template<class T, template<class > class FactoryPolicy>
T *TFactory<T, FactoryPolicy>::CreateClass(IdentifierType ClassId) {
   typename CallbackMapType::const_iterator it = createCallbacks_.find(ClassId);
   if (it == createCallbacks_.end()) {
      throw std::runtime_error(
            "TFactory::pSelectClass(" + ClassId + "): " + message_CLASS_NOT_FOUND);  // No se traduce,
                                                                                     // msg bajo nivel
   }
   // Llamo a la funcion de creado.
   return FactoryPolicy<T>::GetObject((it->second));
}
/* ////////////////////////// PROTECTED  //////////////////////////////// */
/* ////////////////////////// PRIVATE    //////////////////////////////// */
#endif   // TFACTORY_H_
