/** \file ExactCoordinatesTransformation.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef EXACTCOORDINATESTRANSFORMATION_H_
#define EXACTCOORDINATESTRANSFORMATION_H_

#include <vector>
#include <string>
#include <limits>

#include <ogr_spatialref.h>

#include "suri/CoordinatesTransformation.h"
#include "suri/Coordinates.h"
#include "suri/Subset.h"

/** namespace suri */
namespace suri {

/**
 * Implementa los metodos correspondientes de CoordinatesTransformation
 * para poder generar una trasnformacion exacta.
 */
class ExactCoordinatesTransformation : public CoordinatesTransformation {
public:
   /**
    * Construye un objeto ExactCoordinatesTransformation
    */
   ExactCoordinatesTransformation(const std::string& WktIn,
                                    const std::string& WktOut,
                                    bool IsIdentity = true,
                                    OGRCoordinateTransformation* PTransform = NULL,
                                    OGRCoordinateTransformation* PITransform = NULL);

   /**
    * Destructor
    */
   virtual ~ExactCoordinatesTransformation();

   /**
    * Realiza una transformacion exacta de las coordenadas en el formato de
    * origen al formato de salida utilizando como salida un objeto Coordinates.
    */
   virtual int Transform(Coordinates &CoordinatesP, bool Inverse = false) const;

   /**
    * Realiza una transformacion exacta de las coordenadas en el formato de
    * origen al formato de salida utilizando como salida un vector de Coordinates.
    */
   virtual int Transform(std::vector<Coordinates> &CoordinatesP,
                         bool Inverse = false) const;

   /**
    * Realiza una transformacion exacta de las coordenadas en el formato de
    * origen al formato de salida utilizando como salida un Subset.
    */
   virtual int Transform(Subset &SubsetP,
                         int CalculationPoints = DEFAULT_CALCULATION_POINTS,
                         bool Inverse = false) const;

   /**
    * Indica si es la transformacon identidad.
    */
   virtual bool IsIdentity() const;
   /** Devuelve el la transformacion OGR directa o inversa */
   OGRCoordinateTransformation * GetOGRCT(bool Inverse = false) const;
   /** Compara si dos transformaciones son iguales **/
   virtual bool Equals(CoordinatesTransformation* pTransform) const;
   /** Devuelve el wkt del sr de salida **/
   std::string GetWktIn() const;
   /** Devuelve el wkt del sr de llegada **/
   std::string GetWktOut() const;

private:
   bool isIdentity_; /** es identidad */
   std::string wktIn_; /** Referencia de entrada */
   std::string wktOut_; /** Referencia de salida */
   OGRCoordinateTransformation* pTransform_; /** Puntero a la transformacion directa */
   OGRCoordinateTransformation* pITransform_; /** Puntero a la transformacion inversa */
};

}  // namespace suri

#endif  // EXACTCOORDINATESTRANSFORMATION_H_
