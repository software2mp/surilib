/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ENHANCEMENTSELECTIONTEST_H_
#define ENHANCEMENTSELECTIONTEST_H_

// Includes suri
#include "suri/Tests.h"

/** namespace suri */
namespace suri {
/** Test para la clase EnhancementSelectionPart, verifica que se muestre la lista de realces. */
class EnhancementSelectionTest : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase EnhancementSelectionTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(ColorTableRendererTest);
      /** Evalua resultado de CreateElementPropertiesWindow */
      CPPUNIT_TEST(CreateElementPropertiesWindow);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;
public:
   /** Ctor. */
   EnhancementSelectionTest();
   /** Dtor. */
   virtual ~EnhancementSelectionTest();
protected:
   /** Verifica que se cargue la lista de realces */
   bool CreateElementPropertiesWindow();
};
}

#endif /* ENHANCEMENTSELECTIONTEST_H_ */
