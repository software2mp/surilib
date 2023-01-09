/*! \file Mask.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
