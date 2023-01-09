/*! \file SpectralSignEditionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <vector>
#include <string>

// Includes Suri
#include "SpectralSignEditionPart.h"
#include "DefaultTable.h"
#include "suri/LibraryItem.h"
#include "suri/SpectralSignItemAttribute.h"
#include "suri/LibraryItemAttributeFactory.h"
#include "suri/AuxiliaryFunctions.h"
#include "wxGenericTableBase.h"
#include "TablePart.h"
#include "suri/MemoryDriver.h"
#include "LibraryItemEditorPart.h"

// Includes Wx
#include "wx/choice.h"

// Defines

namespace suri {

/** Nombre del XRC que representa la ventana. */
const std::string SpectralSignEditionPart::SPECTRAL_SIGN_EDITION_XRC_NAME = "ID_SS_EDITOR_PANEL";
/** Nombre del XRC que representa el panel del cgu */
const std::string SpectralSignEditionPart::SPECTRAL_SIGN_CGU_XRC_NAME = "ID_SPECTRAL_CGU_PANEL";

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(SpectralSignTableEventHandler, SpectralSignEditionPart)
   IMPLEMENT_EVENT_CALLBACK(OnAddRowClick, OnAddRowClick(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnDeleteRowClick, OnDeleteRowClick(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnChoiseWavelengthClicked, OnChoiseWavelengthClicked(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnChoiseMagnitudeClicked, OnChoiseMagnitudeClicked(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor
 * @param LibraryId id de la biblioteca de firmas espectrales
 * @param pManager manager que administra la biblioteca
 * @param Feature layout de features del cgu
 */
SpectralSignEditionPart::SpectralSignEditionPart(
      const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
      UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature) :
      LibraryTableEditionPart(LibraryId, pManager, SPECTRAL_SIGN_EDITION_XRC_NAME,
                            SPECTRAL_SIGN_CGU_XRC_NAME, Feature),
      NEW_EVENT_OBJECT(SpectralSignTableEventHandler), wavelengthindex_(0), magnitudeindex_(0) {
}

/** Destructor **/
SpectralSignEditionPart::~SpectralSignEditionPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Metodo auxiliar que carga el contenido del item complejo seleccionado en la tabla del Part
 * @param[in] pItem item que sera representado por la tabla
 */
void SpectralSignEditionPart::LoadItemToTable(const LibraryItem* pItem, bool AddRangeToItem) {
   wxChoice* pwavelengthchoice = XRCCTRL(*pToolWindow_, wxT("ID_WAVELENGTH_CHOICE"), wxChoice);
   wxChoice* pmagnitudechoice = XRCCTRL(*pToolWindow_, wxT("ID_MAGNITUDE_CHOICE"), wxChoice);
   if (!pTable_) {
      ConfigureFeatures();
   }
   pTable_->Clear();
   if (pTable_->GetColumns() == 0)
	   AppendColumns();
   LibraryItem::MapAttribute attributes = pItem->GetAttributes();
   LibraryItem::MapAttribute::iterator it = attributes.begin();
   std::vector<LibraryItemAttribute*> pvectorattr;
   for (; it != attributes.end(); ++it) {
      LibraryItemAttribute* pattr = it->second;
      if (pattr->GetName().find(LibraryItemAttributeFactory::SpectralSignString)
            != std::string::npos) {
        const SpectralSignItemAttribute* pcomplex =
               LibraryItemAttributeFactory::CreateSpectralSignItemAttribute(pattr);
         AddValuesToTable(pcomplex);
         std::string type = pcomplex->GetWavelengthType();
         for (unsigned int i = 0; i < pwavelengthchoice->GetCount(); ++i) {
            if (type.compare(pwavelengthchoice->GetString(i).c_str()) == 0) {
               pwavelengthchoice->Select(i);
               break;
            }
         }
         type = pcomplex->GetMagnitudeType();
         for (unsigned int i = 0; i < pmagnitudechoice->GetCount(); ++i) {
            if (type.compare(pmagnitudechoice->GetString(i).c_str()) == 0) {
               pmagnitudechoice->Select(i);
               break;
            }
         }
      }
   }
   pTablePart_->CommitChanges();
}

/**
 * Agrega un item complejo a la tabla de intervalos
 * @param pComplex
 */
void SpectralSignEditionPart::AddValuesToTable(const SpectralSignItemAttribute* pComplex) {
   if (pComplex && pTable_ && pTable_->AppendRow()) {
      int row = pTable_->GetRows() - 1;
      std::string wavelength = NumberToString<double>(pComplex->GetWavelengthValue());
      pTable_->SetCellValue(WAVELENGTH_COL, row, wavelength);
      std::string amplitude = NumberToString<double>(pComplex->GetAmplitudeValue());
      pTable_->SetCellValue(AMPLITUDE_COL, row, amplitude);
      std::string reflectance = NumberToString<double>(pComplex->GetReflectanceValue());
      pTable_->SetCellValue(REFLECTANCE_COL, row, reflectance);
   }
}

/**
 * Obtiene de la tabla los items complejos cargados y crea un item asociado
 * @param pAttributes vector en donde se guardaran los atributos de la tabla
 */
void SpectralSignEditionPart::LoadAttributesFromTable(
      std::vector<LibraryItemAttribute*>* &pAttributes) {
   // Obtengo los rangos de la tabla
   wxChoice* pwavelengthchoice = XRCCTRL(*pToolWindow_, wxT("ID_WAVELENGTH_CHOICE"), wxChoice);
   wxChoice* pmagnitudechoice = XRCCTRL(*pToolWindow_, wxT("ID_MAGNITUDE_CHOICE"), wxChoice);
   for (int row = 0; row < pTable_->GetRows(); ++row) {
      std::string complexattributename = LibraryItemAttributeFactory::SpectralSignString;
      complexattributename.append(NumberToString<int>(row));
      std::string cellvalue;
      std::string attrvalue = ComplexItemAttribute::StartAttributeChar;
      pTable_->GetCellValue(WAVELENGTH_COL, row, cellvalue);
      attrvalue += cellvalue;
      pTable_->GetCellValue(AMPLITUDE_COL, row, cellvalue);
      attrvalue += ComplexItemAttribute::AttributesToken + cellvalue;
      pTable_->GetCellValue(REFLECTANCE_COL, row, cellvalue);
      attrvalue += ComplexItemAttribute::AttributesToken + cellvalue;
      attrvalue += ComplexItemAttribute::AttributesToken
            + pwavelengthchoice->GetString(wavelengthindex_);
      attrvalue += ComplexItemAttribute::AttributesToken
            + pmagnitudechoice->GetString(magnitudeindex_);
      attrvalue += ComplexItemAttribute::FinishAttributeChar;

      pAttributes->push_back(new LibraryItemAttribute(complexattributename, attrvalue));
   }
}

/** Agrega las columnas de la tabla */
void SpectralSignEditionPart::AppendColumns() {
    pTable_->AppendColumn("Centro de banda", Table::STRING);
    pTable_->AppendColumn("Ancho de banda", Table::STRING);
    pTable_->AppendColumn("Valor", Table::STRING);
}

/**
 * Metodo auxiliar privado que configura el part en si para cada editor en particular.
 * Este metodo es el que realmente tienen que implementar las clases hijas para lograr
 * la especializacion a la hora de iniciar el part (metodo que se llama en
 * CreateToolWindow, a modo de Template Method).
 */
bool SpectralSignEditionPart::ConfigureFeatures() {
   if (!pTable_) {
      wxChoice* pwavelengthchoice = XRCCTRL(*pToolWindow_, wxT("ID_WAVELENGTH_CHOICE"), wxChoice);
      wxChoice* pmagnitudechoice = XRCCTRL(*pToolWindow_, wxT("ID_MAGNITUDE_CHOICE"), wxChoice);
      wxBitmapButton* paddbutton = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                           wxT("ID_ADDROW_BUTTON"), wxBitmapButton);
      wxBitmapButton* pdelbutton = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                           wxT("ID_DELROW_BUTTON"), wxBitmapButton);

      paddbutton->Connect(
            wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(SpectralSignTableEventHandler::OnAddRowClick), NULL,
            pEventHandler_);

      pdelbutton->Connect(
            wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(SpectralSignTableEventHandler::OnDeleteRowClick),
            NULL, pEventHandler_);

      pwavelengthchoice->Connect(
            wxEVT_COMMAND_CHOICE_SELECTED,
            wxCommandEventHandler(
                  SpectralSignTableEventHandler::OnChoiseWavelengthClicked),
            NULL, pEventHandler_);

      pmagnitudechoice->Connect(
            wxEVT_COMMAND_CHOICE_SELECTED,
            wxCommandEventHandler(
                  SpectralSignTableEventHandler::OnChoiseMagnitudeClicked),
            NULL, pEventHandler_);

      ConfigureChoices();
      pTable_ = new DefaultTable();
      pTable_->SetDriver(new MemoryDriver);
      AppendColumns();
      pTablePart_ = new TablePart(pTable_, false, false, NULL, false);
      AddControl(pTablePart_->GetWidget(), "ID_TABLE_PANEL");
   }
   return true;
}

/** Metodo que se llama cuando se presiona el boton de agregar columna */
void SpectralSignEditionPart::OnAddRowClick(wxCommandEvent& Event) {
   pTable_->AppendRow();
}

/** Metodo que se llama cuando se presiona el boton de eliminar columna */
void SpectralSignEditionPart::OnDeleteRowClick(wxCommandEvent& Event) {
   wxArrayInt rows = pTablePart_->GetSelectedRows();
   size_t count = rows.GetCount();
   for (size_t i = 0; i < count; i++) {
      pTable_->DeleteRow(rows.Item(i));
   }
}

/** Metodo auxiliar privado que configura las opciones disponibles en los choices */
void SpectralSignEditionPart::ConfigureChoices() {
   wxChoice* pwavelengthchoice = XRCCTRL(*pToolWindow_, wxT("ID_WAVELENGTH_CHOICE"), wxChoice);
   wxArrayString wavelengthtypes;
   wavelengthtypes.Add(wxT("Nanometros"));
   wavelengthtypes.Add(wxT("Micrometros"));
   wavelengthtypes.Add(wxT("Angstrom"));
   pwavelengthchoice->Append(wavelengthtypes);
   pwavelengthchoice->Select(wavelengthindex_);
   wxChoice* pmagnitudechoice = XRCCTRL(*pToolWindow_, wxT("ID_MAGNITUDE_CHOICE"), wxChoice);
   wxArrayString magnitudetypes;
   magnitudetypes.Add(wxT("Unitaria"));
   magnitudetypes.Add(wxT("Reflectancia 0-1000"));
   pmagnitudechoice->Append(magnitudetypes);
   pmagnitudechoice->Select(magnitudeindex_);
}

/** Metodo que se llama cuando se presiona el choice de longitud de onda */
void SpectralSignEditionPart::OnChoiseWavelengthClicked(wxCommandEvent& Event) {
   wxChoice* pwavelengthchoice = XRCCTRL(*pToolWindow_, wxT("ID_WAVELENGTH_CHOICE"), wxChoice);
   wavelengthindex_ = pwavelengthchoice->GetSelection();
}

/** Metodo que se llama cuando se presiona el choice de longitud de magnitud */
void SpectralSignEditionPart::OnChoiseMagnitudeClicked(wxCommandEvent& Event) {
   wxChoice* pmagnitudechoice = XRCCTRL(*pToolWindow_, wxT("ID_MAGNITUDE_CHOICE"), wxChoice);
   magnitudeindex_ = pmagnitudechoice->GetSelection();
}
} /** namespace suri **/
