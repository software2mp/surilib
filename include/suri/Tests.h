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

#ifndef TESTS_H_
#define TESTS_H_

// Includes standard

// Includes Wx

// Includes App

// Defines
/** Macro para ejecutar los tests */
#  define EXECUTE_TEST(Test) do {REPORT_DEBUG("D: Ejecutando " # Test);        \
                                 if ( Test ) { SHOW_INFO("D: Test " # Test " ejecutado con exito"); }           \
                                 else { SHOW_ERROR("D: Test " # Test " fallido"); } } while ( 0 )

/** directorio donde buscar los tests (dependiente del PWD de la ejecucion) */
#  define __TEST_PATH__ "lib/suri/test/"
/** directorio donde buscar tests de source */
#  define __SRC_TEST_PATH__ "src/test/"
/** namespace de test */
#  define CPPUNIT_NS suri_test

namespace CPPUNIT_NS {
/** Clase que heredan todos los test */
class TestFixture {
public:
   /** Ctor */
   TestFixture() {
   }
   /** Dtor */
   virtual ~TestFixture() {
   }
   /** Configuracion inicial */
   virtual void setUp() {
   }
   /** Limpia al final */
   virtual void tearDown() {
   }
};
}  // namespace CPPUNIT_NS
/** Inicia conjunto de pruebas */
# define CPPUNIT_TEST_SUITE(ClassName) bool testResult_; bool anyAssert_;      \
public:                                                     \
   void RunTests() {setUp();                                   \
                    const char *pclassname = # ClassName;      \
                    testResult_ = true; anyAssert_ = false
/** Declara las pruebas */
# define CPPUNIT_TEST(Name) Name();                                           \
   if ( !testResult_ ) { SHOW_ERROR("D: %s::" # Name " fallido",       \
                                    pclassname); }               \
   else { SHOW_INFO("D: %s::" # Name " exitoso", pclassname); }         \
   if ( !anyAssert_ ) { SHOW_ERROR("D: %s::" # Name "no fue validado"  \
                                   " en forma correcta"); }           \
   testResult_ = true; anyAssert_ = false
/** Finaliza conjunto de pruebas */
# define CPPUNIT_TEST_SUITE_END()tearDown(); }
/** Imprime msg si la comparacion falla */
#  define CPPUNIT_ASSERT_EQUAL(lhs, rhs) anyAssert_ = true;                    \
   CPPUNIT_ASSERT_MESSAGE(# lhs "!=" # rhs, lhs==rhs)
/** Registra conjunto de pruebas */
#  define CPPUNIT_TEST_SUITE_REGISTRATION(...)
/** Imprime msg si la condicion falla */
#  define CPPUNIT_ASSERT_MESSAGE(Msg, AssertCondition) anyAssert_ = true;      \
   if ( !(AssertCondition) ) {                  \
      SHOW_ERROR("D:UnitTest::Fail: %s", Msg);   \
      testResult_ = false; }
/** Corre los tests */
#  define RUN_TESTS(TestClass) TestClass().RunTests()

// Includes Suri
#  include "logmacros.h"

#endif /* TESTS_H_ */
