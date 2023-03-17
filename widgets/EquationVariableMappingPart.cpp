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

// Includes Estandard
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <utility>

// Includes Suri
#include "wxGenericTableBase.h"
#include "suri/EquationVariableMappingPart.h"
#include "suri/Table.h"
#include "DefaultTable.h"
#include "suri/AuxiliaryFunctions.h"

// Includes wx
#include "wx/grid.h"

// define
#define ID_BAND_VARIABLE_MAPPING_PANEL "ID_BAND_VARIABLE_MAPPING_PANEL"
#define label_DESCRIPTION "Descripcion"
#define VARIABLECOLUMNPOSITION 0
#define BANDCOLUMNPOSITION 1
#define FIRSTBANDNAMENUMBER 1
#define DEFAULTBANDNAME "b"

namespace suri {

/**
 * Retorna mapa con errores
 */
std::map<int, const char*> GetParserErrorMap() {
   std::map<int, const char*> errormap;
   errormap[EquationParser::ecUNEXPECTED_OPERATOR] = message_UNEXPECTED_BINARY_OPERATOR;
   errormap[EquationParser::ecUNASSIGNABLE_TOKEN] = message_UNKNOWN_TOKEN;
   errormap[EquationParser::ecUNEXPECTED_EOF] = message_UNEXPECTED_EOF;
   errormap[EquationParser::ecUNEXPECTED_ARG_SEP] = message_UNEXPECTED_COMMA;
   errormap[EquationParser::ecUNEXPECTED_ARG] = message_UNEXPECTED_ARGUMENT;
   errormap[EquationParser::ecUNEXPECTED_VAL] = message_UNEXPECTED_VALUE;
   errormap[EquationParser::ecUNEXPECTED_VAR] = message_UNEXPECTED_VARIABLE;
   errormap[EquationParser::ecUNEXPECTED_PARENS] = message_UNEXPECTED_PARENTHESIS;
   errormap[EquationParser::ecUNEXPECTED_STR] = message_UNEXPECTED_STRING_POSITION;
   errormap[EquationParser::ecSTRING_EXPECTED] = message_UNEXPECTED_STRING_FUNCTION_PARAMETER;
   errormap[EquationParser::ecVAL_EXPECTED] = message_UNEXPECTED_VALUE_FUNCTION_PARAMETER;
   errormap[EquationParser::ecMISSING_PARENS] = message_MISSING_PARENTHESIS;
   errormap[EquationParser::ecUNEXPECTED_FUN] = message_UNEXPECTED_FUNCITON;
   errormap[EquationParser::ecUNTERMINATED_STRING] = message_UNKNOWN_CONSTANT;
   errormap[EquationParser::ecTOO_MANY_PARAMS] = message_TOO_MANY_PARAMS;
   errormap[EquationParser::ecTOO_FEW_PARAMS] = message_TOO_FEW_PARAMS;
   errormap[EquationParser::ecOPRT_TYPE_CONFLICT] = message_TYPE_CONFLICT;
   errormap[EquationParser::ecSTR_RESULT] = message_STR_RESULT;
   errormap[EquationParser::ecGENERIC] = message_GENERIC_ERROR;
   errormap[EquationParser::ecLOCALE] = message_CONFLICTLOCAL;
   errormap[EquationParser::ecUNEXPECTED_CONDITIONAL] = message_UNEXPECTED_CONDITIONAL;
   errormap[EquationParser::ecMISSING_ELSE_CLAUSE] = message_MISSING_ELSE;
   errormap[EquationParser::ecMISPLACED_COLON] = message_MISPLACED_COLON;
   errormap[EquationParser::ecINTERNAL_ERROR] = message_INTERNAL_ERROR;
   errormap[EquationParser::ecUNDEFINED] = message_UNDEFINED_ERROR;
   return errormap;
}

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(EquationVariableMappingPartEventHandler, EquationVariableMappingPart)
   IMPLEMENT_EVENT_CALLBACK(OnUpdateEquationButton, OnUpdateEquationButton(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

EquationVariableMappingPart::EquationVariableMappingPart(const std::vector< std::pair<
                                                       std::string, std::string> > &VarDescription,
                                                       int LayoutFlags) :
                              layoutFlags_(LayoutFlags), pVariablesTable_(NULL),
                              pTable_(NULL),
                              pEventHandler_(new EquationVariableMappingPartEventHandler(this)),
                              variableDescription_(VarDescription) {
}

EquationVariableMappingPart::EquationVariableMappingPart(const std::vector<std::string>& BandNames,
                          const std::vector< std::pair<std::string, std::string> >& VarDescription,
                          int LayoutFlags) :
                              layoutFlags_(LayoutFlags), pVariablesTable_(NULL),
                              pTable_(NULL), imageBands_(BandNames),
                              pEventHandler_(new EquationVariableMappingPartEventHandler(this)),
                              variableDescription_(VarDescription) {
}

EquationVariableMappingPart::~EquationVariableMappingPart() {
   DELETE_EVENT_OBJECT;
   pVariablesTable_ = NULL;
}

bool EquationVariableMappingPart::CreateToolWindow() {
   // ID_BAND_VARIABLE_MAPPING_PANEL
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_, ID_BAND_VARIABLE_MAPPING_PANEL);
   if (!pToolWindow_)
      return false;
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_UPDATEVARIABLES_BITMAPBUTTON", wxBitmapButton)->Connect(
            wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(
            EquationVariableMappingPartEventHandler::OnUpdateEquationButton),
            NULL, pEventHandler_);
   SetInitialValues();
   return true;
}

/** Se obtiene la asociacion de variables con bandas actual */
std::map<std::string, int> EquationVariableMappingPart::GetVariableBands() const {
   return variableBands_;
}
/** se obtiene la descripcion de las bandas actual **/
std::vector<std::pair<std::string, std::string> > EquationVariableMappingPart::GetVariableDescription() const {
   return variableDescription_;
}

/** obtiene la ecuacion **/
std::string EquationVariableMappingPart::GetEquation() const {
   return equation_;
}

/** configura la ecuacion a utilizar **/
void EquationVariableMappingPart::SetEquation(const std::string& Equation) {
   widgetEquation_ = equation_ = Equation;
   ReadEquationVariables(widgetEquation_, widgetVariableBands_);
   if (pToolWindow_) {
      XRCCTRL(*(this->GetWidget()->GetWindow()), wxT("ID_EQUATION_TEXT"),
            wxTextCtrl)->SetValue(equation_);
      UpdateParametersToWidget();
   }
}

void EquationVariableMappingPart::SetEquation(const std::string& Equation,
                                    const std::vector< std::pair<std::string, std::string> >
                                    &VarDescription) {
   variableDescription_ = VarDescription;
   SetEquation(Equation);
}


/** Indica si el Part tiene cambios para salvar. */
bool EquationVariableMappingPart::HasChanged() {
   if (pVariablesTable_ && (pVariablesTable_->IsEditing() || !ReadParametersFromWidget()))
      return true;

   return !(widgetEquation_ == equation_ && widgetVariableBands_.size() == variableBands_.size()
         && std::equal(widgetVariableBands_.begin(), widgetVariableBands_.end(),
                       variableBands_.begin()));
}

/** Salva los cambios realizados en el Part. */
bool EquationVariableMappingPart::CommitChanges() {
   if (!HasValidData()) {
      return false;
   }
   pVariablesTable_->EndEdition();
   equation_ = widgetEquation_;
   variableBands_ = widgetVariableBands_;

   // Elimino variables de mas en la tabla
   ReadEquationVariables(widgetEquation_, widgetVariableBands_);

   equation_ = widgetEquation_;
   variableBands_.clear();
   variableBands_.insert(widgetVariableBands_.begin(), widgetVariableBands_.end());
   variableDescription_.clear();
   int descpos = imageBands_.empty() ? 1 : 2;
   variableDescription_.clear();
   for (int r = 0; r < pVariablesTable_->GetNumberRows(); ++r) {
      std::string variable = pVariablesTable_->GetValue(r, 0).c_str();
      std::string variabledescp = pVariablesTable_->GetValue(r, descpos).c_str();
      variableDescription_.push_back(std::pair<std::string, std::string>(variable, variabledescp));
   }
   // Actualizo la gui por si se eliminaron variables en la tabla
   UpdateParametersToWidget();
   return true;
}

/** Restaura los valores originales del Part. */
bool EquationVariableMappingPart::RollbackChanges() {
   pVariablesTable_->EndEdition();
   widgetEquation_ = equation_;
   widgetVariableBands_.clear();
   widgetVariableBands_.insert(variableBands_.begin(), variableBands_.end());
   UpdateParametersToWidget();
   return true;
}

/** Inicializa el part */
void EquationVariableMappingPart::SetInitialValues() {
     // Configuro tabla con datos
   pTable_ = new suri::DefaultTable();
   pTable_->AppendColumn(label_VARIABLES, Table::STRING);
   if (!imageBands_.empty())
      pTable_->AppendColumn(label_BANDS, Table::STRING);
   if (layoutFlags_ & VariableDescriptionField)
      pTable_->AppendColumn(label_DESCRIPTION, Table::STRING);
   pVariablesTable_ = new suri::wxGenericTableBase(pTable_);
   wxGrid* pgrid = XRCCTRL(*(this->GetWidget()->GetWindow()), wxT("ID_VARIABLES_GRID"), wxGrid);
   pgrid->SetTable(pVariablesTable_);
   pgrid->SetRowLabelSize(0);
   if (!imageBands_.empty())
      SetBandFieldRestrictions();

   // Configuro la primer columna para que no se pueda modificar (despues de cargar los datos)
   pVariablesTable_->SetReadOnlyColumnView(label_VARIABLES, true);
   if ((layoutFlags_ & ReadOnlyDescription) && layoutFlags_ & VariableDescriptionField)
      pVariablesTable_->SetReadOnlyColumnView(label_DESCRIPTION, true);
   UpdateParametersToWidget();
}

/** metodo privado encargado de configurar la cantidad de
 * valores que puede tomar la columna de bandas**/
void EquationVariableMappingPart::SetBandFieldRestrictions() {
   // Se agregan restricciones a la tabla
   std::string restrictions = "";
   for (size_t i = 0; i < imageBands_.size(); i++) {
      std::stringstream num;
      num << i;
      restrictions.append(num.str());
      restrictions.append(Table::KeyValueSeparator);
      restrictions.append(_(imageBands_[i].c_str()));
      if (i != (imageBands_.size() - 1)) {
         restrictions.append(Table::TokenSeparator);
      }
   }
   pVariablesTable_->GetTable()->ChangeRestrictions(1, restrictions);
}

/** Actualiza el estado de la parte */
void EquationVariableMappingPart::Update() {
}

/**
 * Valida la ecuacion ingresada por el usuario.
 * Si ocurre un error muestra msg por pantalla.
 * En caso de exito se actualiza la tabla con las variables.
 * @param[in] Event evento generado por wx.
 */
void EquationVariableMappingPart::OnUpdateEquationButton(wxCommandEvent &Event) {
   if (pVariablesTable_->IsEditing()) {
      /* TCK #11048: Si hay alguna celda en edicion cuando el boton es presionado,
       * se guarda el ultimo valor ingresado por el usuario en dicha celda y se fuerza
       * el fin de la edicion poniendo el cursor en la celda superior izquierda.
       */
      wxGrid* pgrid;
      pgrid = XRCCTRL(*(this->GetWidget()->GetWindow()), wxT("ID_VARIABLES_GRID"), wxGrid);
      int currentRow = pgrid->GetGridCursorRow();
      int currentCol = pgrid->GetGridCursorCol();
      wxGridCellEditor* currentCellEditor = pgrid->GetCellEditor(currentRow, currentCol);
      wxString currentValue = currentCellEditor->GetValue();
      pVariablesTable_->SetValue(currentRow, currentCol, currentValue);
      pgrid->SetGridCursor(0,0);
   }
   if (!ReadParametersFromWidget())
      return;
   EquationParser::EquationError error = ReadEquationVariables(widgetEquation_,
                                                               widgetVariableBands_);
   if (error.code_ != EquationParser::ecNO_ERROR) {
     std::map<int, const char*> errors = GetParserErrorMap();
     char a;    // contienen los parametros q se van a mostrar junto al error
     char b;
     if (error.code_ == EquationParser::ecUNEXPECTED_VAR) {
        a = widgetEquation_[error.position_-1];
        b = widgetEquation_[error.position_];
        SHOW_ERROR(_(message_ERROR_MULTIPLY_cc), a, b);
     }else if (error.code_==EquationParser::ecUNASSIGNABLE_TOKEN) {
        a = widgetEquation_[error.position_];
        SHOW_ERROR(_(message_ERROR_UNEXPECTED_TOKEN_c), a);
     }else if (error.code_==EquationParser::ecUNEXPECTED_OPERATOR) {
        a = widgetEquation_[error.position_-1];
        b = widgetEquation_[error.position_];
        SHOW_ERROR(_(message_UNEXPECTED_OPERATOR_cc), a, b);
     }else{
      SHOW_ERROR(_(message_ERROR_IN_POSITION_sd), errors[error.code_], error.position_);
     }
     return;
   }
   UpdateParametersToWidget();
}

/**
 * Actualiza los atributos con datos del control grafico.
 * @return bool que indica si ocurrio un error al leer los datos.
 */
bool EquationVariableMappingPart::ReadParametersFromWidget() {
   widgetEquation_ = USE_CONTROL(*GetWidget()->GetWindow(),
         wxT("ID_EQUATION_TEXT"), wxTextCtrl, GetValue(), "");
   if (!pVariablesTable_)
      return false;
   // Actualizo listado de variables seleccionadas
   std::map<std::string, int>::iterator it;
   for (it = widgetVariableBands_.begin(); it != widgetVariableBands_.end(); it++)
      it->second = GetVariableSelectedBand(it->first);
   return true;
}


/**
 * Actualiza el control grafico con datos en los atributos widgetEquation_,
 * @return bool que indica si ocurrio un error al actualizar los datos.
 */
bool EquationVariableMappingPart::UpdateParametersToWidget() {
   GET_CONTROL(*GetWidget()->GetWindow(), "ID_EQUATION_TEXT",
         wxTextCtrl)->SetValue(widgetEquation_.c_str());

   // Actualizo la tabla
   int i = 0;
   int descpos = imageBands_.empty() ? 1 : 2;
   for (int r = pTable_->GetRows() - 1; r >= 0; --r)
      pTable_->DeleteRow(r);
   for (std::map<std::string, int>::iterator it = widgetVariableBands_.begin();
         it != widgetVariableBands_.end(); ++it) {
      pVariablesTable_->AppendRows();
      std::string variablestring = _(it->first.c_str());
      pVariablesTable_->SetValue(i, VARIABLECOLUMNPOSITION,
                                 wxString(variablestring.c_str()));
      if (!imageBands_.empty()) {
         std::string bandstring = _(imageBands_[it->second].c_str());
         pVariablesTable_->SetValue(i, BANDCOLUMNPOSITION,
                                    wxString(bandstring.c_str()));
     }
     if (layoutFlags_ & VariableDescriptionField)
         pVariablesTable_->SetValue(i, descpos, "");
      i++;
   }
   std::vector<std::pair<std::string, std::string> >::iterator desc =
         variableDescription_.begin();
   for (int k = 0;
         layoutFlags_ & VariableDescriptionField && desc != variableDescription_.end();
         ++desc, ++k) {
      pVariablesTable_->SetValue(k, descpos, desc->second);
   }
   pToolWindow_->Layout();
   return true;
}

/**
 * Retorna la banda seleccionada por el usuario para una variable en la
 * ecuacion.
 * @param[in] VariableName nombre de la variable
 * @return posicion de la banda seleccionada en la imagen
 */
int EquationVariableMappingPart::GetVariableSelectedBand(const std::string &VariableName) {
   wxString newvariable = wxString(VariableName);
   wxString variablename;
   wxString bandname;

   // Busco variable en tabla
   for (int i = 0; i < pVariablesTable_->GetNumberRows() && bandname.empty(); i++) {
      variablename = pVariablesTable_->GetValue(i, VARIABLECOLUMNPOSITION);

      if (variablename.CompareTo(newvariable) == 0) {
         bandname = pVariablesTable_->GetValue(i, BANDCOLUMNPOSITION);
      }
   }

   // Busco a que banda corresponde el valor seleccionado
   size_t band;
   for (band = 0;
         band < imageBands_.size() && bandname.CompareTo(wxString(imageBands_[band].c_str())) != 0;
         band++) {
   }
   return band;
}

/**
 * Analiza si la parte tiene datos validos . En caso de encontrar un error
 * actualiza errorMessage_ con msg de error.
 * @return bool que indica si los datos son validos
 */
bool EquationVariableMappingPart::HasValidData() {
   std::map<std::string, int> variables;
   if (!ReadParametersFromWidget())
      return false;
   EquationParser::EquationError error = ReadEquationVariables(widgetEquation_, variables);
   if (error.code_ != EquationParser::ecNO_ERROR) {
      std::map<int, const char*> errors = GetParserErrorMap();
      errorMessage_ = wxString::Format(_(message_ERROR_IN_POSITION_sd), errors[error.code_],
                                       error.position_);
      return false;
   }

   // Veo si se modificaron las variables en la ecuacion.
   std::map<std::string, int>::iterator it;
   for (it = variables.begin(); it != variables.end(); it++)
      if (widgetVariableBands_.find(it->first) == widgetVariableBands_.end()) {
         errorMessage_ = _(message_UPDATE_VARIABLES_TO_CONTINUE);
         return false;
      }

   return true;
}

/**
 * Retorna las variables generadas al parsear Equation.
 * Si la ecuacion esta vacia se configuran las variables default: b1...bn
 * @param[in] Ecuacion a parsear
 * @param[out] Variables obtenidas al parsear la ecuacion. Si ocurrio un
 * error, no se modifica.
 * @return EquationParser::EquationError Flag que retorna el parser
 * En caso de error, el codigo indica el tipo de error cometido.
 */
EquationParser::EquationError EquationVariableMappingPart::ReadEquationVariables(
      const std::string &Equation, std::map<std::string, int> &Variables) {
   std::map<std::string, int> newvariables;
   // Lleno newvariables con variables en ecuacion o con variables default
   if (!Equation.empty()) {
      EquationParser eqparser;
      std::vector < std::string > variablenames;
      if (!eqparser.SetEquation(Equation))
         return eqparser.GetError();

      eqparser.GetVariableNames(variablenames);
      for (size_t i = 0; i < variablenames.size(); i++)
         newvariables.insert(std::pair<std::string, int>(variablenames[i],
                              !imageBands_.empty() ? i % imageBands_.size() : i));
   } else {
      for (size_t i = 0; i < imageBands_.size(); i++) {
         std::string bandname = _(DEFAULTBANDNAME);
         bandname += LongToString(i + FIRSTBANDNAMENUMBER).c_str();
         newvariables.insert(std::pair<std::string, int>(bandname, i));
      }
   }

   // Si hay variables con el mismo nombre en Variables copio las bandas asignadas.
   std::map<std::string, int>::iterator itnew, itold;
   for (itnew = newvariables.begin(); itnew != newvariables.end(); ++itnew) {
      itold = Variables.find(itnew->first);
      if (itold != Variables.end()) {
         int aux = itold->second;
         itnew->second = aux;
      }
   }

   // Actualizo Variables
   Variables.clear();
   Variables.insert(newvariables.begin(), newvariables.end());

   EquationParser::EquationError noerror = { EquationParser::ecNO_ERROR, -1 };
   return noerror;
}

}  // namespace suri
