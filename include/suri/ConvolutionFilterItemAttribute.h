/*! \file ConvolutionFilterItemAttribute.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CONVOLUTIONFILTERITEMATTRIBUTE_H_
#define CONVOLUTIONFILTERITEMATTRIBUTE_H_

// Includes Estandar
// Includes Suri
#include "suri/ComplexItemAttribute.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class ConvolutionFilterItemAttribute : public suri::ComplexItemAttribute {
public:
   /* Constructor */
   ConvolutionFilterItemAttribute(const std::string& Name,
                                  const std::string& Value,
                                  int Dimension);
   /* Destructor */
   virtual ~ConvolutionFilterItemAttribute();
   /* Devuelve los valores de la matriz */
   double GetConvolutionFilterMatrixValues(int x, int y) const;
   /* Setea los valores de la matriz */
   void SetConvolutionFilterMatrixValues(int x, int y, double Value);
   /* Setea el valor nominal */
   void SetNominalValue(int num);
   /* Devuelve el valor nominal */
   int GetNominalValue();

private:
   int dimension_;
};

} /** namespace suri */

#endif /* CONVOLUTIONFILTERITEMATTRIBUTE_H_ */
