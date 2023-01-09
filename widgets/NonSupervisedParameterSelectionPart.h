/*! \file NonSupervisedParameterSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
// Includes Suri
#include "ClassificationPart.h"
#include "suri/StatisticsFunctions.h"
#include "ClassificationAlgorithmInterface.h"
// Includes Wx
// Defines

#ifndef NONSUPERVISEDPARAMETERSELECTIONPART_H_
#define NONSUPERVISEDPARAMETERSELECTIONPART_H_

#include "ParametersSelectionPart.h"

namespace suri {
/** Factory para almacenar algoritmos de clasificacion no supervisada */
/**
 * Factory para almacenar algoritmos de clasificacion no supervisada.
 * Todos los algoritmos de clasif no supervisada deberan heredar
 * y se registraran en esta
 *
 */
class NonSupervisedParameterSelectionPart : public suri::ParametersSelectionPart {
public:
   /** Macro que declara metodos de la clase */
HAS_FACTORY(NonSupervisedParameterSelectionPart, std::string);
public:

   /** Ctor */
   NonSupervisedParameterSelectionPart();
   /** Dtor */
   virtual ~NonSupervisedParameterSelectionPart();

   /** Elimina todas las clases cargadas. */
   virtual bool Reset()=0;
   /** Retorna el nombre del algoritmo que se usara para clasificar */
   virtual std::string GetAlgorithmName()=0;
   /** Agrega clases que se usan para clasificar */
   virtual bool AddClass(int ClassId, Statistics Stats)=0;
   /** Retorna un puntero al clasificador creado por part */
   virtual ClassificationAlgorithmInterface* GetClassificationManager()=0;
   /** Retorna la cantidad de clases */
   virtual int GetClassCount()=0;
   /** Retorna las propiedades de la clase en la posicion indicada. Las Posiciones van de 0 a */
// GetClassCount()-1.
   virtual bool GetClassProperties(ClassInformation &Properties)=0;

#ifdef NOT_SUPERVISED_CODE
   /** Informa si el part necesita que se configuren las clases. */
   virtual bool NeedClasses() {
      return true;
   }
#endif

protected:

private:
};
}

#endif /* NONSUPERVISEDPARAMETERSELECTIONPART_H_ */
