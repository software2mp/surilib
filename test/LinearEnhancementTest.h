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

// Includes standar
#include <utility>
#include <vector>

// Includes Suri
#include "suri/Tests.h"
#include "suri/LookUpTable.h"

#ifndef LINEARENHANCEMENTTEST_H_
#define LINEARENHANCEMENTTEST_H_

/** namespace suri */
namespace suri {
/** Tests para la clase LinearEnhancement */
class LinearEnhancementTest : public CPPUNIT_NS::TestFixture {
public:
   /** Inicializa test para la clase LinearEnhancementTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(LinearEnhancementTest);
      /** Evalua resultado de TestZeroBins */
      CPPUNIT_TEST(TestZeroBins);
      /** Evalua resultado de TestInvalidStats */
      CPPUNIT_TEST(TestInvalidStats);
      /** Evalua resultado de TestEqualValuesInsideOutputRange */
      CPPUNIT_TEST(TestEqualValuesInsideOutputRange);
      /** Evalua resultado de TestEqualValuesOutsideOutputRange */
      CPPUNIT_TEST(TestEqualValuesOutsideOutputRange);
      /** Evalua resultado de TestEqualOutputRange */
      CPPUNIT_TEST(TestEqualOutputRange);
      /** Evalua resultado de TestEnhancementValues */
      CPPUNIT_TEST(TestEnhancementValues);
      /** Evalua resultado de TestEnhancementValuesInvertedOutputRange */
      CPPUNIT_TEST(TestEnhancementValuesInvertedOutputRange);
      /** Evalua resultado de TestEnhancementInvertedValues */
      CPPUNIT_TEST(TestEnhancementInvertedValues);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;

   /** Ctor. */
   LinearEnhancementTest();
   /** Dtor. */
   virtual ~LinearEnhancementTest();
   /** Crear lut pasando cero bin como parametro */
   void TestZeroBins();
   /** Crea lut a partir de estadisticas invalidas */
   void TestInvalidStats();
   /** Maximo y minimo de la imagen iguales y dentro del rango de salida */
   void TestEqualValuesInsideOutputRange();
   /** Maximo y minimo de la imagen iguales y fuera del rango de salida */
   void TestEqualValuesOutsideOutputRange();
   /** Minimo y maximo del rango de salida iguales */
   void TestEqualOutputRange();
   /** Verifica que los valores de la lut correspondan a un realce lineal */
   void TestEnhancementValues();
   /** Verifica que los valores de la lut correspondan a un realce lineal */
   void TestEnhancementInvertedValues();
   /** Verifica que los valores de la lut, con rango de salida invertido */
   void TestEnhancementValuesInvertedOutputRange();
   /** Usa la clase realce para crear una lut */
   void CreateLut(LookUpTable &Lut, double Mins, double Maxs,
                  std::pair<double, double> OutputRange,
                  int BinsCount);
   /** Verifica que una lut represente un realce lineal */
   bool TestLut(LookUpTable lut, double Min, double Max,
                std::pair<double, double> OutputRange,
                int Binscount);
};
}

#endif /* LINEARENHANCEMENTTEST_H_ */
