/*! \file Class.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CLASS_H_
#define CLASS_H_

#include <string>

#include "suri/SuriObject.h"
#include "suri/VectorStyle.h"

namespace suri {

/**
 * Informacion sobre una clase de un proceso de clasificacion
 */
class ClassInformation {
public:
   /**
    * Constructor.
    */
   ClassInformation(
         int Index = NoClassIndex,
         const std::string& Name = NoClassName,
         const VectorStyle::Color& Color = VectorStyle::Color(NoClassRedColor,
                                                              NoClassGreenColor,
                                                              NoClassBlueColor,
                                                              NoClassAlphaColor),
         const SuriObject::UuidType& Id = SuriObject::NullClassId);

   /**
    * Destructor.
    */
   virtual ~ClassInformation();

   /** Tipo de Dato del Indice */
   typedef int IndexType;

   /** Obtiene el indice de la clase */
   IndexType GetIndex() const;

   /** Obtiene el nombre de la clase */
   std::string GetName() const;

   /** Obtiene el Id del Datasource */
   SuriObject::UuidType GetDatasourceId();

   /** Obtiene el color de la clase */
   VectorStyle::Color GetColor();

   /** Setea el nombre de la clase */
   void SetName(const std::string& Name);

   /** Setea el color de la clase */
   void SetColor(const VectorStyle::Color& Color);

   /** Determina si dos conjuntos de informacion de clases son iguales */
   bool Equals(ClassInformation& ClassInfo);

   /** Tipo de clase "Sin Clase" */
   static const int InvalidClassIndex = 0;

   /** Tipo de clase "Valor no valido" */
   static const std::string NDVName;

   /** Indice de la clase "Valor no valido" */
   static const int NDVIndex;

   /** Tipo de clase "Sin Clase" */
   static const std::string NoClassName;

   /** Indice de la clase "Sin Clase" */
   static const int NoClassIndex;

   /** Color Rojo de la clase "Sin Clase" */
   static const unsigned char NoClassRedColor;

   /** Color Azul de la clase "Sin Clase" */
   static const unsigned char NoClassBlueColor;

   /** Color Verde de la clase "Sin Clase" */
   static const unsigned char NoClassGreenColor;

   /** Color Alfa de la clase "Sin Clase" */
   static const unsigned char NoClassAlphaColor;

   /** Nombre por defecto para las clases con indices no encontrados */
   static const std::string DefaultClassNotFoundName;

private:
   /** Setea el indice de la clase */
   void SetIndex(IndexType Index);

   /** Cambia el Id del recinto */
   void SetDatasourceId(const SuriObject::UuidType& Id);

   /** Nomre de la clase */
   std::string name_;

   /** Indice de la clase */
   IndexType index_;

   /** Id del recinto que la origino */
   SuriObject::UuidType id_;

   /** Color de la clase */
   VectorStyle::Color color_;
};

} /** namespace suri */

#endif /* CLASS_H_ */
