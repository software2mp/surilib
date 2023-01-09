/*! \file Option.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/Option.h"

/** namespace suri */
namespace suri {
/**
 * Constructor
 */
Option::Option() {
}

/**
 * Destructor
 */
Option::~Option() {
}

/**
 * Agrega o cambia opciones
 *  Permite almacenar opciones diversas asociadas a la imagen.
 * \post La opcion se agrega con el valor deseado o se modifica el valor de
 *  existir.
 * @param[in] Name nombre de la opcion
 * @param[in] Value valor de la opcion
 */
void Option::SetOption(const std::string &Name, const std::string &Value) {
   // inserto el valor (asumiendo que no existe)
   std::pair<OptionsMapType::iterator, bool> ret = options_.insert(
         std::make_pair(Name, Value));
   // si no se produce insercion, asumo que existe y lo modifico
   if (!ret.second) {
      options_[Name] = Value;
   }
}

/**
 * Agrega o cambia opciones
 *  Permite almacenar opciones diversas asociadas a la imagen.
 * \post Las opciones se agregan con el valor deseado o se modifican los valores
 * que existen.
 * @param[in] Options mapa con las opciones
 */
void Option::SetOption(const OptionsMapType &Options) {
   OptionsMapType::const_iterator it = Options.begin();
   while (it != Options.end()) {
      SetOption(it->first, it->second);
      it++;
   }
}

/**
 *  Permite obtener una opcion
 *  Obtiene opciones de el valor de una opcion cuyo nombre se recibe por parametro
 *  @param[in] Name nombre de la opcion
 *  @return string con el valor de la opcion, o vacio si no se encontro la opcion
 */
std::string Option::GetOption(const std::string &Name) const {
   OptionsMapType::const_iterator ret = options_.find(Name);
   if (ret != options_.end()) {
      return ret->second;
   }
   return "";
}

/**
 * Retorna todas las opciones asociadas a una imagen.
 * @return mapa con el nombre / valor de todas las opciones
 */
Option::OptionsMapType Option::GetOption() const {
   return options_;
}
}
