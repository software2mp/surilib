/*! \file Meassure.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "Meassure.h"

// Includes standard

// Includes Suri
#include "suri/messages.h"

// Includes Wx

// Includes App

/** namespace suri */
namespace suri {
const char *Meassure::pUnitStrings_[] = { name_METER_UNIT, name_KILOMETER_UNIT,
                                          name_HECTARE_UNIT };
const char *Meassure::pAreaSuffix_[] = { label_SQUARE, label_SQUARE, "" };
const double Meassure::unitMultiplier_[] = { 1.0, 0.001, 0.01 };

/**
 * Obtiene la medicion.
 * @return string con la medicion, o un string con invalid_MEASSURE
 */
wxString Meassure::GetMeassurement() const {
   if (area_ >= 0) {
      return wxString::Format(
            format_MEASSURE_DISPLAY_fss,
            area_
                  * (unitMultiplier_[GetUnit()]
                        * (isArea_ ? unitMultiplier_[GetUnit()] : 1.0)),
            pUnitStrings_[GetUnit()], isArea_ ? _(pAreaSuffix_[GetUnit()]) : "");
   } else {
      return invalid_MEASSURE;
   }
}
}
