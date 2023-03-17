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
#include <vector>
#include <algorithm>

// Includes Suri
#include "CsvFunctions.h"
#include "logmacros.h"
#include "suri/AuxiliaryFunctions.h"

// Defines
#define PRETY_TABLE  "suri_name"  \
                     "suri_table"
#define NAMEFLAG "suri_name"
#define TABLEFLAG "suri_table"

namespace suri {

/**
 * Agrega header al final de documento html
 * @param[out] Html documento html que se esta generando
 * @param[in] HeadeContent Texto que despliega el header
 */
void AddCsvHeader(std::string &Csv, const std::string &HeaderContent) {
   Csv = Csv + HeaderContent;
}

/**
 * Genera una matriz de strings con el contenido de TableContent.
 * Llama a AddCsvTable con esta matriz.
 * @param[out] Csv Documento csv que se esta generando
 * @param[in] TableName encabezado que debera tener la tabla.
 * @param[in] TableHeader vector de strings con nombre de columnas de la tabla.
 * Tiene un elemento mas que filas en TableContent, el primer elemento
 * es el valor que se despliega sobre los nombres de filas.
 * @param[in] RowNames vector de strings con nombre de las filas. Debe tener
 * tantos elementos como filas halla en TableContent
 * @param[in] TableContent Tabla de doubles que quiero agregar a docuemento html
 * @param[in] Precision cantidad de decimales despues de la coma en la tabla
 */
void AddCsvNumericTable(
      std::string &Csv, std::string TableName, std::vector<std::string> TableHeader,
      std::vector<std::string> RowNames, std::vector<std::vector<double> > TableContent,
      double ScaleFactor, int Precision) {

   std::vector<std::vector<std::string> > tablecontent;
   for (size_t i = 0; i < TableContent.size(); i++) {
      std::vector<std::string> row;
      for (size_t j = 0; j < TableContent[i].size(); j++) {
         row.push_back(DoubleToString(TableContent[i][j] * ScaleFactor, Precision));
      }
      tablecontent.push_back(row);
   }

   AddCsvTable(Csv, TableName, TableHeader, RowNames, tablecontent);
}

/**
 * Genera una tabla csv usando los campos que recibe por parametro. Y la
 * concatena a Csv.
 * @param[out] Csv documento csv que se esta generando
 * @param[in] TableName encabezado que debera tener la tabla.
 * @param[in] TableHeader vector de strings con nombre de columnas de la tabla.
 * Tiene un elemento mas que filas en TableContent, el primer elemento
 * es el valor que se despliega sobre los nombres de filas.
 * @param[in] RowNames vector de strings con nombre de las filas. Debe tener
 * tantos elementos como filas halla en TableContent
 * @param[in] TableContent Tabla de doubles que quiero agregar a documento html
 */
void AddCsvTable(
      std::string &Csv, std::string TableName, std::vector<std::string> TableHeader,
      std::vector<std::string> RowNames, std::vector<std::vector<std::string> > TableContent) {
   // Genera tabla con valores double
   std::string table;
   table = "\n";
   for (size_t i = 0; i < TableHeader.size(); i++)
      table = table + TableHeader[i] + ",";
   if (!TableHeader.empty())
      table += "\n";

   for (size_t i = 0; i < TableContent.size(); i++) {
      table = table + RowNames[i];
      for (size_t j = 0; j < TableContent[i].size(); j++) {
         table += ",";
         table += TableContent[i][j];
      }

      table += "\n";
   }

   // Encapsula la tabla para que tenga un formato consistente con otros htmls
   // de la aplicacion
   std::string pretytable = PRETY_TABLE;
   size_t pos = pretytable.find(NAMEFLAG);
   std::string tablename = "[";
   tablename += TableName;
   tablename += "]";
   if (pos != std::string::npos)
      pretytable.replace(pos, sizeof(NAMEFLAG) - 1, tablename);
   pos = pretytable.find(TABLEFLAG);
   if (pos != std::string::npos) pretytable.replace(pos, sizeof(TABLEFLAG) - 1, table);

   REPORT_DEBUG("D: pretytable: %s", pretytable.c_str());

   Csv += pretytable;
}

/** Busca un string y si lo encuentra lo reemplaza */
bool Replace(std::string& str, const std::string& from, const std::string& to) {
   size_t start_pos = str.find(from);
       if(start_pos == std::string::npos)
           return false;
       str.replace(start_pos, from.length(), to);
       return true;
}

} /** namespace suri */
