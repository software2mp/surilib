/*! \file Dimension.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DIMENSION_H_
#define DIMENSION_H_

// Include suri
#include "Subset.h"

/** namespace suri */
namespace suri {
/** Clase con signo y magnitud de un corrimeinto(de Subset) que permite trabajar independientemente */
// del sist de coord
/**
 * Permite trabajar independientemente del sistema de coord de la imagen
 * si considero en las formulas el signo de Dimension
 * Cuando le pasas los extremos Ul y Lr se da cuenta que coordenadas estas usando y
 * guarda en el signo el valor que corresponde.
 * \todo Considerar migrar los signos a Subset.
 * \todo Width y Height pueden ser propiedades constantes (lo mismo los signos
 *       en caso de no migrarse)
 */
class Dimension {
public:
   /** ctor */
   Dimension(const Subset &TargetSubset);
   /** ctor */
   Dimension(const Coordinates &Ul, const Coordinates &Lr);
   /** retorna el ancho(X) de Dimension */
   double GetWidth() const {
      return width_;
   }
   /** retorna la altura(y) de Dimension */
   double GetHeight() const {
      return height_;
   }
   /** Retorna el signo de los ejes con respecto a los ejes raster */
   /**
    *  Los ejes raster son Positivo a la derecha y abajo.
    *  @param[out] SignX Signo del eje x del subset
    *  @param[out] SignY Signo del eje y del subset
    */
   void GetSign(int &SignX, int &SignY) const {
      SignX = signX_;
      SignY = signY_;
   }
   /** retorna el signo de Dimension en x */
   int XSign() const {
      return signX_;
   }
   /** retorna el signo de Dimension en y */
   int YSign() const {
      return signY_;
   }
private:
   double width_; /*! Valor en x de Dimension */
   double height_; /*! Valor en y de Dimension */
   int signX_; /*! Signo del eje x del subset con respecto a sistema de */
   /* coord de pantalla */
   int signY_; /*! Signo del eje y del subset con respecto a sistema de */
   /* coord de pantalla */
};

/** Operador igualdad */
bool operator==(const Dimension &Lhs, const Dimension &Rhs);

/** Operador desigualdad */
bool operator!=(const Dimension &Lhs, const Dimension &Rhs);

/** operator suma de una Dimension a un subset (corrimiento) */
Subset operator+(const Subset &Lhs, const Dimension &Rhs);
}

#endif /*DIMENSION_H_*/