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

#include "suri/LutArray.h"

// Includes estandar

// Includes Suri
#include "suri/xmlnames.h"

// Includes Wx

// Includes App

// Defines
/** Se utiliza para mantener compatibilidad con LutRenderer (genera la propiedad outputband del */
// xml)
#define __MAINTAIN_LUTRENDERER_COMPATIBILITY__

/** namespace suri */
namespace suri {
/** ctor */
LutArray::LutArray() :
      active_(false) {
}

/** dtor */
LutArray::~LutArray() {
}

/**
 * Genera un array a partir del Nodo xml.
 *
 * @param[in] pLutArrayNode
 * @param[out] Parameters Array donde se cargan las tablas que se encuentran
 *  definidas en el XML.
 * @return True En caso de que haya cargado el array.
 * @return False Si hubo error al cargar alguna de las tablas
 */
bool LutArray::FromXml(const wxXmlNode *pLutArrayNode, LutArray &Parameters) {
   // Si es NULL o no se trata de un Nodo LUT, falla
   if (!pLutArrayNode || pLutArrayNode->GetName() != wxT(LUT_NODE)) {
      return false;
   }
   LutArray parameters;
   // Lee el estado de activacion
   parameters.active_ = pLutArrayNode->GetPropVal(wxT(ACTIVE_PROPERTY),
                                                  wxT(INACTIVE_PROPERTY_VALUE)) ==
                                                  wxT(ACTIVE_PROPERTY_VALUE);
   // Lee el nombre
   wxString value;
   if (pLutArrayNode->GetPropVal(wxT(NAME_PROPERTY), &value)) {
      parameters.lutName_ = value.c_str();
   }

   // Recorre las tablas
   wxXmlNode *plutnodes = pLutArrayNode->GetChildren();
   while (plutnodes) {
      if (plutnodes->GetName().compare("etiquetas") != 0) {
         /** Lee la banda de entrada o falla */
         wxString value = wxT("0");
         if (!plutnodes->GetPropVal(wxT(INPUT_BAND_PROPERTY), &value)) {
            return false;
         }
         long inputband = -1;
         LookUpTable table;
         // Convierte el nodo a LookUpTable y lo agrega al array o falla
         if (value.ToLong(&inputband) && LookUpTable::FromXml(plutnodes, table)) {
            parameters.AddLookUpTable(table, inputband);
         } else {
            return false;
         }
      }
      plutnodes = plutnodes->GetNext();
   }
   Parameters = parameters;
   return true;
}

/**
 *  Genera un XML a partir de los datos del array.
 * \attention El Nodo debe ser eliminado por el codigo cliente
 * @param[in] Parameters Array con las tablas a convertir a XML
 * @param[out] pLutArrayNode Nodo xml con el array convertido. Debe ser
 *  eliminado por el codigo cliente.
 * \post Si el valor de retorno es False, pLutArrayNode valdra NULL.
 * @return True En caso de que se haya podido hacer la serializacion
 * @return False En caso de error al convertir los datos.
 */
bool LutArray::ToXml(const LutArray &Parameters, wxXmlNode* &pLutArrayNode) {
   // Nodo temporal
   wxXmlNode *plutarraynode = NULL;
   for (int i = 0; i < Parameters.GetCount(); i++) {
      // El primer ciclo, lo inicializo
      if (!plutarraynode) {
         // Creo el nodo
         plutarraynode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(LUT_NODE));
         // Agrego la propiedad de activacion
         plutarraynode->AddProperty(
               new wxXmlProperty(wxT(ACTIVE_PROPERTY),
                                 Parameters.active_ ? wxT(
                                       ACTIVE_PROPERTY_VALUE) :
                                                      wxT(INACTIVE_PROPERTY_VALUE)));
         // Agrego la propiedad nombre
         if (!Parameters.lutName_.empty()) {
            plutarraynode->AddProperty(
                  new wxXmlProperty(wxT(NAME_PROPERTY), Parameters.lutName_.c_str()));
         }
      }
      // Nodo de 1 tabla
      wxXmlNode *plutnode = NULL;
      int inputband = -1;
      // Si serializa con exito la tabla, agrega propiedades
      if (LookUpTable::ToXml(Parameters.GetLookUpTable(i, inputband), plutnode)
            && plutnode) {
         plutnode->AddProperty(
               new wxXmlProperty(wxT(INPUT_BAND_PROPERTY),
                                 wxString::Format(wxT("%d"), inputband)));
#ifdef __MAINTAIN_LUTRENDERER_COMPATIBILITY__
         plutnode->AddProperty(
               new wxXmlProperty(wxT(OUTPUT_BAND_PROPERTY),
                                 wxString::Format(wxT("%d"), i)));
#endif   // __MAINTAIN_LUTRENDERER_COMPATIBILITY__
         // lo agrega como hijo del nodo LUT
         plutarraynode->AddChild(plutnode);
         plutnode = NULL;
      } else  // Si falla, elimina el nodo y sale
      {
         delete plutarraynode;
         plutarraynode = NULL;
         break;
      }
   }
   // Actualiza el nodo de salida (si hubo un error => plutarraynode == NULL)
   pLutArrayNode = plutarraynode;
   return pLutArrayNode;   // Si pLutArrayNode == NULL, retorna false
}

/**
 *  Convierte la tabla interna.
 *
 * @param[out] pLutArrayNode Nodo con la tabla
 * @return El mismo valor de retorno que el metodo estatico
 */
bool LutArray::ToXml(wxXmlNode* &pLutArrayNode) const {
   return ToXml(*this, pLutArrayNode);
}

/**
 * @return La cantidad de tablas.
 */
int LutArray::GetCount() const {
   return lutArray_.size();
}

/**
 *  Retorna la tabla asociada a la banda de salida.
 * @param[in] OutputBand Banda de salida asociada a la tabla deseada.
 * @param[out] InputBand Banda de entrada para la tabla retornada.
 * @return Si OutputBand < GetCount() la tabla asociada a dicha banda de salida.
 * @return Si OutputBand >= GetCount() la ultima tabla del array. InputBand se
 *  carga con el valor de OutputBand.
 * @return Si OutputBand < 0 o GetCount() == 0, retorna una tabla vacia y banda
 *  de entrada 0.
 */
LookUpTable LutArray::GetLookUpTable(int OutputBand, int &InputBand) const {
   // No hay tablas o me piden banda de salida negativa, salgo con tabla vacia
   if (OutputBand < 0 || GetCount() == 0) {
      InputBand = 0;
      return LookUpTable();
   }
   // Si se pasa de las tablas, retorno la ultima tabla con banda de entrada
   // igual a la de salida
   if (OutputBand >= GetCount()) {
      InputBand = OutputBand;
      int dummy = -1;
      return GetLookUpTable(GetCount() - 1, dummy);
   }
   // Encuentra la tabla y retorna los valores
   const LookUpTablePair &pair = lutArray_.at(OutputBand);
   InputBand = pair.first;
   return pair.second;
}

/**
 *  Agrega una tabla al array.
 * @param[in] Lut Tabla a agregar
 * @param[in] InputBand Banda de entrada asociada a dicha tabla, la banda
 *  de salida correspondera al valor de GetCount previo a la llamada a este
 *  metodo.
 * \post La tabla quedara asociada a la banda de salida correspondiente a la
 *  posicion en que se encuentra en el array. Dicha posicion estara dada por el
 *  orden en el que se agregaron las tablas.
 */
void LutArray::AddLookUpTable(const LookUpTable &Lut, const int InputBand) {
   lutArray_.push_back(std::make_pair(InputBand, Lut));
}

/** Retorna el nombre de la lut */
std::string LutArray::GetName() const {
   return lutName_;
}

/** Modifica el nombre de la lut */
void LutArray::SetName(const std::string &LutName) {
   lutName_ = LutName;
}
}
