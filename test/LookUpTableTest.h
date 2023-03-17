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

#ifndef LOOKUPTABLETEST_H_
#define LOOKUPTABLETEST_H_
// Includes estandar

// Includes Suri
#include "suri/Tests.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Tests para la clase LookUpTable */
class LookUpTableTest : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase LookUpTableTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(LookUpTableTest);
      /** Evalua resultado de TestEmptyLutToXml */
      CPPUNIT_TEST(TestEmptyLutToXml);
      /** Evalua resultado de TestLimits */
      CPPUNIT_TEST(TestLimits);
      /** Evalua resultado de TestDoubleLimits */
      CPPUNIT_TEST(TestDoubleLimits);
      /** Evalua resultado de TestFromXml */
      CPPUNIT_TEST(TestFromXml);
      /** Evalua resultado de TestToXml */
      CPPUNIT_TEST(TestToXml);
      /** Evalua resultado de TestDoubleToXmlFromXml */
      CPPUNIT_TEST(TestDoubleToXmlFromXml);
      /** Evalua resultado de TestFromXmlLutRenderer */
      CPPUNIT_TEST(TestFromXmlLutRenderer);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;

public:
   /** ctor */
   LookUpTableTest();
   /** dtor */
   virtual ~LookUpTableTest();

protected:
   /** Verifico que se detecten Luts vacias */
   void TestEmptyLutToXml();
   /** Verifico que los limites se tomen correctamente */
   void TestLimits();
   /** Verifico que los limites se tomen correctamente(doubles) */
   void TestDoubleLimits();
   /** Verifico conversion nodo a parametros */
   void TestFromXml();
   /** Verifico conversion parametros a nodo */
   void TestToXml();
   /** Transformo lut->xml luego xml->lut y analizo resultado */
   void TestDoubleToXmlFromXml();
   /** Verifico que lut del renderizador se cargue correctamente. */
   void TestFromXmlLutRenderer();
};
}

#endif /* LOOKUPTABLETEST_H_ */
