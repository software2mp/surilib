/*! \file suri/Tests.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
