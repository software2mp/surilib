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

#ifndef LUTRENDERERTEST_H_
#define LUTRENDERERTEST_H_

// Includes standard
// Includes Suri
#include "suri/Tests.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Tests para la clase LutRenderer */
class LutRendererTest : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase LutRendererTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(LutRendererTest);
      /** Evalua resultado de TestCreateOk */
      CPPUNIT_TEST(TestCreateOk);
      /** Evalua resultado de TestCreateError */
      CPPUNIT_TEST(TestCreateError);
      /** Evalua resultado de TestGetXmlNode */
      CPPUNIT_TEST(TestGetXmlNode);
      /** Evalua resultado de TestGetParameters */
      CPPUNIT_TEST(TestGetParameters);
      /** Evalua resultado de TestRender */
      CPPUNIT_TEST(TestRender);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;
public:
   /** Ctor */
   LutRendererTest();
   /** Dtor */
   virtual ~LutRendererTest() {
   }
   ;
protected:
   /** Test de cargar xml */
   void TestCreateOk();
   /** Test de cargar xml */
   void TestCreateError();
   /** Test de cargar xml */
   void TestGetXmlNode();
   /** Test de crear objeto Parameters */
   void TestGetParameters();
   /** Test de renderizado */
   void TestRender();
};
}  // namespace suri

#endif /* LUTRENDERERTEST_H_ */
