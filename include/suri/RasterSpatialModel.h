/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#ifndef RASTERSPATIALMODEL_H_
#define RASTERSPATIALMODEL_H_

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "suri/Coordinates.h"
#include "suri/Subset.h"

/** namespace suri */
namespace suri {
/** Clase Modelo Espacial Raster */
/**
 *  Esta clase implementa un modelo de transformacion espacial de tipo "afin"
 *  Contiene un modelo LINEAL de transformacion 2D entre coordenadas de imagen y
 *  coordenadas x-y (planas o geograficas)
 *  El modelo debe ser INVERSIBLE
 *  Modelo:
 *  |x|   |Ax_0 Ax_P Ax_L| |1|
 *  |y| = |Ay_0 Ay_P Ay_L| |P|
 *								   |L|
 *  Las coordenadas x-y pueden corresponder:
 *  1. Proyeccion (PCS)
 *  2. Geografica (GCS)
 *  El sistema de referencia (PCS o GCS) no se incluye en esta clase y debe
 *  manejarse por separado
 */
class RasterSpatialModel {
   /** Constructor */
   RasterSpatialModel();

   /** Destructor */
   ~RasterSpatialModel();

public:

   /** Clase para manejo de parametros de la clase RasterSpatialModel */
   /**
    *  Contiene los parametros del modelo matricial y/o los parametros del
    *  modelo de pixel
    *
    *  Tiene precedencia el modelo matricial
    *
    *  Hay dos tipos de modelos: matricial y pixels
    *
    *  Modelo matricial (vector con los 6 elementos de la matriz,
    *  ordenados de izq a der y de arriba a abajo):
    *  |x|   |Ax_0 Ax_P Ax_L| |1|
    *  |y| = |Ay_0 Ay_P Ay_L| |P|
    *								   |L|
    *  Modelo pixels (4 parametros):
    *  1: tamanios de pixel en P y en L
    *  2: Rotacion del eje y respecto al eje L en sentido horario (en grados)
    *  3: Tiepoint en el espacio P-L
    *  4: Tiepoint en el espacio x-y
    */
   class Parameters {
   public:
      /** Ctor */
      Parameters();
      /** Dtor */
      ~Parameters();
      /** Indica si es valido el modelo matricial */
      bool MatrixIsValid() const;
      /** Indica si es valido el modelo pixels */
      bool PixelsIsValid() const;
      std::vector<double> matrixModel_; /*! Modelo matricial */
      std::vector<double> pixelSizes_; /*! Tamanios de pixel en P y en L */
      double rotation_; /*! Rotacion del eje y respecto al eje L en sentido */
      /* horario (en grados) */
      Coordinates tiePointPl_; /*! Tiepoint en el espacio P-L */
      Coordinates tiePointXy_; /*! Tiepoint en el espacio X-Y */

      /** */
      void SetDirty(bool Dirty = true) { isDirty_ = Dirty; }

      /** */
      bool IsDirty() const { return isDirty_; }

   private:
      bool isDirty_;
   };

// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------

   /** Devuelve la WKT del modelo a partir de los parametros (matriz o pixel) */
   static std::string ParametersToWkt(const Parameters &ModelParameters,
                                      const bool Inverse = false);

   /** Devuelve los parametros del modelo (matriz y pixel) a partir de la WKT */
   static Parameters WktToParameters(const std::string &WktString, const bool Inverse =
         false);

// --------------------- METODOS ESTATICOS DE CREACION ----------------------

   /** Destruye el objeto */
   static void Destroy(RasterSpatialModel * &pModel);

   /** Crea el modelo de transformacion a partir del WKT */
   static RasterSpatialModel * Create(const std::string &WktString, const bool Inverse =
         false);

   /** Crea el modelo de transformacion a partir de los parametros */
   static RasterSpatialModel * Create(const Parameters &ModelParameters,
                                      const bool Inverse = false);

   /** Crea el modelo de transformacion a partir de las esquinas de entrada y salida */
   static RasterSpatialModel * Create(const Coordinates &InUl, const Coordinates &InLr,
                                      const Coordinates &OutUl,
                                      const Coordinates &OutLr, const double &Rotation =
                                            0.0);

// ------------------- METODOS DE CONSULTA DE CONFIGURACION -----------------

   /** Devuelve el WKT de la transformacion espacial */
   std::string GetWkt(const bool Inverse = false) const;

   /** Devuelve parametros del modelo (matriz y pixel) */
   Parameters GetModelParameters(const bool Inverse = false) const;

// -------------------- METODOS DE OPERACION --------------------------------

   /** Transforma de P-L a X-Y */
   void Transform(Coordinates &InOut) const;

   /** Transforma de P-L a X-Y */
   void Transform(std::vector<Coordinates> &InOut) const;

   /** Transforma un subset de (pixel, linea) a (x, y) */
   void Transform(Subset &SubsetInOut) const;

   /** Transforma de X-Y a P-L */
   void InverseTransform(Coordinates &InOut) const;
   
   /** Transforma un subset de X-Y a P-L */
   Subset InverseTransform(const Subset &In) const;

   /** Transforma de X-Y a P-L */
   void InverseTransform(std::vector<Coordinates> &InOut) const;

   /** */
   void SetDirty(bool Dirty = true) { isDirty_ = Dirty; }

   /** */
   bool IsDirty() const { return isDirty_; }

   /** */
   void GetGeoTransform(double* GetTrans) const;

protected:
   /** Metodo auxiliar que dado un vector de coordenadas busca la menor en coordenadas
    *  del eje x. */
   double GetMinX(const std::vector<Coordinates*>& CoordinatesVector) const;
   /** Metodo auxiliar que dado un vector de coordenadas busca la menor en coordenadas
    *  del eje y */
   double GetMinY(const std::vector<Coordinates*>& CoordinatesVector) const;
   /** Metodo auxiliar que dado un vector de coordenadas busca la mayor en coordenadas
    *  del eje x. */
   double GetMaxX(const std::vector<Coordinates*>& CoordinatesVector) const;
   /** Metodo auxiliar que dado un vector de coordenadas busca la mayor en coordenadas
    *  del eje y. */
   double GetMaxY(const std::vector<Coordinates*>& CoordinatesVector) const;

   std::vector<double> transformModel2d_; /*! Modelo de transformacion P-L a */
   /* X-Y (polinomio 2D) */
   std::vector<double> inverseTransformModel2d_; /*! Modelo de transformacion */
   /* inversa X-Y a P-L */
   /* (polinomio 2D) */
private:
   bool isDirty_;
};
} /*namespace suri*/

#endif /*RASTERSPATIALMODEL_H_*/
