/*! \file BandMathPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard

#include <assert.h>
#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// Includes Suri
#include "BandMathPart.h"
#include "EquationParser.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/BandMathProcess.h"
#include "suri/Configuration.h"
#include "suri/DataTypes.h"
#include "suri/Part.h"
#include "suri/ProcessAdaptLayer.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/Subject.h"
#include "BandInfo.h"
#include "UniversalLibraryNameEditionPart.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/EquationVariableMappingPart.h"
#include "resources.h"

// Includes wx
#include "wx/grid.h"

// Defines
#define VARIABLECOLUMNPOSITION 0
#define BANDCOLUMNPOSITION 1
#define FIRSTBANDNAMENUMBER 1
#define DEFAULTBANDNAME "b"

/** Genera un part que permite editar las opciones de algebra de bandas */
/** Namespace suri */
namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(BandMathPartEvent, BandMathPart)
   IMPLEMENT_EVENT_CALLBACK(OnLibraryCheckboxClicked, OnLibraryCheckboxClicked(event),
                            wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Id del panel
 */
const std::string BandMathPart::BAND_MATH_XRC_NAME = wxT("ID_BAND_MATH_PANEL");

/**
 * Constructor
 */
BandMathPart::BandMathPart(const std::vector<std::string> &BandNames, LibraryManager* pManager) :
      widgetSelectedDataType_(0), selectedDataType_(0), NEW_EVENT_OBJECT(BandMathPartEvent),
      pBandMappingPart_(new EquationVariableMappingPart(BandNames,
                           std::vector< std::pair<std::string, std::string> >())) {
   windowTitle_ = _(caption_BAND_MATH_PART);
   LibraryClient client(pManager, LibraryManager::READONLY);
   const Library* pLib = client.GetLibraryByCode(LibraryManagerFactory::EquationLibraryCode);
   pItemSelector_ = new BasicLibraryItemSelector(
         this, pLib->GetId(), pManager, "ID_LIBRARY_SELECTION_PANEL", "ID_LIBRARY_SELECTION_PANEL",
         true, UniversalGraphicalComponentPartInterface::ExtendedROWithFF);

}

/**
 * Destructor
 */
BandMathPart::~BandMathPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Indica si el Part tiene cambios para salvar
 */
bool BandMathPart::HasChanged() {
   return pBandMappingPart_->HasChanged() || widgetSelectedDataType_ == selectedDataType_;
}

/**
 * Actualiza la ecuacion, bandas seleccionadas y tipo de dato de salida.
 * @return bool que indica si tuvo exito
 */
bool BandMathPart::CommitChanges() {
   if (!pBandMappingPart_->CommitChanges())
      return false;
   selectedDataType_ = widgetSelectedDataType_;
   // Actualizo la gui por si se eliminaron variables en la tabla
   UpdateParametersToWidget();
   std::map<std::string, int> variables;
   GetVariableBands(variables);
   pPal_->AddAttribute<std::map<std::string, int> >(
         BandMathProcess::EquationVariablesKey, variables);
   std::string datatype = GetOutputDataType();
   pPal_->AddAttribute<std::string>(BandMathProcess::DataTypeKeyAttr, datatype);
   std::string eq = GetEquation();
   pPal_->AddAttribute<std::string>(BandMathProcess::EquationKey, eq);
   return true;
}

/** Se crea la ventana de herramienta en las clases derivadas */
bool BandMathPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_, BandMathPart::BAND_MATH_XRC_NAME);
   if (!pToolWindow_)
      return false;
   
   AddControl(pBandMappingPart_->GetWidget(), "ID_EQUATION_MAPPING_PANEL");
   if (pItemSelector_)
      AddControl(pItemSelector_->GetWidget(), "ID_EQUATIONS_LIBRARY_PANEL");
   SetInitialValues();
   return true;
}

/**
 * Restaura los valores originales del Part.
 * @return bool que indica si ocurrio un error.
 */
bool BandMathPart::RollbackChanges() {
   widgetSelectedDataType_ = selectedDataType_;
   UpdateParametersToWidget();
   return true;
}

/** Inicializa el Part */
void BandMathPart::SetInitialValues() {
   ConfigureDataTypeChoice();
   SetInitialOutputDataType();

   RollbackChanges();

   GET_CONTROL(*(this->GetWidget()->GetWindow()), wxT("ID_EQUATIONS_LIBRARY_PANEL"),
               wxPanel)->Enable(false);
   GET_CONTROL(*(this->GetWidget()->GetWindow()), "ID_LIBRARY_CHECKBOX", wxCheckBox)->Connect(
         wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(
               BandMathPartEvent::OnLibraryCheckboxClicked), NULL, pEventHandler_);
}

/**
 * Analiza si la parte tiene datos validos . En caso de encontrar un error
 * actualiza errorMessage_ con msg de error.
 * @return bool que indica si los datos son validos
 */
bool BandMathPart::HasValidData() {
   return pBandMappingPart_->HasValidData();
}

/** Actualiza el estado del Part */
void BandMathPart::Update() {
}

/**
 * Retorna la ecuacion seleccionada por el usuario
 * @return string con ecuacion
 */
std::string BandMathPart::GetEquation() {
   return pBandMappingPart_->GetEquation();
}

/**
 * Retorna las bandas asignadas a cada variable
 * param[out] VariableBands mapa variable->banda
 */
void BandMathPart::GetVariableBands(std::map<std::string, int> &VariableBands) {
   if (pBandMappingPart_)
      VariableBands = pBandMappingPart_->GetVariableBands();
}

/**
 * Retorna el tipo de dato de la salida
 * @return tipo de dato a la salida.
 */
std::string BandMathPart::GetOutputDataType() {
   return dataTypeNames_[selectedDataType_];
}

/**
 * Actualiza los atributos widgetEquation_, widgetVariableBands_ y
 * widgetSelectedDataType_ con datos en control grafico.
 * @return bool que indica si ocurrio un error al leer los datos.
 */
bool BandMathPart::ReadParametersFromWidget() {
   // Nota: A diferencia de Update, solo se leen los valores asignados a las variables.
   // El nombre/cantidad de variables solo se actualiza con el boton 'Validar ecuacion' o en commit.
   widgetSelectedDataType_ = USE_CONTROL(*GetWidget()->GetWindow(),
         wxT("ID_DATATYPE_CHOICE"), wxChoice, GetCurrentSelection(), -1);
   return true;
}

/**
 * Actualiza el control grafico con datos en los atributos widgetEquation_,
 * widgetVariableBands_ y widgetSelectedDataType_
 * @return bool que indica si ocurrio un error al actualizar los datos.
 */
bool BandMathPart::UpdateParametersToWidget() {
   GET_CONTROL(*GetWidget()->GetWindow(), wxT("ID_DATATYPE_CHOICE"),
         wxChoice)->SetSelection(widgetSelectedDataType_);

   return true;
}

/**
 * Configura los tipos de dato que puede tener la imagen de salida
 * El tamanio del tipo de dato se obtiene usando DataInfo<unsigned char>::Size
 * por si modifica para algun compilador/arquitectura.
 */
void BandMathPart::ConfigureDataTypeChoice() {
   wxArrayString pdatatypes;

   dataTypeNames_.push_back(DataInfo<unsigned char>::Name);
   pdatatypes.Add(
         wxString::Format(_(content_UNSIGNEDINTEGEROF_d), DataInfo<unsigned char>::Size * 8));

   dataTypeNames_.push_back(DataInfo<unsigned short>::Name);
   pdatatypes.Add(
         wxString::Format(_(content_UNSIGNEDINTEGEROF_d), DataInfo<unsigned short>::Size * 8));

   dataTypeNames_.push_back(DataInfo<unsigned int>::Name);
   pdatatypes.Add(
         wxString::Format(_(content_UNSIGNEDINTEGEROF_d), DataInfo<unsigned int>::Size * 8));

   dataTypeNames_.push_back(DataInfo<short>::Name);
   pdatatypes.Add(wxString::Format(_(content_INTEGEROF_d), DataInfo<short>::Size * 8));

   dataTypeNames_.push_back(DataInfo<int>::Name);
   pdatatypes.Add(wxString::Format(_(content_INTEGEROF_d), DataInfo<int>::Size * 8));

   dataTypeNames_.push_back(DataInfo<float>::Name);
   pdatatypes.Add(wxString::Format(_(content_FLOATOF_d), DataInfo<float>::Size * 8));

   dataTypeNames_.push_back(DataInfo<double>::Name);
   pdatatypes.Add(wxString::Format(_(content_FLOATOF_d), DataInfo<double>::Size * 8));

   GET_CONTROL(*GetWidget()->GetWindow(), wxT("ID_DATATYPE_CHOICE"),
         wxChoice)->Clear();
   GET_CONTROL(*GetWidget()->GetWindow(), wxT("ID_DATATYPE_CHOICE"),
         wxChoice)->Append(pdatatypes);
}

/**
 * Configura el tipo de dato de salida default
 */
void BandMathPart::SetInitialOutputDataType() {
   std::string initialdatatype = suri::Configuration::GetParameter("tool_band_math",
                                                                   DataInfo<float>::Name);
   for (size_t i = 0; i < dataTypeNames_.size(); i++) {
      if (dataTypeNames_[i] == initialdatatype) {
         selectedDataType_ = i;
         break;
      }
   }
}

/** Evento que se ejecuta al hacer click sobre checkbox utilizar biblioteca */
void BandMathPart::OnLibraryCheckboxClicked(wxCommandEvent &Event) {
   bool ischecked = Event.IsChecked();
   if (ischecked) {
      GET_CONTROL(*(this->GetWidget()->GetWindow()), wxT("ID_EQUATIONS_LIBRARY_PANEL"),
                     wxPanel)->Enable(ischecked);
      SetActiveItem(pItemSelector_->GetActiveItem());
   } else {
      GET_CONTROL(*(this->GetWidget()->GetWindow()), wxT("ID_EQUATIONS_LIBRARY_PANEL"),
                     wxPanel)->Enable(ischecked);
   }
}

/**
 * Configura el editor con el item que se pasa por parametro. Se pasa por parametro
 * un boolean que indica si se trata de la primera seleccion (para que el part no piense
 * que posee cambios cuando en realidad es la seleccion inicial del CGU)
 */
void BandMathPart::SetActiveItem(const LibraryItem* pItem) {
   if (pBandMappingPart_)
      pBandMappingPart_->SetEquation(pItem->GetAttribute("EQ")->GetValue());
}

/*!
 *  Retorna un bitmap con el icono de la ventana.
 * Si se pasa solo X o solo Y el otro parametro tomara un valor que conserve
 * la relacion entre lados con la imagen.
 * Al final de ser necesario hace Rescale.
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 */
void  BandMathPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
    GET_BITMAP_RESOURCE(tool_BAND_MATH, ToolBitmap); 
}

} /** namespace suri */
