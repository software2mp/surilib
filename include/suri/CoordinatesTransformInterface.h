/*! \file CoordinateTransformInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COORDINATETRANSFORMINTERFACE_H_
#define COORDINATETRANSFORMINTERFACE_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/Subset.h"
#include "suri/Coordinates.h"
#include "suri/GcpList.h"

// Includes Wx
// Defines
/** Puntos a tener en cuenta en una transformacion de coordenadas */
#define DEFAULT_CALCULATION_POINTS 300
// forwards

namespace suri {

// forwards
class GCPList;

/*!
 * Interaz que representa el comportamiento en comun a todo Transformacion Geometrica
 */
class CoordinatesTransformInterface {
   CoordinatesTransformInterface(const CoordinatesTransformInterface&);

public:
   /** Destruye el objeto */
   static void Destroy(CoordinatesTransformInterface * &pCTransform);

   /** Crea el objeto a partir del WKT */
   /**
    *  Para transformar entre un PCS y su respectivo GCS (forma 1)
    *  @param[in] SRWkt WKT del sistema de coordenadas
    *  @return objeto CoordinatesTransform creado
    */
   static CoordinatesTransformInterface * Create(const std::string& TransformationType,
                                                 const std::string &SRWkt);

   /** Crea el objeto a partir de los WKTs */
   /**
    * Para transformar entre un PCS/GCS y otro PCS/GCS (forma 2)
    * @param[in] SRWktIn WKT del sistema de coordenadas de entrada
    * @param[in] SRWktOut WKT del sistema de coordenadas de salida
    * @return objeto CoordinatesTransform creado
    */
   static CoordinatesTransformInterface * Create(const std::string& TransformationType,
                                                 const std::string &SRWktIn,
                                                 const std::string &SRWktOut);

   /** Transforma de X-Y(in) a X-Y(out) */
   virtual int TransformCoordinates(Coordinates &InOut, bool Inverse = false) const=0;

   /** Transforma de X-Y(in) a X-Y(out) */
   virtual int TransformCoordinates(std::vector<Coordinates> &InOut, bool Inverse =
         false) const=0;
   /** Transforma de Subset(in) a Subset(out) */
   virtual int TransformCoordinates(Subset &InOut, int CalculationPoints =
         DEFAULT_CALCULATION_POINTS,
                                    bool Inverse = false) const=0;
   /** Configura el orden sobre el algoritmo para la transformacion **/
   virtual void SetOrder(const int Order) = 0;
   /** Configura el orden sobre el algoritmo para la transformacion **/
   virtual void SetGcpList(GcpList* pCcpList) = 0;

   /** Devuelve el WKT del sistema de coordenadas de entrada */
   virtual std::string GetWktIn() const=0;

   /** Devuelve el WKT del sistema de coordenadas de salida */
   virtual std::string GetWktOut() const=0;

   /** Indica si es la transformacon identidad */
   virtual bool IsIdentity() const=0;

protected:
   CoordinatesTransformInterface();
   virtual ~CoordinatesTransformInterface();
};
} /** namespace suri */

#endif /* COORDINATETRANSFORMINTERFACE_H_ */
