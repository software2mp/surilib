/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LUTRENDERERTEST_H_
#define LUTRENDERERTEST_H_

// Includes standard
// Includes Suri
#include "suri/Tests.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Tests para la clase LutRenderer */
class LutRendererTest : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase LutRendererTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(LutRendererTest);
      /** Evalua resultado de TestCreateOk */
      CPPUNIT_TEST(TestCreateOk);
      /** Evalua resultado de TestCreateError */
      CPPUNIT_TEST(TestCreateError);
      /** Evalua resultado de TestGetXmlNode */
      CPPUNIT_TEST(TestGetXmlNode);
      /** Evalua resultado de TestGetParameters */
      CPPUNIT_TEST(TestGetParameters);
      /** Evalua resultado de TestRender */
      CPPUNIT_TEST(TestRender);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;
public:
   /** Ctor */
   LutRendererTest();
   /** Dtor */
   virtual ~LutRendererTest() {
   }
   ;
protected:
   /** Test de cargar xml */
   void TestCreateOk();
   /** Test de cargar xml */
   void TestCreateError();
   /** Test de cargar xml */
   void TestGetXmlNode();
   /** Test de crear objeto Parameters */
   void TestGetParameters();
   /** Test de renderizado */
   void TestRender();
};
}  // namespace suri

#endif /* LUTRENDERERTEST_H_ */
