/*! \file RgbColor.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RGBCOLOR_H_
#define RGBCOLOR_H_

/** namespace suri */
namespace suri {
/** Clase que guarda color RGB en unsigned char */
class RgbColor {
public:
   /**
    * Ctor
    * @param[in] R rojo
    * @param[in] G verde
    * @param[in] B azul
    */
   RgbColor(unsigned char R, unsigned char G, unsigned char B) :
         r_(R), g_(G), b_(B) {
   }
   unsigned char r_; /*! rojo (unsigned char) */
   unsigned char g_; /*! verde (unsigned char) */
   unsigned char b_; /*! azul (unsigned char) */
};
}

#endif /*RGBCOLOR_H_*/
