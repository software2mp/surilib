/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
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
