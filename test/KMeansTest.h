/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
