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

#ifndef TASSELEDCAPFACTORY_H_
#define TASSELEDCAPFACTORY_H_

#include <vector>

#include "TasseledCapTransformation.h"
#include "suri/ParameterCollection.h"

#define REGISTER_TASSELEDCAP_TRANSFORMATION(M_TCTName) \
namespace { \
class _M_Register_TasseledCap_Tranformation_ { \
public: \
   _M_Register_TasseledCap_Tranformation_() { \
      suri::raster::transformation::TasseledCapFactory::GetInstance() \
         ->Register(#M_TCTName, &M_TCTName::Create, &M_TCTName::GetOutputBands); \
   } \
}; \
_M_Register_TasseledCap_Tranformation_ _m_register_tasseledcap_tranformation_; \
} \

namespace suri {
namespace raster {
namespace transformation {

typedef TasseledCapTransformation* (*CREATETCTRANSFORMATIONCB)(suri::ParameterCollection& Params);
typedef std::map<int, std::string> (*GETOUTPUTBANDSCB)();

class TasseledCapFactory {
public:
   /**
    * Dtor (no es virtual porque no se deberia heredar de esta clase).
    */
   ~TasseledCapFactory();

   /**
    * Devuelve la unica instancia.
    */
   static TasseledCapFactory* GetInstance();

   /**
    * Crea una instancia para utilizar algun metodo de transformacion tasseled-cap.
    */
   TasseledCapTransformation* Create(const std::string& TCTName, suri::ParameterCollection& Params);

   /**
    * Registra un nuevo tipo metodo para transformacion tasseled-cap.
    */
   void Register(const std::string& TCTName, CREATETCTRANSFORMATIONCB pCreateCb, GETOUTPUTBANDSCB pOutputCb);

   /**
    * Obtiene los nombres de los tipos de transformaciones tasseled-cap disponibles.
    */
   std::vector<std::string> GetAvailableTCTransformations();

   /**
    * Obtiene las bandas de salida para cada transformacion soportada.
    */
   std::map<int, std::string> GetOutputBandsForTCTransformation(const std::string& TCTName);

private:
   /**
    * Ctor (singleton).
    */
   TasseledCapFactory();

   std::map<std::string, CREATETCTRANSFORMATIONCB> factoryCbs_;
   std::map<std::string, GETOUTPUTBANDSCB> outputCbs_;
};

}  // namespace transformation
}  // namespace raster
}  // namespace suri

#endif  // TASSELEDCAPFACTORY_H_
