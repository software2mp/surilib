/*! \file SpectralSignItemAttribute.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SPECTRALSIGNITEMATTRIBUTE_H_
#define SPECTRALSIGNITEMATTRIBUTE_H_

// Includes Estandar
// Includes Suri
#include "ComplexItemAttribute.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 *  Atributo para item de biblioteca de tabla de intervalos.
 *  Representa una firma espectral que viene definida por longitud de onda,
 *  amplitud y reflectancia.
 */
class SpectralSignItemAttribute : public suri::ComplexItemAttribute {
public:
   /** Constructor */
   explicit SpectralSignItemAttribute(const std::string& Name, const std::string& Value);
   /** Destructor */
   virtual ~SpectralSignItemAttribute();
   /** Enum con la definicion de posiciones del valores de atributos dentro del rango **/
   typedef enum {
      WavelengthValuePosition = 0,
      AmplitudeValuePosition = 1,
      RelevanceValuePosition = 2,
      WavelengthTypePosition = 3,
      MagnitudeTypePosition = 4
   } AtributeValuesPosition;
   /** Configura el valor de longitud de onda */
   void SetWavelengthValue(const double Value);
   /** Configura el valor de amplitud */
   void SetAmplitudeValue(const double Value);
   /** Configura el valor de relevancia */
   void SetReflectanceValue(const double Value);
   /** Configura el tipo de longitud de onda */
   void SetWavelengthType(const std::string Value);
   /** Configura el tipo de magnitud */
   void SetMagnitudeType(const std::string Value);
   /** Retorna el valor de longitud de onda */
   double GetWavelengthValue() const;
   /** Retorna el valor de amplitud */
   double GetAmplitudeValue() const;
   /** Retorna el valor de relevancia */
   double GetReflectanceValue() const;
   /** Retorna el tipo de longitud de onda */
   std::string GetWavelengthType() const;
   /** Retorna el tipo de magnitud */
   std::string GetMagnitudeType() const;
};

} /** namespace suri */

#endif /* SPECTRALSIGNITEMATTRIBUTE_H_ */
