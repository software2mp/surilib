/*! \file RangeItemAttribute.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RANGEITEMATTRIBUTE_H_
#define RANGEITEMATTRIBUTE_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "ComplexItemAttribute.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 *  Atributo para item de biblioteca de tabla de intervalos.
 *  Representa un rango que viene definido por el nombre del rango,
 *  el valor de inicio y el valor de fin del mismo.
 */
class RangeItemAttribute : public suri::ComplexItemAttribute {
   /** ctor copia **/
   RangeItemAttribute(const RangeItemAttribute&);

public:
   /** constructor **/
   explicit RangeItemAttribute(const std::string& Name, const std::string& Value);
   /** destructor **/
   virtual ~RangeItemAttribute();
   /** Configura el valor de inicio del rango*/
   void SetStartValue(const double Value);
   /** Configura el valor de fin del rango*/
   void SetFinishValue(const double Value);
   /** Configura el valor del color del rango en hexa */
   void SetHexaColor(const std::string& HexaColor);
   /** Devuelve el nombre del rango **/
   std::string GetRangeName() const;
   /** Devuelve el valor de inicio del rango*/
   double GetStartValue() const;
   /** Devuelve el valor de fin del rango*/
   double GetFinishValue() const;
   /** Devuelve el valor del color del rango en hexa */
   std::string GetHexaColor() const;
   /** Enum con la definicion de posiciones del valores de atributos dentro del rango **/
   typedef enum {
      RangeNamePosition = 0,
      StartValuePosition = 1,
      FinishValuePosition = 2,
      HexaColorPosition = 3
   } AtributeValuesPosition;
   /**
    * Convierte un string que contiene un double teniendo en cuenta que los strings con inf
    * @param DoubleString string que representa un double
    * @return double asociado
    */
   static double RangeDoubleToString(const std::string& DoubleString);

private:
   /** nombre del rango **/
   std::string rangeName_;
   // TODO: Si se replica lo de spectral no hace falta los atributos.
   /** valor de inicio del rango */
   double startValue_;
   /** valor de fin del rango **/
   double finishValue_;
   /** color del rango en hexa **/
   std::string hexaColor_;
};

} /** namespace suri */

#endif /* RANGEITEMATTRIBUTE_H_ */
