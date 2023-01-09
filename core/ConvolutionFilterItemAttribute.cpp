/*! \file ConvolutionFilterItemAttribute.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "suri/AuxiliaryFunctions.h"
#include "suri/ConvolutionFilterItemAttribute.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/* Constructor */
ConvolutionFilterItemAttribute::ConvolutionFilterItemAttribute(const std::string& Name,
                                                               const std::string& Value,
                                                               int Dimension) :
      ComplexItemAttribute(Name, Value), dimension_(Dimension) {
}
/* Destructor */
ConvolutionFilterItemAttribute::~ConvolutionFilterItemAttribute() {
   // TODO Auto-generated destructor stub
}
/* Devuelve los valores de la matriz */
double ConvolutionFilterItemAttribute::GetConvolutionFilterMatrixValues(int x, int y) const {
   int Index = x + (y*dimension_);
   return StringToNumber<double>(attributes_[Index+1]->GetValue());
}
/* Setea los valores de la matriz */
void ConvolutionFilterItemAttribute::SetConvolutionFilterMatrixValues(int x, int y, double Value) {
   int Index = x*y;
   std::string strval = NumberToString<double>(Value);
   attributes_[Index]->SetValue(strval);
}
/* Setea el valor nominal */
void ConvolutionFilterItemAttribute::SetNominalValue(int Value) {
   std::string strval = NumberToString<int>(Value);
   attributes_[0]->SetValue(strval);
}
/* Devuelve el valor nominal */
int ConvolutionFilterItemAttribute::GetNominalValue() {
   return StringToNumber<int>(attributes_[0]->GetValue());
}
} /** namespace suri */
