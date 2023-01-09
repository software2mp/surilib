/*! \file SRSSharpeningFactory.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "SRSSharpeningFactory.h"

namespace suri {
namespace raster {
namespace sharpening {

/**
 * Ctor (singleton)
 */
SharpeningFactory::SharpeningFactory() {

}

/**
 * Dtor (no es virtual porque no se deberia heredar de esta clase)
 */
SharpeningFactory::~SharpeningFactory() {

}

/**
 * Devuelve la unica instancia.
 */
SharpeningFactory* SharpeningFactory::GetInstance() {
   static SharpeningFactory instance;
   return &instance;
}

/**
 * Crea una instancia para utilizar algun metodo para fusion de bandas
 * con pancromatica segun los parametros dados.
 */
Sharpening* SharpeningFactory::Create(const std::string& EName,
                                        suri::ParameterCollection& Params) {
   std::map<std::string, CREATESHARPENINGCB>::iterator it = factoryCbs_.find(EName);

   if (it != factoryCbs_.end())
      return it->second(Params);

   return NULL;
}

/**
 * Registra un nuevo tipo metodo para fusion de bandas con pancromatica.
 */
void SharpeningFactory::Register(const std::string& EName, CREATESHARPENINGCB pCreateCb) {
   factoryCbs_[EName] = pCreateCb;
}

} /** namespace sharpening */
} /** namespace raster */
} /** namespace suri */
