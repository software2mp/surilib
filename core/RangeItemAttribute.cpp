/*! \file RangeItemAttribute.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "suri/RangeItemAttribute.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
// Defines
#define NEG_INF_STR "-inf"
#define INF_STR "inf"

// forwards

namespace suri {

/**
 * Constructor
 * @param Name nombre del atributo
 * @param RangeName nombre del rango
 * @param Value valor del atributo
 */
RangeItemAttribute::RangeItemAttribute(const std::string& Name, const std::string& Value) :
      ComplexItemAttribute(Name, Value) {
}

/** Destructor **/
RangeItemAttribute::~RangeItemAttribute() {
}

/** Configura el valor de inicio del rango */
void RangeItemAttribute::SetStartValue(const double Value) {
   std::string strvalue = NumberToString<double>(Value);
   RangeDoubleToString(strvalue);
   attributes_[StartValuePosition]->SetValue(strvalue);
}

/** Configura el valor de fin del rango*/
void RangeItemAttribute::SetFinishValue(const double Value) {
   std::string strvalue = NumberToString<double>(Value);
   RangeDoubleToString(strvalue);
   attributes_[FinishValuePosition]->SetValue(strvalue);
}

/** Configura el valor del color del rango en hexa */
void RangeItemAttribute::SetHexaColor(const std::string& HexaColor) {
   attributes_[HexaColorPosition]->SetValue(HexaColor);
}

/** Devuelve el nombre del rango **/
std::string RangeItemAttribute::GetRangeName() const {
   return (attributes_.size() > 0) ? attributes_[RangeNamePosition]->GetValue() : "";
}

/** Devuelve el valor de inicio del rango*/
double RangeItemAttribute::GetStartValue() const {
   return
         (attributes_.size() > 1) ? RangeDoubleToString(
                                          attributes_[StartValuePosition]->GetValue()) :
                                    0;
}

/** Devuelve el valor de fin del rango*/
double RangeItemAttribute::GetFinishValue() const {
   return
         (attributes_.size() > 2) ? RangeDoubleToString(
                                          attributes_[FinishValuePosition]->GetValue()) :
                                    0;
}

/** Devuelve el valor del color del rango en hexa */
std::string RangeItemAttribute::GetHexaColor() const {
   return (attributes_.size() > 3) ? attributes_[HexaColorPosition]->GetValue() : "";
}

/**
 * Convierte un string que contiene un double teniendo en cuenta que los strings con inf
 * @param DoubleString string que representa un double
 * @return double asociado
 */
double RangeItemAttribute::RangeDoubleToString(const std::string& DoubleString) {
   double value = 0;
   bool inf = DoubleString.find(INF_STR) != std::string::npos;
   bool neginf = DoubleString.find(NEG_INF_STR) != std::string::npos;
   if (!inf && !neginf) {
      value = StringToNumber<double>(DoubleString);
   } else {
      value = std::numeric_limits<double>::infinity();
      if (neginf)
         value *= -1;
   }
   return value;
}
} /** namespace suri */
