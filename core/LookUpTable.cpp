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

#include "suri/LookUpTable.h"

// Includes estandar
#include <limits>

// Includes Suri
#include "LutRenderer.h"
#include "suri/XmlFunctions.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** ctor */
LookUpTable::LookUpTable() :
      defaultValue_(0) {
}

/** dtor */
LookUpTable::~LookUpTable() {
}

// ------------------- METODOS DE SERIALIZACION DE UNA TABLA -------------------
/**
 *  Convierte de un Nodo xml a una tabla de busqueda de valores.
 * @param[in] pLutNode Nodo con la informacion de la tabla
 * @param[out] Parameters Tabla de busqueda
 * @return True si se encontro una tabla valida (2 filas y al menos 1 columna)
 */
bool LookUpTable::FromXml(const wxXmlNode *pLutNode, LookUpTable &Parameters) {
   std::vector<std::vector<double> > luttable;
   int rows = 0, columns = 0;
   GetDimension(pLutNode, rows, columns);
   // son 2 filas (entrada/salida) y al menos 1 columna (bin)
   if (rows != 2 && columns < 1) {
      return false;
   }
   LoadTable(pLutNode, luttable);
   LookUpTable::LutType lut;
   for (int i = 0; i < columns; i++)
      lut.insert(std::make_pair(luttable.at(0).at(i), luttable.at(1).at(i)));
   Parameters.SetTable(lut);
   return true;
}

/**
 *  Convierte una tabla a un nodo XML.
 * \attention Eliminar el nodo es responsabilidad del codigo cliente
 * @param[in] Parameters Tabla
 * @param[out] pLutNode Nodo con la serializacion de la tabla
 * \pre La Tabla debe tener exactamente 2 filas y al menos 1 columna.
 * \post pLutNode tendra valor NULL en caso de que haya error (retorno False)
 * \attention defaultValue_ no se almacena en el XML
 * @return True Si se pudo generar una tabla xml con los valores. Las
 *  dimensiones deber ser exactamente 2 filas y al menos una columna.
 * @return False Si no cumple con las condiciones
 */
bool LookUpTable::ToXml(const LookUpTable &Parameters, wxXmlNode* &pLutNode) {
   std::vector<std::vector<double> > luttable(2);
   LutType::const_iterator it = Parameters.lookUpTable_.begin();
   while (it != Parameters.lookUpTable_.end()) {
      luttable[0].push_back(it->first);
      luttable[1].push_back(it->second);
      it++;
   }
   wxXmlNode *plutnode = NULL;
   CreateTable(plutnode, luttable);
   int rows = 0, columns = 0;
   GetDimension(plutnode, rows, columns);
   // son dos filas (entrada/salida) y N columnas (bins)
   if (rows == 2 && static_cast<size_t>(columns) == Parameters.lookUpTable_.size()) {
      pLutNode = plutnode;
   } else {
      delete plutnode;
   }
   // si son iguales distintos de NULL tuvo exito
   return pLutNode == plutnode && plutnode;
}

/** Convierte la tabla local en xml */
bool LookUpTable::ToXml(wxXmlNode *pLutNode) const {
   return ToXml(*this, pLutNode);
}

/**
 *  Metodo que aplica la tabla para un dato de entrada (tipo de dato nativo).
 *
 * \post El valor es de entrada se trunca utilizando la misma funcion que lo
 *  pasa a XML para que al guardar una tabla y volverla a cargar sean iguales.
 * @param[in] InputData Valor para el cual se busca la salida en la tabla.
 * @return El valor asociado al menor limite mayor o igual que InputData
 * @return Si InputData es mayor que el mayor limite, se retorna el valor
 *  asociado este ultimo.
 * @return Si la tabla esta vacia, retorna defaultValue_
 */
double LookUpTable::ApplyLut(const double InputData) const {
   // los paso por join/split para ajustar los decimales al XML
   std::vector<double> inputdata(1, InputData);
   inputdata = split<double>(join(inputdata.begin(), inputdata.end()));
   if (lookUpTable_.empty() || inputdata.empty()) {
      return defaultValue_;
   }
   LutType::const_iterator it = lookUpTable_.lower_bound(inputdata.at(0));
   double retval = lookUpTable_.rbegin()->second;
   if (it != lookUpTable_.end()) {
      retval = it->second;
   }
   return retval;
}

/**
 *  Agrega un LowerBound como limite a la tabla y el valor de salida asociado
 * a todos los valores de entrada menores o igual al LowerBound, pero mayores al
 * mayor limite menor que LowerBound.
 *
 * @param[in] LowerBound Limite
 * @param[in] Output Valor que se obtiene cuando se aplica la tabla (ApplyLut)
 *  con un valor menor que LowerBound pero mayor al limite inmediatamente menor.
 * @return True si puede realizar la insercion en el mapa.
 */
bool LookUpTable::AddBin(const double LowerBound, const double Output) {
   // los paso por join/split para ajustar los decimales
   std::vector<double> inputdata(1, LowerBound);
   inputdata = split<double>(join(inputdata.begin(), inputdata.end()));
   return lookUpTable_.insert(std::make_pair(inputdata.at(0), Output)).second;
}

/**
 *  Asigna la tabla interna pasandola por AddBin para normalizar los decimales
 * de los valores y garantizar compatibilidad a nivel XML.
 * @param[in] Table Nueva tabla.
 */
void LookUpTable::SetTable(const LutType &Table) {
   lookUpTable_.clear();
   LutType::const_iterator it = Table.begin();
   while (it != Table.end()) {
      AddBin(it->first, it->second);
      it++;
   }
}

/**
 * Obtiene la tabla interna.
 * @param[out] Table tabla donde copia los datos internos.
 */
void LookUpTable::GetTable(LutType &Table) const {
   Table = lookUpTable_;
}
}

