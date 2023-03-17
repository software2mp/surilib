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

// Includes Estandar
#include <vector>
#include <string>
#include <set>

// Includes Suri
#include "VectorStyleClassGenerationWidget.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/VectorEditor.h"
#include "VectorLayer.h"
#include "suri/DataTypes.h"
#include "suri/VectorElement.h"
#include "suri/VectorStyle.h"
#include "SimpleVectorStylePart.h"
#include "suri/PartContainerWidget.h"

// Includes Wx
#include "wx/spinctrl.h"
#include "wx/colour.h"
#include "wx/clrpicker.h"

// Includes Gdal
#include "ogrsf_frmts.h"

// Defines
#define NUM_RANGE_CHOICE_POS 0
#define UNIQUE_VALUE_CHOICE_POS 1

#define SOLID_FILL_POLYGON_STYLE \
   "VECTORSTYLE[3,\"Poligono azul\",BRUSH[\"Suri-brush\",1,COLOR[255,0,0,255]],PEN[\"Suri-pen\",1,COLOR[1,1,1,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",0,COLOR[0,0,0,255],SIZE[1]],LABEL[\"Suri-label\",0,EXPRESSION[],COLOR[0,0,0,255],BACKCOLOR[0,0,0,0],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[4]]]"
#define SOLID_FILL_LINE_STYLE \
   "VECTORSTYLE[2,\"Linea celeste\",PEN[\"Suri-pen\",1,COLOR[0,0,255,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",0,COLOR[0,0,0,255],SIZE[1]],LABEL[\"Suri-label\",0,EXPRESSION[],COLOR[0,0,0,255],BACKCOLOR[0,0,0,0],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[4]]]"
#define SOLID_FILL_POINT_STYLE \
   "VECTORSTYLE[1,\"Triangulo verde\",SYMBOL[\"Suri-symbol\",3,COLOR[0,255,0,255],SIZE[1]],LABEL[\"Suri-label\",0,EXPRESSION[],COLOR[0,0,0,255],BACKCOLOR[0,0,0,0],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[4]]]"


// forwards

namespace suri {

namespace ui {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(VectorStyleClassGenerationEventHandler,
                             VectorStyleClassGenerationWidget)
   IMPLEMENT_EVENT_CALLBACK(OnCancelClick, OnCancelClick(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnOkClick, OnOkClick(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnGenMethodChanged, OnGenMethodChanged(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnNumericFieldChange, OnNumericFieldChange(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnBaseStyleClicked, OnBaseStyleClicked(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */


bool fieldvaluecomp(const std::string& lhs, const std::string& rhs) {
   double lvalue = StringToNumber<double>(lhs);
   double rvalue = StringToNumber<double>(rhs);
   if (lvalue != 0 || rvalue != 0)
      return lvalue < rvalue;
   return (lhs.compare(rhs) < 0);
}

VectorStyleClassGenerationWidget::VectorStyleClassGenerationWidget(VectorLayer *pVectorLayer) :
   pEventHandler_(new VectorStyleClassGenerationEventHandler(this)),
         pVectorLayer_(pVectorLayer) {
   std::string type =  pVectorLayer_->GetElement()->GetTypeAsString().c_str();
   if (type.compare(TYPE_DECORATOR(message_POINT)) == 0)
      baseStyle_ = SOLID_FILL_POINT_STYLE;
   else if (type.compare(TYPE_DECORATOR(message_LINE)) == 0)
      baseStyle_ = SOLID_FILL_LINE_STYLE;
   else
      baseStyle_ = SOLID_FILL_POLYGON_STYLE;
}

VectorStyleClassGenerationWidget::~VectorStyleClassGenerationWidget() {
   delete pEventHandler_;
}

/** Configuracion inicial de los datos del Part **/
void VectorStyleClassGenerationWidget::SetInitialValues() {
   VectorEditor veditor;
   if (!veditor.OpenVector(pVectorLayer_->GetElement()->GetUrl().c_str()) ||
         !veditor.OpenLayer(
               dynamic_cast<VectorElement*>(pVectorLayer_->GetElement())->GetActiveLayer()))
      return;

   fieldmaxs_.clear();
   fieldmins_.clear();

   for (int f = 0; f < veditor.GetFieldCount(); ++f) {
      std::string fname = veditor.GetFieldName(f);
      std::string ftype = veditor.GetFieldType(f);
      if ((ftype.compare(DataInfo<unsigned char>::Name) != 0) &&
            (IsIntegerType(ftype) || (ftype.compare(DataInfo<double>::Name) == 0))) {
         std::string query ="SELECT MIN([" + fname + "]) MIN, MAX([" + fname + "]) MAX FROM " +
                                                            "\"" + veditor.GetOpenLayerName()+ "\"";
         OGRLayer* player = veditor.CreateSqlLayer(query,NULL,"sqlite");
         if (player) {
            // si pudo generar el query entonces lo agrego como campo valido
            XRCCTRL(*pToolWindow_, "ID_NUMERIC_CHOICE", wxChoice)->Append(fname);
            player->ResetReading();
            OGRFeature* pfeature = player->GetNextFeature();
            int minindex = pfeature->GetFieldIndex("MIN");
            int maxindex = pfeature->GetFieldIndex("MAX");
            if (pfeature) {
               fieldmaxs_.push_back(pfeature->GetFieldAsDouble(maxindex));
               fieldmins_.push_back(pfeature->GetFieldAsDouble(minindex));
            }
         } else {
            REPORT_DEBUG("No es posible realizar la consulta %s", query.c_str());
         }
      }
      XRCCTRL(*pToolWindow_, "ID_FIELD_UNIQUE_VALUE_CHOICE", wxChoice)->Append(fname);
   }
   XRCCTRL(*pToolWindow_, "ID_NUMERIC_CHOICE", wxChoice)->SetSelection(0);
   XRCCTRL(*pToolWindow_, "ID_FIELD_UNIQUE_VALUE_CHOICE", wxChoice)->SetSelection(0);
   XRCCTRL(*pToolWindow_, "ID_FIELD_UNIQUE_VALUE_CHOICE", wxChoice)->Show(false);
   if (!fieldmaxs_.empty())
      XRCCTRL(*pToolWindow_, "ID_MAX_TXT", wxTextCtrl)->
                              SetValue(NumberToString<double>(fieldmaxs_[0]));
   if (!fieldmins_.empty())
      XRCCTRL(*pToolWindow_, "ID_MIN_TXT", wxTextCtrl)->
                              SetValue(NumberToString<double>(fieldmins_[0]));
}

/** Se crea la ventana de herramienta en las clases derivadas */
bool VectorStyleClassGenerationWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_GEN_CLASS_PANEL"));
   if (!pToolWindow_)
      return false;

   GET_CONTROL(*pToolWindow_, "ID_CLASS_GEN_CHOICE", wxChoice)->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(VectorStyleClassGenerationEventHandler::OnGenMethodChanged),
         NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_,
         "ID_BASE_STYLE_BTN", wxButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(VectorStyleClassGenerationEventHandler::OnBaseStyleClicked),
                                                 NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_NUMERIC_CHOICE", wxChoice)->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(VectorStyleClassGenerationEventHandler::OnNumericFieldChange),
         NULL,
         pEventHandler_);
   SetInitialValues();
   return true;
}

/** Metodo que se llama cuando se presiona Cancel **/
void VectorStyleClassGenerationWidget::OnCancelClick(wxCommandEvent& Event) {
   EndModal(wxID_CANCEL);
}

/** Metodo que se llama cuando se presiona Aceptar **/
void VectorStyleClassGenerationWidget::OnOkClick(wxCommandEvent& Event) {
   UpdateClasses();
   EndModal(wxID_OK);
}

/** Metodo auxiliar para el calculo de clases por rango numerico **/
void VectorStyleClassGenerationWidget::GenerateNumericRangeClasses(
      const std::string& FieldName, double Min, double Max, int ClassCount) {
   double range = (Max - Min) / static_cast<double>(ClassCount);
   VectorStyle* pvstyle = VectorStyle::Create(baseStyle_);
   wxColourPickerCtrl* pcolor = XRCCTRL(*pToolWindow_, "ID_INITIAL_COLOR",
                                        wxColourPickerCtrl);
   int r = pcolor->GetColour().Red(), g = pcolor->GetColour().Green(), b =
         pcolor->GetColour().Blue();
   wxColourPickerCtrl* pcolorf = XRCCTRL(*pToolWindow_, "ID_FINAL_COLOR",
                                         wxColourPickerCtrl);
   int rf = pcolorf->GetColour().Red(), gf = pcolorf->GetColour().Green(), bf =
         pcolorf->GetColour().Blue();
   if (pvstyle->GetBrush())
      pvstyle->GetBrush()->color_ = VectorStyle::Color(r, g, b, 255);
   if (pvstyle->GetPen())
      pvstyle->GetPen()->color_ = VectorStyle::Color(r, g, b, 255);
   if (pvstyle->GetSymbol())
      pvstyle->GetSymbol()->color_ = VectorStyle::Color(r, g, b, 255);
   /* Se suma la clase sin clase que tiene como objetivo englobar valores NULL**/
   int rstep = (rf - r) / ClassCount;
   int gstep = (gf - g) / ClassCount;
   int bstep = (bf - b) / ClassCount;

   for (int c = 0; c < ClassCount; ++c, r += rstep, g += gstep, b += bstep) {
      ClassDefinition classdef;
      classdef.name_ = "Clase ";
      classdef.name_.append(NumberToString(c+1));
      if (pvstyle->GetBrush())
         pvstyle->GetBrush()->color_ = VectorStyle::Color(r, g, b, 255);
      if (pvstyle->GetPen())
         pvstyle->GetPen()->color_ = VectorStyle::Color(r, g, b, 255);

      if (pvstyle->GetSymbol())
         pvstyle->GetSymbol()->color_ = VectorStyle::Color(r, g, b, 255);

      classdef.wktStyle_ = pvstyle->GetWkt();
      classdef.type_ = 1;
      classdef.condition_ = "'" + FieldName + "'";
      classdef.condition_.append(">=");
      classdef.condition_.append(DoubleToString(Min + range * static_cast<double>(c)));
      classdef.condition_.append(" AND ");
      classdef.condition_ += "'" + FieldName + "'";
      if (!(c + 1 == ClassCount))
         classdef.condition_.append("<");
      else
         classdef.condition_.append("<=");

      classdef.condition_.append(DoubleToString(Min + range * static_cast<double>((c + 1))));
      classes_.push_back(classdef);
   }
   ClassDefinition classdef;
   if (pvstyle->GetBrush())
      pvstyle->GetBrush()->color_ = VectorStyle::Color(r, g, b, 255);
   else if (pvstyle->GetPen())
      pvstyle->GetPen()->color_ = VectorStyle::Color(r, g, b, 255);
   else
      pvstyle->GetSymbol()->color_ = VectorStyle::Color(r, g, b, 255);
   classdef.wktStyle_ = pvstyle->GetWkt();
   classdef.type_ = 1;
   classdef.name_ = "Sin clase";
   classdef.condition_ = "'" + FieldName + "'" + "=NULL";
   classes_.push_back(classdef);

   VectorStyle::Destroy(pvstyle);
}

/** Metodo auxiliar para el calculo de clases por valores unicos **/
void VectorStyleClassGenerationWidget::GenerateUniqueValueClasses(
      const std::string& FieldName) {
   VectorEditor veditor;
   if (!veditor.OpenVector(pVectorLayer_->GetElement()->GetUrl().c_str())
         || !veditor.OpenLayer(
               dynamic_cast<VectorElement*>(pVectorLayer_->GetElement())->GetActiveLayer()))
      return;
   int fieldpos = veditor.GetFieldPosition(FieldName);
   std::string ftype = veditor.GetFieldType(fieldpos);
   bool isstrtype = ftype.compare(DataInfo<unsigned char>::Name) == 0;
   std::string query = "SELECT DISTINCT '" + FieldName + "' FROM " +
                     "'" + veditor.GetOpenLayerName()+ "'";
   OGRLayer* player = veditor.CreateSqlLayer(query);
   if (!player)
      return;
   player->ResetReading();
   std::set<std::string,
         bool(*)(const std::string&, const std::string&)> fieldvalues(fieldvaluecomp);
   while (OGRFeature* pfeature = player->GetNextFeature()) {
      int findex = pfeature->GetFieldIndex(FieldName.c_str());
      std::string fvalue = pfeature->GetFieldAsString(findex);
      fieldvalues.insert(fvalue);
   }

   if (fieldvalues.empty())
      return;
   VectorStyle* pvstyle = VectorStyle::Create(baseStyle_);
   int r = rand() % 256, g = rand() % 256, b = rand() % 256;
   if (pvstyle->GetBrush())
      pvstyle->GetBrush()->color_ = VectorStyle::Color(r, g, b, 255);
   if (pvstyle->GetPen())
      pvstyle->GetPen()->color_ = VectorStyle::Color(r, g, b, 255);

   if (pvstyle->GetSymbol())
      pvstyle->GetSymbol()->color_ = VectorStyle::Color(r, g, b, 255);


   std::set<std::string, bool(*)(std::string&, std::string&)>::iterator it = fieldvalues.begin();
   /** Se agrega soporta para clase con valores NULL dentro de la escala de colores **/
   std::vector<std::string> unwantedchars(1, "'");
   std::vector<std::string> escapedchars(1, "\\'");
   for (int c = 0; it != fieldvalues.end(); ++it, ++c) {
      ClassDefinition classdef;
      if ((*it).empty())
         continue;
      classdef.name_ = *it;
      r = rand() % 256, g = rand() % 256, b = rand() % 256;
      if (pvstyle->GetBrush())
         pvstyle->GetBrush()->color_ = VectorStyle::Color(r, g, b, 255);
      if (pvstyle->GetPen())
         pvstyle->GetPen()->color_ = VectorStyle::Color(r, g, b, 255);
      if (pvstyle->GetSymbol())
         pvstyle->GetSymbol()->color_ = VectorStyle::Color(r, g, b, 255);
      classdef.wktStyle_ = pvstyle->GetWkt();
      classdef.type_ = 2;
      classdef.condition_ = "'" + FieldName + "'" + "=";
      std::string fvalue = *it;
      ReplaceCharactersFromString(unwantedchars, escapedchars, fvalue);
      classdef.condition_  += isstrtype ? "'" + fvalue + "'" : *it;
      classes_.push_back(classdef);
   }
   ClassDefinition classdef;
   r = rand() % 256, g = rand() % 256, b = rand() % 256;
   if (pvstyle->GetBrush())
      pvstyle->GetBrush()->color_ = VectorStyle::Color(r, g, b, 255);
   if (pvstyle->GetPen())
      pvstyle->GetPen()->color_ = VectorStyle::Color(r, g, b, 255);
   if (pvstyle->GetSymbol())
      pvstyle->GetSymbol()->color_ = VectorStyle::Color(r, g, b, 255);
   classdef.wktStyle_ = pvstyle->GetWkt();
   classdef.name_ = "Sin clase";
   classdef.condition_ = "'" + FieldName + "'" + "=NULL";
   classdef.type_ = 2;
   classes_.push_back(classdef);
}

/** Metodo que se llama cuando se cambia el choice de generacion de clases**/
void VectorStyleClassGenerationWidget::OnGenMethodChanged(wxCommandEvent& Event) {
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, "ID_CLASS_GEN_CHOICE", wxChoice);
   int choicesel = pchoice->GetSelection();
   UpdateClassGenControls(choicesel);
}

/** Obtiene las clases generadas con el widget **/
std::vector<VectorStyleClassGenerationWidget::ClassDefinition>
VectorStyleClassGenerationWidget::GetClasses() {
   classes_.clear();
   UpdateClasses();
   return classes_;
}

/** Metodo auxiliar para la actualizacion de las clases**/
void VectorStyleClassGenerationWidget::UpdateClasses() {
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, "ID_CLASS_GEN_CHOICE", wxChoice);
   int choicesel = pchoice->GetSelection();
   if (choicesel == NUM_RANGE_CHOICE_POS) {
    std::string fieldname =
         XRCCTRL(*pToolWindow_, "ID_NUMERIC_CHOICE", wxChoice)->GetStringSelection().c_str();
     // Generar clase utilizando el rango configurado
      double max = StringToNumber<double>(
            XRCCTRL(*pToolWindow_, "ID_MAX_TXT", wxTextCtrl)->GetValue().c_str());
      double min = StringToNumber<double>(
            XRCCTRL(*pToolWindow_, "ID_MIN_TXT", wxTextCtrl)->GetValue().c_str());
      int classcount =
            XRCCTRL(*pToolWindow_, "ID_CLASS_COUNT_SPINCTRL", wxSpinCtrl)->GetValue();
      if (classcount <= 0) {
         SHOW_ERROR("No es posible generar clases con la informacion indicada");
         return;
      }
      GenerateNumericRangeClasses(fieldname, min, max, classcount);
   } else {
      // Genera clase utilizando los valores unicos de la tabla asociada a la capa vectorial
     std::string fieldname = XRCCTRL(*pToolWindow_, "ID_FIELD_UNIQUE_VALUE_CHOICE",
                                    wxChoice)->GetStringSelection().c_str();
      GenerateUniqueValueClasses(fieldname);
   }
}
/** Metodo que se llama cuando se presiona el boton de definicion de estilo base **/
void VectorStyleClassGenerationWidget::OnBaseStyleClicked(wxCommandEvent& Event) {
   SimpleVectorStylePart* pstyleeditor = new ui::SimpleVectorStylePart(&baseStyle_);
   pstyleeditor->SetVectorStyleWkt(baseStyle_);
   PartContainerWidget* pcontainer = new PartContainerWidget(
         pstyleeditor, _(pstyleeditor->GetWindowTitle()));
   if (pcontainer->ShowModal(true) == wxID_OK) {
      return;
   }
}

/** Metodo que se llama cuando cambia la seleccion del choice de campos para rango numerico **/
void VectorStyleClassGenerationWidget::OnNumericFieldChange(wxCommandEvent& Event) {
   size_t fieldnum = XRCCTRL(*pToolWindow_, "ID_NUMERIC_CHOICE", wxChoice)->GetSelection();

   if (fieldmaxs_.size() > fieldnum) {
      XRCCTRL(*pToolWindow_, "ID_MAX_TXT", wxTextCtrl)->
                                       SetValue(NumberToString<double>(fieldmaxs_[fieldnum]));
   }
   if (fieldmins_.size() > fieldnum) {
      XRCCTRL(*pToolWindow_, "ID_MIN_TXT", wxTextCtrl)->
                                       SetValue(NumberToString<double>(fieldmins_[fieldnum]));
   }
}
/** Configura el color initial **/
void VectorStyleClassGenerationWidget::SetInitialColor(const VectorStyle::Color& InitialColor) {
   int r = InitialColor.red_, g = InitialColor.green_, b = InitialColor.blue_;
   wxColourPickerCtrl* pcolor = XRCCTRL(*pToolWindow_, "ID_INITIAL_COLOR",
                                        wxColourPickerCtrl);
   pcolor->SetColour(wxColor(r, g, b));
}

/** Configura el color final **/
void VectorStyleClassGenerationWidget::SetFinalColor(const VectorStyle::Color& FinalColor) {
   int r = FinalColor.red_, g = FinalColor.green_, b = FinalColor.blue_;
   wxColourPickerCtrl* pcolorf = XRCCTRL(*pToolWindow_, "ID_FINAL_COLOR",
                                         wxColourPickerCtrl);
   pcolorf->SetColour(wxColor(r, g, b));
}

/** Configura el tipo de generacion de clases. 1. Rango, 2. Valores unicos*/
void VectorStyleClassGenerationWidget::SetClassGenerationType(int Type) {
   int choicesel = Type == 1 ? 0 : 1;
   GET_CONTROL(*pToolWindow_, "ID_CLASS_GEN_CHOICE", wxChoice)->SetSelection(choicesel);
   UpdateClassGenControls(choicesel);
}

/** Metodo auxiliar que actualiza los controle min y max dependiendo el tipo de generacion de clases
 *  que se indica
 *  @param[in] ChoicePos posicion para seleccion del choice de generacion de clases 
 **/
void VectorStyleClassGenerationWidget::UpdateClassGenControls(int ChoicePos) {
   XRCCTRL(*pToolWindow_, "ID_MAX_TXT", wxTextCtrl)->Enable(
         ChoicePos == NUM_RANGE_CHOICE_POS);
   XRCCTRL(*pToolWindow_, "ID_MIN_TXT", wxTextCtrl)->Enable(
         ChoicePos == NUM_RANGE_CHOICE_POS);
   XRCCTRL(*pToolWindow_, "ID_CLASS_COUNT_SPINCTRL", wxSpinCtrl)->Enable(
         ChoicePos == NUM_RANGE_CHOICE_POS);
   XRCCTRL(*pToolWindow_, "ID_NUMERIC_CHOICE", wxChoice)->Show(ChoicePos == NUM_RANGE_CHOICE_POS);
   XRCCTRL(*pToolWindow_, "ID_FIELD_UNIQUE_VALUE_CHOICE",
            wxChoice)->Show(ChoicePos != NUM_RANGE_CHOICE_POS);
   XRCCTRL(*pToolWindow_, "ID_INITIAL_COLOR",
            wxColourPickerCtrl)->Enable(ChoicePos == NUM_RANGE_CHOICE_POS);
   XRCCTRL(*pToolWindow_, "ID_FINAL_COLOR",
            wxColourPickerCtrl)->Enable(ChoicePos == NUM_RANGE_CHOICE_POS);
   pToolWindow_->Layout();
}

/** **/
double VectorStyleClassGenerationWidget::GetMinValue() const {
   return StringToNumber<double>(XRCCTRL(*pToolWindow_, "ID_MIN_TXT",
            wxTextCtrl)->GetValue().c_str());
}

double VectorStyleClassGenerationWidget::GetMaxValue() const {
   return StringToNumber<double>(XRCCTRL(*pToolWindow_, "ID_MAX_TXT",
            wxTextCtrl)->GetValue().c_str());
}

void VectorStyleClassGenerationWidget::SetMinValue(double Min) {
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, "ID_CLASS_GEN_CHOICE", wxChoice);
   int choicesel = pchoice->GetSelection();
   if (choicesel == NUM_RANGE_CHOICE_POS)
      XRCCTRL(*pToolWindow_, "ID_MIN_TXT", wxTextCtrl)->SetValue(DoubleToString(Min));
}

void VectorStyleClassGenerationWidget::SetMaxValue(double Max) {
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, "ID_CLASS_GEN_CHOICE", wxChoice);
   int choicesel = pchoice->GetSelection();
   if (choicesel == NUM_RANGE_CHOICE_POS)
      XRCCTRL(*pToolWindow_, "ID_MAX_TXT", wxTextCtrl)->SetValue(DoubleToString(Max));
}

std::string VectorStyleClassGenerationWidget::GetField() const {
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, "ID_CLASS_GEN_CHOICE", wxChoice);
   int choicesel = pchoice->GetSelection();
   std::string fieldname;
   if (choicesel == NUM_RANGE_CHOICE_POS) {
      fieldname =
         XRCCTRL(*pToolWindow_, "ID_NUMERIC_CHOICE", wxChoice)->GetStringSelection().c_str();
   } else {
      fieldname =
         XRCCTRL(*pToolWindow_, "ID_FIELD_UNIQUE_VALUE_CHOICE",
               wxChoice)->GetStringSelection().c_str();
   }
   return fieldname;
}

void VectorStyleClassGenerationWidget::SetField(const std::string& Field) {
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, "ID_CLASS_GEN_CHOICE", wxChoice);
   int choicesel = pchoice->GetSelection();
   if (choicesel == NUM_RANGE_CHOICE_POS) {
      XRCCTRL(*pToolWindow_, "ID_NUMERIC_CHOICE", wxChoice)->SetStringSelection(Field);
   } else {
      XRCCTRL(*pToolWindow_, "ID_FIELD_UNIQUE_VALUE_CHOICE", wxChoice)->SetStringSelection(Field);
   }
}

int VectorStyleClassGenerationWidget::GetClassGenerationType() const {
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, "ID_CLASS_GEN_CHOICE", wxChoice);
   int choicesel = pchoice->GetSelection();
   return (choicesel == NUM_RANGE_CHOICE_POS) ? 1 : 2;
}

} /** namespace ui */
} /** namespace suri */
