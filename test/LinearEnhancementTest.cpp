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

#include "LinearEnhancementTest.h"

#include "LinearEnhancement.h"
#include "Enhancement.h"
#include "suri/LookUpTable.h"
#include "suri/LutArray.h"
#include "suri/AuxiliaryFunctions.h"

/** namespace suri */
namespace suri {
// Defines
#define ENHANCEMENT_TYPE "LinearEnhancement"

/**
 * Constructor
 */
LinearEnhancementTest::LinearEnhancementTest() {
}

/**
 * Destructor
 */
LinearEnhancementTest::~LinearEnhancementTest() {
}

/**
 * Crea una lut pasando como parametro cero bins, debe devolver falso el
 * metodo ComputeLut
 */
void LinearEnhancementTest::TestZeroBins() {
   suri::Enhancement *penhancement = Enhancement::Create(ENHANCEMENT_TYPE);
   CPPUNIT_ASSERT_MESSAGE("No se pudo crear una instancia del realce", penhancement);
   LutArray lut;
   Statistics stats;
   stats.count_ = 100;
   stats.min_.push_back(10);
   stats.max_.push_back(50);

   CPPUNIT_ASSERT_MESSAGE(
         "No se tendria que crear una tabla de luts a partir de cero bins",
         (!penhancement->ComputeLut(lut, stats, std::make_pair(20, 50), 0) ));
   Enhancement::Destroy(penhancement);
}

/**
 * Crea una lut pasando como parametro estadisticas invalidas, debe devolver falso el
 * metodo ComputeLut
 */
void LinearEnhancementTest::TestInvalidStats() {
   suri::Enhancement *penhancement = Enhancement::Create(ENHANCEMENT_TYPE);
   CPPUNIT_ASSERT_MESSAGE("No se pudo crear una instancia del realce", penhancement);
   LutArray lut;
   Statistics stats;
   stats.count_ = 0;
   stats.min_.push_back(10);
   stats.max_.push_back(50);

   CPPUNIT_ASSERT_MESSAGE(
         "No se tendria que crear una tabla de luts a partir de cero bins",
         (!penhancement->ComputeLut(lut, stats, std::make_pair(20, 50), 10) ));
   Enhancement::Destroy(penhancement);
}

/**
 * Crea una lut a partir de unas estadisticas en donde el maximo y el minimo
 * son iguales y estan dentro del rando de la lut.
 * Se compara con una lut a la cual se le cargo un valor igual al que deberia
 * tener la lut creada.
 * Ambas lut se transforman en noo xml.
 * @return true si ambos nodos son iguales.
 * @return false si los nodos son distintos.
 */
void LinearEnhancementTest::TestEqualValuesInsideOutputRange() {
   LookUpTable lut;

   double min = 50;
   double max = 50;
   std::pair<double, double> outputrange = std::make_pair(10, 100);
   int bins = 5;

   CreateLut(lut, min, max, outputrange, bins);
   CPPUNIT_ASSERT_MESSAGE("La lut generada no es valida",
                          TestLut(lut, min, max, outputrange, bins));
}

/**
 * Crea una lut a partir de unas estadisticas en donde el maximo y el minimo
 * son iguales y estan fuera del rando de la lut.
 * Se compara con una lut a la cual se le cargo un valor igual al que deberia
 * tener la lut creada.
 * Ambas lut se transforman en noo xml.
 * @return true si ambos nodos son iguales.
 * @return false si los nodos son distintos.
 */
void LinearEnhancementTest::TestEqualValuesOutsideOutputRange() {
   LookUpTable lutmaxoutside;

   double min = 50;
   double max = 50;
   std::pair<double, double> outputrange = std::make_pair(10, 20);
   int bins = 10;

   CreateLut(lutmaxoutside, min, max, outputrange, bins);
   CPPUNIT_ASSERT_MESSAGE("La lut generada no es valida",
                          TestLut(lutmaxoutside, min, max, outputrange, bins));

   LookUpTable lutminoutside;
   min = 5;
   max = 5;
   bins = 10;

   CreateLut(lutminoutside, min, max, outputrange, bins);
   CPPUNIT_ASSERT_MESSAGE("La lut generada no es valida",
                          TestLut(lutminoutside, min, max, outputrange, bins));
}

/**
 * Crea una lut a partir de un rango de salida cuyos maximos y minimos son iguales
 */
void LinearEnhancementTest::TestEqualOutputRange() {
   LookUpTable lut;

   double min = 10;
   double max = 50;
   std::pair<double, double> outputrange = std::make_pair(20, 20);
   int bins = 5;

   CreateLut(lut, min, max, outputrange, bins);
   CPPUNIT_ASSERT_MESSAGE("La lut generada no es valida",
                          TestLut(lut, min, max, outputrange, bins));
}

/**
 * Metodo que genera una variable de clase Statistics con los parametros recibidos
 * y utiliza la clase realce lineal para crear una lut.
 * @param[out] Lut lut generada
 * @param[in] Mins vector con los minimos
 * @param[in] Maxs vector con los maximos
 * @param[in] OutputRange rango de valores de salida de la lut
 * @param[in] BinsCount cantidad de bines
 */
void LinearEnhancementTest::CreateLut(LookUpTable &Lut, double Mins, double Maxs,
                                      std::pair<double, double> OutputRange,
                                      int BinsCount) {
   suri::Enhancement *penhancement = Enhancement::Create(ENHANCEMENT_TYPE);
   CPPUNIT_ASSERT_MESSAGE("No se pudo crear una instancia del realce", penhancement);
   LutArray lutarray;
   Statistics stats;
   stats.count_ = 100;
   stats.min_.push_back(Mins);
   stats.max_.push_back(Maxs);
   CPPUNIT_ASSERT_MESSAGE(
         "No se pudo crear la tabla de luts",
         penhancement->ComputeLut(lutarray, stats, OutputRange, BinsCount));
   Enhancement::Destroy(penhancement);
   int inputband;
   Lut = lutarray.GetLookUpTable(0, inputband);
}

/**
 * Crea una lut y verifica que los valores de salida coincidan con un realce
 * lineal
 */
void LinearEnhancementTest::TestEnhancementValues() {
   LookUpTable lut;

   double min = 0;
   double max = 5;
   std::pair<double, double> outputrange = std::make_pair(0, 5);
   int bins = 5;
   CreateLut(lut, min, max, outputrange, bins);

   bool result = true;

   // Comparo los valores internos
   if (0 != lut.ApplyLut(-1) || 0 != lut.ApplyLut(-0.5) || 0 != lut.ApplyLut(0)
         || 1 != lut.ApplyLut(0.4) || 1 != lut.ApplyLut(1) || 2 != lut.ApplyLut(1.5)
         || 2 != lut.ApplyLut(2) || 3 != lut.ApplyLut(2.3) || 3 != lut.ApplyLut(3)
         || 4 != lut.ApplyLut(3.8) || 4 != lut.ApplyLut(4) || 5 != lut.ApplyLut(4.6)
         || 5 != lut.ApplyLut(5) || 5 != lut.ApplyLut(5.5)) {
      result = false;
   }

   CPPUNIT_ASSERT_MESSAGE("La lut generada no corresponde a un realce lineal", result);
}

/**
 * Crea una lut y verifica que los valores de salida coincidan con un realce
 * lineal, usando un rango de salida invertido
 */
void LinearEnhancementTest::TestEnhancementValuesInvertedOutputRange() {
   LutArray lut;
   suri::Enhancement *penhancement = Enhancement::Create(ENHANCEMENT_TYPE);
   CPPUNIT_ASSERT_MESSAGE("No se pudo crear una instancia del realce", penhancement);

   std::pair<double, double> outputrange = std::make_pair(5, 0);
   Statistics stats;
   stats.count_ = 100;
   stats.min_.push_back(0);
   stats.max_.push_back(5);
   CPPUNIT_ASSERT_MESSAGE(
         "No se tendria que crear una tabla de luts a partir de cero bins",
         (!penhancement->ComputeLut(lut, stats, outputrange, 5) ));
   Enhancement::Destroy(penhancement);
}

/**
 * Crea una lut y verifica que los valores de salida coincidan con un realce
 * lineal, usando maximo y minimo invertidos
 */
void LinearEnhancementTest::TestEnhancementInvertedValues() {
   LookUpTable lut;

   double min = 5;
   double max = 0;
   std::pair<double, double> outputrange = std::make_pair(0, 5);
   int bins = 5;
   CreateLut(lut, min, max, outputrange, bins);

   bool result = true;

   // Comparo los valores con los esperados
   if (5 != lut.ApplyLut(-1) || 5 != lut.ApplyLut(-0.5) || 5 != lut.ApplyLut(0)
         || 4 != lut.ApplyLut(0.4) || 4 != lut.ApplyLut(1) || 3 != lut.ApplyLut(1.5)
         || 3 != lut.ApplyLut(2) || 2 != lut.ApplyLut(2.3) || 2 != lut.ApplyLut(3)
         || 1 != lut.ApplyLut(3.8) || 1 != lut.ApplyLut(4) || 0 != lut.ApplyLut(4.6)
         || 0 != lut.ApplyLut(5) || 0 != lut.ApplyLut(5.5)) {
      result = false;
   }

   CPPUNIT_ASSERT_MESSAGE("La lut generada no corresponde a un realce lineal", result);
}
/**
 * Verifica que una lut represente un realce lineal.
 * parametros
 * @param[in] Lut lut
 * @param[in] Min minimo
 * @param[in] Max maximo
 * @param[in] OutputRange rango para los valores de salida
 * @param[in] BinsCount cantidad de bines
 */
bool LinearEnhancementTest::TestLut(LookUpTable Lut, double Min, double Max,
                                    std::pair<double, double> OutputRange,
                                    int BinsCount) {
   // Pruebo valores fuera de los valores extremos de la lut
   if (Lut.ApplyLut(Min - 1) != Lut.ApplyLut(Min)
         || Lut.ApplyLut(Max + 1) != Lut.ApplyLut(Max + 1)) {
      return false;
   }

   // Si el minimo y el maximo son iguales, en los puntos intermedios del rango
   // de la lut la entrada es igual a la salida
   if (Min == Max) {
      if (Min > OutputRange.first && Min <= OutputRange.second
            && Min != Lut.ApplyLut(Min)) {
         return false;
      }
   }
   // Si no son iguales compara las salidas con el resultado de aplicar la recta.
   else {
      float segment = (Max - Min) / BinsCount;
      // Obtengo los parametros de la recta
      double Slope = (OutputRange.second - OutputRange.first) / (Max - Min);
      double Intercept = (OutputRange.first * Max - OutputRange.second * Min)
            / (Max - Min);

      for (int i = 0; i <= BinsCount; i++)
         if (SURI_ROUND(int, (Min+segment*i)*Slope+Intercept)
               != Lut.ApplyLut(Min + segment * i)) {
            return false;
         }
   }
   return true;
}
}
