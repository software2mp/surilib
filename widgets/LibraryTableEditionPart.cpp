/*! \file LibraryTableEditionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "suri/PartCollection.h"
#include "LibraryTableEditionPart.h"
#include "DefaultTable.h"
#include "suri/LibraryItem.h"
#include "suri/LibraryItemAttributeFactory.h"
#include "wxGenericTableBase.h"
#include "TablePart.h"
#include "suri/MemoryDriver.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(LibraryTableEventHandler, LibraryTableEditionPart)
            IMPLEMENT_EVENT_CALLBACK(OnAddRowClick, OnAddRowClick(event), wxCommandEvent)
            IMPLEMENT_EVENT_CALLBACK(OnDeleteRowClick, OnDeleteRowClick(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

/** Constructor
* @param LibraryId id de la biblioteca de firmas espectrales
* @param pManager manager que administra la biblioteca
* @param Feature layout de features del cgu
*/
LibraryTableEditionPart::LibraryTableEditionPart(
      const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
      const std::string EditionXrcName, const std::string CguPanel,
      UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature) :
      LibraryItemEditorPart(LibraryId, pManager, EditionXrcName, CguPanel, false, Feature),
      NEW_EVENT_OBJECT(LibraryTableEventHandler), pTable_(NULL),
      pTablePart_(NULL), pCurrentItem_(NULL) {
}

/** Destructor */
LibraryTableEditionPart::~LibraryTableEditionPart() {
   if (pTable_)
      delete pTable_;
   pTable_ = NULL;
   pTablePart_ = NULL;
}

/**
 * Metodo auxiliar privado que configura el part en si para cada editor en particular.
 * Este metodo es el que realmente tienen que implementar las clases hijas para lograr
 * la especializacion a la hora de iniciar el part (metodo que se llama en
 * CreateToolWindow, a modo de Template Method).
 */
bool LibraryTableEditionPart::ConfigureFeatures() {
   if (!pTable_) {
      wxBitmapButton* paddbutton = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                           wxT("ID_ADDROW_BUTTON"), wxBitmapButton);
      wxBitmapButton* pdelbutton = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                           wxT("ID_DELROW_BUTTON"), wxBitmapButton);
      paddbutton->Connect(
            wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(LibraryTableEventHandler::OnAddRowClick), NULL,
            pEventHandler_);
      pdelbutton->Connect(
            wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(LibraryTableEventHandler::OnDeleteRowClick), NULL,
            pEventHandler_);

      pTable_ = new DefaultTable();
      pTable_->SetDriver(new MemoryDriver);
      AppendColumns();
      pTablePart_ = new TablePart(pTable_, false, false, NULL, false);
      AddControl(pTablePart_->GetWidget(), "ID_SLICE_TABLE_PANEL");
   }
   return true;
}

/**
 * Metodo auxiliar privado que agrega comportamiento adicional al commit de la
 * clase base. Este comportamiento adicional es necesario cuando
 * el commit involucra objetos adicionales a los items de biblioteca
 * del editor (ej. reproyeccion, en donde se necesita actualizar los params
 * de reproyeccion ademas de leer el item de biblioteca)
 * @return true en caso de realizar correctamente el commit "interno"
 * @return false en caso de que ocurra algun error durante el commit
 */
bool LibraryTableEditionPart::OnCommitChanges() {
   pTablePart_->CommitChanges();
   return true;
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
bool LibraryTableEditionPart::OnRollback() {
   if (pCurrentItem_) {
      LoadItemToTable(pCurrentItem_);
   }
   return true;
}

/**
 *   Metodo que obtiene a partir de los datos del editor, un vector con los atributos
 *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
 *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
 *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
 *   @return vector con atributos
 *   @return NULL en caso de que la informnacion del part no sea valida
 *   **/
std::vector<LibraryItemAttribute*>* LibraryTableEditionPart::CreateAttributesFromFields() {
   if (!pTable_)
      return NULL;
   std::vector<LibraryItemAttribute*>* pattributes = new std::vector<
         LibraryItemAttribute*>;
   LoadAttributesFromTable(pattributes);
   return pattributes;
}

/** Indica si el Part tiene cambios para salvar */
bool LibraryTableEditionPart::HasChanged() {
   return pTablePart_->HasChanged();
}

/** Metodo que se llama cuando se presiona el boton de agregar columna*/
void LibraryTableEditionPart::OnAddRowClick(wxCommandEvent& Event) {
   pTable_->AppendRow();
}

/** Metodo que se llama cuando se presiona el boton de eliminar columna*/
void LibraryTableEditionPart::OnDeleteRowClick(wxCommandEvent& Event) {
   wxArrayInt rows = pTablePart_ ->GetSelectedRows();
   size_t count = rows.GetCount();
   for (size_t i = 0; i < count; i++) {
      pTable_->DeleteRow(rows.Item(i));
   }
}

/**
 * Metodo auxiliar privado que es necesario implementar en las clases hijas.
 * Este metodo es el encargado de actualizar realmente la informacion en el part
 * Es llamado en el metodo SetActiveItem a modo de Template Method
 **/
void LibraryTableEditionPart::UpdateFields(const LibraryItem* pItem, bool Modifiable) {
   pCurrentItem_ = pItem;
   if (pItem) {
      LoadItemToTable(pItem);
      if (pTablePart_)
         pTablePart_->SetEnabled(Modifiable);
   }
}
} /** namespace suri */
