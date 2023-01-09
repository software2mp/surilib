/*! \file Option.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef OPTION_H_
#define OPTION_H_

// Includes standard
#include <map>
#include <string>

/** namespace suri */
namespace suri {
/** Clase que guarda opciones mapeando dos strings */
class Option {
public:
   /** typedef para las opciones */
   typedef std::map<std::string, std::string> OptionsMapType;
   /** ctor */
   Option();
   /** dtor */
   virtual ~Option();

   /** Agrega o cambia opciones */
   void SetOption(const std::string &Name, const std::string &Value);
   /** Agrega o cambia opciones */
   void SetOption(const OptionsMapType &Options);
   /** Retorna el valor de una opcion */
   std::string GetOption(const std::string &Name) const;
   /** Retorna todas las opciones */
   OptionsMapType GetOption() const;
private:
   OptionsMapType options_; /*! Opciones */
};
}

#endif /*OPTION_H_*/
