/*! \file CoordinatesTransform.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COORDINATETRANSFORM_H_
#define COORDINATETRANSFORM_H_

// Include standard
#include <vector>
#include <string>

// Include suri
#include "suri/Subset.h"
#include "suri/CoordinatesTransformInterface.h"
#include "suri/GcpList.h"
// Clase OGR para transformacion de coordenadas
class OGRCoordinateTransformation;

/** Funcion que transforma grados decimales a grados, minutos y segundos */
#define CONVERT_DEGREES(DecimalDegrees, Degrees, Minutes, Seconds, Sign) do {     \
      Sign = DecimalDegrees<0 ? -1 : 1;                                        \
      double absdecdeg = fabs(DecimalDegrees);                             \
      Degrees = static_cast<int>(absdecdeg);                               \
      double aux = 60.0 * (absdecdeg - Degrees);                           \
      Minutes = static_cast<int>(aux);                                     \
      Seconds = (aux - Minutes) * 60.0; } while ( 0 )
/** Referencia espacial para imagenes pixel-linea */
#define RASTER_SPATIAL_REFERENCE "[Raster]"
/** Referencia espacial para latitud-longitud */
#define LATLON_SPATIAL_REFERENCE \
   "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.2572235630016,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433],AUTHORITY[\"EPSG\",\"4326\"]]"

/** namespace suri */
namespace suri {

/** Clase para Tranformacion de Coordenadas: X-Y(in) a X-Y(out) */
/**
 *  Esta clase implementa una tranformacion de coordenadas entre dos sistemas de
 *  referencia
 *  Las coordenadas (X-Y) pueden corresponder a:
 *  1. Proyeccion cartografica (PROJCS)
 *  2. Sistema geografico (GEOGCS)
 *  3. Sistema local (LOCAL_CS)
 *  Hay dos formas de instanciar estos objetos:
 *  1. Mediante un WKT de un PROJCS: Transforma de planas a geograficas
 *  2. Mediante dos WKTs de tipo PROJCS/GEOGCS/LOCAL_CS: Transforma desde el
 *     primero hacia el segundo
 *  En el caso de un LOCAL_CS, solo se soporta que los sistemas de entrada y
 *  salida sean iguales.
 *  O sea, solo se soporta la transformacion identidad para los LOCAL_CS
 */
class CoordinatesTransform : public CoordinatesTransformInterface {
   /** Constructor */
   CoordinatesTransform(const std::string &SRWktIn, const std::string &SRWktOut);

   /** Destructor */
   ~CoordinatesTransform();

public:

// --------------------- METODOS ESTATICOS DE CREACION/DESTRUCCION -----------

   /** Destruye el objeto */
   static void Destroy(CoordinatesTransform * &pCTransform);

   /** Crea el objeto a partir del WKT */
   /**
    *  Para transformar entre un PCS y su respectivo GCS (forma 1)
    *  @param[in] SRWkt WKT del sistema de coordenadas
    *  @return objeto CoordinatesTransform creado
    */
   static CoordinatesTransform * Create(const std::string &SRWkt);

   /** Crea el objeto a partir de los WKTs */
   /**
    * Para transformar entre un PCS/GCS y otro PCS/GCS (forma 2)
    * @param[in] SRWktIn WKT del sistema de coordenadas de entrada
    * @param[in] SRWktOut WKT del sistema de coordenadas de salida
    * @return objeto CoordinatesTransform creado
    */
   static CoordinatesTransform * Create(const std::string &SRWktIn,
                                        const std::string &SRWktOut);

// ------------------- METODOS DE CONSULTA DE CONFIGURACION -----------------

   /** Devuelve el WKT del sistema de coordenadas de entrada */
   virtual std::string GetWktIn() const;

   /** Devuelve el WKT del sistema de coordenadas de salida */
   virtual std::string GetWktOut() const;

   /** Indica si es la transformacon identidad */
   virtual bool IsIdentity() const;

   /** Devuelve el la transformacion OGR directa o inversa */
   OGRCoordinateTransformation * GetOGRCT(bool Inverse = false) const;

// -------------------- METODOS DE OPERACION --------------------------------
   /** Transforma de X-Y(in) a X-Y(out) */
   static int TransformCoordinates(Coordinates &InOut, const std::string &SRWktIn,
                                   const std::string &SRWktOut, bool Inverse = false);

   /** Transforma de Subset(in) a Subset(out) */
   static int TransformCoordinates(Subset &InOut, const std::string &SRWktIn,
                                   const std::string &SRWktOut, int CalculationPoints =
                                         DEFAULT_CALCULATION_POINTS,
                                   bool Inverse = false);

   /** Transforma de X-Y(in) a X-Y(out) */
   static int TransformCoordinates(std::vector<Coordinates> &InOut,
                                   const std::string &SRWktIn,
                                   const std::string &SRWktOut, bool Inverse = false);
   /** Transforma de X-Y(in) a X-Y(out) */
   virtual int TransformCoordinates(Coordinates &InOut, bool Inverse = false) const;

   /** Transforma de X-Y(in) a X-Y(out) */
   virtual int TransformCoordinates(std::vector<Coordinates> &InOut, bool Inverse =
         false) const;
   /** Transforma de Subset(in) a Subset(out) */
   virtual int TransformCoordinates(Subset &InOut, int CalculationPoints =
         DEFAULT_CALCULATION_POINTS,
                                    bool Inverse = false) const;

   /** Configura el orden sobre el algoritmo para la transformacion **/
   virtual void SetOrder(const int Order) {}
   /** Configura el orden sobre el algoritmo para la transformacion **/
   virtual void SetGcpList(GcpList* pCcpList) {}

protected:
   bool identityTransform_; /*! Indica si la transformacion es la identidad */
   std::string sRWktIn_; /*! WKT del SR de entrada */
   std::string sRWktOut_; /*! WKT del SR de salida */
   OGRCoordinateTransformation * pTransform_; /*! Puntero a la transformacion */
   OGRCoordinateTransformation * pITransform_; /*! Puntero a la transformacion */
   /* inversa */
private:
};
} /*namespace suri*/

#endif /*COORDINATETRANSFORM_H_*/
