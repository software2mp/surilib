/*! \file da/tests/main.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

/**
 * realiza tests
 * @param[in] argc cantidad de argumentos
 * @param[in] argv argumentos
 * @return resultado ejecucion
 */
int main(int argc, char* argv[]) {
   // Create the event manager and test controller
   CPPUNIT_NS::TestResult controller;

   // Add a listener that colllects test result
   CPPUNIT_NS::TestResultCollector result;
   controller.addListener(&result);

   // Add a listener that print dots as test run.
   CPPUNIT_NS::BriefTestProgressListener progress;
   controller.addListener(&progress);

   // Add the top suite to the test runner
   CPPUNIT_NS::TestRunner runner;
   runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
   runner.run(controller);

   // Print test in a compiler compatible format.
   CPPUNIT_NS::CompilerOutputter outputter(&result, CPPUNIT_NS::stdCOut());
   outputter.write();

   return result.wasSuccessful() ? 0 : 1;
}

