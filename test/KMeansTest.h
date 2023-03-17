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

#ifndef KMEANSTEST_H_
#define KMEANSTEST_H_

// Includes estandar
#include <vector>
// Includes Suri
#include "suri/Tests.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Tests para la clase KMeansAlgorithm */
class KMeansTest : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase KMeansTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(KMeansTest);
      /** Evalua resultado de TestClassificationRendererToNode */
      CPPUNIT_TEST(TestClassificationRendererToNode);
      /** Evalua resultado de TestNodeToClassificationRenderer */
      CPPUNIT_TEST(TestNodeToClassificationRenderer);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;
public:
   /** Ctor. */
   KMeansTest();
   /** Dtor. */
   virtual ~KMeansTest();
protected:
// Tests
   /** Analizo si el nodo del renderizador se genero correctamente */
   void TestClassificationRendererToNode();
   /** Analizo si MaxLikelihoodAlgorith::Update funciona correctamente */
   void TestNodeToClassificationRenderer();
   /** Compara dos vectores */
   bool TestVector(std::vector<double> &Vector1, std::vector<double> &Vector2);
};
}

#endif /* KMEANSTEST_H_ */
