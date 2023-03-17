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

// Includes estandar

// Includes Suri
#include "LutArrayTest.h"
#include "suri/XmlFunctions.h"
#include "LutRenderer.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {

/** Compara una variable de tipo LutRenderer::Parameters con una LutArray */
bool CompareLuts(const LutRenderer::Parameters &Parameters, const LutArray &Lut);

/**
 * Constructor
 */
LutArrayTest::LutArrayTest() {
}

/**
 * Destructor
 */
void LutArrayTest::TestEmptyToXml() {
   LutArray lut;
   wxXmlNode *plutnode = NULL;
   CPPUNIT_ASSERT_MESSAGE("Convertir Array vacio debe dar error", !lut.ToXml(plutnode));
   delete plutnode;
}

/**
 * A una variable LutArray le agraga una LookUpTable vacia, y lo convierte a un
 * nodo xml.
 * El test tiene exito si falla (devuelve false) el metodo que convierte a xml.
 */
void LutArrayTest::TestToXmlEmptyTable() {
   LutArray lut;
   lut.AddLookUpTable(LookUpTable(), 0);
   wxXmlNode *plutnode = NULL;
   CPPUNIT_ASSERT_MESSAGE("Convertir Array con LookUpTable vacia debe dar error",
                          !lut.ToXml(plutnode));
   delete plutnode;
}

/**
 * Crea una variable LutArray y le agrega dos LookUpTables. Convierte la variable
 * LutArray a un nodo xml.
 * El test tiene exito si el metodo que convierte a xml devuelve true.
 */
void LutArrayTest::TestToXmlTable() {
   LutArray lut;
   LookUpTable table;
   table.AddBin(0, 0);
   table.AddBin(1, 1);
   table.AddBin(2, 2);
   lut.AddLookUpTable(table, 4);
   table.SetTable();
   table.AddBin(0, 0);
   table.AddBin(1.0 / 3.0, 1);
   table.AddBin(2.0 / 3.0, 2);
   lut.AddLookUpTable(table, 1);

   wxXmlNode *plutnode = NULL;
   CPPUNIT_ASSERT_MESSAGE("Falla al convertir LutArray a Nodo", lut.ToXml(plutnode));
   delete plutnode;
}

/**
 * Carga un archivo xml con un nodo lutarray y lo convierte a una variable LutArray.
 * El test tiene exito si el metodo que convierte de xml a LutArray devuelve true.
 */
void LutArrayTest::TestFromXml() {
   wxXmlDocument doc(__TEST_PATH__ "nodosxml/SimpleLutArray.xml");
   CPPUNIT_ASSERT_MESSAGE("No pudo leer el XML del test", doc.IsOk());
   LutArray lut;
   CPPUNIT_ASSERT_MESSAGE("Falla al convertir Nodo a LutArray",
                          LutArray::FromXml(doc.GetRoot(), lut));
}

/**
 * A partir de un archivo xml con un nodo Lut, crea una variable LutRenderer::Parameters
 * y una variable LutArray, y las compara.
 * El test tiene exito si tienen exito los metodos que obtienen las variables
 * a partir de un nodo xml, y ademas si las variables obtenidas (LutRenderer::Parameters
 * y LutArray) son iguales.
 */
void LutArrayTest::TestLutRendererToXmlFromXml() {
   wxXmlDocument doc(__TEST_PATH__ "nodosxml/Lut.xml");
   CPPUNIT_ASSERT_MESSAGE("No pudo leer el XML del test", doc.IsOk());
   LutRenderer::Parameters params = LutRenderer::GetParameters(doc.GetRoot());
   LutArray lut;
   CPPUNIT_ASSERT_MESSAGE(
         "No pudo convertir XML leido a LutArray",
         LutArray::FromXml(doc.GetRoot()->GetChildren()->GetChildren(), lut));

   CPPUNIT_ASSERT_MESSAGE(
         "Las LutArray y LutRenderer::Parameters leidos del mismo XML son distintos",
         CompareLuts(params, lut));
}

/**
 * Carga un archivo xml con un LutArray, y obtiene una variable LutArray a
 * partir del nodo. Usa esta variable para convertirla a un nodo Lut. Con este
 * nodo se genera una variable de tipo Lut.
 * El test tiene exito si las dos variables (LutArray y Lut) son iguales.
 */
void LutArrayTest::TestToXmlLutRendererFromXml() {
   wxXmlDocument doc(__TEST_PATH__ "nodosxml/SimpleLutArray.xml");
   CPPUNIT_ASSERT_MESSAGE("No pudo leer el XML del test", doc.IsOk());
   LutArray lut;
   wxXmlNode *plutnode = doc.GetRoot();
   doc.DetachRoot();
   // Cargo una lut
   CPPUNIT_ASSERT_MESSAGE("Falla al convertir Nodo a LutArray",
                          LutArray::FromXml(plutnode, lut));
   delete plutnode;
   // la convierto en XML
   CPPUNIT_ASSERT_MESSAGE("Falla al convertir LutArray a Nodo",
                          LutArray::ToXml(lut, plutnode));
   wxXmlNode *pelementnode = new wxXmlNode(wxXML_ELEMENT_NODE, wxEmptyString,
                                           wxEmptyString);
   (new wxXmlNode(pelementnode, wxXML_ELEMENT_NODE, wxT(RENDERIZATION_NODE)))->AddChild(
         plutnode);
   LutRenderer::Parameters params = LutRenderer::GetParameters(pelementnode);
   delete pelementnode;

   CPPUNIT_ASSERT_MESSAGE(
         "Los valores de LutRenderer::Parameters leidos de XML generado por LutArray son distintos a los de LutArray",
         CompareLuts(params, lut));
}

/**
 * Metodo que permite comparar una variable de tipo LutRenderer::Parameters y
 * una variable de tipo LutArray
 * @param[in] Parameters variable de tipo LutRenderer::Parameters
 * @param[in] Lut variable de tipo LutArray
 * @return true si para cada lut incluida en ambas variables:
 *    -	La banda de salida en LutRenderer::Parameters es igual a la posicion
 *       de la lut en LutArray
 *    -  Si para cada banda de salida, la banda de entrada es la misma en
 *       ambas variables.
 *    -  Si el resultado de aplicar la tabla a cada uno de los valores de la Lut
 *       es el mismo para ambas variables.
 */
bool CompareLuts(const LutRenderer::Parameters &Parameters, const LutArray &Lut) {
   // El numero de bandas de salida debe ser 3
   if (Lut.GetCount() != Parameters.lut_.GetCount()) {
      return false;
   }

   // Analizo que no se modifique el orden de las bandas.
   int inputband = 0;
   int otherinputband = 0;
   LookUpTable::LutType luttype;
   LookUpTable::LutType otherluttype;
   for (int i = 0; i < Parameters.lut_.GetCount(); i++) {
      Lut.GetLookUpTable(i, inputband).GetTable(luttype);
      Parameters.lut_.GetLookUpTable(i, otherinputband).GetTable(otherluttype);
      if (luttype != otherluttype || inputband != otherinputband) {
         return false;
      }
   }
   return true;
}
}
