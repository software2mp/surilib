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

#include "suri/Operations.h"

// Includes suri
#include "BrightnessRenderer.h"

/** namespace suri */
namespace suri {
// Defines
/* *
 * \attention Si se sacan o agregan valores tambien hay que modificar el control
 *             grafico para que tenga mas posiciones.
 */
double ContrastValues[] = { 0.0173, 0.0260, 0.0390, 0.0585, 0.0878, 0.1317, 0.1975,
                            0.2963, 0.4444, 0.6667, 1, 1.5000, 2.25, 3.375, 5.0625,
                            7.5938, 11.3906, 17.0859, 25.6289, 38.4434, 57.665 }; /*! Valores que puede */
// tomar contraste.

/** Transforma el contraste de absoluto (+/- 50) a relativo (+/- 1) */
#define FW_ADJUST_CONTRAST(Contrast) (double)Contrast/50.0+0.001
/** Transforma el contraste de relativo (+/- 1) en absoluto (+/- 50) */
#define BW_ADJUST_CONTRAST(Contrast) (int)( (Contrast-0.001)*50.0)

/**
 *  Modifica los valores de brillo y contraste de un elemento.
 * \pre pElement debe tener el nodo correspondiente al Renderer BrightnessRenderer.
 * @param[in] pElement Elemento al cual se le debe modificar los valores
 * @param[in] Brightness Valor -50;50 de brillo
 * @param[in] Contrast Valor 0;100 de contraste
 */
void SetBrightnessContrast(Element *pElement, int Brightness, int Contrast) {
   BrightnessRenderer::Parameters params = BrightnessRenderer::GetParameters(
         BrightnessRenderer::GetXmlNode(pElement));
   params.brightness_ = Brightness;
   if (Contrast < 0) {
      Contrast = 0;
   }
   if (Contrast >= CONTRAST_SIZE) {
      Contrast = CONTRAST_SIZE - 1;
   }
   params.contrast_ = ContrastValues[Contrast];
   BrightnessRenderer::SetXmlNode(pElement, BrightnessRenderer::GetXmlNode(params));
}

/**
 *  Obtiene los valores de brillo y contraste de un elemento dado.
 * \pre pElement debe ser de tipo RasterElement y poseer el nodo correspondiente
 *              al Renderer BrightnessRenderer.
 * @param[in] pElement Elemento del cual se obtienen los valores
 * @param[out] Brightness Valor -50;50 de brillo
 * @param[out] Contrast Valor 0;100 de contraste
 */
void GetBrightnessContrast(Element *pElement, int &Brightness, int &Contrast) {
   BrightnessRenderer::Parameters params = BrightnessRenderer::GetParameters(
         BrightnessRenderer::GetXmlNode(pElement));

   // Busco la posicion que corresponde al contraste
   Contrast = 0;
   for (int i = 0; i < CONTRAST_SIZE; i++) {
      if (params.contrast_ >= ContrastValues[i]) {
         Contrast = i;
      }
   }
   Brightness = static_cast<int>(params.brightness_);
}
}
;
// namespace suri

