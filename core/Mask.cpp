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
#include <cmath>
#include <string>
#include <vector>

// Includes suri
#include "Mask.h"
#include "suri/messages.h"
#include "suri/DataTypes.h"

// Includes wx
#include "wx/wx.h"

// Defines
#define __USE_MASK_AS_ALPHA_CHANNEL__

/** namespace suri */
namespace suri {
/** funcion template que copia memoria aplicando la mascara */
/**
 *  Copia la memoria de un buffer a otro aplicando la mascara.
 * @param[in] pSource Buffer con los datos de entrada.
 * @param[out] pDest Buffer destino de los datos enmascarados.
 * @param[in] pMask Buffer con la mascara.
 * @param[in] Size Tamanio de los buffers.
 * @param[in] NoDataValue Valor a utilizar para el area que no contiene datos.
 */
template<class T>
void memmaskcpy(void* pSource, void* pDest, unsigned char *pMask, int Size, bool NoDataValueSet,
                double NoDataValue) {
   T* psource = static_cast<T*>(pSource);
   T* pdest = static_cast<T*>(pDest);
   for (int i = 0; i < Size; i++) {
#ifdef __USE_MASK_AS_ALPHA_CHANNEL__
      if (pMask[i] == Mask::Valid) {
         double alpha = static_cast<double>(pMask[i])
               / static_cast<double>(Mask::Opaque);
         pdest[i] = static_cast<T>(alpha * psource[i] + (1.0 - alpha) * pdest[i]);
      } else {
         if (NoDataValueSet)
            pdest[i] = static_cast<T>(NoDataValue);
      }
#else
      if (pMask[i] == Mask::Opaque) {
         pdest[i] = psource[i];
      }
#endif   // __USE_MASK_AS_ALPHA_CHANNEL__
   }
}
/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(void (*)(void*, void*, unsigned char*, int, bool, double), memmaskcpy);

const unsigned char Mask::Transparent = 0;
const unsigned char Mask::Opaque = 255;
const unsigned char Mask::Invalid = Mask::Transparent;
const unsigned char Mask::Valid = Mask::Opaque;


/** ctor */
Mask::Mask() {
   MemoryCanvas::SetDataType(DataInfo<unsigned char>::Name);
   MemoryCanvas::SetBandCount(1);
}

/** dtor */
Mask::~Mask() {
}

/** Limpia los datos del DC */
void Mask::ClearDc() {
   int x, y;
   GetSize(x, y);
   wxDC *pdc = GetDC();
   if (pdc && pdc->IsOk()) {
      wxBrush brush;
      brush.SetColour(wxColour(Transparent, Transparent, Transparent, Opaque));
      pdc->SetBackground(brush);
      pdc->Clear();
   }
}

// -------------------------- MULTI-RASTER --------------------------
/** Permite setear la cantidad de bandas */
void Mask::SetBandCount(int BandCount) {
   MemoryCanvas::SetBandCount(BandCount>1?1:BandCount);
}

/**
 * Asigna el nombre del tipo de dato
 *
 *  Impide cambiar el tipo de dato a otra cosa que no sea unsigned char.
 * @param[in] DataType Tipo de dato deseado.
 */
void Mask::SetDataType(const std::string& DataType) {
   if (DataType != DataInfo<unsigned char>::Name) {
      REPORT_AND_RETURN("D:El tipo de datos de la mascara debe ser: %s.",
                        DataInfo<unsigned char>::Name);
   }
   MemoryCanvas::SetDataType(DataInfo<unsigned char>::Name);
}

// ----------------------------- BANDAS -----------------------------

/** Copia los datos de un canvas a otro aplicando la mascara */
/**
 *  Copia los datos de pSource a pTarget utilizando la informacion de los
 * datos de la mascara y solo copiando aquellos pixeles cuyo valor en la
 * mascara sea distinto de 0.
 *
 *  Utiliza solo la primer banda para realizar el enmascaramiento.
 *
 * \pre Los canvas deben ser iguales.
 * \post El contenido de pTarget se modifica solo en los pixeles cuyo valor
 * \todo Realizar enmascaramiento in-place (sin copiar datos)
 *       en la mascara es distinto de 0, el resto resultan inalterados.
 * @param[in] pSource canvas de lectura de datos.
 * @param[out] pTarget canvas destino de los datos enmascarados.
 *
 * Verifica que mascara y canvas tengan el mismo tamano, luego para cada banda
 * le aplica la mascara usando template memmaskcpy con unsigned char
 * \todo remover el uso de la primera banda de la mascara
 */
void Mask::ApplyMask(Canvas *pSource, Canvas *pTarget) const {
   if (!pSource || !pTarget) {
      return;
   }
   if (*pSource != *pTarget) {
      REPORT_AND_RETURN(
            "D:La operacion de mascara requiere dos canvas del mismo tipo.");
   }
   int x, y, s = pSource->GetDataSize();
   pSource->GetSize(x, y);
   int u, v;
   GetSize(u, v);
   if (u != x || v != y) {
      REPORT_AND_RETURN("D:Dimensiones distintas; source(%d;%d), mask(%d,%d)",
                        x, y, u, v);
   }
   int count = pSource->GetBandCount();
   int size = x * y;
   std::vector<void*> source;
   std::vector<int> idx(count);
   for (int i = 0; i < count; i++) {
      source.push_back(new unsigned char[size * s]);
      idx[i] = i;
   }
   pSource->Read(idx, source);

   ApplyMask(idx, source, pTarget);
   for (int i = 0; i < count; i++)
      delete[] static_cast<unsigned char*>(source[i]);
}

/** Escribe los datos al canvas usando la mascara */
/**
 * Aplica la mascara a los datos de entrada y los escribe en el canvas destino.
 * @param[in] BandIndex indice de bandas
 * @param[in] SourceData datos de entrada
 * @param[out] pTarget canvas destino
 */
void Mask::ApplyMask(std::vector<int> &BandIndex, std::vector<void*> &SourceData,
                     Canvas *pTarget) const {

   if (!pTarget) {
      REPORT_AND_FAIL("D: El canvas destino no existe");
   }
   if (BandIndex.size() != SourceData.size()) {
      REPORT_AND_FAIL("D: Inconsistencia entre los datos");
   }
   std::vector<void*> targetdata(BandIndex.size(), NULL);
   int x = 0, y = 0;
   pTarget->GetSize(x, y);
   for (int b = 0; b < pTarget->GetBandCount(); b++)
      targetdata[b] = new unsigned char[x * y * pTarget->GetDataSize()];
   pTarget->Read(BandIndex, targetdata);
   // Deberia funcionar pero siempre deja todo negro.
#ifdef __UNUSED_CODE__
   std::vector<void*> mask;
   std::vector<int> bands(1, 0);
   const_cast<Mask*>(this)->GetInternalData(bands, mask);
   unsigned char *pmaskbuffer = static_cast<unsigned char*>(mask.at(0));
   // \todo remover el uso de la primera banda de la mascara
   for (int i = 0; i < pTarget->GetBandCount(); i++) {
      memmaskcpyTypeMap[pTarget->GetDataType()](SourceData[i], targetdata[i],
                                                pmaskbuffer, x * y);
   }
#else  // __UNUSED_CODE__
   // \todo remover el uso de la primera banda de la mascara
   for (int i = 0; i < pTarget->GetBandCount(); i++) {
      unsigned char *pmaskbuffer = static_cast<unsigned char*>(GetBand(0)->GetBlock(0,
                                                                                    0));
      memmaskcpyTypeMap[pTarget->GetDataType()](SourceData[i], targetdata[i],
                                                pmaskbuffer, x * y, IsNoDataValueAvailable(),
                                                GetNoDataValue());
   }
#endif  // __UNUSED_CODE__
   pTarget->Write(BandIndex, targetdata, NULL);
   for (int b = 0; b < pTarget->GetBandCount(); b++)
      delete[] static_cast<unsigned char*>(targetdata[b]);
}
}
