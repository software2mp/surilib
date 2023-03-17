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

#include "KMeansCanvas.h"

// Includes standard
#include <limits>
#include <vector>
// Includes Suri
#include "suri/DataTypes.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/**
 * Recorre la imagen de entrada, para cada pixel busca la media mas cercana
 * (en InitialMeans) y actualiza las posiciones correspondientes de NewMeans
 * (sumando el valor del pixel) y PixelCount(suma 1).
 * @param[in] pSrc imagen con datos a clasificar
 * @param[in] Size tamanio de la imagen
 * @param[in] BandCount cantidad de bandas en la imagen
 * @param[in] InitialMeans medias originales, se usan para determinar a que
 * clase pertenece cada pixel
 * @param[out] NewMeans sumatoria de todos los pixles de cada clase.
 * @param[out] PixelCount cantidad de pixeles asignados a cada clase.
 */
template<typename T>
void updatemeans(void** pSrc, size_t Size, size_t BandCount,
                 const std::vector<std::vector<double> > &InitialMeans,
                 std::vector<std::vector<double> > &NewMeans,
                 std::vector<double> &PixelCount) {
   if (InitialMeans.empty()) {
      return;
   }
   // reinterpret es peligroso, pero static no anda con **
   T** psrc = reinterpret_cast<T**>(pSrc);
   size_t classcount = InitialMeans.size();
   // Para cada pixel
   for (size_t pixel = 0; pixel < Size; pixel++) {
      // Busco a que clase pertenece
      double mindistance = std::numeric_limits<double>::max();
      size_t winnerclassposition = 0;
      double classdistance = 0;
      double tempvalue = 0;
      for (size_t classpos = 0; classpos < classcount; classpos++) {
         // Calculo la distancia a la media
         classdistance = 0;
         for (size_t band = 0; band < BandCount; band++) {
            tempvalue = (psrc[band][pixel] - InitialMeans[classpos][band]);
            classdistance += tempvalue * tempvalue;
         }
         if (classdistance < mindistance) {
            mindistance = classdistance;
            winnerclassposition = classpos;
         }
      }

      // Actualizo NewMeans y PixelCount
      if (PixelCount.size() > winnerclassposition) {
         PixelCount[winnerclassposition]++;
      }
      if (NewMeans.size() > winnerclassposition) {
         for (size_t j = 0; j < BandCount; j++)
            NewMeans[winnerclassposition][j] += psrc[j][pixel];
      }
   }
}

/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(
      void (*)(void**, size_t, size_t, const std::vector<std::vector<double> > &, std::vector<std::vector<double> >&, std::vector<double>&),
      updatemeans);

/**
 * Constructor
 */
KMeansCanvas::KMeansCanvas() {
   Reset();
}

/**
 * Destructor
 */
KMeansCanvas::~KMeansCanvas() {
}

/**
 * Canvas vectorial
 * @return NULL
 */
wxDC* KMeansCanvas::GetDC() {
   return NULL;
}

/**
 * Computa la nueva media usando los pixeles en canvas
 */
void KMeansCanvas::Flush(const Mask *pMask) {
   // Obtengo los datos del canvas
   std::vector<void*> data;
   std::vector<int> idx;
   for (int i = 0; i < GetBandCount(); i++)
      idx.push_back(i);
   GetInternalData(idx, data);
   int x, y;
   GetSize(x, y);
   void **pdata = reinterpret_cast<void**>(new unsigned char*[GetBandCount()]);
   for (int b = 0; b < GetBandCount(); b++)
      pdata[b] = data[b];

   // calculo los parametros en forma incremental
   updatemeansTypeMap[GetDataType()](pdata, x * y, GetBandCount(), initialMeans_,
                                     newMeans_, pixelCount_);
}

/**
 * Retorna la media calculada durante la renderizacion.
 * @param[out] NewMeans medias calculadas durante renderizacion. Debe estar vacio.
 */
void KMeansCanvas::GetNewMeans(std::vector<std::vector<double> > &NewMeans) {
   size_t classcount = initialMeans_.size();
   for (size_t i = 0; i < classcount; i++) {
      NewMeans.push_back(std::vector<double>());
      for (size_t j = 0; j < initialMeans_[i].size(); j++) {
         REPORT_DEBUG("D: suma pixels clase: %f, cantidad pixeles: %d",
                      newMeans_[i][j], pixelCount_[i]);
         if (pixelCount_[i] > 0) {
            NewMeans[i].push_back(newMeans_[i][j] / pixelCount_[i]);
         } else {
            NewMeans[i].push_back(initialMeans_[i][j]);
         }
      }
   }
}

/**
 * Guarda el array de medias que se van a utilizar para determinar en que
 * clase esta cada pixel.
 * Limpia los vectores newMeans_ y pixelCount_ para que tengan dimensiones
 * correctas y esten inicializados en 0.
 * @param[in] InitialMeans array con las medias
 */
void KMeansCanvas::SetInitialMeans(
      const std::vector<std::vector<double> > &InitialMeans) {
   Reset();
   size_t classcount = InitialMeans.size();
   for (size_t i = 0; i < classcount; i++) {
      pixelCount_.push_back(0);
      newMeans_.push_back(std::vector<double>());
      initialMeans_.push_back(std::vector<double>());
      for (size_t j = 0; j < InitialMeans[i].size(); j++) {
         initialMeans_[i].push_back(InitialMeans[i][j]);
         newMeans_[i].push_back(0);
      }
   }
}

/**
 * Limpia los dato internos
 */
void KMeansCanvas::Reset() {
   initialMeans_ = std::vector<std::vector<double> >();
   newMeans_ = std::vector<std::vector<double> >();
   pixelCount_ = std::vector<double>();
}
}
