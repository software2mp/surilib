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

#ifndef STATISTICNODETEST_H_
#define STATISTICNODETEST_H_

// Includes standard
// Includes Suri
#include "suri/Tests.h"
#include "ClassificationRenderer.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Test de lectura/escritura de nodo de estadisticas. */
/**
 * Verifica que se pueda generar un nodo estadisticas a partir
 * de los parametros y a la inversa.
 */
class StatisticNodeTest : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase StatisticNodeTest. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(StatisticNodeTest);
      /** Evalua resultado de CreateParameterFromEmptyNode */
      CPPUNIT_TEST(CreateParameterFromEmptyNode);
      /** Evalua resultado de CreateParameterFromNode */
      CPPUNIT_TEST(CreateParameterFromNode);
      /** Evalua resultado de CreateNodeFromParameter */
      CPPUNIT_TEST(CreateNodeFromParameter);
      /** Evalua resultado de TestParametersDimension */
      CPPUNIT_TEST(TestParametersDimension);
      /** Evalua resultado de TesXmlInvalidDimension */
      CPPUNIT_TEST(TesXmlInvalidDimension);
      /** Evalua resultado de CompareParameterFromDifferentNodes */
      CPPUNIT_TEST(CompareParameterFromDifferentNodes);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;
public:
   /** Ctor. */
   StatisticNodeTest();
   /** Dtor. */
   virtual ~StatisticNodeTest();
protected:
   /** Compara parameters vacio contra uno creado a partir de un nodo vacio */
   void CreateParameterFromEmptyNode();
   /** Crea una variable parameter desde un nodo xml */
   void CreateParameterFromNode();
   /** Crea nodo a partir de parametros */
   void CreateNodeFromParameter();
   /** Analiza que la dimension se que se obtuvo del nodo sea valida */
   void TestParametersDimension();
   /** Crea una variable parameters a partir de un nodo xml con dimensiones inconsistentes */
   void TesXmlInvalidDimension();
   /** compara parameters generados desde nodos con distinto orden */
   void CompareParameterFromDifferentNodes();
};

/** compara dos variables de tipo parameters */
bool operator==(const ClassificationRenderer::Parameters &Lhs,
                const ClassificationRenderer::Parameters &Rhs);

/** Compara dos variables de tipo statistics */
bool operator==(const Statistics &Lhs, const Statistics &Rhs);
}

#endif /* STATISTICNODETEST_H_ */
