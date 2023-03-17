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

#include <cmath>
#include <map>

#include "SREGaussianEnhancement.h"
#include "SREEnhancement.h"
#include "SREGaussianEnhancement.h"
#include "SREEnhancementFactory.h"
#include "suri/ParameterCollection.h"

namespace suri {
namespace raster {
namespace enhancement {

/** Funcion auxiliar para armar el rango **/
GaussianEnhancement::range make_range(double lower, double upper) {
   if (upper < lower) {
      return std::make_pair(upper, lower);
   }
   return std::make_pair(lower, upper);
}

/**
 * Registro el realce gaussiano.
 */
REGISTER_ENHANCEMENT(GaussianEnhancement, true)

/**
 * Ctor.
 */
GaussianEnhancement::GaussianEnhancement(int BandCount, int* pNumBins, double* pMin,
                                         double* pMax, double** ppBAFP, double* pMean,
                                         double* pStdev) :
      Enhancement(BandCount, pNumBins), pMin_(pMin), pMax_(pMax), pMean_(pMean),
      pStdev_(pStdev), ppBAFP_(ppBAFP) {
   LoadRangeTable();
}

/**
 * Dtor.
 */
GaussianEnhancement::~GaussianEnhancement() {

}

/**
 * Crea una lookup table a partir de un histograma de intensidad
 * utilizando una distribucion normal con media 127 y desvio 42.5
 *
 */
int** GaussianEnhancement::CreateLUT(int** ppHistA, int** ppHistB) {
   int** lut = new int*[bandCount_];
   for (int ix = 0; ix < bandCount_; ++ix) {
      lut[ix] = new int[pNumBins_[ix]];
      memset(lut[ix], 0, sizeof(int) * pNumBins_[ix]);
   }

   // Para cada banda se calcula
   for (int bandix = 0; bandix < bandCount_; ++bandix) {
      // Por cada banda se tiene asociado una cantidad de bin. Por cada
      // bin se calcula el valor
      for (int fix = 0; fix < 256; ++fix) {
         int value = GetValueFromMap(make_range(ppBAFP_[bandix][fix], ppBAFP_[bandix][fix]));
         lut[bandix][fix] = value;
      }
   }
   return lut;
}

/**
 * Crea una instancia de GaussianEnhancement.
 */
Enhancement* GaussianEnhancement::Create(suri::ParameterCollection& Params) {
   int bandcount = 0;
   Params.GetValue<int>("BandCount", bandcount);

   int* pnumbins = NULL;
   Params.GetValue<int*>("NumBins", pnumbins);

   double* pmin = NULL;
   Params.GetValue<double*>("Min", pmin);

   double* pmax = NULL;
   Params.GetValue<double*>("Max", pmax);

   double* pmean = NULL;
   Params.GetValue<double*>("Mean", pmean);

   double* pstdev = NULL;
   Params.GetValue<double*>("Stdev", pstdev);

   double** ppbafp = NULL;
   Params.GetValue<double**>("BAFP", ppbafp);

   return new GaussianEnhancement(bandcount, pnumbins, pmin, pmax, ppbafp, pmean,
                                  pstdev);
}

/** Metodo auxiliar que llena la tabla de rangos para la dist normal **/
void GaussianEnhancement::LoadRangeTable() {
   // Tabla normal media 127 y desvio 42.5
   gaussRangeTable_.clear();
   gaussRangeTable_.insert(std::make_pair(make_range(0.00, 0.14), 0));
   gaussRangeTable_.insert(std::make_pair(make_range(0.14, 0.15), 1));
   gaussRangeTable_.insert(std::make_pair(make_range(0.15, 0.16), 2));
   gaussRangeTable_.insert(std::make_pair(make_range(0.16, 0.18), 3));
   gaussRangeTable_.insert(std::make_pair(make_range(0.18, 0.19), 4));
   gaussRangeTable_.insert(std::make_pair(make_range(0.19, 0.21), 5));
   gaussRangeTable_.insert(std::make_pair(make_range(0.21, 0.22), 6));
   gaussRangeTable_.insert(std::make_pair(make_range(0.22, 0.24), 7));
   gaussRangeTable_.insert(std::make_pair(make_range(0.24, 0.26), 8));
   gaussRangeTable_.insert(std::make_pair(make_range(0.26, 0.28), 9));
   gaussRangeTable_.insert(std::make_pair(make_range(0.28, 0.30), 10));
   gaussRangeTable_.insert(std::make_pair(make_range(0.30, 0.32), 11));
   gaussRangeTable_.insert(std::make_pair(make_range(0.32, 0.34), 12));
   gaussRangeTable_.insert(std::make_pair(make_range(0.34, 0.37), 13));
   gaussRangeTable_.insert(std::make_pair(make_range(0.37, 0.39), 14));
   gaussRangeTable_.insert(std::make_pair(make_range(0.39, 0.42), 15));
   gaussRangeTable_.insert(std::make_pair(make_range(0.42, 0.45), 16));
   gaussRangeTable_.insert(std::make_pair(make_range(0.45, 0.48), 17));
   gaussRangeTable_.insert(std::make_pair(make_range(0.48, 0.52), 18));
   gaussRangeTable_.insert(std::make_pair(make_range(0.52, 0.55), 19));
   gaussRangeTable_.insert(std::make_pair(make_range(0.55, 0.59), 20));
   gaussRangeTable_.insert(std::make_pair(make_range(0.59, 0.63), 21));
   gaussRangeTable_.insert(std::make_pair(make_range(0.63, 0.67), 22));
   gaussRangeTable_.insert(std::make_pair(make_range(0.67, 0.72), 23));
   gaussRangeTable_.insert(std::make_pair(make_range(0.72, 0.77), 24));
   gaussRangeTable_.insert(std::make_pair(make_range(0.77, 0.82), 25));
   gaussRangeTable_.insert(std::make_pair(make_range(0.82, 0.87), 26));
   gaussRangeTable_.insert(std::make_pair(make_range(0.87, 0.93), 27));
   gaussRangeTable_.insert(std::make_pair(make_range(0.93, 0.99), 28));
   gaussRangeTable_.insert(std::make_pair(make_range(0.99, 1.06), 29));
   gaussRangeTable_.insert(std::make_pair(make_range(1.06, 1.12), 30));
   gaussRangeTable_.insert(std::make_pair(make_range(1.12, 1.20), 31));
   gaussRangeTable_.insert(std::make_pair(make_range(1.20, 1.27), 32));
   gaussRangeTable_.insert(std::make_pair(make_range(1.27, 1.35), 33));
   gaussRangeTable_.insert(std::make_pair(make_range(1.35, 1.43), 34));
   gaussRangeTable_.insert(std::make_pair(make_range(1.43, 1.52), 35));
   gaussRangeTable_.insert(std::make_pair(make_range(1.52, 1.61), 36));
   gaussRangeTable_.insert(std::make_pair(make_range(1.61, 1.71), 37));
   gaussRangeTable_.insert(std::make_pair(make_range(1.71, 1.81), 38));
   gaussRangeTable_.insert(std::make_pair(make_range(1.81, 1.92), 39));
   gaussRangeTable_.insert(std::make_pair(make_range(1.92, 2.03), 40));
   gaussRangeTable_.insert(std::make_pair(make_range(2.03, 2.15), 41));
   gaussRangeTable_.insert(std::make_pair(make_range(2.15, 2.28), 42));
   gaussRangeTable_.insert(std::make_pair(make_range(2.28, 2.41), 43));
   gaussRangeTable_.insert(std::make_pair(make_range(2.41, 2.68), 44));
   gaussRangeTable_.insert(std::make_pair(make_range(2.68, 2.68), 45));
   gaussRangeTable_.insert(std::make_pair(make_range(2.68, 2.83), 46));
   gaussRangeTable_.insert(std::make_pair(make_range(2.83, 2.99), 47));
   gaussRangeTable_.insert(std::make_pair(make_range(2.99, 3.15), 48));
   gaussRangeTable_.insert(std::make_pair(make_range(3.15, 3.32), 49));
   gaussRangeTable_.insert(std::make_pair(make_range(3.32, 3.50), 50));
   gaussRangeTable_.insert(std::make_pair(make_range(3.50, 3.69), 51));
   gaussRangeTable_.insert(std::make_pair(make_range(3.69, 3.88), 52));
   gaussRangeTable_.insert(std::make_pair(make_range(3.88, 4.08), 53));
   gaussRangeTable_.insert(std::make_pair(make_range(4.08, 4.29), 54));
   gaussRangeTable_.insert(std::make_pair(make_range(4.29, 4.51), 55));
   gaussRangeTable_.insert(std::make_pair(make_range(4.51, 4.74), 56));
   gaussRangeTable_.insert(std::make_pair(make_range(4.74, 4.98), 57));
   gaussRangeTable_.insert(std::make_pair(make_range(4.98, 5.22), 58));
   gaussRangeTable_.insert(std::make_pair(make_range(5.22, 5.48), 59));
   gaussRangeTable_.insert(std::make_pair(make_range(5.48, 5.75), 60));
   gaussRangeTable_.insert(std::make_pair(make_range(5.75, 6.02), 61));
   gaussRangeTable_.insert(std::make_pair(make_range(6.02, 6.31), 62));
   gaussRangeTable_.insert(std::make_pair(make_range(6.31, 6.61), 63));
   gaussRangeTable_.insert(std::make_pair(make_range(6.61, 6.91), 64));
   gaussRangeTable_.insert(std::make_pair(make_range(6.91, 7.23), 65));
   gaussRangeTable_.insert(std::make_pair(make_range(7.23, 7.56), 66));
   gaussRangeTable_.insert(std::make_pair(make_range(7.56, 7.90), 67));
   gaussRangeTable_.insert(std::make_pair(make_range(7.90, 8.25), 68));
   gaussRangeTable_.insert(std::make_pair(make_range(8.25, 8.62), 69));
   gaussRangeTable_.insert(std::make_pair(make_range(8.62, 8.99), 70));
   gaussRangeTable_.insert(std::make_pair(make_range(8.99, 9.38), 71));
   gaussRangeTable_.insert(std::make_pair(make_range(9.38, 9.78), 72));
   gaussRangeTable_.insert(std::make_pair(make_range(9.78, 10.19), 73));
   gaussRangeTable_.insert(std::make_pair(make_range(10.19, 10.62), 74));
   gaussRangeTable_.insert(std::make_pair(make_range(10.62, 11.06), 75));
   gaussRangeTable_.insert(std::make_pair(make_range(11.06, 11.51), 76));
   gaussRangeTable_.insert(std::make_pair(make_range(11.51, 11.97), 77));
   gaussRangeTable_.insert(std::make_pair(make_range(11.97, 12.45), 78));
   gaussRangeTable_.insert(std::make_pair(make_range(12.45, 12.94), 79));
   gaussRangeTable_.insert(std::make_pair(make_range(12.94, 13.44), 80));
   gaussRangeTable_.insert(std::make_pair(make_range(13.44, 13.96), 81));
   gaussRangeTable_.insert(std::make_pair(make_range(13.96, 14.48), 82));
   gaussRangeTable_.insert(std::make_pair(make_range(14.48, 15.03), 83));
   gaussRangeTable_.insert(std::make_pair(make_range(15.03, 15.58), 84));
   gaussRangeTable_.insert(std::make_pair(make_range(15.58, 16.15), 85));
   gaussRangeTable_.insert(std::make_pair(make_range(16.15, 16.73), 86));
   gaussRangeTable_.insert(std::make_pair(make_range(16.73, 17.33), 87));
   gaussRangeTable_.insert(std::make_pair(make_range(17.33, 17.94), 88));
   gaussRangeTable_.insert(std::make_pair(make_range(17.94, 18.56), 89));
   gaussRangeTable_.insert(std::make_pair(make_range(18.56, 19.20), 90));
   gaussRangeTable_.insert(std::make_pair(make_range(19.20, 19.85), 91));
   gaussRangeTable_.insert(std::make_pair(make_range(19.85, 20.51), 92));
   gaussRangeTable_.insert(std::make_pair(make_range(20.51, 21.19), 93));
   gaussRangeTable_.insert(std::make_pair(make_range(21.19, 21.87), 94));
   gaussRangeTable_.insert(std::make_pair(make_range(21.87, 22.57), 95));
   gaussRangeTable_.insert(std::make_pair(make_range(22.57, 23.29), 96));
   gaussRangeTable_.insert(std::make_pair(make_range(23.29, 24.01), 97));
   gaussRangeTable_.insert(std::make_pair(make_range(24.01, 24.75), 98));
   gaussRangeTable_.insert(std::make_pair(make_range(24.75, 25.50), 99));
   gaussRangeTable_.insert(std::make_pair(make_range(25.50, 26.26), 100));
   gaussRangeTable_.insert(std::make_pair(make_range(26.26, 27.03), 101));
   gaussRangeTable_.insert(std::make_pair(make_range(27.03, 27.82), 102));
   gaussRangeTable_.insert(std::make_pair(make_range(27.82, 28.61), 103));
   gaussRangeTable_.insert(std::make_pair(make_range(28.61, 29.42), 104));
   gaussRangeTable_.insert(std::make_pair(make_range(29.42, 30.24), 105));
   gaussRangeTable_.insert(std::make_pair(make_range(30.24, 31.06), 106));
   gaussRangeTable_.insert(std::make_pair(make_range(31.06, 31.90), 107));
   gaussRangeTable_.insert(std::make_pair(make_range(31.90, 32.74), 108));
   gaussRangeTable_.insert(std::make_pair(make_range(32.74, 33.60), 109));
   gaussRangeTable_.insert(std::make_pair(make_range(33.60, 34.46), 110));
   gaussRangeTable_.insert(std::make_pair(make_range(34.46, 35.33), 111));
   gaussRangeTable_.insert(std::make_pair(make_range(35.33, 36.21), 112));
   gaussRangeTable_.insert(std::make_pair(make_range(36.21, 37.09), 113));
   gaussRangeTable_.insert(std::make_pair(make_range(37.09, 37.98), 114));
   gaussRangeTable_.insert(std::make_pair(make_range(37.98, 38.88), 115));
   gaussRangeTable_.insert(std::make_pair(make_range(38.88, 39.79), 116));
   gaussRangeTable_.insert(std::make_pair(make_range(39.79, 40.70), 117));
   gaussRangeTable_.insert(std::make_pair(make_range(40.70, 41.61), 118));
   gaussRangeTable_.insert(std::make_pair(make_range(41.61, 42.53), 119));
   gaussRangeTable_.insert(std::make_pair(make_range(42.53, 43.46), 120));
   gaussRangeTable_.insert(std::make_pair(make_range(43.46, 44.39), 121));
   gaussRangeTable_.insert(std::make_pair(make_range(44.39, 45.32), 122));
   gaussRangeTable_.insert(std::make_pair(make_range(45.32, 46.25), 123));
   gaussRangeTable_.insert(std::make_pair(make_range(46.25, 47.19), 124));
   gaussRangeTable_.insert(std::make_pair(make_range(47.19, 48.12), 125));
   gaussRangeTable_.insert(std::make_pair(make_range(48.12, 49.06), 126));
   gaussRangeTable_.insert(std::make_pair(make_range(49.06, 50.00), 127));
   gaussRangeTable_.insert(std::make_pair(make_range(50.00, 50.94), 128));
   gaussRangeTable_.insert(std::make_pair(make_range(50.94, 51.88), 129));
   gaussRangeTable_.insert(std::make_pair(make_range(51.88, 52.81), 130));
   gaussRangeTable_.insert(std::make_pair(make_range(52.81, 53.75), 131));
   gaussRangeTable_.insert(std::make_pair(make_range(53.75, 54.68), 132));
   gaussRangeTable_.insert(std::make_pair(make_range(54.68, 55.61), 133));
   gaussRangeTable_.insert(std::make_pair(make_range(55.61, 56.54), 134));
   gaussRangeTable_.insert(std::make_pair(make_range(56.54, 57.47), 135));
   gaussRangeTable_.insert(std::make_pair(make_range(57.47, 58.39), 136));
   gaussRangeTable_.insert(std::make_pair(make_range(58.39, 59.30), 137));
   gaussRangeTable_.insert(std::make_pair(make_range(59.30, 60.21), 138));
   gaussRangeTable_.insert(std::make_pair(make_range(60.21, 61.12), 139));
   gaussRangeTable_.insert(std::make_pair(make_range(61.12, 62.02), 140));
   gaussRangeTable_.insert(std::make_pair(make_range(62.02, 62.91), 141));
   gaussRangeTable_.insert(std::make_pair(make_range(62.91, 63.79), 142));
   gaussRangeTable_.insert(std::make_pair(make_range(63.79, 64.67), 143));
   gaussRangeTable_.insert(std::make_pair(make_range(64.67, 65.54), 144));
   gaussRangeTable_.insert(std::make_pair(make_range(65.54, 66.41), 145));
   gaussRangeTable_.insert(std::make_pair(make_range(66.41, 67.26), 146));
   gaussRangeTable_.insert(std::make_pair(make_range(67.26, 68.10), 147));
   gaussRangeTable_.insert(std::make_pair(make_range(68.10, 68.94), 148));
   gaussRangeTable_.insert(std::make_pair(make_range(68.94, 69.77), 149));
   gaussRangeTable_.insert(std::make_pair(make_range(69.77, 70.58), 150));
   gaussRangeTable_.insert(std::make_pair(make_range(70.58, 71.39), 151));
   gaussRangeTable_.insert(std::make_pair(make_range(71.39, 72.18), 152));
   gaussRangeTable_.insert(std::make_pair(make_range(72.18, 72.97), 153));
   gaussRangeTable_.insert(std::make_pair(make_range(72.97, 73.74), 154));
   gaussRangeTable_.insert(std::make_pair(make_range(73.74, 74.50), 155));
   gaussRangeTable_.insert(std::make_pair(make_range(74.50, 75.25), 156));
   gaussRangeTable_.insert(std::make_pair(make_range(75.25, 75.99), 157));
   gaussRangeTable_.insert(std::make_pair(make_range(75.99, 76.71), 158));
   gaussRangeTable_.insert(std::make_pair(make_range(76.71, 77.43), 159));
   gaussRangeTable_.insert(std::make_pair(make_range(77.43, 78.13), 160));
   gaussRangeTable_.insert(std::make_pair(make_range(78.13, 78.82), 161));
   gaussRangeTable_.insert(std::make_pair(make_range(78.82, 79.49), 162));
   gaussRangeTable_.insert(std::make_pair(make_range(79.49, 80.15), 163));
   gaussRangeTable_.insert(std::make_pair(make_range(80.15, 80.80), 164));
   gaussRangeTable_.insert(std::make_pair(make_range(80.80, 81.44), 165));
   gaussRangeTable_.insert(std::make_pair(make_range(81.44, 82.06), 166));
   gaussRangeTable_.insert(std::make_pair(make_range(82.06, 82.67), 167));
   gaussRangeTable_.insert(std::make_pair(make_range(82.67, 83.27), 168));
   gaussRangeTable_.insert(std::make_pair(make_range(83.27, 83.85), 169));
   gaussRangeTable_.insert(std::make_pair(make_range(83.85, 84.42), 170));
   gaussRangeTable_.insert(std::make_pair(make_range(84.42, 84.97), 171));
   gaussRangeTable_.insert(std::make_pair(make_range(84.97, 85.52), 172));
   gaussRangeTable_.insert(std::make_pair(make_range(85.52, 86.05), 173));
   gaussRangeTable_.insert(std::make_pair(make_range(86.05, 86.56), 174));
   gaussRangeTable_.insert(std::make_pair(make_range(86.56, 87.06), 175));
   gaussRangeTable_.insert(std::make_pair(make_range(87.06, 87.55), 176));
   gaussRangeTable_.insert(std::make_pair(make_range(87.55, 88.03), 177));
   gaussRangeTable_.insert(std::make_pair(make_range(88.03, 88.49), 178));
   gaussRangeTable_.insert(std::make_pair(make_range(88.49, 88.94), 179));
   gaussRangeTable_.insert(std::make_pair(make_range(88.94, 89.38), 180));
   gaussRangeTable_.insert(std::make_pair(make_range(89.38, 89.81), 181));
   gaussRangeTable_.insert(std::make_pair(make_range(89.81, 90.22), 182));
   gaussRangeTable_.insert(std::make_pair(make_range(90.22, 90.62), 183));
   gaussRangeTable_.insert(std::make_pair(make_range(90.62, 91.01), 184));
   gaussRangeTable_.insert(std::make_pair(make_range(91.01, 91.38), 185));
   gaussRangeTable_.insert(std::make_pair(make_range(91.38, 91.75), 186));
   gaussRangeTable_.insert(std::make_pair(make_range(91.75, 92.10), 187));
   gaussRangeTable_.insert(std::make_pair(make_range(92.10, 92.44), 188));
   gaussRangeTable_.insert(std::make_pair(make_range(92.44, 92.77), 189));
   gaussRangeTable_.insert(std::make_pair(make_range(92.77, 93.09), 190));
   gaussRangeTable_.insert(std::make_pair(make_range(93.09, 93.40), 191));
   gaussRangeTable_.insert(std::make_pair(make_range(93.40, 93.69), 192));
   gaussRangeTable_.insert(std::make_pair(make_range(93.69, 93.98), 193));
   gaussRangeTable_.insert(std::make_pair(make_range(93.98, 94.25), 194));
   gaussRangeTable_.insert(std::make_pair(make_range(94.25, 94.52), 195));
   gaussRangeTable_.insert(std::make_pair(make_range(94.52, 94.78), 196));
   gaussRangeTable_.insert(std::make_pair(make_range(94.78, 95.02), 197));
   gaussRangeTable_.insert(std::make_pair(make_range(95.02, 95.26), 198));
   gaussRangeTable_.insert(std::make_pair(make_range(95.26, 95.49), 199));
   gaussRangeTable_.insert(std::make_pair(make_range(95.49, 95.71), 200));
   gaussRangeTable_.insert(std::make_pair(make_range(95.71, 95.92), 201));
   gaussRangeTable_.insert(std::make_pair(make_range(95.92, 96.12), 202));
   gaussRangeTable_.insert(std::make_pair(make_range(96.12, 96.31), 203));
   gaussRangeTable_.insert(std::make_pair(make_range(96.31, 96.50), 204));
   gaussRangeTable_.insert(std::make_pair(make_range(96.50, 96.68), 205));
   gaussRangeTable_.insert(std::make_pair(make_range(96.68, 96.85), 206));
   gaussRangeTable_.insert(std::make_pair(make_range(96.85, 97.01), 207));
   gaussRangeTable_.insert(std::make_pair(make_range(97.01, 97.17), 208));
   gaussRangeTable_.insert(std::make_pair(make_range(97.17, 97.32), 209));
   gaussRangeTable_.insert(std::make_pair(make_range(97.32, 97.46), 210));
   gaussRangeTable_.insert(std::make_pair(make_range(97.46, 97.60), 211));
   gaussRangeTable_.insert(std::make_pair(make_range(97.60, 97.73), 212));
   gaussRangeTable_.insert(std::make_pair(make_range(97.73, 97.85), 213));
   gaussRangeTable_.insert(std::make_pair(make_range(97.85, 97.97), 214));
   gaussRangeTable_.insert(std::make_pair(make_range(97.97, 98.08), 215));
   gaussRangeTable_.insert(std::make_pair(make_range(98.08, 98.19), 216));
   gaussRangeTable_.insert(std::make_pair(make_range(98.19, 98.29), 217));
   gaussRangeTable_.insert(std::make_pair(make_range(98.29, 98.39), 218));
   gaussRangeTable_.insert(std::make_pair(make_range(98.39, 98.48), 219));
   gaussRangeTable_.insert(std::make_pair(make_range(98.48, 98.57), 220));
   gaussRangeTable_.insert(std::make_pair(make_range(98.57, 98.65), 221));
   gaussRangeTable_.insert(std::make_pair(make_range(98.65, 98.73), 222));
   gaussRangeTable_.insert(std::make_pair(make_range(98.73, 98.81), 223));
   gaussRangeTable_.insert(std::make_pair(make_range(98.81, 98.88), 224));
   gaussRangeTable_.insert(std::make_pair(make_range(98.88, 98.94), 225));
   gaussRangeTable_.insert(std::make_pair(make_range(98.94, 99.01), 226));
   gaussRangeTable_.insert(std::make_pair(make_range(99.01, 99.07), 227));
   gaussRangeTable_.insert(std::make_pair(make_range(99.07, 99.13), 228));
   gaussRangeTable_.insert(std::make_pair(make_range(99.13, 99.18), 229));
   gaussRangeTable_.insert(std::make_pair(make_range(99.18, 99.23), 230));
   gaussRangeTable_.insert(std::make_pair(make_range(99.23, 99.28), 231));
   gaussRangeTable_.insert(std::make_pair(make_range(99.28, 99.33), 232));
   gaussRangeTable_.insert(std::make_pair(make_range(99.33, 99.37), 233));
   gaussRangeTable_.insert(std::make_pair(make_range(99.37, 99.41), 234));
   gaussRangeTable_.insert(std::make_pair(make_range(99.41, 99.45), 235));
   gaussRangeTable_.insert(std::make_pair(make_range(99.45, 99.48), 236));
   gaussRangeTable_.insert(std::make_pair(make_range(99.48, 99.52), 237));
   gaussRangeTable_.insert(std::make_pair(make_range(99.52, 99.55), 238));
   gaussRangeTable_.insert(std::make_pair(make_range(99.55, 99.58), 239));
   gaussRangeTable_.insert(std::make_pair(make_range(99.58, 99.61), 240));
   gaussRangeTable_.insert(std::make_pair(make_range(99.61, 99.63), 241));
   gaussRangeTable_.insert(std::make_pair(make_range(99.63, 99.66), 242));
   gaussRangeTable_.insert(std::make_pair(make_range(99.66, 99.68), 243));
   gaussRangeTable_.insert(std::make_pair(make_range(99.68, 99.71), 244));
   gaussRangeTable_.insert(std::make_pair(make_range(99.71, 99.73), 245));
   gaussRangeTable_.insert(std::make_pair(make_range(99.73, 99.74), 246));
   gaussRangeTable_.insert(std::make_pair(make_range(99.74, 99.76), 247));
   gaussRangeTable_.insert(std::make_pair(make_range(99.76, 99.78), 248));
   gaussRangeTable_.insert(std::make_pair(make_range(99.78, 99.80), 249));
   gaussRangeTable_.insert(std::make_pair(make_range(99.80, 99.81), 250));
   gaussRangeTable_.insert(std::make_pair(make_range(99.81, 99.82), 251));
   gaussRangeTable_.insert(std::make_pair(make_range(99.82, 99.84), 252));
   gaussRangeTable_.insert(std::make_pair(make_range(99.84, 99.85), 253));
   gaussRangeTable_.insert(std::make_pair(make_range(99.85, 99.86), 254));
   gaussRangeTable_.insert(std::make_pair(make_range(99.86, 100.00), 255));
}

/**
 * Obtiene el valor de intensidad correspondiente al rango dado.
 */
int GaussianEnhancement::GetValueFromMap(const std::pair<double, double>& Pair) {
   std::map<range, int>::iterator mapit;
   for (mapit = gaussRangeTable_.begin(); mapit != gaussRangeTable_.end(); ++mapit) {
      if (mapit->first.first <= Pair.first && Pair.second <= mapit->first.second)
         return mapit->second;
   }
   return 0;
}

}  // namespace enhancement
}  // namespace raster
}  // namespace suri
