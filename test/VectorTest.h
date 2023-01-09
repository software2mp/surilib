/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
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
