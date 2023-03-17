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

#ifndef XMLFUNCTIONS_H_
#define XMLFUNCTIONS_H_

// Includes standard
#include <string>
#include <vector>

// Includes Suri
#include "suri/AuxiliaryFunctions.h"
#include "suri/xmlnames.h"
#include "logmacros.h"

// Includes Wx
#include "wx/xml/xml.h"

// Includes App

// Defines

/** namespace suri */
namespace suri {


// Creacion de nodo xml

/**
 * Funcion auxiliar que crea un nodo xml con el nombre y valor que se indica por parametro.
 * El objetivo de esta funcion es el de uniformizar la creacion de un nodo y su contenido. 
 * @param[in] NodeName nombre del nodo a crear
 * @param[in] NodeValue valor del nodo xml. El contenido se guarda en un nodo hijo del mismo (de
 * tipo texto) 
 * @param[in] pParentNode padre del nodo a crear.
 * @return nueva instancia de nodo xml con el nombre y valor deseado
 * @return NULL en caso contrario    
**/
wxXmlNode* CreateKeyValueNode(const std::string& NodeName, const std::string& NodeValue,
                              wxXmlNode* pParentNode = NULL);
// TABLAS

/** Crea nodo tabla de tamanio indicado */
void CreateTable(wxXmlNode* &pNode, const std::string &NodeName, int Rows, int Columns);

/** Retorna la dimansion del nodo */
void GetDimension(const wxXmlNode* pNode, int &Rows, int &Columns);

/** Retorna la cantidad de columnas de la tabla */
int GetRows(const wxXmlNode *pNode);

/** Retorna la cantidad de filas de la tabla */
int GetColumns(const wxXmlNode *pNode);

/** Cambia el contenido de una fila en el nodo */
/**
 * @param[out] pNode nodo con la tabla
 * @param[in] Row fila que quiero remplazar
 * @param[in] RowValues vector con nuevos valores de la fila
 */
template<class T>
void SetRow(wxXmlNode* pNode, int Row, const std::vector<T> &RowValues) {
   if (Row >= GetRows(pNode)) {
      REPORT_DEBUG("D: Fila fuera de rango");
      return;
   }

   wxXmlNode* pnode = pNode->GetChildren();
   for (int i = 0; i < Row; i++)
      pnode = pnode->GetNext();

   pnode->GetChildren()->SetContent(join(RowValues.begin(), RowValues.end()).c_str());
}

/** Crea nodo con el contenido de la Tabla. */
/**
 * Permite cambiar el nombre del nodo
 * @param[out] pNode nodo donde se debe agregar la tabla. Si es NULL
 * se crea uno nuevo
 * @param[in] NodeName nombre del nodo
 * @param[in] Table tabla con los datos.
 */
template<class T>
void CreateTable(wxXmlNode* &pNode, const std::string &NodeName,
                 const std::vector<std::vector<T> > &Table) {
   typedef std::vector<std::vector<T> > TableType;
   if (Table.empty() || Table.begin()->empty()) {
      return;
   }
   size_t rows = Table.size();
   // aca al menos tiene 1 fila
   size_t cols = Table.begin()->size();
   bool isconsistent = true;
   // verifico que tenga la misma cantidad de columnas en todas las filas
   typename TableType::const_iterator it = Table.begin();
   while (it != Table.end()) {
      isconsistent &= (cols == (*it).size());
      it++;
   }
   if (!isconsistent) {
      return;
   }
   if (!pNode) {
      pNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, NodeName);
   } else {
      pNode->SetName(NodeName);
   }

   // Cargo la cantidad de columnas y filas en el nodo.
   std::stringstream rowsstream;
   rowsstream << rows;
   pNode->AddProperty(wxT(ROWS_PROPERTY), rowsstream.str());
   std::stringstream columnsstream;
   columnsstream << cols;
   pNode->AddProperty(wxT(COLUMNS_PROPERTY), columnsstream.str());

   // Creo los nodos de las filas
   for (size_t i = 0; i < rows; i++)
      new wxXmlNode(new wxXmlNode(pNode, wxXML_ELEMENT_NODE, wxT(ROW_NODE)),
                    wxXML_TEXT_NODE, wxEmptyString);
   // Carglo los valores en las filas
   it = Table.begin();
   size_t i = 0;
   while (it != Table.end()) {
      SetRow(pNode, i++, *it);
      it++;
   }
}

/** Crea nodo con el contenido de la Tabla. El nodo tiene nombre default */
/**
 * @param[out] pNode nodo donde se debe agregar la tabla. Si es NULL
 * se crea uno nuevo
 * @param[in] Table tabla con los datos.
 */
template<class T>
void CreateTable(wxXmlNode* &pNode, const std::vector<std::vector<T> > &Table) {
   CreateTable(pNode, wxT(TABLE_NODE), Table);
}

/** Retorna la fila indicada */
/**
 * @param[in] pNode nodo con la tabla
 * @param[in] Row fila que quiero
 * @return contenido de la fila Row en un vector.
 */
template<class T>
std::vector<T> GetRow(const wxXmlNode* pNode, int Row) {
   std::vector<T> returnvalue;

   if (Row >= GetRows(pNode)) {
      REPORT_DEBUG("D: Fila fuera de rango");
      return returnvalue;
   }

   wxXmlNode* pnode = pNode->GetChildren();
   for (int i = 0; i < Row; i++)
      pnode = pnode->GetNext();

   // Obtengo contenido de nodos keys y values
   returnvalue = split<T>(pnode->GetNodeContent().c_str());
   return returnvalue;
}

/** Retorna el valor en la fila y columna indicada */
/**
 * @param[in] pNode nodo con la tabla
 * @param[in] Row fila donde esta el dato
 * @param[in] Column columna donde esta el dato
 * @return valor de la tabla en fila y columna indicados
 */
template<class T>
T GetValue(const wxXmlNode* pNode, int Row, int Column) {
   int rows, columns;
   GetDimension(pNode, rows, columns);
   if (Column >= columns || Row >= rows) {
      REPORT_DEBUG("D: Posicion fuera de rango");
      return T();
   }

   std::vector<T> rowvector = GetRow<T>(pNode, Row);
   return rowvector.size() >= Column ? rowvector[Column] : T();
}

/** Retorna la columna indicada */
/**
 * @param[in] pNode nodo con la tabla
 * @param[in] Column columna que quiero
 * @return contenido de la columna Column en un vector.
 */
template<class T>
std::vector<T> GetColumn(const wxXmlNode* pNode, int Column) {
   std::vector<T> returnvalue;

   int rows, columns;
   GetDimension(pNode, rows, columns);

   if (Column >= columns) {
      REPORT_DEBUG("D: Columna fuera de rango");
      return returnvalue;
   }

   for (int i = 0; i < rows; i++)
      returnvalue.push_back(GetValue<T>(pNode, i, Column));

   return returnvalue;
}

/** Cambia el valor de una posicion en el nodo */
/**
 * @param[out] pNode nodo con la tabla
 * @param[in] Row fila donde esta el dato que quiero remplazar
 * @param[in] Column columna donde esta el dato que quiero remplazar
 * @param[in] Value nuevo valor de la tabla en fila y columna indicados
 */
template<class T>
void SetValue(wxXmlNode* pNode, int Row, int Column, const T &Value) {
   int rows, columns;
   GetDimension(pNode, rows, columns);
   if (Column >= columns || Row >= rows) {
      REPORT_DEBUG("D: Posicion fuera de rango");
      return;
   }

   // Obtengo la fila, modifico el valor y la guardo en xml
   std::vector<std::string> fullrow = GetRow<std::string>(pNode, Row);
   std::stringstream ss;
   ss << Value;
   fullrow[Column] = ss.str();
   SetRow<T>(pNode, Row, fullrow);
}

/** Cambia el contenido de una columna en el nodo */
/**
 * @param[out] pNode nodo con la tabla
 * @param[in] Column columna que quiero remplazar
 * @param[in] ColumnValues vector con nuevos valores de la columna
 */
template<class T>
void SetColumn(wxXmlNode* pNode, int Column, const std::vector<T> &ColumnValues) {
   int rows, columns;
   GetDimension(pNode, rows, columns);
   if (Column >= columns) {
      REPORT_DEBUG("D: Columna fuera de rango");
      return;
   }

   for (int i = 0; i < rows; i++)
      SetValue<T>(pNode, i, Column, ColumnValues[i]);
}

/** Carga una tabla dada */
/**
 * @param[in] pNode nodo con la tabla
 * @param[out] Table tabla donde cargo los datos del nodo
 */
template<class T>
void LoadTable(const wxXmlNode *pNode, std::vector<std::vector<T> > &Table) {
   int rows = 0, cols = 0;
   // dimensiones de la tabla en el XML
   GetDimension(pNode, rows, cols);
   // Inicializo la tabla
   Table.clear();
   // cargo las filas
   for (int i = 0; i < rows; i++)
      Table.push_back(GetRow<T>(pNode, i));
}

/** Compara dos nodos XML */
bool operator==(const wxXmlNode &lhs, const wxXmlNode &rhs);

/** Guarda el nodo como string */
std::string Xml2String(const wxXmlNode *pNode);

/** Permite acceder a un nodo XML basado en nombre */
wxXmlNode *GetNodeByName(const wxString &Name, wxXmlNode *pSeed);

/** Permite acceder a un nodo XML basado en nombre */
const wxXmlNode *GetNodeByName(const wxString &Name, const wxXmlNode *pSeed);
}

#endif /* XMLFUNCTIONS_H_ */
