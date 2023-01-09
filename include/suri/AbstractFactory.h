/*! \file AbstractFactory.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ABSTRACTFACTORY_H_
#define ABSTRACTFACTORY_H_

// Includes standard
#include <string>
#include <map>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <stdarg.h>

// Includes suri
#include "TSingleton.h"
#include "FactoryHelpers.h"

/** Politica de Error que retorna NULL frente a un error de creacion */
/**
 *  Esta clase implement una politica de la factoria para el manejo de los
 * errores (de creacion y Destruccion) de los objetos registrados.
 *
 *  Posee 3 metodos.
 *   - OnCreateError : se llama cuando la factoria no puede crear una clase.
 *   - OnDestroyError : se llama cuando la factoria no puede destruir una clase.
 *   - ReturnNull : es el metodo que se retorna en OnCreateError.
 *
 */
template<class AbstractProduct, class IdentifierType>
class ReturnNullErrorPolicy {
public:
   /** Constructor */
   ReturnNullErrorPolicy() {
   }
   /** destructor */
   ~ReturnNullErrorPolicy() {
   }
   /** Metodo que retorna NULL */
   static void *ReturnNull() {
      return NULL;
   }
   /** Metodo llamado al no poder crear la clase dada */
   /**
    * @param[in] ClassId Identificador de la clase que no se pudo crear.
    * @return Puntero a la funcion que utiliza la factoria cuyo valor de retorno
    *  es pasado al codigo cliente.
    */
   typename AbstractProduct::CreatorType OnCreateError(const IdentifierType &ClassId) {
      std::stringstream ss;
      ss << "AbstractFactory::pSelectClass(" << ClassId
         << "): Clase no se encuentra registrada";
      return (typename AbstractProduct::CreatorType) (&ReturnNull);
   }
   /** Metodo llamado al haber un error en la destruccion de la clase */
   void OnDestroyError() {
   }
};

/** Politica de error que emite una excepcion */
/**
 *  Esta clase implementa la politica de error de la factoria que tira una
 * excepcion de timo std::runtime_error con la descripcion de error.
 */
template<class AbstractProduct, class IdentifierType>
class ThrowExceptionErrorPolicy {
public:
   /** Constructor */
   ThrowExceptionErrorPolicy() {
   }
   /** Destructor */
   ~ThrowExceptionErrorPolicy() {
   }
   /** Metodo llamado al no poder crear la clase dada */
   /**
    * \throw std::runtime_error con la descripcion del error
    * @param[in] ClassId Identificador de la clase que no se pudo crear.
    * @return No retorna valor ya que tira excepcion
    */
   typename AbstractProduct::CreatorType OnCreateError(const IdentifierType &ClassId)
         throw (std::runtime_error) {
      std::stringstream ss;
      ss << "AbstractFactory::pSelectClass(" << ClassId
         << "): Clase no se encuentra registrada";
      throw std::runtime_error(ss.str());  // No se traduce, msg bajo nivel
   }
   /** Metodo llamado al haber un error en la destruccion de la clase */
   /**
    * \throw std::runtime_error con la descripcion del error
    */
   void OnDestroyError() throw (std::runtime_error) {
      throw std::runtime_error("AbstractFactory::Destroy(): Error.");
   }
};

/** Politica creadora de objetos */
/**
 *  Es una politica para ser usada en la factoria que permite decidir la forma
 * en que se crearan los objetos
 */
template<class AbstractProduct>
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
   typedef AbstractProduct* (*CreatorType)();
   /** Tipo destructor de objetos */
   typedef void (*DestructorType)(AbstractProduct*&);
   /** Forma de obtener un objeto */
   /**
    * @param ObjectCreator Funtor que, ejecutado, retorna una instancia de la
    *        clase
    * @return Puntero a la nueva instancia de la clase abstracta AbstractProduct
    */
   static AbstractProduct* GetObject(CreatorType ObjectCreator) {
      return ObjectCreator();
   }
   ;
   /** Forma de descartar (destruir) un objeto */
   /**
    * @param ObjectDestructor Funtor que ejecutado con el parametro pClass, lo
    *        destruye.
    * @param pClass Instancia de la clase a destruir.
    */
   static void DisposeObject(DestructorType ObjectDestructor, AbstractProduct*&pClass) {
      ObjectDestructor(pClass);
      pClass = NULL;
   }
   ;
protected:
private:
};

/** Template Factoria de objetos */
/**
 * Factoria abstracta de clases que utiliza politicas para el manejo de error.
 *
 * Maneja una jerarquia de clases a partir de la base. Para cada nuevo tipo, se
 * registra un IdentifierType con su ProductCreator y ProductDestroyer
 * utilizados para identificar, crear y destruir instancias de las clases
 * derivadas.
 *
 * Maneja el error mediante FactoryErrorPolicy.
 *
 * \todo La factoria tiene la informacion para poder validar si un puntero
 *       creado con la misma es valido o no (fue destruido). Se puede agregar
 *       un metodo de validacion de los punteros.
 */
template<class AbstractProduct, class IdentifierType = std::string,
      class ProductCreator = AbstractProduct* (*)(), class ProductDestroyer = void(*)(
            AbstractProduct*&),
      template<class, class > class FactoryErrorPolicy = ReturnNullErrorPolicy>
class AbstractFactory : public FactoryErrorPolicy<AbstractProduct, IdentifierType> {
public:
   /** Friend el Singleton */
   friend class TSingleton<
         AbstractFactory<AbstractProduct, IdentifierType, ProductCreator,
               ProductDestroyer> > ;
   /** El vector donde se guardan los identificadores */
   typedef std::vector<IdentifierType> IdentifierVector;
   /** El tipo del identificadores */
   typedef IdentifierType Identifier;
   /** tipo de construtor default (sin parametros) */
   typedef typename AbstractProduct::DefaultCreatorType DefaultProductCreator;

// OPERADORES
// CONSULTA
   /** Regresa un vector de los ClassId registrados en la Factoria */
   const IdentifierVector& Enumerate();
// OPERACIONES
   /** Registra la clase y los Funtores de creacion y destruccion */
   bool Register(const IdentifierType &ClassId, ProductCreator Creator,
                 DefaultProductCreator DefaultCreator, ProductDestroyer Destructor);
   /** Elimina una clase de la factoria */
   bool UnRegister(const IdentifierType &ClassId);
   /** Crea una instancia de la clase ClassId */
   ProductCreator Create(const IdentifierType &ClassId);
   /** Crea una instancia de la clase ClassId (ctor default) */
   DefaultProductCreator DefaultCreate(const IdentifierType &ClassId);
   /** Elimina la instancia de la clase usando el destructor registrado */
   void Destroy(AbstractProduct* &pClassInstance);
   /** Registra la instancia creada */
   void RegisterCreatedInstance(AbstractProduct* &pClassInstance,
                                const IdentifierType &ClassId);
// ACCESO
protected:
private:
   /** Tipo donde se guardan los pares, id/objeto_creador */
   typedef std::map<const IdentifierType,
         std::pair<std::pair<ProductCreator, DefaultProductCreator>,
         ProductDestroyer> > CallbackMapType;
   /** Tipo donde se guardan los pares, instancia/destructor */
   typedef std::map<const AbstractProduct*, const IdentifierType> CreationMapType;
// CONSTRUCTORES/DESTRUCTOR
   /** ctor privado (patron singleton). */
   AbstractFactory() {
   }
   ;
   virtual ~AbstractFactory() {
   }
   ;
   /** ctor privado (patron singleton). */
   AbstractFactory(const AbstractFactory &);
   CallbackMapType createCallbacks_; /*! gurada los pares */
   CreationMapType createdObjects_; /*! guarda los punteros creados junto con */
   /* su destructor */
   IdentifierVector idVector_; /*! guarda los identificadores en el orden que */
   /* se agregan */
};

/* //////////////////////////// PUBLIC ////////////////////////////////// */
// ============================ OPERADORES ===============================
// =========================== OPERACIONES ===============================
// ============================= ACCESO ==================================
// ============================ CONSULTA =================================
/** Retorna la enumeracion de las clases registradas en el orden en que se registraron */
template<class AbstractProduct, class IdentifierType, class ProductCreator,
      class ProductDestroyer, template<class, class > class FactoryErrorPolicy>
const typename AbstractFactory<AbstractProduct, IdentifierType, ProductCreator,
      ProductDestroyer, FactoryErrorPolicy>::IdentifierVector&
AbstractFactory<AbstractProduct, IdentifierType, ProductCreator, ProductDestroyer,
      FactoryErrorPolicy>::Enumerate() {
   return idVector_;
}
// =========================== OPERACIONES ===============================
/**
 *  Registra un nuevo tipo en la factoria.
 *
 * @param[in] ClassId Identificador de la clase.
 * @param[in] Creator Funtor para crear la clase identificada
 * @param[in] DefaultCreator Funtor para crear la clase identificada usando la
 *        definicion de la misma clase. Equivalente al ctor default.
 * @param[in] Destructor Funtor para destruir la clase.
 * @return true Al poder registrar la clase.
 * @return false El identificador ya existe.
 */
template<class AbstractProduct, class IdentifierType, class ProductCreator,
      class ProductDestroyer, template<class, class > class FactoryErrorPolicy>
bool AbstractFactory<AbstractProduct, IdentifierType, ProductCreator, ProductDestroyer,
      FactoryErrorPolicy>::Register(
      const IdentifierType &ClassId, ProductCreator Creator,
      typename AbstractFactory::DefaultProductCreator DefaultCreator,
      ProductDestroyer Destructor) {
   bool result =
         createCallbacks_.insert(
               typename CallbackMapType::value_type(
                     ClassId,
                     std::make_pair(std::make_pair(Creator, DefaultCreator),
                                    Destructor))).second;
   if (result) {
      idVector_.push_back(ClassId);
   }
   return result;
}

/**
 *  Elimina del listado la clase identificada de manera que no es posible
 * seguir creandola a travez de la factoria.
 *
 * @param ClassId Identificador de la clase a eliminar de la factoria.
 * @return true ClassId habia sido registrado.
 * @return false ClassId no exitia como identificador registrado
 */
template<class AbstractProduct, class IdentifierType, class ProductCreator,
      class ProductDestroyer, template<class, class > class FactoryErrorPolicy>
bool AbstractFactory<AbstractProduct, IdentifierType, ProductCreator, ProductDestroyer,
      FactoryErrorPolicy>::UnRegister(const IdentifierType &ClassId) {
   return createCallbacks_.erase(ClassId) == 1;
}

/**
 *  Utiliza el Funtor de creacion por default (== ctor default)
 *
 * @param[in] ClassId Identificador de la clase que se desea crear.
 * @return Instancia de la clase
 * @return En caso de un error en la creacion, retorna
 *         FactoryErrorPolicy::OnCreateError
 */
template<class AbstractProduct, class IdentifierType, class ProductCreator,
      class ProductDestroyer, template<class, class > class FactoryErrorPolicy>
typename AbstractFactory<AbstractProduct, IdentifierType, ProductCreator,
      ProductDestroyer, FactoryErrorPolicy>::DefaultProductCreator AbstractFactory<
      AbstractProduct, IdentifierType, ProductCreator, ProductDestroyer,
      FactoryErrorPolicy>::DefaultCreate(const IdentifierType &ClassId) {
   typename CallbackMapType::const_iterator it = createCallbacks_.find(ClassId);
   if (it == createCallbacks_.end()) {
      return (typename AbstractFactory::DefaultProductCreator) (this->OnCreateError(ClassId));
   }

   // Llamo a la funcion de creado.
   return it->second.first.second;
}

/**
 *  Utiliza el Funtor de creacion de la clase
 *
 * @param[in] ClassId Identificador de la clase que se desea crear.
 * @return Instancia de la clase
 * @return En caso de un error en la creacion, retorna
 *         FactoryErrorPolicy::OnCreateError
 */
template<class AbstractProduct, class IdentifierType, class ProductCreator,
      class ProductDestroyer, template<class, class > class FactoryErrorPolicy>
ProductCreator AbstractFactory<AbstractProduct, IdentifierType, ProductCreator,
      ProductDestroyer, FactoryErrorPolicy>::Create(const IdentifierType &ClassId) {
   typename CallbackMapType::const_iterator it = createCallbacks_.find(ClassId);
   if (it == createCallbacks_.end())
// Por analogia deberia se asi, pero genera errores al compilar. \todo ver por que
#ifdef __UNUSED_CODE__
         {  return (typename AbstractFactory::DefaultProductCreator)(OnCreateError(ClassId) );}
#else
         {
      return this->OnCreateError(ClassId);
   }
#endif   // __UNUSED_CODE__
   // Llamo a la funcion de creado.
   return it->second.first.first;
}

/**
 *  Destruye la instancia de la clase y pone el puntero en NULL.
 *
 * \post Se modifica el valor de pClassInstance a NULL.
 * \todo Llamar a OnDestroyError()
 * @param[in] pClassInstance Instancia de la clase a eliminar
 */
template<class AbstractProduct, class IdentifierType, class ProductCreator,
      class ProductDestroyer, template<class, class > class FactoryErrorPolicy>
void AbstractFactory<AbstractProduct, IdentifierType, ProductCreator, ProductDestroyer,
      FactoryErrorPolicy>::Destroy(AbstractProduct* &pClassInstance) {
   typename CreationMapType::const_iterator it = createdObjects_.find(pClassInstance);
   if (it == createdObjects_.end()) {
   }   // OnDestroyError();
   else {
      typename CallbackMapType::const_iterator cbit = createCallbacks_.find(it->second);
      if (cbit != createCallbacks_.end()) {
         /* Llamo a la funcion de destruccion.*/
         cbit->second.second(pClassInstance);
      }
   }
   // Elimino la instancia del mapa de objetos creados
   createdObjects_.erase(pClassInstance);
   pClassInstance = NULL;
}

template<class AbstractProduct, class IdentifierType, class ProductCreator,
      class ProductDestroyer, template<class, class > class FactoryErrorPolicy>
void AbstractFactory<AbstractProduct, IdentifierType, ProductCreator, ProductDestroyer,
      FactoryErrorPolicy>::RegisterCreatedInstance(AbstractProduct* &pClassInstance,
                                                   const IdentifierType &ClassId) {
   if (pClassInstance) {
      createdObjects_.insert(std::make_pair(pClassInstance, ClassId));
   }
}

/* ////////////////////////// PROTECTED  //////////////////////////////// */
/* ////////////////////////// PRIVATE    //////////////////////////////// */
/* ////////////////////////// MACROS DE IMPLEMENTACION ///////////////// */
/** Implementa los metodos comunes de las clases que utilizan la factoria */
/**
 *  Implementa los metodos estaticos de la clase base :
 *   - Register
 *   - Unregister
 *   - Destroy
 *   - Enumerate
 *   - Create
 * param[in] Class Clase base sobre la cual se implementan las funciones.
 * param[in] Count Cantidad de parametros pasados.
 * param[in] ParameterDeclaration Parametros variadicos (cantidad indefinida)
 *           con los parametros que se le pasan a Create.
 */
#define HAS_FACTORY_IMPLEMENTATION_COMMON(Class, Count, ParameterDeclaration ...) \
   typedef AbstractFactory<Class, Class::IdentifierType,                       \
                           Class::CreatorType,                                \
                           Class::DestructorType> Class ## Factory;           \
                                                                              \
   bool Class::Register(const IdentifierType &Type, CreatorType Creator,       \
                        Class ## Factory::DefaultProductCreator   \
                        DefaultCreator,   \
                        DestructorType Destructor) { \
      return TSingleton< Class ## Factory >::Instance().                      \
             Register(Type, Creator, DefaultCreator, Destructor); } \
                                                                              \
   bool Class::UnRegister(const IdentifierType &Type) {                        \
      return TSingleton< Class ## Factory >::Instance().UnRegister(Type); }    \
                                                                              \
   void Class::Destroy(Class* &pClass) {                                       \
      TSingleton< Class ## Factory >::Instance().Destroy(pClass); }            \
                                                                              \
   const std::vector<Class::IdentifierType>& Class::Enumerate() {              \
      return TSingleton< Class ## Factory >::Instance().Enumerate(); }         \
                                                                              \
   Class* Class::Create(const IdentifierType &Type, ## ParameterDeclaration) { \
      Class *pinstance = TSingleton< Class ## Factory >::Instance().          \
                         Create(Type) (ImplementParameters_ ## Count() );          \
      if ( pinstance ) {                                                        \
         pinstance->classId_ = Type;                                          \
         TSingleton< Class ## Factory >::Instance().                          \
         RegisterCreatedInstance(pinstance, Type); } \
      return pinstance; }

/** Implementa los metodos Create extra */
/**
 *  Utiliza DefaultCreate para obtener una instancia y luego ejecuta Initialize
 * con los parametros pasados.
 *
 * param[in] Class Clase base sobre la cual se implementan las funciones.
 * param[in] Count Cantidad de parametros pasados.
 * param[in] ParameterDeclaration Parametros variadicos (cantidad indefinida)
 *           con los parametros que se le pasan a Initialize.
 *
 */
#define HAS_FACTORY_IMPLEMENTATION(Class, Count, ParameterDeclaration ...)       \
   Class* Class::Create(const IdentifierType &Type, ## ParameterDeclaration) {   \
      Class *pinstance = TSingleton< Class ## Factory >::                        \
                         Instance().DefaultCreate(Type) ();                      \
      if (pinstance) pinstance->classId_ = Type;                                 \
      if (!pinstance || pinstance->Initialize(                                   \
                                ImplementParameters_ ## Count()) != pinstance) { \
         TSingleton< Class ## Factory >::Instance().Destroy(pinstance); }        \
      TSingleton< Class ## Factory >::Instance().                                \
                     RegisterCreatedInstance(pinstance, Type);                   \
      return pinstance; }

#endif   // ABSTRACTFACTORY_H_
