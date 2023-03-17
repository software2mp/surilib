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

// Includes sopi
#include "HtmlFunctions.h"
#include "logmacros.h"
#include "suri/AuxiliaryFunctions.h"

// Defines
#define PRETY_TABLE  "<table cellpadding=\"0\" cellspacing=\"20\">"           \
                     " <tr><td>"                                              \
                     "  <table cellpadding=\"0\" cellspacing=\"0\">"          \
                     "   <tr><td valign=\"top\">"                             \
                     "    <h1><font color=\"#041ACC\">suri_name</font></h1>"  \
                     "   </td></tr>"                                          \
                     "   <tr valign=\"top\">"                                 \
                     "    <td valign=\"top\">suri_table</td></tr>"            \
                     "  </table>"                                             \
                     " </td></tr>"                                            \
                     "</table>"
#define NAMEFLAG "suri_name"
#define TABLEFLAG "suri_table"
#define GENERIC_TABLE_ATRIBUTES "border=\"1\""

#define HEADER_ATRIBUTES "color=\"#041ACC\""



namespace suri {

/**
 * Agrega header al final de documento html
 * @param[out] Html documento html que se esta generando
 * @param[in] HeadeContent Texto que despliega el header
 */
void AddHtmlHeader(std::string &Html,
                                        const std::string &HeaderContent) {
   Html = Html + "<h1><font " + HEADER_ATRIBUTES + ">" + HeaderContent + "</font></h1>";
}

/**
 * Genera una matriz de strings con el contenido de TableContent.
 * Llama a AddHtmlTable con esta matriz.
 * @param[out] Html documento html que se esta generando
 * @param[in] TableName encabezado que debera tener la tabla.
 * @param[in] TableHeader vector de strings con nombre de columnas de la tabla.
 * Tiene un elemento mas que filas en TableContent, el primer elemento
 * es el valor que se despliega sobre los nombres de filas.
 * @param[in] RowNames vector de strings con nombre de las filas. Debe tener
 * tantos elementos como filas halla en TableContent
 * @param[in] TableContent Tabla de doubles que quiero agregar a docuemento html
 * @param[in] Precision cantidad de decimales despues de la coma en la tabla
 */
void AddHtmlNumericTable(
      std::string &Html, std::string TableName, std::vector<std::string> TableHeader,
      std::vector<std::string> RowNames, std::vector<std::vector<double> > TableContent,
      double ScaleFactor, int Precision) {

   std::vector<std::vector<std::string> > tablecontent;
   for (size_t i = 0; i < TableContent.size() && i < RowNames.size(); i++) {
      std::vector<std::string> row;
      for (size_t j = 0; j < TableContent[i].size(); j++) {
         row.push_back(DoubleToStringEx(TableContent[i][j] * ScaleFactor, Precision));
      }
      tablecontent.push_back(row);
   }

   AddHtmlTable(Html, TableName, TableHeader, RowNames, tablecontent);
}

/**
 * Genera una tabla html usando los campos que recibe por parametro. Y la
 * concatena a Html.
 * @param[out] Html documento html que se esta generando
 * @param[in] TableName encabezado que debera tener la tabla.
 * @param[in] TableHeader vector de strings con nombre de columnas de la tabla.
 * Tiene un elemento mas que filas en TableContent, el primer elemento
 * es el valor que se despliega sobre los nombres de filas.
 * @param[in] RowNames vector de strings con nombre de las filas. Debe tener
 * tantos elementos como filas halla en TableContent
 * @param[in] TableContent Tabla de doubles que quiero agregar a documento html
 */
void AddHtmlTable(
      std::string &Html, std::string TableName, std::vector<std::string> TableHeader,
      std::vector<std::string> RowNames, std::vector<std::vector<std::string> > TableContent) {
   // Genera tabla con valores double
   std::string table;
   table = table + "<table " + GENERIC_TABLE_ATRIBUTES + ">";
   table += "<tr>";
   for (size_t i = 0; i < TableHeader.size(); i++)
      table = table + "<th>" + TableHeader[i] + "</th>";
   table += "</tr>";

   for (size_t i = 0; i < TableContent.size(); i++) {
      table = table + "<tr><td>" + RowNames[i] + "</td>";
      for (size_t j = 0; j < TableContent[i].size(); j++) {
         table += "<td>";
         table += TableContent[i][j];
         table += "</td>";
      }

      table += "</tr>";
   }
   table += "</table>";

   // Encapsula la tabla para que tenga un formato consistente con otros htmls
   // de la aplicacion
   std::string pretytable = PRETY_TABLE;
   size_t pos = pretytable.find(NAMEFLAG);
   if (pos != std::string::npos)
      pretytable.replace(pos, sizeof(NAMEFLAG) - 1, TableName);
   pos = pretytable.find(TABLEFLAG);
   if (pos != std::string::npos) pretytable.replace(pos, sizeof(TABLEFLAG) - 1, table);

   REPORT_DEBUG("D: pretytable: %s", pretytable.c_str());

   Html += pretytable;
}

}  // namespace
