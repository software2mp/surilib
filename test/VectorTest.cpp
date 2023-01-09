/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#include "VectorTest.h"

// Includes estandar

// Includes Suri
#include "suri/Vector.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/**
 * Constructor.
 */
VectorTest::VectorTest() {
}

/**
 * Destructor.
 */
VectorTest::~VectorTest() {
}

/** Test que analiza si copiar vectores entre disco y memoria funciona */
void VectorTest::TestCopy() {
   suri::Vector *pin = suri::Vector::Open(
         __TEST_PATH__ "coberturas/TestShapefile-Lines.shp");

   CPPUNIT_ASSERT_MESSAGE(
         "No se pudo abrir el archivo de entrada: " __TEST_PATH__ "coberturas/TestShapefile-Lines.shp",
         pin);
   if (!pin) {
      return;
   }
   suri::Vector *pout = suri::Vector::Open("shpmemory:/test.shp",
                                           suri::Vector::ReadWrite);
   CPPUNIT_ASSERT_MESSAGE("shpmemory:/test.shp", pout);
   suri::Vector *pout2 =
         suri::Vector::Open(
               "No se pudo abrir el archivo de salida: " __TEST_PATH__ "temp/VectorCopy-test.shp",
               suri::Vector::ReadWrite);
   CPPUNIT_ASSERT_MESSAGE(
         "No se pudo abrir el archivo de salida: " __TEST_PATH__ "temp/VectorCopy-test.shp",
         pout2);
   if (!pout || !pout2) {
      suri::Vector::Close(pin);
      suri::Vector::Close(pout);
      suri::Vector::Close(pout2);
      return;
   }
   pin->Copy(pout);
   pin->Copy(pout2);
   suri::Vector::Close(pin);
   pout->Save(__TEST_PATH__ "temp/VectorCopy-salida.xml", "KML");
   pout->Save(__TEST_PATH__ "temp/VectorCopy-salida.shp", "ESRI Shapefile");
   suri::Vector::Close(pout);
   suri::Vector::Close(pout2);
   pout = suri::Vector::Open(__TEST_PATH__ "temp/VectorCopy-salida.shp");
   CPPUNIT_ASSERT_MESSAGE(
         "No se pudo abrir el archivo copiado: " __TEST_PATH__ "temp/VectorCopy-salida.shp",
         pout);
   if (!pout) {
      suri::Vector::Close(pout);
   }
   suri::Vector::Close(pout);
#ifdef __GDAL_SUPPORTS_KML_READ__
   pout = suri::Vector::Open(__TEST_PATH__ "temp/VectorCopy-salida.xml");
   CPPUNIT_ASSERT_MESSAGE(
         "No se pudo abrir el archivo copiado: " __TEST_PATH__ "temp/VectorCopy-salida.shp", pout);
   if ( !pout ) {
      suri::Vector::Close(pout);
   }
#endif
   return;
}
}
