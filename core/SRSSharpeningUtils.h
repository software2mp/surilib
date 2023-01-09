/*! \file SRSSharpeningUtils.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SRSSHARPENINGUTILS_H_
#define SRSSHARPENINGUTILS_H_

namespace suri {
namespace raster {
namespace sharpening {
namespace utils {

typedef struct s_rgb {
   unsigned char red;
   unsigned char green;
   unsigned char blue;
} RGB;

typedef struct s_hsv {
   float hue;
   float saturation;
   float value;
} HSV;

/**
 * Convierte un valor RGB a un valor HSV.
 */
void RGB2HSV(unsigned char Red, unsigned char Green, unsigned char Blue, float& Hue, float& Saturation, float& Value);

/**
 * Convierte un valor HSV a un valor RGB.
 */
void HSV2RGB(float Hue, float Saturation, float Value, unsigned char &Red, unsigned char &Green, unsigned char &Blue);

}  // namespace utils
}  // namespace sharpening
}  // namespace raster
}  // namespace suri

#endif  // SRSSHARPENINGUTILS_H_
