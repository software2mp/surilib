/** \file HtmlFunctions.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef HTMLFUNCTIONS_H_
#define HTMLFUNCTIONS_H_

// Includes standard
#include <string>
#include <vector>

namespace suri {

// Agrega header a documento
void AddHtmlHeader(std::string &Html, const std::string &HeaderContent);

/** Agrega una matriz con el contenido de TableContent a Html */
void AddHtmlNumericTable(std::string &Html, std::string TableName,
                            std::vector<std::string> TableHeader,
                            std::vector<std::string> RowNames,
                            std::vector<std::vector<double> > TableContent,
                            double ScaleFactor, int Precision = 3);

/** Agrega una matriz con el contenido de TableContent a Html */
void AddHtmlTable(std::string &Html, std::string TableName,
                            std::vector<std::string> TableHeader,
                            std::vector<std::string> RowNames,
                            std::vector<std::vector<std::string> > TableContent);

}  // namespace

#endif /* HTMLFUNCTIONS_H_ */
