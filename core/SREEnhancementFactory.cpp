/** \file SREEnhancementFactory.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "SREEnhancementFactory.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Ctor (singleton)
 */
EnhancementFactory::EnhancementFactory() {

}

/**
 * Dtor (no es virtual porque no se deberia heredar de esta clase)
 */
EnhancementFactory::~EnhancementFactory() {

}

/**
 * Devuelve la unica instancia.
 */
EnhancementFactory* EnhancementFactory::GetInstance() {
   static EnhancementFactory instance;
   return &instance;
}

/**
 * Crea una instancia de realce segun los parametros dados.
 */
Enhancement* EnhancementFactory::Create(const std::string& EName,
                                        suri::ParameterCollection& Params) {
   std::map<std::string, std::pair<CREATEENHANCEMENTCB, bool> >::iterator it = factoryCbs_.find(EName);

   if (it != factoryCbs_.end())
      return it->second.first(Params);

   return NULL;
}

/**
 * Registra un tipo de realce.
 */
void EnhancementFactory::Register(const std::string& EName, CREATEENHANCEMENTCB pCreateCb, bool Visible) {
   factoryCbs_[EName] = std::make_pair(pCreateCb, Visible);
}

/**
 * Obtiene los nombres de los tipos de realce disponibles.
 */
std::vector<std::string> EnhancementFactory::GetAvailableEnhancements(bool VisibleOnly) {
   std::vector<std::string> avails;
   std::map<std::string, std::pair<CREATEENHANCEMENTCB, bool> >::iterator it;
   for (it = factoryCbs_.begin(); it != factoryCbs_.end(); ++it) {
      if ((VisibleOnly == true && it->second.second == true) || VisibleOnly == false)
         avails.push_back(it->first);
   }
   return avails;
}

}  // namespace enhancement
}  // namespace raster
}  // namespace suri
