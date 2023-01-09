/*! \file CsvFunctions.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CSVFUNCTIONS_H_
#define CSVFUNCTIONS_H_

// Includes standard
#include <string>
#include <vector>

namespace suri {

// Agrega header a documento
void AddCsvHeader(std::string &Csv, const std::string &HeaderContent);

/** Agrega una matriz con el contenido de TableContent a Csv */
void AddCsvNumericTable(std::string &Csv, std::string TableName,
                            std::vector<std::string> TableHeader,
                            std::vector<std::string> RowNames,
                            std::vector<std::vector<double> > TableContent,
                            double ScaleFactor, int Precision = 3);

/** Agrega una matriz con el contenido de TableContent a Csv */
void AddCsvTable(std::string &Csv, std::string TableName,
                            std::vector<std::string> TableHeader,
                            std::vector<std::string> RowNames,
                            std::vector<std::vector<std::string> > TableContent);

/** Busca un string y si lo encuentra lo reemplaza */
bool Replace(std::string& str, const std::string& from, const std::string& to);

} /** namespace suri */

#endif /* CSVFUNCTIONS_H_ */
