/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MAXLIKELIHOODTEST_H_
#define MAXLIKELIHOODTEST_H_

// Includes estandar
#include <vector>
// Includes Suri
#include "suri/Tests.h"
#include "suri/StatisticsFunctions.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Test de clasificacion de maxima verosimilitud */
class MaxLikelihoodTest : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase MaxLikelihoodTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(MaxLikelihoodTest);
      /** Evalua resultado de TestGetClassificationManager */
      CPPUNIT_TEST(TestGetClassificationManager);
      /** Evalua resultado de TestClassificationRendererToNode */
      CPPUNIT_TEST(TestClassificationRendererToNode);
      /** Evalua resultado de TestNodeToClassificationRenderer */
      CPPUNIT_TEST(TestNodeToClassificationRenderer);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;
public:
   /** Ctor. */
   MaxLikelihoodTest();
   /** Dtor. */
   virtual ~MaxLikelihoodTest();
protected:
// Tests
   /** Analiza si se calculo correctamente descomposicion LU */
   void TestGetClassificationManager();
   /** Analizo si el nodo del renderizador se genero correctamente */
   void TestClassificationRendererToNode();
   /** Analizo si MaxLikelihoodAlgorith::Update funciona correctamente */
   void TestNodeToClassificationRenderer();

// Metodos internos
   /** Carga estadisticas default */
   void LoadStatistics(Statistics &ClassStatistics);
   /** Compara dos vectores(dim y contenido) */
   bool TestVector(std::vector<double> &Vector1, std::vector<double> &Vector2);
   /** Compara dos matrices(dim y contenido) */
   bool TestMatrix(std::vector<std::vector<double> > &Matrix1,
                   std::vector<std::vector<double> > &Matrix2);
};
}

#endif /* MAXLIKELIHOODTEST_H_ */
