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

// Includes Suri
#include "DensitySliceEditionPart.h"
#include "DefaultTable.h"
#include "suri/LibraryItem.h"
#include "suri/RangeItemAttribute.h"
#include "suri/LibraryItemAttributeFactory.h"
#include "suri/AuxiliaryFunctions.h"
#include "wxGenericTableBase.h"
#include "TablePart.h"
#include "suri/MemoryDriver.h"

// Includes Wx

// Defines
#define RANGE_ATTR "range"

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(DensitySliceEventHandler, DensitySliceEditionPart)
	IMPLEMENT_EVENT_CALLBACK(OnCellChangeEvent, OnCellChangeEvent(event), wxGridEvent)
	IMPLEMENT_EVENT_CALLBACK(OnAddRowClick, OnAddRowClick(event), wxCommandEvent)
	IMPLEMENT_EVENT_CALLBACK(OnDeleteRowClick, OnDeleteRowClick(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

/** Nombre del XRC que representa la ventana. */
const std::string DensitySliceEditionPart::DS_EDITION_XRC_NAME = wxT("ID_DS_EDITOR_PANEL");
const std::string DensitySliceEditionPart::DS_CGU_PANEL = wxT("ID_SLICE_CGU_PANEL");

/**
 * Constructor
 * @param LibraryId id de la biblioteca de tabla de intervalos
 * @param pManager manager que administra la biblioteca
 * @param Feature layout de features del cgu
 */
DensitySliceEditionPart::DensitySliceEditionPart(
      const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
      UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature) :
      LibraryTableEditionPart(LibraryId, pManager, DS_EDITION_XRC_NAME, DS_CGU_PANEL, Feature),
      NEW_EVENT_OBJECT(DensitySliceEventHandler), continuosRange_(true) {
}

/** destructor **/
DensitySliceEditionPart::~DensitySliceEditionPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Metodo auxiliar privado que configura el part en si para cada editor en particular.
 * Este metodo es el que realmente tienen que implementar las clases hijas para lograr
 * la especializacion a la hora de iniciar el part (metodo que se llama en
 * CreateToolWindow, a modo de Template Method).
 */
bool DensitySliceEditionPart::ConfigureFeatures() {
   if (!pTable_) {
		wxBitmapButton* paddbutton = XRCCTRL(*(this->GetWidget()->GetWindow()),
				wxT("ID_ADDROW_BUTTON"), wxBitmapButton);
		wxBitmapButton* pdelbutton = XRCCTRL(*(this->GetWidget()->GetWindow()),
				wxT("ID_DELROW_BUTTON"), wxBitmapButton);

		paddbutton->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
				wxCommandEventHandler(DensitySliceEventHandler::OnAddRowClick),
				NULL, pEventHandler_);
		pdelbutton->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
				wxCommandEventHandler(
						DensitySliceEventHandler::OnDeleteRowClick), NULL,
				pEventHandler_);

		pTable_ = new DefaultTable();
		pTable_->SetDriver(new MemoryDriver);
		AppendColumns();
		pTablePart_ = new TablePart(pTable_, false, false, NULL, false);
		AddControl(pTablePart_->GetWidget(), "ID_SLICE_TABLE_PANEL");
		pTablePart_->GetGridControl()->Connect(wxEVT_GRID_CELL_CHANGE,
				wxGridEventHandler(DensitySliceEventHandler::OnCellChangeEvent),
				NULL, pEventHandler_);
   }
   return true;
}

/**
 * Metodo auxiliar privado que es necesario implementar en las clases hijas.
 * Este metodo es el encargado de actualizar realmente la informacion en el part
 * Es llamado en el metodo SetActiveItem a modo de Template Method
 **/
void DensitySliceEditionPart::UpdateFields(const LibraryItem* pItem, bool Modifiable) {
   pCurrentItem_ = pItem;
   bool unrangeitem = !IsRangeItem(pItem);
   if (pItem) {
      LoadItemToTable(pItem, unrangeitem);
      if (pTablePart_)
         pTablePart_->SetEnabled(Modifiable);
   }
}

/**
 * Metodo auxiliar privado que agrega comportamiento adicional al rollback de la
 * clase base. Este comportamiento adicional es necesario cuando
 * el rollback involucra objetos adicionales a los items de biblioteca
 * del editor (ej. parts compuesto en donde se necesita llamar a los rollbacks de
 * los mismos)
 * @return true en caso de realizar correctamente el rollback "interno"
 * @return false en caso de que ocurra algun error durante el rollback
 */
bool DensitySliceEditionPart::OnRollback() {
   if (pCurrentItem_) {
      LoadItemToTable(pCurrentItem_, !IsRangeItem(pCurrentItem_));
   }
   return true;
}

/** Metodo auxiliar que carga el contenido del item seleccionado en la tabla del Part
 * @param[in] pItem item que sera representado por la tabla
 * @param[in] AddRangeToItem bool que indica si es necesario agregar rangos al item
 * que se quiere mostrar en la tabla
 */
void DensitySliceEditionPart::LoadItemToTable(const LibraryItem* pItem,
                                              bool AddRangeToItem) {
   if (!pTable_) {
      // Este metodo se llama ya que en el primer click se hacen dos cosas
      // juntas: Crear controles por primera vez y mostrar los datos del
      // item seleccionado. Por como es la inicializacion la parte de
      // mostrar datos necesariamente se ejecuta antes de que los controles
      // hayan sido creados.
      ConfigureFeatures();
   }

   pTable_->Clear();

   if (pTable_->GetColumns() == 0) {
      AppendColumns();
   }
   if (!AddRangeToItem) {
      LibraryItem::MapAttribute attributes = pItem->GetAttributes();
      LibraryItem::MapAttribute::iterator it = attributes.begin();
      for (; it != attributes.end(); ++it) {
         LibraryItemAttribute* pattr = it->second;
         if (pattr->GetName().find(RANGE_ATTR) != std::string::npos) {
            RangeItemAttribute* prange =
                  LibraryItemAttributeFactory::CreateRangeItemAttribute(pattr);
            AddRangeToTable(prange);
         }
      }
   } else {
      std::string complexattributename = LibraryItemAttributeFactory::RangeString;
      complexattributename.append(NumberToString<int>(0));
      std::string attrvalue = ComplexItemAttribute::StartAttributeChar;
      attrvalue += "Intervalo 0";
      attrvalue += ComplexItemAttribute::AttributesToken + "-inf";
      attrvalue += ComplexItemAttribute::AttributesToken + "inf";
      attrvalue += ComplexItemAttribute::AttributesToken + "000000";
      attrvalue += ComplexItemAttribute::FinishAttributeChar;
      LibraryItemAttribute* pattr = new LibraryItemAttribute(complexattributename, attrvalue);
      RangeItemAttribute* prange =
            LibraryItemAttributeFactory::CreateRangeItemAttribute(pattr);
      AddRangeToTable(prange);
   }
   pTablePart_->CommitChanges();
}

/**
 * Agrega un rango perteneciente a un item a la tabla de intervalos
 * @param pRange
 */
void DensitySliceEditionPart::AddRangeToTable(RangeItemAttribute* pRange) {
   if (pRange && pTable_ && pTable_->AppendRow()) {
      int row = pTable_->GetRows() - 1;
      std::string name = pRange->GetRangeName();
      pTable_->SetCellValue(RANGE_NAME_COL, row, name);
      std::string hexacolor = pRange->GetHexaColor();
      pTable_->SetCellValue(HEXCOLOR_COL, row, hexacolor);
      std::string start = NumberToString<double>(pRange->GetStartValue());
      pTable_->SetCellValue(LOW_COL, row, start);
      std::string finish = NumberToString<double>(pRange->GetFinishValue());
      pTable_->SetCellValue(HIGH_COL, row, finish);
   }
}

/**
 * Obtiene de la tabla los rangos cargados
 * y crea un item asociado
 * @param pAttributes vector en donde se guardaran los atributos de la tabal
 */
void DensitySliceEditionPart::LoadAttributesFromTable(
      std::vector<LibraryItemAttribute*>* &pAttributes) {
   // Obtengo los rangos de la tabla
   for (int row = 0; row < pTable_->GetRows(); ++row) {
      std::string complexattributename = LibraryItemAttributeFactory::RangeString;
      complexattributename.append(NumberToString<int>(row));
      std::string cellvalue;
      std::string attrvalue = ComplexItemAttribute::StartAttributeChar;
      pTable_->GetCellValue(RANGE_NAME_COL, row, cellvalue);
      attrvalue += cellvalue;
      pTable_->GetCellValue(LOW_COL, row, cellvalue);
      attrvalue += ComplexItemAttribute::AttributesToken + cellvalue;
      pTable_->GetCellValue(HIGH_COL, row, cellvalue);
      attrvalue += ComplexItemAttribute::AttributesToken + cellvalue;
      pTable_->GetCellValue(HEXCOLOR_COL, row, cellvalue);
      attrvalue += ComplexItemAttribute::AttributesToken + cellvalue
            + ComplexItemAttribute::FinishAttributeChar;
      pAttributes->push_back(new LibraryItemAttribute(complexattributename, attrvalue));
   }
}

/** Metodo que se llama cuando se cambia el valor de una celda **/
void DensitySliceEditionPart::OnCellChangeEvent(wxGridEvent& Event) {
   int row = Event.GetRow();
   int col = Event.GetCol();
   if (col != LOW_COL && col != HIGH_COL) {
      Event.Skip();
      return;
   }
   std::string newvalue;
   pTable_->GetCellValue(col, row, newvalue);
   if (continuosRange_) {
      UpdateRangeLimits(col, row, newvalue);
   }
}

/**
 * A partir del valor actualizado en la columna y fila que se indica, se actualizan
 * los limites de los rangos qeu contiene la tabla
 * @param Column columna en donde se cambio el valor
 * @param Row fila en donde se cambio el valor
 * @param Value valor actualizado
 */
void DensitySliceEditionPart::UpdateRangeLimits(int Column, int Row,
                                                const std::string& Value) {
   bool lowlimitupdate = (Column == LOW_COL);
   double value = RangeItemAttribute::RangeDoubleToString(Value);
   std::string cmpvaluestr;
   if (lowlimitupdate) {
      pTable_->GetCellValue(HIGH_COL, Row, cmpvaluestr);
   } else {
      pTable_->GetCellValue(LOW_COL, Row, cmpvaluestr);
   }
   // actualizo en las dos direcciones
   BackwardRangeLimitsUpdate(Column, Row, value);
   ForwardRangeLimitsUpdate(Column, Row, value);
}

/**
 * Actualiza hacia los limites de los rangos de la tabla hacia adelante
 * @param Column columna en donde se cambio el valor
 * @param Row fila en donde se cambio el valor
 * @param Value valor actualizado
 */
void DensitySliceEditionPart::ForwardRangeLimitsUpdate(int Column, int Row,
                                                       const double Value) {
   bool updatecomplete = false;
   for (int row = Row; !updatecomplete && row < pTable_->GetRows(); ++row) {
      std::string cellstr;
      pTable_->GetCellValue(LOW_COL, row, cellstr);
      double lowvalue = RangeItemAttribute::RangeDoubleToString(cellstr);
      pTable_->GetCellValue(HIGH_COL, row, cellstr);
      double highvalue = RangeItemAttribute::RangeDoubleToString(cellstr);
      std::string newvaluestr = NumberToString<double>(Value);
      if (row != Row || (Value <= lowvalue && row == Row))
         pTable_->SetCellValue(LOW_COL, row, newvaluestr);
      if (Value >= highvalue)
         pTable_->SetCellValue(HIGH_COL, row, newvaluestr);
      else
         updatecomplete = true;
   }
}

/**
 * Actualiza hacia los limites de los rangos de la tabla hacia atras
 * @param Column columna en donde se cambio el valor
 * @param Row fila en donde se cambio el valor
 * @param Value valor actualizado
 */
void DensitySliceEditionPart::BackwardRangeLimitsUpdate(int Column, int Row,
                                                        const double Value) {
   bool updatecomplete = false;
   for (int row = Row; row >= 0 && !updatecomplete; --row) {
      std::string cellstr;
      pTable_->GetCellValue(LOW_COL, row, cellstr);
      double lowvalue = RangeItemAttribute::RangeDoubleToString(cellstr);
      pTable_->GetCellValue(HIGH_COL, row, cellstr);
      double highvalue = RangeItemAttribute::RangeDoubleToString(cellstr);
      std::string newvaluestr = NumberToString<double>(Value);
      if (row != Row || (Value >= highvalue && row == Row))
         pTable_->SetCellValue(HIGH_COL, row, newvaluestr);
      if (Value <= lowvalue)
         pTable_->SetCellValue(LOW_COL, row, newvaluestr);
      else
         updatecomplete = true;
   }

}

/** Metodo que se llama cuando se presiona el boton de agregar columna*/
void DensitySliceEditionPart::OnAddRowClick(wxCommandEvent& Event) {
   wxArrayInt rows = pTablePart_->GetSelectedRows();
   size_t count = rows.GetCount();
   int maxrow = -1;
   for (size_t i = 0; i < count; i++) {
      if (rows.Item(i) > maxrow)
         maxrow = rows.Item(i);
   }
   int rowpos = -1;
   if (maxrow > 0) {
      rowpos = pTable_->GetRows() > 1 ? maxrow + 1 : -1;
      pTable_->InsertRow(maxrow + 1);
   } else {
      pTable_->AppendRow();
      rowpos = pTable_->GetRows() > 1 ? pTable_->GetRows() - 1 : -1;
   }
   if (rowpos >= 0) {
      std::string highvaluestr;
      pTable_->GetCellValue(HIGH_COL, rowpos - 1, highvaluestr);
      pTable_->SetCellValue(LOW_COL, rowpos, highvaluestr);
      pTable_->SetCellValue(HIGH_COL, rowpos, highvaluestr);
      std::string hexacolor = "000000";
      pTable_->SetCellValue(HEXCOLOR_COL, rowpos, hexacolor);
      double value = RangeItemAttribute::RangeDoubleToString(highvaluestr);
      BackwardRangeLimitsUpdate(LOW_COL, rowpos, value);
      if (rowpos != pTable_->GetRows() - 1) {
         ForwardRangeLimitsUpdate(HIGH_COL, rowpos, value);
      }
   }
}

/** Metodo que dado un item busca que tenga la menos un atributo correspondiente a un
 *  intervalo de tabla de colores
 *  @param[in] pItem item sobre el cual se quiere consultar
 *  @return true en caso de que tenga al menos un atributo, false en caso contrario
 */
bool DensitySliceEditionPart::IsRangeItem(const LibraryItem* pItem) {
   if (!pItem)
      return false;
   LibraryItem::MapAttribute attributes = pItem->GetAttributes();
   LibraryItem::MapAttribute::const_iterator it = attributes.begin();
   bool rangefound = false;
   for (; !rangefound && it != attributes.end(); ++it) {
      std::string attrname = it->first;
      // verifico si el nombre del atributo contiene "range"
      rangefound = attrname.find(RANGE_ATTR) != std::string::npos;
   }
   return rangefound;
}

/** Agrega las columnas de la tabla */
void DensitySliceEditionPart::AppendColumns() {
	pTable_->AppendColumn("Nombre", Table::STRING);
	pTable_->AppendColumn("Limite inferior", Table::STRING);
	pTable_->AppendColumn("Limite superior", Table::STRING);
	pTable_->AppendColumn("Color", Table::COLOR_RGBA_HEXA);
}
} /** namespace suri **/
