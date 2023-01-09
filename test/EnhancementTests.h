/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GAUSSENHANCEMENTTESTS_H_
#define GAUSSENHANCEMENTTESTS_H_

// Includes estandar
// Includes Suri
#include "suri/Tests.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
class LookUpTable;
class Statistics;

/** Tests para la clace Enhancement (y sus clases derivadas) */
/**
 * Compara enhancements que genera la aplicacion con los que genera el
 * envi. Utiliza las estadisticas que genera el envi para que esto
 * no sea fuente de error.
 */
class EnhancementTests : public CPPUNIT_NS::TestFixture {
   /** Inicializa test para la clase EnhancementTests. Invoca a setUp. */
   CPPUNIT_TEST_SUITE(GaussEnhancementTests);
      /** Evalua resultado de CompareEnviTest */
      CPPUNIT_TEST(CompareEnviTest);
      /** Finaliza test. Invoca a tearDown. */
      CPPUNIT_TEST_SUITE_END()
   ;
public:
   /** Ctor. */
   EnhancementTests();
   /** Dtor. */
   virtual ~EnhancementTests();
protected:
// Tests
   /** Analizo la LUT generada es igual a la obtenida con ENVI. */
   void CompareEnviTest();

   /** Comparo el enhancement de la aplicacion con la del ENVI */
   void CompareImage(const std::string &Size, const std::string &Enhancement,
                     const std::string &ImageType, LookUpTable &LookUpTable);
   /** Genero tabla de Enhancement utilizada por la aplicacion */
   LookUpTable GetEnhancementTable(Statistics Stats, const std::string &Enhancement);
   /** Obtengo estadisticas usando datos de ENVI */
   bool GetStatistics(const std::string &Size, const std::string &Type,
                      Statistics &Stats);
};
}

#endif /* GAUSSENHANCEMENTTESTS_H_ */
