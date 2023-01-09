/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
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
