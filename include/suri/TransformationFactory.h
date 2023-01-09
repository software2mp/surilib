/** \file TransformationFactory.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TRANSFORMATIONFACTORY_H_
#define TRANSFORMATIONFACTORY_H_

#include <string>

#include "suri/ParameterCollection.h"
#include "suri/CoordinatesTransformation.h"

namespace suri {

/**
 * Es la clase base donde se declara el metodo "Create" para la creacion de
 * objetos del tipo CoordinatesTransformation. Al mismo tiempo mantiene una
 * referencia a un sucesor para la cadena de responsabilidad.
 */
class TransformationFactory {
public:
   /**
    * Tipos de transformaciones disponibles.
    */
   static const std::string kExact;
   static const std::string kPolynomial;
   static const std::string kTriangulation;

   /**
    * Nombres de parametros.
    */
   static const std::string kParamWktIn;
   static const std::string kParamWktOut;
   static const std::string kParamGcps;
   static const std::string kParamOrder;

   /**
    * Destructor (elimina al sucesor)
    */
   virtual ~TransformationFactory();

   /**
    * Metodo a implementar por las clases de tranformaciones concretas para
    * crear una instancia concreta de transformacion.
    * @param[in] Type Tipo de transformacion que se desea crear.
    * @param[in] Params Parametros necesarios para la creacion de cada transformacion.
    * @return Devuelve una instancia a una transformacion de coordenadas.
    */
   virtual CoordinatesTransformation* Create(const std::string& Type,
                                             const suri::ParameterCollection& ParamsP) = 0;

protected:
   /**
    * Constructor protegido para evitar que se pueda crear un TransformFactory
    * pero que al mismo tiempo pueda ser utilizado por las clases que heredan para
    * configurar al sucesor.
    */
   explicit TransformationFactory(TransformationFactory* pSuccessor);

   TransformationFactory* pSuccessor_; /** Sucesor en la cadena de responsabilidad de creacion. */

   /**
    * Devuelve al sucesor.
    */
   inline TransformationFactory* GetSuccessor() const {
      return pSuccessor_;
   }

   /**
    * Configura al sucesor de esta factoria.
    */
   inline void SetSuccessor(TransformationFactory* pSuccessor) {
      pSuccessor_ = pSuccessor;
   }
};

}  // namespace suri

#endif  // TRANSFORMATIONFACTORY_H_
