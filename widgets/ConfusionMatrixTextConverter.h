/** \file ConfusionMatrixTextConverter.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CONFUSIONMATRIXTEXTCONVERTER_H_
#define CONFUSIONMATRIXTEXTCONVERTER_H_

// Includes standard
#include <string>
#include <map>
#include <vector>

// Includes suri
#include "TextFormatConverterInterface.h"

namespace suri {

/**
 * Transforma nodo xml con matriz de confusion a html. Hereda de
 * TextFormatConverterInterface
 */
class ConfusionMatrixTextConverter : public suri::TextFormatConverterInterface {
public:
   /** Ctor */
   ConfusionMatrixTextConverter(std::map<std::string, std::string> Options);
   /** Dtor */
   virtual ~ConfusionMatrixTextConverter();

   /** Transforma el contenido del texto. */
   bool Transform(const std::string &InputText, std::string &OutputText);

   /** Opciones que soporta el generador de html */
   static const std::string URL_OP;
   static const std::string EXTENT_OP;
   static const std::string DATATYPE_OP;
   static const std::string CLASSES_OP;
   static const std::string TRAIN_ENCLOSURES_OP;
   static const std::string TEST_ENCLOSURES_OP;
private:
   /** Agrega tabla html con matriz de confusion para el tipo indicado */
   std::vector<std::vector<double> > GetConfusionMatrix(
                                    const std::string& InputText,
                                    const std::string& StatsNodeName);
   // Retorna el valor de una opcion
   bool ReadOption(const std::string &Option, std::string &OptionValue);

   std::map<std::string, std::string> options_;

};

} /* namespace suri */
#endif /* CONFUSIONMATRIXTEXTCONVERTER_H_ */
