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

#ifndef HISTOGRAMTEST_H_
#define HISTOGRAMTEST_H_

#include "suri/Histogram.h"
#include "suri/Tests.h"

/** namespace suri */
namespace suri {
/** Test para la clase Histograma */
class HistogramTest : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase HistogramTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(HistogramTest);
      /** Evalua resultado de TestEmptyHistograms */
      CPPUNIT_TEST(TestEmptyHistograms);
      /** Evalua resultado de TestParameters */
      CPPUNIT_TEST(TestParameters);
      /** Evalua resultado de TestFrequencyValues */
      CPPUNIT_TEST(TestFrequencyValues);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;
public:
   /** ctor */
   HistogramTest();
   /** dtor */
   virtual ~HistogramTest();
protected:
   /** Verifica que no falle si valores generan histograma vacio */
   void TestEmptyHistograms();
   /** Verifica metodos de clase Histogram */
   void TestParameters();
   /** Agrego valores al histograma y verifico las frecuencias en los limites y para valores */
// intermedios.
   void TestFrequencyValues();
};
}

#endif /* HISTOGRAMTEST_H_ */
