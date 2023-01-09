/** \file TextFormatConverterInterface.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
