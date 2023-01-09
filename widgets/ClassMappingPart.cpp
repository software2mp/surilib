/*! \file ClassMappingPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <map>
#include <vector>
#include <string>

// Includes Suri
#include "wxmacros.h"
#include "DefaultTable.h"
#include "ClassMappingPart.h"
#include "ClassInformation.h"
#include "wxGenericTableBase.h"
#include "suri/Element.h"
#include "suri/messages.h"
#include "suri/VectorStyle.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/grid.h"
#include "wx/xrc/xmlres.h"

// Includes App

// Defines
#define ID_TABLE_CONTAINER "ID_TABLE_CONTAINER"

namespace suri {

/**
 * Nombre de columna de origen
 */
const std::string ClassMappingPart::OriginField = "Origen";

/**
 * Nombre de columna de destino
 */
const std::string ClassMappingPart::DestinationField = "Destino";

/** Constructor
 * @param[in] pElement referencia al elemento al que pertenece el mapeo de clases
 * @param[in] ChoiceValues valores iniciales para el choice
 * @param[in] OriginColumnName Nombre de la primera columna
 * @param[in] DestinationColumnName Nombre de la segunda columna
 * @param[in] Enable
 * @param[in] Modified
 * */
ClassMappingPart::ClassMappingPart(Element *pElement, const std::vector<MappingTable>& MapTable,
                                   const std::vector<ChoiceValues>& ChoiceOptionValues,
                                   const std::string& OriginColumnName,
                                   const std::vector<std::string>& DestinationColumnName,
                                   bool Enable, bool Modified) :
      Part(Enable, Modified), pVariablesTable_(NULL), pElement_(pElement),
      originColumnName_(OriginColumnName), destinationColumnName_(DestinationColumnName) {
   mappingTable_ = MapTable;
   rollbackMappingTable_ = mappingTable_;
   choiceValues_ = ChoiceOptionValues;
}

/**
 *  Destructor
 */
ClassMappingPart::~ClassMappingPart() {
}

/**
 * Indica si el Part tiene cambios para salvar.
 * Se verifica si la tabla tiene algun cambio
 */
bool ClassMappingPart::HasChanged() {
   return pTablePart_->HasChanged();
}

/**
 * Salva los cambios realizados en el Part
 * Guarda el resultado en mappingTable para que
 * luego se obtenga el mapa resultado de la asociacion
 */
bool ClassMappingPart::CommitChanges() {
   bool result = LoadMapFromTable();
   rollbackMappingTable_ = mappingTable_;
   return result;
}

/* Carga la tabla a un map**/
bool ClassMappingPart::LoadMapFromTable() {
   if (pTablePart_->HasChanged()) {
      mappingTable_.clear();
      for (int c = 1; c < pVariablesTable_->GetColumns(); ++c)
         mappingTable_.push_back(MappingTable());

      int rows = pVariablesTable_->GetRows();
      for (int i = 0; i < rows; ++i) {
         // Obtengo los valores de la tabla y los inserto al map
         std::string originvalue;
         pVariablesTable_->GetCellValue(0, i, originvalue);
         std::string destinationvalue;
         /** Para cada columna de relacion se arma el mapa de relaciones **/
         for (int c = 1; c < pVariablesTable_->GetColumns(); ++c) {
            pVariablesTable_->GetCellValue(c, i, destinationvalue);
            MappingTablePair pair(originvalue, destinationvalue);
            mappingTable_[c-1].insert(pair);
         }
      }
   }
   return true;
}
/**
 * Restaura los valores originales del Part.
 * Se vuelve a configurar la grilla con los valores iniciales
 */
bool ClassMappingPart::RollbackChanges() {
   mappingTable_ = rollbackMappingTable_;
   ConfigureGuiTable();
   return true;
}

/**
 * Configura la tabla del part con la informacion que contiene el elemento
 * Configurando la primer columna como readonly y la segunda con
 * un editor de tipo combobox
 */
void ClassMappingPart::ConfigureGuiTable() {
   // Cargo los valores
   bool append = pVariablesTable_->GetRows() == 0;
   for (size_t j = 0; j < mappingTable_.size(); ++j) {
      MappingTable::const_iterator rowit = mappingTable_[j].begin();
      for (int row = 0; rowit != mappingTable_[j].end(); ++rowit, ++row) {
         if (append && j == 0)
            pVariablesTable_->AppendRow();
         std::string originvalue = rowit->first;
         std::string destination = rowit->second;
         pVariablesTable_->SetCellValue(0, row, originvalue);
      }
   }
/**
   int row = 0;
   for (int k = 0; mapit != mappingTable_.end(); ++mapit, ++k) {
      if (append)
         pVariablesTable_->AppendRow();
      MappingTable::const_iterator relation = (*mapit).begin();
      std::string originvalue = relation->first;
      std::string destination = relation->second;
      pVariablesTable_->SetCellValue(0, row, originvalue);
      pVariablesTable_->SetCellValue(k + 1, row, destination);
      row++;
   }
**/
   // Se agregan restricciones a la tabla
   for (size_t r = 0; r < choiceValues_.size(); ++r) {
      std::string restrictions = "";
      size_t size = choiceValues_[r].size();
      std::vector<std::string>::const_iterator optionsit = choiceValues_[r].begin();
      for (size_t i = 0; optionsit != choiceValues_[r].end(); optionsit++, i++) {
         std::stringstream num;
         num << i;
         restrictions.append(num.str());
         restrictions.append(Table::KeyValueSeparator);
         restrictions.append(*optionsit);
         if (i != (size - 1))
            restrictions.append(Table::TokenSeparator);
      }
      pVariablesTable_->ChangeRestrictions(r + 1, restrictions);
   }
}

/**
 * Crea la ventana de la parte.
 * Inicializa la tabla con el map pasado en el constructor
 * y configura wxgrid para que tome como editor un wxchoice
 * @return true si pudo crear la ventana
 */
bool ClassMappingPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_, wxT("ID_CLASS_MAPPING_PANEL"));
   pVariablesTable_ = new suri::DefaultTable();
   pTablePart_ = new TablePart(pVariablesTable_);
   pVariablesTable_->AppendColumn(originColumnName_, Table::STRING);
   for (size_t i = 0; i < destinationColumnName_.size(); ++i)
      pVariablesTable_->AppendColumn(destinationColumnName_[i], Table::STRING);
   pTablePart_->SetReadOnlyColumnView(originColumnName_);
   pTablePart_->SetSelectionNotifier(NULL);
   AddControl(pTablePart_, wxT(ID_TABLE_CONTAINER));
   return true;
}

/**
 * Inicializa el part
 */
void ClassMappingPart::SetInitialValues() {
   ConfigureGuiTable();
   //this->pTablePart_->CommitChanges();
}

/**
 * Actualiza el estado de la parte
 */
void ClassMappingPart::Update() {
}

/** Agrega una opcion al choice que posee el part
 * @param[in] OptionValue string con el valor de la opcion a agregar al choice
 * @return true si pude agregar el valor al choice
 * @return false en caso de que la opcion no tenga texto o ya exista.
 */
bool ClassMappingPart::AddChoiceOption(const std::string& OptionValue, int MappingIndex) {
   // TODO(Gabriel - TCK #862): Cuando se agrega un valor no se tendria que volver
   // a generar toda la configuracionde la columna
   if (OptionValue.size() == 0 && MappingIndex >= (int)choiceValues_.size())
      return false;
   wxArrayString options;
   ChoiceValues::const_iterator optionsit = choiceValues_[MappingIndex].begin();
   bool exists = false;
   for (; optionsit != choiceValues_[MappingIndex].end() && !exists; ++optionsit) {
      if ((*optionsit).compare(OptionValue) != 0)
         options.Add(*optionsit);
      else
         exists = true;
   }
   if (!exists) {
      choiceValues_[MappingIndex].push_back(OptionValue);
      options.Add(OptionValue);

      // Se agregan restricciones a la tabla
      std::string restrictions = "";
      size_t i = 0;
      size_t size = choiceValues_[MappingIndex].size();
      std::vector<std::string>::const_iterator it = choiceValues_[MappingIndex].begin();
      for (; it != choiceValues_[MappingIndex].end(); it++, i++) {
         std::stringstream num;
         num << i;
         restrictions.append(num.str());
         restrictions.append(Table::KeyValueSeparator);
         restrictions.append(*it);
         if (i != (size - 1)) {
            restrictions.append(Table::TokenSeparator);
         }
      }
      pVariablesTable_->ChangeRestrictions(MappingIndex + 1, restrictions);
   }

   return (!exists);
}

/** Si se hizo commit, devuelve la informacion actual de la tabla en un map
 * @return MappingTable mapa con la informacion de la tabla
 */
std::vector<ClassMappingPart::MappingTable> ClassMappingPart::GetResultMappingTable() {
   LoadMapFromTable();
   return mappingTable_;
}

/**
 * Agrega una clase nueva a la tabla.
 */
void ClassMappingPart::AppendClass(const std::string& Value) {
   if (Value.empty())
      return;

   std::string val1 = Value;
   std::string val2 = Value;

   pTablePart_->GetTable()->AppendRow();
   int rowscount = pTablePart_->GetTable()->GetRows();

   pTablePart_->GetTable()->SetCellValue(0, rowscount - 1, val1);
   pTablePart_->GetTable()->SetCellValue(1, rowscount - 1, val2);
}

}/** namespace suri */
