/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standar
#include <utility>
#include <vector>

// Includes Suri
#include "suri/Tests.h"
#include "suri/LookUpTable.h"

#ifndef LINEARENHANCEMENTTEST_H_
#define LINEARENHANCEMENTTEST_H_

/** namespace suri */
namespace suri {
/** Tests para la clase LinearEnhancement */
class LinearEnhancementTest : public CPPUNIT_NS::TestFixture {
public:
   /** Inicializa test para la clase LinearEnhancementTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(LinearEnhancementTest);
      /** Evalua resultado de TestZeroBins */
      CPPUNIT_TEST(TestZeroBins);
      /** Evalua resultado de TestInvalidStats */
      CPPUNIT_TEST(TestInvalidStats);
      /** Evalua resultado de TestEqualValuesInsideOutputRange */
      CPPUNIT_TEST(TestEqualValuesInsideOutputRange);
      /** Evalua resultado de TestEqualValuesOutsideOutputRange */
      CPPUNIT_TEST(TestEqualValuesOutsideOutputRange);
      /** Evalua resultado de TestEqualOutputRange */
      CPPUNIT_TEST(TestEqualOutputRange);
      /** Evalua resultado de TestEnhancementValues */
      CPPUNIT_TEST(TestEnhancementValues);
      /** Evalua resultado de TestEnhancementValuesInvertedOutputRange */
      CPPUNIT_TEST(TestEnhancementValuesInvertedOutputRange);
      /** Evalua resultado de TestEnhancementInvertedValues */
      CPPUNIT_TEST(TestEnhancementInvertedValues);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;

   /** Ctor. */
   LinearEnhancementTest();
   /** Dtor. */
   virtual ~LinearEnhancementTest();
   /** Crear lut pasando cero bin como parametro */
   void TestZeroBins();
   /** Crea lut a partir de estadisticas invalidas */
   void TestInvalidStats();
   /** Maximo y minimo de la imagen iguales y dentro del rango de salida */
   void TestEqualValuesInsideOutputRange();
   /** Maximo y minimo de la imagen iguales y fuera del rango de salida */
   void TestEqualValuesOutsideOutputRange();
   /** Minimo y maximo del rango de salida iguales */
   void TestEqualOutputRange();
   /** Verifica que los valores de la lut correspondan a un realce lineal */
   void TestEnhancementValues();
   /** Verifica que los valores de la lut correspondan a un realce lineal */
   void TestEnhancementInvertedValues();
   /** Verifica que los valores de la lut, con rango de salida invertido */
   void TestEnhancementValuesInvertedOutputRange();
   /** Usa la clase realce para crear una lut */
   void CreateLut(LookUpTable &Lut, double Mins, double Maxs,
                  std::pair<double, double> OutputRange,
                  int BinsCount);
   /** Verifica que una lut represente un realce lineal */
   bool TestLut(LookUpTable lut, double Min, double Max,
                std::pair<double, double> OutputRange,
                int Binscount);
};
}

#endif /* LINEARENHANCEMENTTEST_H_ */
