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
