/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes estandar
#include <iostream>
#include <limits>

// Includes Suri
#include "LookUpTableTest.h"
#include "suri/XmlFunctions.h"

// Includes Wx
#include "LutRenderer.h"
#include "suri/LookUpTable.h"

// Includes App

// Defines

#define VERIFY_LUT(Table)  Table.ApplyLut(-1)  == 0 &&                        \
   Table.ApplyLut(0)   == 0 &&                        \
   Table.ApplyLut(0.5) == 1 &&                        \
   Table.ApplyLut(1)   == 1 &&                        \
   Table.ApplyLut(1.5) == 2 &&                        \
   Table.ApplyLut(2)   == 2 &&                        \
   Table.ApplyLut(2.5) == 2
#define LOAD_LUT(Table) Table.AddBin(0, 0);                                    \
   Table.AddBin(1, 1);                                    \
   Table.AddBin(2, 2);
#define VERIFY_FLOAT_LUT(Table)  Table.ApplyLut(-1)== 0 &&                    \
   Table.ApplyLut(0)       == 0 &&                    \
   Table.ApplyLut(1.0/3.0 - 0.00001)  == 1 &&         \
   Table.ApplyLut(1.0/3.0) == 1 &&                    \
   Table.ApplyLut(1.0/3.0 + 0.00001)  == 2 &&         \
   Table.ApplyLut(2.0/3.0 - 0.00001)  == 2 &&         \
   Table.ApplyLut(2.0/3.0) == 2 &&                    \
   Table.ApplyLut(2.0/3.0 + 0.00001)  == 2

#define LOAD_FLOAT_LUT(Table) Table.AddBin(0.0, 0);                            \
   Table.AddBin(1.0/3.0, 1);                              \
   Table.AddBin(2.0/3.0, 2);

/** namespace suri */
namespace suri {

/**
 * Constructor
 */
LookUpTableTest::LookUpTableTest() {
}

/**
 * Destructor
 */
LookUpTableTest::~LookUpTableTest() {
}

/**
 * Trata de generar un nodo xml a partir de una lut vacia.
 */
void LookUpTableTest::TestEmptyLutToXml() {
   LookUpTable lut;
   wxXmlNode *plutnode = NULL;
   bool retval = !lut.ToXml(plutnode);
   delete plutnode;

   CPPUNIT_ASSERT_MESSAGE("Error. No detecto lut vacia", retval);
}

/**
 * Crea una lut con enteros y verifica el resultado de aplicar la lut
 * a los valores limites
 */
void LookUpTableTest::TestLimits() {
   LookUpTable lut;
   LOAD_LUT(lut);

   CPPUNIT_ASSERT_MESSAGE("Error en limites de lut", VERIFY_LUT(lut));
}

/**
 * Crea una lut con floats y verifica el resultado de aplicar la lut
 * a los valores limites
 */
void LookUpTableTest::TestDoubleLimits() {
   LookUpTable lut;
   LOAD_FLOAT_LUT(lut);
   CPPUNIT_ASSERT_MESSAGE("Error en limites de lut", VERIFY_FLOAT_LUT(lut));
}

/**
 * Carga un archivo xml con una lut y la transforma en una variable LookUpTable
 */
void LookUpTableTest::TestFromXml() {
   wxXmlDocument doc(__TEST_PATH__ "nodosxml/SimpleLut.xml");
   if (!doc.IsOk()) {
      CPPUNIT_ASSERT_MESSAGE("Error en documento", false);
      return;
   }
   LookUpTable lut;
   CPPUNIT_ASSERT_MESSAGE("Error al leer lut de xml",
                          LookUpTable::FromXml(doc.GetRoot(), lut) && VERIFY_LUT(lut));
}

/**
 * Carga un archivo xml con un nodo lut. crea una lut (con los mismos datos que
 * el nodo del archivo) y usa el metodo que transforma la lut en un nodo xml.
 * Compara ambos nodos (el obtenido del archivo y el generado a partir de una
 * variable LookUpTable).
 */
void LookUpTableTest::TestToXml() {
   wxXmlDocument doc(__TEST_PATH__ "nodosxml/SimpleLut.xml");
   if (!doc.IsOk()) {
      CPPUNIT_ASSERT_MESSAGE("Error en documento", false);
      return;
   }
   LookUpTable lut;
   LOAD_LUT(lut);
   wxXmlNode *pnode = NULL;
   bool retval = LookUpTable::ToXml(lut, pnode);
   // depende de la implementacion de == para XML
   retval &= *pnode == *doc.GetRoot();
   delete pnode;

   CPPUNIT_ASSERT_MESSAGE("Error al transformar lut a xml", retval);
}

/**
 * Crea una variable LookUpTable con valores float. Crea un nodo a partir de esta
 * variable. Transforma el nodo en una variable LookUpTable y verifica el resultado
 * de aplicar esta lut.
 */
void LookUpTableTest::TestDoubleToXmlFromXml() {
   LookUpTable lut;
   LOAD_FLOAT_LUT(lut);
   wxXmlNode *plutnode = NULL;
   bool retval = LookUpTable::ToXml(lut, plutnode);
   LookUpTable lutfromxml;
   retval &= LookUpTable::FromXml(plutnode, lutfromxml);
   delete plutnode;
   CPPUNIT_ASSERT_MESSAGE("Error al transformar a xml y luego a lut",
                          retval && VERIFY_FLOAT_LUT(lutfromxml));
}

/**
 * Verifico que lut del renderizador se cargue correctamente.
 */
void LookUpTableTest::TestFromXmlLutRenderer() {
   LookUpTable lut;
   LOAD_FLOAT_LUT(lut);
   LutRenderer::Parameters parameters;
   LookUpTable::LutType table;
   lut.GetTable(table);
   // inserta uno mas porque LutRenderer saca el "infinito"
   LookUpTable lookuptable;
   lookuptable.SetTable(table);
   table.insert(std::make_pair(std::numeric_limits<double>::max(), 0));
   parameters.lut_.AddLookUpTable(lookuptable, 0);

   wxXmlNode *prenderernode = LutRenderer::GetXmlNode(parameters);
   if (!prenderernode) {
      CPPUNIT_ASSERT_MESSAGE("Error al calcular el nodo", false);
      return;
   }
   LookUpTable lutfromrenderer;
   LookUpTable::FromXml(prenderernode->GetChildren(), lutfromrenderer);

   CPPUNIT_ASSERT_MESSAGE("Error en TestFromXmlLutRenderer",
                          VERIFY_FLOAT_LUT(lutfromrenderer));
}

}
