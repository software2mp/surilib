/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef HISTOGRAMTEST_H_
#define HISTOGRAMTEST_H_

#include "suri/Histogram.h"
#include "suri/Tests.h"

/** namespace suri */
namespace suri {
/** Test para la clase Histograma */
class HistogramTest : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase HistogramTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(HistogramTest);
      /** Evalua resultado de TestEmptyHistograms */
      CPPUNIT_TEST(TestEmptyHistograms);
      /** Evalua resultado de TestParameters */
      CPPUNIT_TEST(TestParameters);
      /** Evalua resultado de TestFrequencyValues */
      CPPUNIT_TEST(TestFrequencyValues);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;
public:
   /** ctor */
   HistogramTest();
   /** dtor */
   virtual ~HistogramTest();
protected:
   /** Verifica que no falle si valores generan histograma vacio */
   void TestEmptyHistograms();
   /** Verifica metodos de clase Histogram */
   void TestParameters();
   /** Agrego valores al histograma y verifico las frecuencias en los limites y para valores */
// intermedios.
   void TestFrequencyValues();
};
}

#endif /* HISTOGRAMTEST_H_ */
