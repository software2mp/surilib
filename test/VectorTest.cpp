/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
