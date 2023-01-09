/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef LUTARRAYTEST_H_
#define LUTARRAYTEST_H_

// Includes estandar

// Includes Suri
#include "suri/LutArray.h"
#include "suri/Tests.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Tests para la clase LutArray */
class LutArrayTest : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase LutArrayTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(LutArrayTest);
      /** Evalua resultado de TestEmptyToXml */
      CPPUNIT_TEST(TestEmptyToXml);
      /** Evalua resultado de TestToXmlEmptyTable */
      CPPUNIT_TEST(TestToXmlEmptyTable);
      /** Evalua resultado de TestToXmlTable */
      CPPUNIT_TEST(TestToXmlTable);
      /** Evalua resultado de TestFromXml */
      CPPUNIT_TEST(TestFromXml);
      /** Evalua resultado de TestLutRendererToXmlFromXml */
      CPPUNIT_TEST(TestLutRendererToXmlFromXml);
      /** Evalua resultado de TestToXmlLutRendererFromXml */
      CPPUNIT_TEST(TestToXmlLutRendererFromXml);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;
public:
   /** ctor */
   LutArrayTest();

protected:
   /** Verifica que de error si el vector esta vacio */
   void TestEmptyToXml();
   /** Verifica que de error si el nodo esta vacio */
   void TestToXmlEmptyTable();
   /** Test tabla a nodo */
   void TestToXmlTable();
   /** Test nodo a tabla */
   void TestFromXml();
   /** Verficia que tabla de renderizador sea igual a la del nodo */
   void TestLutRendererToXmlFromXml();
   /** Convierte nodo a lut array, luego lut array a nodo y los compara */
   void TestToXmlLutRendererFromXml();
};
}

#endif /* LUTARRAYTEST_H_ */
