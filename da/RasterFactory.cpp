/*! \file RasterFactory.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "RasterFactory.h"

// Includes suri
#include "TRasterBand.h"
#include "suri/DataTypes.h"

/** namespace suri */
namespace suri {
/** Registra todos los tipos de bandas */
void RegisterAllRasterBands() {
   RasterBandFactory &factory = TheBandFactory::Instance();
   factory.RegisterClass(UCharBandType::GetClassId(), UCharBandType::Create);
   factory.RegisterClass(ShortBandType::GetClassId(), ShortBandType::Create);
   factory.RegisterClass(UShortBandType::GetClassId(), UShortBandType::Create);
   factory.RegisterClass(IntBandType::GetClassId(), IntBandType::Create);
   factory.RegisterClass(UIntBandType::GetClassId(), UIntBandType::Create);
   factory.RegisterClass(FloatBandType::GetClassId(), FloatBandType::Create);
   factory.RegisterClass(DoubleBandType::GetClassId(), DoubleBandType::Create);
}
}

