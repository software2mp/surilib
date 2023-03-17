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
