/*! \file TasseledCapFactory.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "TasseledCapFactory.h"

namespace suri {
namespace raster {
namespace transformation {

/**
 * Ctor (singleton).
 */
TasseledCapFactory::TasseledCapFactory() {

}

/**
 * Dtor (no es virtual porque no se deberia heredar de esta clase).
 */
TasseledCapFactory::~TasseledCapFactory() {

}

/**
 * Devuelve la unica instancia.
 */
TasseledCapFactory* TasseledCapFactory::GetInstance() {
   static TasseledCapFactory s_instance;
   return &s_instance;
}

/**
 * Crea una instancia para utilizar algun metodo de transformacion tasseled-cap.
 */
TasseledCapTransformation* TasseledCapFactory::Create(const std::string& TCTName,
                                  suri::ParameterCollection& Params) {
   std::map<std::string, CREATETCTRANSFORMATIONCB>::iterator it = factoryCbs_.find(TCTName);

   if (it != factoryCbs_.end())
      return it->second(Params);

   return NULL;
}

/**
 * Registra un nuevo tipo metodo para transformacion tasseled-cap.
 */
void TasseledCapFactory::Register(const std::string& TCTName, CREATETCTRANSFORMATIONCB pCreateCb, GETOUTPUTBANDSCB pOutputCb) {
   factoryCbs_[TCTName] = pCreateCb;
   outputCbs_[TCTName] = pOutputCb;
}

/**
 * Obtiene los nombres de los tipos de transformaciones tasseled-cap disponibles.
 */
std::vector<std::string> TasseledCapFactory::GetAvailableTCTransformations() {
   std::vector<std::string> avails;
   std::map<std::string, CREATETCTRANSFORMATIONCB>::iterator it;
   for (it = factoryCbs_.begin(); it != factoryCbs_.end(); ++it) {
      avails.push_back(it->first);
   }
   return avails;
}

/**
 * Obtiene las bandas de salida para cada transformacion soportada.
 */
std::map<int, std::string> TasseledCapFactory::GetOutputBandsForTCTransformation(
      const std::string& TCTName) {
   std::map<std::string, GETOUTPUTBANDSCB>::iterator it = outputCbs_.find(TCTName);

   if (it != outputCbs_.end())
      return it->second();

   return std::map<int, std::string>();
}

}  // namespace transformation
}  // namespace raster
}  // namespace suri
