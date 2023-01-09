/*! \file DatumEditionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>
#include <vector>

// Includes Suri
#include "DatumEditionPart.h"
#include "UniversalGraphicalComponentPartInterface.h"
#include "suri/LibraryManager.h"
#include "SpatialReference.h"
#include "suri/AuxiliaryFunctions.h"
#include "GdalSrsItemOrigin.h"
#include "UniversalGraphicalComponentPart.h"
#include "suri/LibraryManagerFactory.h"
#include "TablePart.h"
#include "suri/Table.h"
#include "DefaultTable.h"
#include "TableTool.h"

// Includes Wx

// Defines
#define ID_BURSA_WOLF_GRID_PANEL "ID_BURSA_WOLF_GRID_PANEL"

/** namespace suri */
namespace suri {

const std::string DatumEditionPart::DATUM_EDITION_XRC_NAME =
      wxT("ID_DATUM_EDITION_PANEL");

const std::string DatumEditionPart::DATUM_EDITION_CGU_XRC_NAME =
      wxT("ID_DATUM_NAME_PANEL");

const std::string DatumEditionPart::ELLIPSOID_EDITION_CGU_XRC_NAME =
      wxT("ID_ELLIPSOID_SELECT_PANEL");

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(DatumParametersTableEventHandler, DatumEditionPart)
   IMPLEMENT_EVENT_CALLBACK(OnAddRowClick, OnAddRowClick(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnDelRowClick, OnDelRowClick(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */


DatumEditionPart::DatumEditionPart(
      const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
      UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature) :
      LibraryItemEditorPart(LibraryId, pManager, DATUM_EDITION_XRC_NAME,
                            DATUM_EDITION_CGU_XRC_NAME, Feature),
      pManager_(pManager), pSpheroidCGgu_(NULL), pTable_(NULL), pTablePart_(NULL),
      NEW_EVENT_OBJECT(DatumParametersTableEventHandler),
      pItemCurrentSel_(NULL) {
}

DatumEditionPart::~DatumEditionPart() {
   DELETE_EVENT_OBJECT;
}

/** Indica si el Part tiene cambios para salvar. */
bool DatumEditionPart::HasChanged() {
   bool textchanged = XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_NAME_TEXT"), wxTextCtrl)->IsModified()
         || XRCCTRL(*(this->GetWidget()->GetWindow()),
               wxT("ID_AUTHORITY_CODE_TEXT"), wxTextCtrl)->IsModified();
   bool spheroidchanged = pSpheroidCGgu_ ? pSpheroidCGgu_->HasChanged() : false;
   bool tablechanged = pTablePart_ ? pTablePart_->HasChanged() : false;
   return textchanged || spheroidchanged || tablechanged;
}

/** Inicializa el part */
void DatumEditionPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void DatumEditionPart::Update() {
}

/**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
 *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
 *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
 *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
 *   @return vector con atributos
 *   @return NULL en caso de que la informnacion del part no sea valida
 *   **/
std::vector<LibraryItemAttribute*>* DatumEditionPart::CreateAttributesFromFields() {
   const LibraryItem* pitem = pSpheroidCGgu_->GetSelectedItem();
   std::string spheroidwkt;
   if (pitem) {
      const LibraryItemAttribute* pattr = pitem->GetAttribute(
            GdalSrsItemOrigin::WKT_ATTRIBUTE_NAME);
      spheroidwkt = pattr ? pattr->GetValue() : "";
   }
   std::string name = pCgu_->GetSelectedItem()->GetPrincipalAttribute()->GetValue();
   std::string authority = XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_NAME_TEXT"), wxTextCtrl)->GetValue().c_str();
   std::string code = XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_CODE_TEXT"), wxTextCtrl)->GetValue().c_str();
   std::string paramswkt = GetParametersWkt();
   std::string wkt = "DATUM[\"" + name + "\"" + ",";
   if (!spheroidwkt.empty())
      wkt += trim(spheroidwkt, "\r\n") + ",";
   if (!paramswkt.empty())
      wkt += paramswkt + ",";

   wkt += "AUTHORITY[\"" + authority + "\"," + "\"" + code + "\"]]";
   std::vector<LibraryItemAttribute*>* pattrs = new std::vector<LibraryItemAttribute*>;
   pattrs->push_back(
         new LibraryItemAttribute(GdalSrsItemOrigin::WKT_ATTRIBUTE_NAME, wkt));
   XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_NAME_TEXT"), wxTextCtrl)->SetModified(false);
   XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_CODE_TEXT"), wxTextCtrl)->SetModified(false);
   return pattrs;
}

/**
 *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
 *  Este metodo es el que realmente tienen que implementar las clases hijas para
 * lograr la especializacion a la hora de iniciar el part (metodo que se llama
 * en CreateToolWindow, a modo de Template Method).
 **/
bool DatumEditionPart::ConfigureFeatures() {
   const Library* plib  =
         editorClient_.GetLibraryByCode(LibraryManagerFactory::EllipsoidLibraryCode);
   if (plib) {
      pSpheroidCGgu_ = new UniversalGraphicalComponentPart(
            plib->GetId(), pManager_, NULL,
            UniversalGraphicalComponentPartInterface::ExtendedRwWithFFAndNoLabel);
      AddControl(pSpheroidCGgu_->GetWidget(), wxT(ELLIPSOID_EDITION_CGU_XRC_NAME.c_str()));
   }
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_ADD_ROW_BTN", wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(DatumParametersTableEventHandler::OnAddRowClick), NULL,
         pEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_DEL_ROW_BTN", wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(DatumParametersTableEventHandler::OnDelRowClick), NULL,
         pEventHandler_);
   return true;
}

/**
 * Metodo auxiliar privado que es necesario implementar en las clases hijas.
 * Este metodo es el encargado de actualizar realmente la informacion en el part
 * Es llamado en el metodo SetActiveItem a modo de Template Method
 **/
void DatumEditionPart::UpdateFields(const LibraryItem* pItem, bool Modifiable) {
   if (pItem && pItemCurrentSel_ && pItem->GetId().compare(pItemCurrentSel_->GetId()) == 0)
      return;
   const LibraryItemAttribute* pattr =
         pItem ? pItem->GetAttribute(GdalSrsItemOrigin::WKT_ATTRIBUTE_NAME) : NULL;
   std::string srwkt = pattr ? pattr->GetValue() : "";
   std::string authorityid = SpatialReference::GetAuthorityId(srwkt);
   std::string spheroidwkt = SpatialReference::GetDatumSpheroidWkt(srwkt);
   std::vector<std::string> authority = tokenizer(authorityid, ":");
   std::string aut, code;
   if (authority.size() == 2) {
      aut = authority[0].c_str();
      code = authority[1].c_str();
   }
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_NAME_TEXT"), wxTextCtrl)->SetValue(aut);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_CODE_TEXT"), wxTextCtrl)->SetValue(code);
   if (pSpheroidCGgu_) {
      if (Modifiable)
         pSpheroidCGgu_->Enable();
      else
         pSpheroidCGgu_->Disable();
      pSpheroidCGgu_->SelectItemByPrincipal(
            SpatialReference::GetAuthorityId(spheroidwkt));
   }
   ConfigureBursaWolfParametersTable(srwkt, Modifiable);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT(ID_BURSA_WOLF_GRID_PANEL), wxPanel)->Enable(Modifiable);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_CODE_TEXT"), wxTextCtrl)->Enable(Modifiable);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_NAME_TEXT"), wxTextCtrl)->Enable(Modifiable);
   pItemCurrentSel_ = pItem;
}

/** Configura la tabla para mostrar los parametros bursa wolf para conversion
 *  del datum a wgs84
 *  @param[in] Wkt wkt del datum
 */
void DatumEditionPart::ConfigureBursaWolfParametersTable(const std::string& Wkt,
                                                         bool Enable) {
   /** ejemplo de nodo wkt de parametros
    *  TOWGS84[-637,-549,-203,0,0,0,0]**/
   std::string parameterswkt = SpatialReference::GetBursaWolfDatumParameters(Wkt);
   if (pTable_) {
      delete pTable_;
      pTable_ = NULL;
   }
   if (pTablePart_) {
      RemoveControl(pTablePart_);
      pTablePart_ = NULL;
   }
   if (Enable) {
      pTable_ = new suri::DefaultTable();
      pTable_->AppendColumn("Parametros", Table::STRING);
      if (!parameterswkt.empty()) {
         std::vector<std::string> params = tokenizer(
               trim(parameterswkt.substr(sizeof("TOWGS84[")), "]"), ",");
         std::vector<std::string>::iterator it = params.begin();
         for (int r = 0; it != params.end(); ++it, ++r) {
            pTable_->AppendRow();
            pTable_->SetCellValue(0, r, *it);
         }
      }
      pTablePart_ = new TablePart(pTable_);
      TableTool* ptool = new TableTool(NULL, pTablePart_);
      pTablePart_->SetTableTool(ptool);
      pTablePart_->SetSelectionNotifier(NULL);
      ptool->SetTablePart(pTablePart_);
      if (Enable)
         pTablePart_->Enable();
      else
         pTablePart_->Disable();
      AddControl(pTablePart_, ID_BURSA_WOLF_GRID_PANEL);
   }
}

/** Obtiene el wkt en funcion de los parametros definidos en la tabla **/
std::string DatumEditionPart::GetParametersWkt() {
   std::string paramswkt = "TOWGS84[";
   std::string rowsvals;

   for (int r = 0, rows = pTable_->GetRows(); r < rows; ++r) {
      if (rowsvals.length() > 0) {
         rowsvals.append(",");
      }
      std::string param;
      pTable_->GetCellValue(0, r, param);
      rowsvals.append(param);
   }
   paramswkt.append(rowsvals);
   paramswkt.append("]");

   return paramswkt;
}

/** Metodo auxiliar que se llama cuando se presiona el boton de agregar fila **/
void DatumEditionPart::OnAddRowClick(const wxCommandEvent& Event) {
   if (pTable_)
      pTable_->AppendRow();
}

/** Metodo auxiliar que se llama cuando se presiona el boton de eliminar fila **/
void DatumEditionPart::OnDelRowClick(const wxCommandEvent& Event) {
   if (pTablePart_ && pTable_) {
      wxArrayInt srows = pTablePart_->GetSelectedRows();
      for (unsigned int r = 0; r < srows.size(); ++r) {
         pTable_->DeleteRow(srows[r]);
      }
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
bool DatumEditionPart::OnRollback() {
   XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_NAME_TEXT"), wxTextCtrl)->SetModified(false);
   XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_AUTHORITY_CODE_TEXT"), wxTextCtrl)->SetModified(false);
   if (pSpheroidCGgu_) {
      return pSpheroidCGgu_->RollbackChanges();
   }
   return true;
}
}/** namespace suri */
