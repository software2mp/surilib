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

// Includes standard
#include <string>
#include <map>

// Includes suri
#include "TextFormatConverterInterface.h"
#include "StatisticsXmlToHtmlConverter.h"
#include "ConfusionMatrixTextConverter.h"
#include "StatisticsXmlToCsvConverter.h"

// Defines
#define STATISTICSXML_FORMATNAME "xml:statistics"
#define STATISTICSHTML_FORMATNAME "html:statistics"
#define CONFUSIONMATRIXXML_FORMATNAME "xml:confusionmatrix"
#define CONFUSIONMATRIXHTML_FORMATNAME "html:confusionmatrix"
#define STATISTICSCSV_FORMATNAME "csv:statistics"

namespace suri {

/**
 * Recibe por parametro los formatos entre los cuales debe trasformar el
 * texto y retorna instancia de TextFormatConverterInterface configurada.
 * @param[in] InputFormat id del formato de entrada
 * @param[in] OutputFormat id del formato de salida
 * @return TextFormatConverterInterface* puntero a format converter configurado
 * @return NULL en caso de error
 */
TextFormatConverterInterface* TextFormatConverterInterface::Create(
                             const std::string &InputFormat,
                             const std::string &OutputFormat,
                             std::map<std::string, std::string> Options) {
   // TODO(Javier - Sin TCK): Usar factorias. Ver TCK #875
   if (InputFormat == STATISTICSXML_FORMATNAME
         && OutputFormat == STATISTICSHTML_FORMATNAME)
      return new StatisticsXmlToHtmlConverter(Options);

   if (InputFormat == CONFUSIONMATRIXXML_FORMATNAME
         && OutputFormat == CONFUSIONMATRIXHTML_FORMATNAME)
      return new ConfusionMatrixTextConverter(Options);

   if (InputFormat == STATISTICSXML_FORMATNAME
            && OutputFormat == STATISTICSCSV_FORMATNAME)
         return new StatisticsXmlToCsvConverter(Options);

   return NULL;
}

/**
 * Destruye instancia de TextFormatConverter
 * @param[in] &pTextFormatConverter Instancia de TextFormatConverter a destruir
 */
void TextFormatConverterInterface::Destroy(
                          TextFormatConverterInterface* &pTextFormatConverter) {
   delete pTextFormatConverter;
   pTextFormatConverter = NULL;
}

} /* namespace suri */
