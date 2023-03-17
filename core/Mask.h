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

#ifndef MASK_H_
#define MASK_H_

// Includes estandar
#include <string>
#include <vector>

// Includes suri
#include "MemoryCanvas.h"

/** namespace suri */
namespace suri {
/** MemoryCanvas que representa una mascara raster binaria (0, 1) */
/**
 *  Esta clase representa una mascara raster de tipo binaria. El tipo de dato
 * manejado es siempre unsigned char donde 0 quivale a opaco y cualquier
 * otro valor, a transparente.
 *
 *  Hereda de MemoryCanvas y fuerza el tipo de dato a unsigned char.
 *
 *  Implementa un metodo extra que es ApplyMask, donde copia los datos de un
 * Canvas a otro aplicando la mascara y dejando sin tocar los pixeles del
 * destino donde la mascara equivale a 0.
 *
 * 0   = Completamente transparente (el destino queda sin alterar).
 * 255 = Completamente opaco (el destino queda igual que el origen).
 */
class Mask : public MemoryCanvas {
   /** Ctor. de Copia. */
   Mask(const Mask &Mask);

public:
   static const unsigned char Transparent;
   static const unsigned char Opaque;
   static const unsigned char Invalid;
   static const unsigned char Valid;
   /** Ctor */
   Mask();
   /** Dtor */
   virtual ~Mask();

   // ----------------------------- VECTORIAL -----------------------------
   /** Limpia los datos del DC */
   virtual void ClearDc();
   // -------------------------- MULTI-RASTER --------------------------
   /** Permite setear la cantidad de bandas */
   virtual void SetBandCount(int BandCount);
   // --------------------------- DATA-TYPE ----------------------------
   /** Nombre del tipo de dato */
   virtual void SetDataType(const std::string& DataType);
// ----------------------------- BANDAS -----------------------------
   /** Copia los datos de un canvas a otro aplicando la mascara */
   virtual void ApplyMask(Canvas *pSource, Canvas *pTarget) const;
   /** Escribe los datos al canvas usando la mascara */
   virtual void ApplyMask(std::vector<int> &BandIndex, std::vector<void*> &Data,
                          Canvas *pTarget) const;
};
}

#endif /*MASK_H_*/
