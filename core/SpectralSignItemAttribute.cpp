/*! \file SpectralSignItemAttribute.cpp */
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
#include "suri/SpectralSignItemAttribute.h"
#include "suri/AuxiliaryFunctions.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor */
SpectralSignItemAttribute::SpectralSignItemAttribute(const std::string& Name,
                                                     const std::string& Value) :
      ComplexItemAttribute(Name, Value) {
}

/** Destructor */
SpectralSignItemAttribute::~SpectralSignItemAttribute() {
}

/** Configura el valor de longitud de onda */
void SpectralSignItemAttribute::SetWavelengthValue(const double Value) {
   attributes_[WavelengthValuePosition]->SetValue(DoubleToString(Value));
}

/** Configura el valor de amplitud */
void SpectralSignItemAttribute::SetAmplitudeValue(const double Value) {
   attributes_[AmplitudeValuePosition]->SetValue(DoubleToString(Value));
}

/** Configura el valor de relevancia */
void SpectralSignItemAttribute::SetReflectanceValue(const double Value) {
   attributes_[RelevanceValuePosition]->SetValue(DoubleToString(Value));
}

/** Configura el tipo de longitud de onda */
void SpectralSignItemAttribute::SetWavelengthType(const std::string Value) {
   attributes_[WavelengthTypePosition]->SetValue(Value);
}

/** Configura el tipo de magnitud */
void SpectralSignItemAttribute::SetMagnitudeType(const std::string Value) {
   attributes_[MagnitudeTypePosition]->SetValue(Value);
}

/** Retorna el valor de longitud de onda */
double SpectralSignItemAttribute::GetWavelengthValue() const {
   return
         (attributes_.size() > 0) ? StringToNumber<double>(
                                          attributes_[WavelengthValuePosition]->GetValue()) :
                                    0;
}

/** Retorna el valor de amplitud */
double SpectralSignItemAttribute::GetAmplitudeValue() const {
   return
         (attributes_.size() > 1) ? StringToNumber<double>(
                                          attributes_[AmplitudeValuePosition]->GetValue()) :
                                    0;
}

/** Retorna el valor de relevancia */
double SpectralSignItemAttribute::GetReflectanceValue() const {
   return
         (attributes_.size() > 2) ? StringToNumber<double>(
                                          attributes_[RelevanceValuePosition]->GetValue()) :
                                    0;
}

/** Retorna el tipo de longitud de onda */
std::string SpectralSignItemAttribute::GetWavelengthType() const {
   return (attributes_.size() > 3) ? attributes_[WavelengthTypePosition]->GetValue() : "";
}

/** Retorna el tipo de magnitud */
std::string SpectralSignItemAttribute::GetMagnitudeType() const {
   return (attributes_.size() > 4) ? attributes_[MagnitudeTypePosition]->GetValue() : "";
}
} /** namespace suri */
