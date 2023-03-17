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

#ifndef VECTORTEST_H_
#define VECTORTEST_H_

// Includes estandar

// Includes Suri
#include "suri/Tests.h"
// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Test de la clase Vector */
/**
 * Verifica que copy funcione correctametne entre memoria y disco
 */
class VectorTest : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase VectorTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(VectorTest);
      /** Evalua resultado de TestCopy */
      CPPUNIT_TEST(TestCopy);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;
public:
   /** ctor */
   VectorTest();
   /** dtor */
   virtual ~VectorTest();
protected:
   /** Test que analiza si copiar vectores entre disco y memoria funciona */
   void TestCopy();
};
}

#endif /* VECTORTEST_H_ */
