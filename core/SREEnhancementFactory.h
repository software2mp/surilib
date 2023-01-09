/** \file SREEnhancementFactory.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SREENHANCEMENTFACTORY_H_
#define SREENHANCEMENTFACTORY_H_

#include <map>
#include <string>
#include <vector>
#include <utility>

#include "SREEnhancement.h"
#include "suri/ParameterCollection.h"

#define REGISTER_ENHANCEMENT(M_EName, M_EVisible) \
namespace { \
class _M_Register_Enhancement_ { \
public: \
   _M_Register_Enhancement_() { \
      suri::raster::enhancement::EnhancementFactory::GetInstance()->Register(#M_EName, &M_EName::Create, M_EVisible); \
   } \
}; \
_M_Register_Enhancement_ _m_register_enhancement_; \
} \

namespace suri {
namespace raster {
namespace enhancement {

typedef Enhancement* (*CREATEENHANCEMENTCB)(suri::ParameterCollection& Params);

class EnhancementFactory {
public:
   /**
    * Dtor (no es virtual porque no se deberia heredar de esta clase)
    */
   ~EnhancementFactory();

   /**
    * Devuelve la unica instancia.
    */
   static EnhancementFactory* GetInstance();

   /**
    * Crea una instancia de realce segun los parametros dados.
    */
   Enhancement* Create(const std::string& EName, suri::ParameterCollection& Params);

   /**
    * Registra un nuevo realce.
    */
   void Register(const std::string& EName, CREATEENHANCEMENTCB pCreateCb, bool Visible = true);

   /**
    * Obtiene los nombres de los tipos de realce disponibles.
    */
   std::vector<std::string> GetAvailableEnhancements(bool VisibleOnly = true);

private:
   /**
    * Ctor (singleton)
    */
   EnhancementFactory();

   std::map<std::string, std::pair<CREATEENHANCEMENTCB, bool> > factoryCbs_;
};

}  // namespace enhancement
}  // namespace raster
}  // namespace suri

#endif  // SREENHANCEMENTFACTORY_H_
