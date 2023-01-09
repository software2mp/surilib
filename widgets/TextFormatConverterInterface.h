/** \file TextFormatConverterInterface.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TEXTFORMATCONVERTERINTERFACE_H_
#define TEXTFORMATCONVERTERINTERFACE_H_

// Includes standard
#include <string>
#include <map>

namespace suri {

/**
 * Interfaz que deben implementar todos las clases que transforman
 * entre formatos.
 */
class TextFormatConverterInterface {
public:
   /** dtor */
   virtual ~TextFormatConverterInterface() {}
   /** Transforma el contenido del texto. */
   virtual bool Transform(const std::string &InputText, std::string &OutputText) = 0;

   /** Crea TextFormatConverterInterface que transforma entre los formatos indicados */
   static TextFormatConverterInterface* Create(const std::string &InputFormat,
                                const std::string &OutputFormat,
                                std::map<std::string, std::string> Options);
   /** Destruye instancia de TextFormatConverterInterface */
   static void Destroy(TextFormatConverterInterface* &pTextFormatConverter);
};

} /* namespace suri */
#endif /* TEXTFORMATCONVERTERINTERFACE_H_ */
