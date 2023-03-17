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

#include "HistogramTest.h"

/** namespace suri */
namespace suri {
/**
 * Constructor
 */
HistogramTest::HistogramTest() {
}

/**
 * Destructor
 */
HistogramTest::~HistogramTest() {
}

/**
 * Prueba valores para los que se genera un histograma vacio, sin intervalos.
 *
 */
void HistogramTest::TestEmptyHistograms() {
   bool resultvalue = true;
   Histogram histogram(0, 0, 0);
   histogram.IncreaseValue(0);
   histogram.IncreaseValue(-1);
   histogram.IncreaseValue(1);
   resultvalue = histogram.GetBinCount() == 0;
   resultvalue = histogram.GetBinSize() == 0;
   resultvalue = histogram.GetFrequency(1) == 0;

   Histogram otherhistogram(2, 2, 1);
   histogram.IncreaseValue(0);
   histogram.IncreaseValue(1);
   histogram.IncreaseValue(2);
   resultvalue = histogram.GetBinCount() == 0;
   resultvalue = histogram.GetBinSize() == 0;
   resultvalue = histogram.GetFrequency(1) == 0;

   Histogram anotherhistogram(0, 5, 0);
   histogram.IncreaseValue(0);
   histogram.IncreaseValue(1);
   histogram.IncreaseValue(2);
   resultvalue = histogram.GetBinCount() == 0;
   resultvalue = histogram.GetBinSize() == 0;
   resultvalue = histogram.GetFrequency(1) == 0;

   CPPUNIT_ASSERT_MESSAGE("Falla con histogramas vacios", resultvalue);
}

/**
 * Crea un histograma con minimo, maximo y cantidad de bines y le agrega valores.
 * El test verifica los metodos que devuelven los parametros del histograma
 * (Bines, Tamanio del bin, datos del histograma, minimo y maximo).
 *
 */
void HistogramTest::TestParameters() {
   Histogram histogram(0, 5, 5);
   histogram.IncreaseValue(0);
   histogram.IncreaseValue(1);
   histogram.IncreaseValue(2);
   histogram.IncreaseValue(3);
   histogram.IncreaseValue(4);
   CPPUNIT_ASSERT_MESSAGE("El numero de bins tiene que ser 5",
                          histogram.GetBinCount()==5);
   CPPUNIT_ASSERT_MESSAGE("El tamanio de bins deberia ser 1",
                          histogram.GetBinSize()==1.0);
   CPPUNIT_ASSERT_MESSAGE("La suma de las frecuencias deberia ser 5",
                          histogram.GetCount()==5);
   CPPUNIT_ASSERT_MESSAGE("El maximo deberia ser 5", histogram.GetMaximum()==5);
   CPPUNIT_ASSERT_MESSAGE("El minimo deberia ser 0", histogram.GetMinimum()==0);
}

/**
 * Agrego valores al histograma y verifico las frecuencias en los limites y para
 * valores intermedios.
 * falla si alguna frecuencia no es la esperada. Para los limites del histograma
 * la frecuencia tiene que se 0
 */
void HistogramTest::TestFrequencyValues() {
   Histogram histogram(0, 5, 5);
   histogram.IncreaseValue(-0.1);
   histogram.IncreaseValue(0);
   histogram.IncreaseValue(1);
   histogram.IncreaseValue(1.5);  // se agrega al 1
   histogram.IncreaseValue(2);
   histogram.IncreaseValue(3);
   histogram.IncreaseValue(4);
   histogram.IncreaseValue(5);
   histogram.IncreaseValue(10);

   bool testresult = true;
   // Si la frecuencia no coincide con la esperada falla
   if (histogram.GetFrequency(-0.1) != 0 || histogram.GetFrequency(0) != 2
         || histogram.GetFrequency(0.5) != 2 || histogram.GetFrequency(1) != 2
         || histogram.GetFrequency(1.5) != 2 || histogram.GetFrequency(4) != 3
         || histogram.GetFrequency(4.5) != 3 || histogram.GetFrequency(5) != 0) {
      testresult = false;
   }

   CPPUNIT_ASSERT_MESSAGE("Error en el calculo de la frecuencia", testresult);
}
}
