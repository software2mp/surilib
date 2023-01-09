/*! \file Operations.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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

