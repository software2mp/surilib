/*! \file NoDataValue.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef NODATAVALUE_H_
#define NODATAVALUE_H_

#include <map>
#include <vector>

namespace suri {

class NoDataValue {
public:
   /**
    * Dtor.
    */
   virtual ~NoDataValue();

   /**
    * Establece el valor a utilizar para el area que no contiene datos.
    */
   void SetNoDataValue(double NoDataValue);

   /**
    * Devuelve el valor a utilizar para el area que no contiene datos.
    */
   double GetNoDataValue() const;

   /**
    * Establece el valor a utilizar para el area que no contiene datos para la
    * banda especificada.
    */
   void SetBandNdv(int Band, double Value);

   /**
    * Devuelve el valor a utilizar para el area que no contiene datos para la
    * banda especificada.
    */
   double GetBandNdv(int Band) const;

   /**
    * Indica si el valor para el area que no contiene datos fue establecido.
    */
   bool IsNoDataValueAvailable() const;

   /**
    * Establece que el valor para el area que no contiene datos fue especificado.
    */
   void SetNoDataValueAvailable(bool Available);

   /**
    * Indica si el valor para el area que no contiene datos fue establecido
    * para la banda especificada.
    */
   bool IsNdvAvailableForBand(int Band) const;

   /**
    * Devuelve el mapa con todas las bandas y sus valores
    * no validos definidos.
    */
   void SetAllBandsNdv(const std::map<int, double>& BandsNdv);

   /**
    * Devuelve el mapa con todas las bandas y sus valores
    * no validos definidos.
    */
   std::map<int, double> GetAllBandsNdv() const;

protected:
   /**
    * Ctor.
    */
   NoDataValue() : noDataValue_(0.0), available_(false) { }

private:
   double noDataValue_;  // valor no valido.
   bool available_;  // valor no valido establecido.
   std::map<int, double> bandsNdv_;
};

} // namespace suri

#endif  // NODATAVALUE_H_
