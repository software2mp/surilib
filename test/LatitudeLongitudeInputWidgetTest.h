/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LATITUDELONGITUDINPUTWIDGETTEST_H_
#define LATITUDELONGITUDINPUTWIDGETTEST_H_

// Includes standard
// Includes Suri
#include "suri/LatitudeLongitudeInputWidget.h"
#include "suri/Tests.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Tests para la clase LatitudeLongitudeInputWidget */
class LatitudeLongitudeInputWidgetTest : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase LatitudeLongitudeInputWidgetTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(LatitudeLongitudeInputWidgetTest);
      /** Evalua resultado de TestSetGet */
      CPPUNIT_TEST(TestSetGet);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;
public:
   /** Ctor */
   LatitudeLongitudeInputWidgetTest(suri::World* pWorld = NULL);
   /** Dtor */
   virtual ~LatitudeLongitudeInputWidgetTest() {
   }
protected:
   /** Verifica que el valor inicial no cambie */
   void TestSetGet();
private:
   suri::World* pWorld_; /*! puntero al mundo */
};
}  // namespace suri

#endif /* LATITUDELONGITUDINPUTWIDGETTEST_H_ */
