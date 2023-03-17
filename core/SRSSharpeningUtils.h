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
