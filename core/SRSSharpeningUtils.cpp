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
