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

#ifndef LUTARRAYTEST_H_
#define LUTARRAYTEST_H_

// Includes estandar

// Includes Suri
#include "suri/LutArray.h"
#include "suri/Tests.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Tests para la clase LutArray */
class LutArrayTest : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase LutArrayTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(LutArrayTest);
      /** Evalua resultado de TestEmptyToXml */
      CPPUNIT_TEST(TestEmptyToXml);
      /** Evalua resultado de TestToXmlEmptyTable */
      CPPUNIT_TEST(TestToXmlEmptyTable);
      /** Evalua resultado de TestToXmlTable */
      CPPUNIT_TEST(TestToXmlTable);
      /** Evalua resultado de TestFromXml */
      CPPUNIT_TEST(TestFromXml);
      /** Evalua resultado de TestLutRendererToXmlFromXml */
      CPPUNIT_TEST(TestLutRendererToXmlFromXml);
      /** Evalua resultado de TestToXmlLutRendererFromXml */
      CPPUNIT_TEST(TestToXmlLutRendererFromXml);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;
public:
   /** ctor */
   LutArrayTest();

protected:
   /** Verifica que de error si el vector esta vacio */
   void TestEmptyToXml();
   /** Verifica que de error si el nodo esta vacio */
   void TestToXmlEmptyTable();
   /** Test tabla a nodo */
   void TestToXmlTable();
   /** Test nodo a tabla */
   void TestFromXml();
   /** Verficia que tabla de renderizador sea igual a la del nodo */
   void TestLutRendererToXmlFromXml();
   /** Convierte nodo a lut array, luego lut array a nodo y los compara */
   void TestToXmlLutRendererFromXml();
};
}

#endif /* LUTARRAYTEST_H_ */
