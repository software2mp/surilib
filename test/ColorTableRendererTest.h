/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COLORTABLERENDERERTEST_H_
#define COLORTABLERENDERERTEST_H_

// Includes standard
// Includes Suri
#include "suri/Tests.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Test para la clase ColorTableRenderer */
class ColorTableRendererTest : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase ColorTableRendererTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(ColorTableRendererTest);
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
      /** Evalua resultado de TestRenderWithDataCast */
      CPPUNIT_TEST(TestRenderWithDataCast);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;
public:
   /** Ctor */
   ColorTableRendererTest();
   /** Dtor */
   virtual ~ColorTableRendererTest() {
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
   /** Test de renderizado */
   void TestRenderWithDataCast();
};
}  // namespace suri

#endif /* COLORTABLERENDERERTEST_H_ */
