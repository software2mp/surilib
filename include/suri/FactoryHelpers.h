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

#ifndef FACTORYHELPERS_H_
#define FACTORYHELPERS_H_

// Includes standard
#include <vector>
#include <string>

/** Declara el metodo Create (estatico) e Initialize para uso por la factoria */
/**
 * Esta Macro debe ir en la parte privada de la clase Class
 * @param Class Nombre de la clase en la que se declaran los metodos.
 * @param ConstructorParameters Parametros para el constructor de la clase.
 */
#define DECLARE_CREATE(Class, ConstructorParameters ...)                        \
public:                                                                    \
   /*! \brief Metodo estatico que crea una instancia de la clase a partir de su identificador.*/  \
   static Class *Create(const IdentifierType &, ## ConstructorParameters);   \
private:                                                                   \
   /*! \brief Inicializa la instancia de la clase y retorna NULL si hubo error, this para exito.*/\
   virtual Class *Initialize(ConstructorParameters)

/** Declara las entidades necesarias para que una clase base funcione como factoria */
/**
 * Esta Macro debe ir en la parte publica de la clase Class
 * @param Class Nombre de la clase
 * @param IdType tipo de dato utilizado para el identificador
 * @param ConstructorParameters Parametros variadicos del constructor de la clase
 */
#define HAS_FACTORY(Class, IdType, ConstructorParameters ...)                    \
private:                                                                   \
   Class(const Class &);                                                    \
public:                                                                    \
   typedef IdType IdentifierType;                                          \
private:                                                                   \
   IdentifierType classId_;                                                \
public:                                                                    \
   typedef std::vector<IdentifierType> EnumerationType;                    \
   typedef Class* (*CreatorType)(ConstructorParameters);                   \
   typedef Class* (*DefaultCreatorType)();                                 \
   typedef void (*DestructorType)(Class* &);                              \
                                                                              \
   const IdentifierType &GetClassId() const {return classId_; }               \
   static bool Register(const IdentifierType &, CreatorType,                 \
                        DefaultCreatorType, DestructorType); \
   static bool UnRegister(const IdentifierType &);                          \
   static void Destroy(Class*&);                                           \
   static const EnumerationType& Enumerate();                              \
   static Class *Create(const IdentifierType &, ## ConstructorParameters)

// ---------------------------------------------------------------------------
//                 TEMPLATES PARA CREACION / DESTRUCCION
// ---------------------------------------------------------------------------
/** Implementa el metodo de creacion de la clase concreta que se le pasa a la factoria */
/**
 * Implementa 3 funciones:
 *  - CreateConcreteProduct que llama al ctor que recibe parametros de ConcreteProduct.
 *  - DefaultCreateConcreteProduct que llama al ctor default de ConcreteProduct.
 *  - DestroyConcreteProduct que realiza la destruccion de la instancia.
 * @param AbstractProduct Clase base de la jerarquia que se esta factorizando
 * @param ConcreteProduct Clase concreta que se esta registrando
 * @param Count Cantidad de parametros
 * @param Parameters Parametros variadicos.
 * \TODO(Javier - Sin TCK): Agregar "pClass = NULL" luego de delete pClass; en Destroy.
 */
#define CLASS_CREATE(AbstractProduct, ConcreteProduct, Count, Parameters ...)     \
/*! \brief Crea la clase con ctor especifico. */                                                   \
   template<class TAbstractProduct, class TConcreteProduct>                       \
   TAbstractProduct *Create ## ConcreteProduct(                                  \
      DeclareParameters_ ## Count(Parameters) ) { \
      return new TConcreteProduct(ImplementParameters_ ## Count() ); }           \
/*! \brief Crea la clase con el ctor default. */                                                                              \
   template<class TAbstractProduct, class TConcreteProduct>                       \
   TAbstractProduct *DefaultCreate ## ConcreteProduct() {                        \
      return new TConcreteProduct(); }                                           \
/*! \brief Destruye la clase. */                                              \
   template<class TAbstractProduct, class TConcreteProduct>                       \
   void Destroy ## ConcreteProduct(TAbstractProduct* &pClass) {                  \
      delete pClass; }

/** Registra una clase en la factoria (asume que IdentifierType es un const char[]) */
/**
 * Llama al metodo Register de la clase abstracta con los parametros de creacion.
 * @param AbstractProduct Clase base de la jerarquia que se esta factorizando
 * @param ConcreteProduct Clase concreta que se esta registrando
 * @param Count Cantidad de parametros
 * @param Parameters Parametros variadicos.
 */
#define REGISTER_CLASS(AbstractProduct, ConcreteProduct, Count, Parameters ...)   \
   AbstractProduct::Register(# ConcreteProduct,                                \
                             Create ## ConcreteProduct<AbstractProduct, ConcreteProduct>,       \
                             DefaultCreate ## ConcreteProduct<AbstractProduct, ConcreteProduct>, \
                             Destroy ## ConcreteProduct<AbstractProduct, ConcreteProduct>)

/** Registra en forma automatica la clase concreta usando el ctor de una clase dummy */
/**
 * Crea un namespace anonimo donde delcara una clase en cuyo constructor se realiza
 * la registracion de la clase ConcreteProduct en la factoria de AbstractProduct.
 *
 * Luego declara una variable de tipo global para que durante la inicializacion
 * de la aplicacion se ejecute el ctor y realice la registracion.
 * \attention Para que funcione esta macro es necesario que los constructores de
 * la clase registrada esten en el .cpp. No deben declararse en el
 * archivo .h ya que sino el linker omitira el codigo de la clase al compilar
 * en modo release.
 * @param AbstractProduct Clase base de la jerarquia que se esta factorizando
 * @param ConcreteProduct Clase concreta que se esta registrando
 * @param Count Cantidad de parametros
 * @param Parameters Parametros variadicos.
 */
#define AUTO_REGISTER_CLASS(AbstractProduct, ConcreteProduct, Count, Parameters ...) \
   namespace {                                                                \
   CLASS_CREATE(AbstractProduct, ConcreteProduct, Count, Parameters)             \
   class _dummy_ ## ConcreteProduct {                                      \
public:                                                                 \
   _dummy_ ## ConcreteProduct() {REGISTER_CLASS(AbstractProduct,         \
                                                ConcreteProduct, Count, Parameters); } \
   ~_dummy_ ## ConcreteProduct() {}                                      \
   }; _dummy_ ## ConcreteProduct dummy ## ConcreteProduct; }

// --------------------------------------------------------------------------
//                MACROS QUE INTERACTUAN CON LOS PARAMETROS
// --------------------------------------------------------------------------
/** Concatena Value con el parametro Order para generar el nombre de un parametro en dicha posicion */
// (Order)
#define ValueName(ParameterType, Order) Value ## Order
/** Declara un parametro para un tipo y posicion dadas */
#define ParameterValue(ParameterType, Order) ParameterType ValueName(, Order)
/** Absorbe Order dejando solo el tipo. Se usa para declaraciones donde no es necesario el nombre */
#define NullJoin(ParameterType, Order) ParameterType
/** Declara 0 parametros */
#define DeclareParameters_0()
/** Declara 1 parametro usando ParameterValue */
#define DeclareParameters_1(ParameterType)                                    \
   Parameters_1(ParameterType, ParameterValue)
/** Delcara 2 parametros usando ParameterValue */
#define DeclareParameters_2(ParameterType1, ParameterType2)                    \
   Parameters_2(ParameterType1, ParameterType2, ParameterValue)
/** Delcara 3 parametros usando ParameterValue */
#define DeclareParameters_3(ParameterType1, ParameterType2, ParameterType3)     \
   Parameters_3(ParameterType1, ParameterType2, ParameterType3,        \
                ParameterValue)
/** Delcara 4 parametros usando ParameterValue */
#define DeclareParameters_4(ParameterType1, ParameterType2, ParameterType3,     \
                            ParameterType4)                                             \
   Parameters_4(ParameterType1, ParameterType2, ParameterType3,        \
                ParameterType4, ParameterValue)
/** Delcara 5 parametros usando ParameterValue */
#define DeclareParameters_5(ParameterType1, ParameterType2, ParameterType3,     \
                            ParameterType4, ParameterType5)                              \
   Parameters_5(ParameterType1, ParameterType2, ParameterType3,        \
                ParameterType4, ParameterType5, ParameterValue)
/** Delcara 6 parametros usando ParameterValue */
#define DeclareParameters_6(ParameterType1, ParameterType2, ParameterType3,     \
                            ParameterType4, ParameterType5, ParameterType6)               \
   Parameters_6(ParameterType1, ParameterType2, ParameterType3,        \
                ParameterType4, ParameterType5, ParameterType6, ParameterValue)
/** Implementa 0 parametros */
#define ImplementParameters_0()
/** Implementa 1 parametro usando ValueName */
/**
 * Utiliza la Macro Parameters_# correspondiente sin tipos y utilizando como
 * macro Join la que solo genera el nombre a partir del orden.
 */
#define ImplementParameters_1() Parameters_1(, ValueName)
/** Implementa 2 parametro usando ValueName */
/**
 * Utiliza la Macro Parameters_# correspondiente sin tipos y utilizando como
 * macro Join la que solo genera el nombre a partir del orden.
 */
#define ImplementParameters_2() Parameters_2(, , ValueName)
/** Implementa 3 parametro usando ValueName */
/**
 * Utiliza la Macro Parameters_# correspondiente sin tipos y utilizando como
 * macro Join la que solo genera el nombre a partir del orden.
 */
#define ImplementParameters_3() Parameters_3(, , , ValueName)
/** Implementa 4 parametro usando ValueName */
/**
 * Utiliza la Macro Parameters_# correspondiente sin tipos y utilizando como
 * macro Join la que solo genera el nombre a partir del orden.
 */
#define ImplementParameters_4() Parameters_4(, , , , ValueName)
/** Implementa 5 parametro usando ValueName */
/**
 * Utiliza la Macro Parameters_# correspondiente sin tipos y utilizando como
 * macro Join la que solo genera el nombre a partir del orden.
 */
#define ImplementParameters_5() Parameters_5(, , , , , ValueName)
/** Implementa 6 parametro usando ValueName */
/**
 * Utiliza la Macro Parameters_# correspondiente sin tipos y utilizando como
 * macro Join la que solo genera el nombre a partir del orden.
 */
#define ImplementParameters_6() Parameters_6(, , , , , , ValueName)

/** Genera 0 parametros */
#define Parameters_0()
/** Genera 1 parametro dependiendo de ParameterJoin (para declarar o definir) */
/**
 * @param ParameterType Tipo para el parametro
 * @param ParameterJoin Macro que realiza la union de parametros utilizando el orden
 */
#define Parameters_1(ParameterType, ParameterJoin) ParameterJoin(ParameterType, 1)
/** Genera 2 parametros dependiendo de ParameterJoin (para declarar o definir) */
/**
 * Utiliza 2 veces ParameterJoin para generar la declaracion de dos parametros
 * @param ParameterType1 Tipo del primer parametro
 * @param ParameterType2 Tipo del segundo parametro
 * @param ParameterJoin Macro a utilizar para generar el tipo y nombre del parametro
 */
#define Parameters_2(ParameterType1, ParameterType2, ParameterJoin)             \
   ParameterJoin(ParameterType1, 1), ParameterJoin(ParameterType2, 2)
/** Genera 3 parametros dependiendo de ParameterJoin (para declarar o definir) */
/**
 * Utiliza Parameters_2 para generar los primeros dos parametros y luego
 * ParameterJoin para generar el tercero.
 * @param ParameterType1 Tipo del primer parametro
 * @param ParameterType2 Tipo del segundo parametro
 * @param ParameterType3 Tipo del tercer parametro
 * @param ParameterJoin Macro a utilizar para generar el tipo y nombre del parametro
 */
#define Parameters_3(ParameterType1, ParameterType2, ParameterType3,            \
                     ParameterJoin)                                           \
   Parameters_2(ParameterType1, ParameterType2, ParameterJoin),     \
   ParameterJoin(ParameterType3, 3)
/** Genera 4 parametros dependiendo de ParameterJoin (para declarar o definir) */
/**
 * Utiliza Parameters_3 para generar los primeros tres parametros y luego
 * ParameterJoin para generar el cuarto.
 * @param ParameterType1 Tipo del primer parametro
 * @param ParameterType2 Tipo del segundo parametro
 * @param ParameterType3 Tipo del tercer parametro
 * @param ParameterType4 Tipo del cuarto parametro
 * @param ParameterJoin Macro a utilizar para generar el tipo y nombre del parametro
 */
#define Parameters_4(ParameterType1, ParameterType2, ParameterType3,            \
                     ParameterType4, ParameterJoin)                            \
   Parameters_3(ParameterType1, ParameterType2, ParameterType3,     \
                ParameterJoin),   \
   ParameterJoin(ParameterType4, 4)
/** Genera 5 parametros dependiendo de ParameterJoin (para declarar o definir) */
/**
 * Utiliza Parameters_4 para generar los primeros cuatro parametros y luego
 * ParameterJoin para generar el quinto.
 * @param ParameterType1 Tipo del primer parametro
 * @param ParameterType2 Tipo del segundo parametro
 * @param ParameterType3 Tipo del tercer parametro
 * @param ParameterType4 Tipo del cuarto parametro
 * @param ParameterType5 Tipo del quinto parametro
 * @param ParameterJoin Macro a utilizar para generar el tipo y nombre del parametro
 */
#define Parameters_5(ParameterType1, ParameterType2, ParameterType3,            \
                     ParameterType4, ParameterType5, ParameterJoin)             \
   Parameters_4(ParameterType1, ParameterType2, ParameterType3,     \
                ParameterType4, ParameterJoin),                           \
   ParameterJoin(ParameterType5, 5)
/** Genera 6 parametros dependiendo de ParameterJoin (para declarar o definir) */
/**
 * Utiliza Parameters_5 para generar los primeros cinco parametros y luego
 * ParameterJoin para generar el sexto.
 *
 * La progresion es clara, si se necesitaran mas parametros en las funciones de
 * Factoria, es necesario extender estas macros para generar las declaraciones.
 * @param ParameterType1 Tipo del primer parametro
 * @param ParameterType2 Tipo del segundo parametro
 * @param ParameterType3 Tipo del tercer parametro
 * @param ParameterType4 Tipo del cuarto parametro
 * @param ParameterType5 Tipo del quinto parametro
 * @param ParameterType6 Tipo del sexto parametro
 * @param ParameterJoin Macro a utilizar para generar el tipo y nombre del parametro
 */
#define Parameters_6(ParameterType1, ParameterType2, ParameterType3,            \
                     ParameterType4, ParameterType5, ParameterType6,            \
                     ParameterJoin)                                           \
   Parameters_5(ParameterType1, ParameterType2, ParameterType3,     \
                ParameterType4, ParameterType5, ParameterJoin),            \
   ParameterJoin(ParameterType6, 6)

// --------------------------------------------------------------------------
//                MACROS QUE INTERACTUAN CON LOS PARAMETROS
// --------------------------------------------------------------------------

#endif /* FACTORYHELPERS_H_ */
