/*! \file SRSSharpeningUtils.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <cmath>
#include <algorithm>

#include "SRSSharpeningUtils.h"

namespace suri {
namespace raster {
namespace sharpening {
namespace utils {

/**
 * Convierte un valor RGB a un valor HSV.
 */
void RGB2HSV(unsigned char Red, unsigned char Green, unsigned char Blue, float& Hue,
             float& Saturation, float& Value) {
   Hue = 0.0, Saturation = 0.0, Value = 0.0;

   float rgbmin = std::min(Red, std::min(Green, Blue));
   float rgbmax = std::max(Red, std::max(Green, Blue));

   Value = rgbmax / 255.0f;

   if (rgbmax != 0.0f)
      Saturation = (rgbmax - rgbmin) / rgbmax;
   else
      Saturation = 0.0f;

   if (Saturation == 0.0f) {
      Hue = 0.0f;
   } else {
      float redc = (rgbmax - Red) / (rgbmax - rgbmin);
      float greenc = ((rgbmax - Green)) / (rgbmax - rgbmin);
      float bluec = (rgbmax - Blue) / (rgbmax - rgbmin);

      if (Red == rgbmax)
         Hue = bluec - greenc;
      else if (Green == rgbmax)
         Hue = 2.0f + redc - bluec;
      else
         Hue = 4.0f + greenc - redc;

      Hue = Hue / 6.0f;
      if (Hue < 0) Hue = Hue + 1.0f;
   }
}

/**
 * Convierte un valor HSV a un valor RGB.
 */
void HSV2RGB(float Hue, float Saturation, float Value, unsigned char &Red,
             unsigned char &Green, unsigned char &Blue) {
   Red = 0.0f, Green = 0.0f, Blue = 0.0f;

   if (Saturation == 0.0f) {
      Red = Green = Blue = (Value * 255.0f) + 0.5f;
   } else {
      float h = (Hue - static_cast<float>(floor(Hue))) * 6.0f;
      float f = h - static_cast<float>(floor(h));
      float p = Value * (1.0f - Saturation);
      float q = Value * (1.0f - Saturation * f);
      float t = Value * (1.0f - (Saturation * (1.0f - f)));
      switch (static_cast<int>(h)) {
         case 0:
            Red = static_cast<unsigned char>(Value * 255.0f + 0.5f);
            Green = static_cast<unsigned char>(t * 255.0f + 0.5f);
            Blue = static_cast<unsigned char>(p * 255.0f + 0.5f);
            break;
         case 1:
            Red = static_cast<unsigned char>(q * 255.0f + 0.5f);
            Green = static_cast<unsigned char>(Value * 255.0f + 0.5f);
            Blue = static_cast<unsigned char>(p * 255.0f + 0.5f);
            break;
         case 2:
            Red = static_cast<unsigned char>(p * 255.0f + 0.5f);
            Green = static_cast<unsigned char>(Value * 255.0f + 0.5f);
            Blue = static_cast<unsigned char>(t * 255.0f + 0.5f);
            break;
         case 3:
            Red = static_cast<unsigned char>(p * 255.0f + 0.5f);
            Green = static_cast<unsigned char>(q * 255.0f + 0.5f);
            Blue = static_cast<unsigned char>(Value * 255.0f + 0.5f);
            break;
         case 4:
            Red = static_cast<unsigned char>(t * 255.0f + 0.5f);
            Green = static_cast<unsigned char>(p * 255.0f + 0.5f);
            Blue = static_cast<unsigned char>(Value * 255.0f + 0.5f);
            break;
         case 5:
            Red = static_cast<unsigned char>(Value * 255.0f + 0.5f);
            Green = static_cast<unsigned char>(p * 255.0f + 0.5f);
            Blue = static_cast<unsigned char>(q * 255.0f + 0.5f);
            break;
      }
   }
}

}  // namespace utils
}  // namespace sharpening
}  // namespace raster
}  // namespace suri
