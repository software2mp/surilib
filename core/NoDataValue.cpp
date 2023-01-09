/*! \file NoDataValue.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <limits>

#include "NoDataValue.h"

namespace suri {

/**
 * Dtor.
 */
NoDataValue::~NoDataValue() {

}

/**
 * Establece el valor a utilizar para el area que no contiene datos.
 */
void NoDataValue::SetNoDataValue(double NoDataValue) {
   noDataValue_ = NoDataValue;
}

/**
 * Devuelve el valor a utilizar para el area que no contiene datos.
 */
double NoDataValue::GetNoDataValue() const {
   return noDataValue_;
}

/**
 * Establece el valor a utilizar para el area que no contiene datos para la
 * banda especificada.
 */
void NoDataValue::SetBandNdv(int Band, double Value) {
   bandsNdv_[Band] = Value;
}

/**
 * Devuelve el valor a utilizar para el area que no contiene datos para la
 * banda especificada.
 */
double NoDataValue::GetBandNdv(int Band) const {
   std::map<int, double>::const_iterator it = bandsNdv_.find(Band);

   if (it == bandsNdv_.end())
      return -std::numeric_limits<double>::max();

   return it->second;
}

/**
 * Indica si el valor para el area que no contiene datos fue establecido.
 */
bool NoDataValue::IsNoDataValueAvailable() const {
   return available_;
}

/**
 * Establece que el valor para el area que no contiene datos fue especificado.
 */
void NoDataValue::SetNoDataValueAvailable(bool Available) {
   available_ = Available;
}

/**
 * Indica si el valor para el area que no contiene datos fue establecido
 * para la banda especificada.
 */
bool NoDataValue::IsNdvAvailableForBand(int Band) const {
   std::map<int, double>::const_iterator it = bandsNdv_.find(Band);
   return it != bandsNdv_.end();
}

/**
 * Devuelve el mapa con todas las bandas y sus valores
 * no validos definidos.
 */
void NoDataValue::SetAllBandsNdv(const std::map<int, double>& BandsNdv) {
   bandsNdv_ = BandsNdv;
}

/**
 * Devuelve el mapa con todas las bandas y sus valores
 * no validos definidos.
 */
std::map<int, double> NoDataValue::GetAllBandsNdv() const {
   return bandsNdv_;
}

}  // namespace suri
