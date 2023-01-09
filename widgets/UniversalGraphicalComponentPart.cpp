/*! \file UniversalGraphicalComponentPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "UniversalGraphicalComponentPart.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Part.h"
#include "suri/LibraryItem.h"
#include "suri/LibraryItemAttribute.h"
#include "LibraryItemEditorNotificator.h"
#include "suri/LibraryItemOrigin.h"
#include "suri/FullFledgeLibraryEditorWidget.h"

// Includes Wx
#include "wx/checkbox.h"
#include "wx/combobox.h"
// Defines
#define MAX_COMBO_TEXT_LENGTH 60
#define ATTRIBUTE_SEPARATOR " - "

// forwards

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(SaveEvent, UniversalGraphicalComponentPart)
      IMPLEMENT_EVENT_CALLBACK(OnSaveButtonEventHandler,
                               OnSaveButtonEventHandler(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(DeleteEvent, UniversalGraphicalComponentPart)
   IMPLEMENT_EVENT_CALLBACK(OnDeleteButtonEventHandler,
                            OnDeleteButtonEventHandler(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(CallFullFledgedLibraryEvent, UniversalGraphicalComponentPart)
   IMPLEMENT_EVENT_CALLBACK(OnCallFullFledgedLibraryButtonEventHandler,
                         OnCallFullFledgedLibraryButtonEventHandler(event),
                         wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(MarkFavoriteEvent, UniversalGraphicalComponentPart)
   IMPLEMENT_EVENT_CALLBACK(OnMarkFavoriteEventHandler,
                            OnMarkFavoriteEventHandler(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ComboBoxSelectionEvent, UniversalGraphicalComponentPart)
   IMPLEMENT_EVENT_CALLBACK(OnComboBoxSelectionEventHandler,
                            OnComboBoxSelectionEventHandler(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnComboBoxTextChanged,
                            OnComboBoxTextChanged(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor
 * @param LibraryId Id de la biblioteca
 * @param pManager puntero al administrador de bibliotecas
 * @param FeatureCombination Combinacion de caracteristicas habilitadas
 * @param FavoriteFilter bool que indica si se desea mostrar solamente los favoritos
 * item en el CGU
 * @param IncludeNameLabel
 */
UniversalGraphicalComponentPart::UniversalGraphicalComponentPart(
      const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
      LibraryItemEditorNotificator* pNotificator,
      FeatureCombinationEnum FeatureCombination,
      bool FavoriteFilter) :
      pSaveEventHandler_(new SaveEvent(this)),
      pDeleteEventHandler_(new DeleteEvent(this)),
      pCallFullFledgedLibraryEventHandler_(new CallFullFledgedLibraryEvent(this)),
      pMarkFavoriteEventHandler_(new MarkFavoriteEvent(this)),
      pComboBoxEventHandler_(new ComboBoxSelectionEvent(this)),
      featureCombinationActive_(FeatureCombination), libraryId_(LibraryId),
      cguClient_(pManager, LibraryManager::ALL),
      lastCheckState_(false), favoriteFilter_(FavoriteFilter), comboTextChanged_(false),
      pNotificator_(pNotificator), pItemSelected_(NULL), pCombo_(NULL),
      comboPositionSelected_(-1), pManager_(pManager), itemFavoriteChecked_(false) {
   if (pNotificator_)
      pNotificator_->SetUniversalGraphicalComponentPart(this);
}

/**
 * Destructor
 */
UniversalGraphicalComponentPart::~UniversalGraphicalComponentPart() {
   delete_all< std::vector<LibraryItemCatalog*> >(catalog_);
   delete pSaveEventHandler_;
   delete  pDeleteEventHandler_;
   delete  pCallFullFledgedLibraryEventHandler_;
   delete  pMarkFavoriteEventHandler_;
}

/** Indica si el Part tiene cambios para salvar */
bool UniversalGraphicalComponentPart::HasChanged() {
   wxCheckBox* pfavoritecheck = XRCCTRL(*(this->GetWidget()->GetWindow()),
         wxT("ID_SHOW_FAVORITES_CHECK"), wxCheckBox);
   return pfavoritecheck->IsChecked() != itemFavoriteChecked_;
}

/** Salva los cambios realizados en el Part */
bool UniversalGraphicalComponentPart::CommitChanges() {
   int position = pCombo_->GetSelection();
   if (position != wxNOT_FOUND)
      comboPositionSelected_ = position;
   bool checked = XRCCTRL(*(this->GetWidget()->GetWindow()),
         "ID_SHOW_FAVORITES_CHECK", wxCheckBox)->GetValue();
   if (checked != itemFavoriteChecked_ && pItemSelected_) {
      itemFavoriteChecked_ = checked;
      if (itemFavoriteChecked_) {
         cguClient_.AddItemToFavorites(libraryId_,
                                       pItemSelected_->GetPrincipalAttributeValue());
         /**
          * Fix bug TCK #7202
          * Si se filtra por favoritos es necesario actualizar el catalog para que muestre 
          * el nuevo item marcado. Para los casos donde se filtro por favoritos, si el item se agrega
          * se encontraria duplicado en la lista y cuando se realice el segundo delete se borrara
          * una memoria ya liberada.
          **/
         if (favoriteFilter_)
            catalog_.push_back(pItemSelected_);
      } else {
         cguClient_.RemoveItemFromFavorites(
               libraryId_, pItemSelected_->GetPrincipalAttributeValue());
         RemoveCatalogByPrincipal(pItemSelected_->GetPrincipalAttributeValue());
      }
   }
   return true;
}

/** Restaura los valor's originales del Part */
bool UniversalGraphicalComponentPart::RollbackChanges() {
   if (pCombo_)
      pCombo_->SetSelection(comboPositionSelected_);
   return true;
}

/** Crea el Tool Window */
bool UniversalGraphicalComponentPart::CreateToolWindow() {
   if (pToolWindow_)
      return false;
   pToolWindow_ =
         wxXmlResource::Get()->LoadPanel(pParentWindow_, wxT("ID_LIBRARY_NAME_EDITION_PANEL"));
   ConfigureFeatures(featureCombinationActive_);
   ConfigureButtonsEvents();
   SetInitialValues();
   return true;
}

/**
 * Metodo auxiliar para contectar los eventos a los botones
 */
void UniversalGraphicalComponentPart::ConfigureButtonsEvents() {
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_SAVE_ITEM_BUTTON", wxBitmapButton)->Connect(
            wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(SaveEvent::OnSaveButtonEventHandler), NULL,
            pSaveEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_DELETE_ITEM_BUTTON", wxBitmapButton)->Connect(
            wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(DeleteEvent::OnDeleteButtonEventHandler), NULL,
            pDeleteEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_LIBRARY_ITEM_SELECTION_BUTTON", wxBitmapButton)->Connect(
            wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(
                  CallFullFledgedLibraryEvent::OnCallFullFledgedLibraryButtonEventHandler),
            NULL, pCallFullFledgedLibraryEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_SHOW_FAVORITES_CHECK", wxCheckBox)->Connect(
            wxEVT_COMMAND_CHECKBOX_CLICKED,
            wxCommandEventHandler(MarkFavoriteEvent::OnMarkFavoriteEventHandler), NULL,
            pCallFullFledgedLibraryEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_NAME_EDITION_COMBO", wxComboBox)->Connect(
            wxEVT_COMMAND_COMBOBOX_SELECTED,
            wxCommandEventHandler(
                  ComboBoxSelectionEvent::OnComboBoxSelectionEventHandler),
            NULL, pComboBoxEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_NAME_EDITION_COMBO", wxComboBox)->Connect(
               wxEVT_COMMAND_TEXT_UPDATED ,
            wxCommandEventHandler(
                  ComboBoxSelectionEvent::OnComboBoxTextChanged),
            NULL, pComboBoxEventHandler_);
}

/** Inicializa el part */
void UniversalGraphicalComponentPart::SetInitialValues() {
   if (!pCombo_)
      return;
   catalog_ = cguClient_.GetCatalog(libraryId_, favoriteFilter_);
   pCombo_->Clear();
   if (!(featureCombinationActive_ & SINGLE_ITEM)) {
      std::vector<LibraryItemCatalog*>::iterator it = catalog_.begin();

      for (; it != catalog_.end(); ++it)
         pCombo_->Append(GetItemComboValue(*it));
   }
   comboPositionSelected_ = 0;
   if (!lastPrincipalSelected_.empty()) {
      SelectItemByPrincipal(lastPrincipalSelected_);
      return;
   }
   pCombo_->SetSelection(comboPositionSelected_);
   int size = catalog_.size();
   if (size > 0 && size > comboPositionSelected_) {
      pItemSelected_ = catalog_[comboPositionSelected_];
      std::string tooltip = (pItemSelected_)->GetExtendedAttributeValue();
      // Para poder configurar el tooltip el combobox no tiene que ser readonly
      if (!(featureCombinationActive_ & COMBO_READ_FLAG))
         pCombo_->SetToolTip(tooltip.c_str());

      // Se notifica la seleccion del item inicial al editor de biblioteca
      // De esta forma si no se cambia la seleccion se queda con ese item
      const LibraryItem* pitem = cguClient_.GetItemFromLibrary(
            libraryId_, pItemSelected_->GetAssociatedItemId());
      if (pNotificator_) {
         const Library* plib = cguClient_.GetLibrary(libraryId_);
         bool modif =
               plib ? plib->HasOriginCapability(pitem->GetOriginId(),
                                                LibraryItemOrigin::WRITE) :
                      false;
         pNotificator_->NotifyItemSelectionFromCgu(pitem, true, modif);
      }
   }
}

/** Actualiza el estado de la parte */
void UniversalGraphicalComponentPart::Update() {
}

/** Metodo auxiliar para configurar las caracteristicas del CGU **/
void UniversalGraphicalComponentPart::ConfigureFeatures(
      FeatureCombinationEnum FeatureCombination) {
   wxCheckBox* pfavoritecheck = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                        wxT("ID_SHOW_FAVORITES_CHECK"), wxCheckBox);
   wxBitmapButton* psavebutton = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                         wxT("ID_SAVE_ITEM_BUTTON"), wxBitmapButton);
   wxBitmapButton* pdeletebutton = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                           wxT("ID_DELETE_ITEM_BUTTON"),
                                           wxBitmapButton);
   wxBitmapButton* plibrarybutton = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                            wxT("ID_LIBRARY_ITEM_SELECTION_BUTTON"),
                                            wxBitmapButton);
   wxStaticText* pnamelabel = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                      wxT("ID_NAME_EDITION_TAG"), wxStaticText);
   pCombo_ = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                   wxT("ID_NAME_EDITION_COMBO"), wxComboBox);

   if (!lastPrincipalSelected_.empty())
      SelectItemByPrincipal(lastPrincipalSelected_);

   bool showsavebutton = FeatureCombination & SAVE_FLAG;
   bool showdeletebutton = FeatureCombination & DELETE_FLAG;
   bool showlibrarybutton = FeatureCombination & FULLFLEDGED_FLAG;
   bool shownamelabel = FeatureCombination & LABEL_FLAG;
   bool showfavorite = FeatureCombination & FAVORITE_FLAG;
   bool readonlycombo = FeatureCombination & COMBO_READ_FLAG;
   bool dropdrowncombo = !(FeatureCombination & NO_DROP_DOWN_FLAG);
   pCombo_->SetEditable(!readonlycombo);
   pCombo_->Enable(dropdrowncombo);
   pfavoritecheck->Show(showfavorite);
   psavebutton->Show(showsavebutton);
   /** porque estaba porfazado a false? **/
//   psavebutton->Enable(false);
   pdeletebutton->Show(showdeletebutton);
   plibrarybutton->Show(showlibrarybutton);
   pnamelabel->Show(shownamelabel);
}

/** Obtiene el item seleccionado**/
const LibraryItem* UniversalGraphicalComponentPart::GetSelectedItem() {
   LibraryItemCatalog* pcatalogitem = GetLibraryItemCatalogSelected();
   if (!pCombo_ || !pcatalogitem)
      return NULL;
   const Library* plib = cguClient_.GetLibrary(libraryId_);
   return plib->GetItemByPrincipal(pcatalogitem->GetPrincipalAttributeValue());
}

/** Evento que se ejecuta al hacer click sobre boton de agregar clase.
 *  En caso de no existir en la lista el valor del combo se agrega un nuevo item
 *  y se agrega el valor al combobox.
 *  En caso de que el valor exista, dependiendo del estado del check de favoritos
 *  se agrega o elimina de la lista de favoritos*/
void UniversalGraphicalComponentPart::OnSaveButtonEventHandler(wxCommandEvent &Event) {
   LibraryItemCatalog* pcatalogitem = GetLibraryItemCatalogSelected();
   SuriObject::UuidType itemid = SuriObject::NullUuid;
   // En caso de poseer informacion invalida, el part tiene que terminar con error
   // y evitar que se propague el evento.
   if (pNotificator_ && !pNotificator_->AskEditorForValidData()) {
      SHOW_ERROR(message_PART_SAVE_ERROR);
      Event.StopPropagation();
      return;
   }
   // si no existe en el catalog se agrega
   if (pcatalogitem) {
      itemid = pcatalogitem->GetAssociatedItemId();
   } else {
      // TODO(Ramiro - TCK #2447): Analizar efecto de flag EXTENDED_COMBO_FLAG en salvar in situ
      std::string principalvalue = pCombo_->GetValue().c_str();
      LibraryItem* pitem = new LibraryItem(libraryId_, principalvalue);
      itemid = pitem->GetId();
      LibraryItemCatalog* pcatalogitem = pitem->GetCatalogInformation();
      catalog_.push_back(pcatalogitem);
      pCombo_->SetSelection(pCombo_->Append(principalvalue));
      cguClient_.AddItemToLibrary(libraryId_, pitem);
   }
   // Se pide los atributos modificados en el editor
   std::vector<LibraryItemAttribute*>* pattributes =
         pNotificator_ ? pNotificator_->GetAttributesModifiedFromEditor() : NULL;
   // Se replica el cambio de los atributos a la biblioteca
   if (pattributes) {
      cguClient_.ReplaceAttributesFromItem(libraryId_, itemid, pattributes);
      delete pattributes;
   }

   pattributes = pNotificator_ ? pNotificator_->GetAttributesRemovedFromEditor() : NULL;
   if (pattributes) {
      cguClient_.RemoveAttributesFromItem(libraryId_, itemid, pattributes);
      delete pattributes;
   }


   if (itemid != SuriObject::NullUuid && lastCheckState_) {
      cguClient_.AddItemToFavorites(libraryId_, itemid);
   } else if (itemid != SuriObject::NullUuid && !lastCheckState_) {
      cguClient_.RemoveItemFromFavorites(libraryId_, itemid);
   }
}

/** Evento que se ejecuta al hacer click sobre boton de agregar clase.
 *  En caso de existir el valor del combo en la lista de items,
 *  Este item es eliminado de favoritos y de la biblioteca */
void UniversalGraphicalComponentPart::OnDeleteButtonEventHandler(
      wxCommandEvent &Event) {
   int position = pCombo_->GetSelection();
   std::string principalvalue = catalog_[pCombo_->GetSelection()]->
                                                GetPrincipalAttributeValue();
   if (position != wxNOT_FOUND) {
      comboPositionSelected_ = position;
      pCombo_->Delete(position);
      SuriObject::UuidType itemid = RemoveCatalogByPrincipal(principalvalue);
      if (itemid.compare(SuriObject::NullUuid) != 0) {
         cguClient_.RemoveItemFromLibrary(libraryId_, itemid);
      }
      if (pNotificator_)
         pNotificator_->NotifyItemSelectionFromCgu();
   }
}

/** Evento que se ejecuta al hacer click sobre boton de agregar clase */
void UniversalGraphicalComponentPart::OnCallFullFledgedLibraryButtonEventHandler(
      wxCommandEvent &Event) {
   const Library* plib = cguClient_.GetLibrary(libraryId_);
   if (plib) {
      FullFledgeLibraryEditorWidget* pcontainer =
            new FullFledgeLibraryEditorWidget(pManager_,
                  plib->GetLibraryCode());
      pcontainer->CreateTool();
      pcontainer->ShowModal();
   }
}

/** Evento que se ejecuta al hacer click sobre boton de agregar clase */
void UniversalGraphicalComponentPart::OnMarkFavoriteEventHandler(
      wxCommandEvent &Event) {
   wxCheckBox* pfavoritecheck = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                        wxT("ID_SHOW_FAVORITES_CHECK"), wxCheckBox);
   lastCheckState_ = pfavoritecheck->IsChecked();
}

/** Evento que se ejecuta al seleccionar un item en el combobox cambiando el
 * tooltip asociado **/
void UniversalGraphicalComponentPart::OnComboBoxSelectionEventHandler(wxCommandEvent &Event) {
   LibraryItemCatalog* pcatalogitem = GetLibraryItemCatalogSelected();
   if (pcatalogitem != NULL) {
      // Mirando http://trac.wxwidgets.org/ticket/12659 junto al codigo fuente
      // se deduce que se tiene que setear primero el tooltip en NULL
      // para luego poder cambiar el tooltip de forma dinamica
      pCombo_->SetToolTip(NULL);
      pCombo_->SetToolTip(pcatalogitem->GetExtendedAttributeValue().c_str());
      if (pNotificator_) {
         const Library* plib = cguClient_.GetLibrary(libraryId_);
         bool modif = plib ? plib->HasOriginCapability(pcatalogitem->GetOriginId(),
                                                       LibraryItemOrigin::WRITE) : false;
         pNotificator_->NotifyItemSelectionFromCgu(
               cguClient_.GetItemFromLibrary(libraryId_,
                                             pcatalogitem->GetAssociatedItemId()),
               false, modif);
      }
   }
}

/**
 * Obtiene el item de catalogo de biblioteca seleccionado en el combobox
 * @return referencia al item de catalogo seleccionado
 * @return NULL en caso de que no exista el item de catalogo seleccionado
 */
LibraryItemCatalog* UniversalGraphicalComponentPart::GetLibraryItemCatalogSelected() {
   LibraryItemCatalog* pcatalogitem = NULL;
   if (!(featureCombinationActive_ & SINGLE_ITEM)) {
      int selection = pCombo_->GetSelection();
      int size = static_cast<int>(catalog_.size());
      if (size > 0 && selection >= 0 && selection < size)
         pcatalogitem = catalog_[selection];
   } else {
      return pItemSelected_;
   }
   return pcatalogitem;
}

/** Evento asociado al cambio del texto en el combobox **/
void UniversalGraphicalComponentPart::OnComboBoxTextChanged(wxCommandEvent &Event) {
   wxBitmapButton* psavebutton = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                            wxT("ID_SAVE_ITEM_BUTTON"), wxBitmapButton);
   bool enablebutton = false;
   if (pNotificator_)
      enablebutton = pNotificator_->AskEditorForValidData();
   psavebutton->Enable(enablebutton);
}

/** Metodo auxiliar para obtener el item de catalogo correspondiente al valor del
 * atributo Principal que se pasa por parametro
 * @param Principal : valor del atributo Principal que se desea buscar
 * @return LibraryItemCatalog* referencia al item de catalogo buscado
 * @return NULL en caso de no encontrar el item de catalogo
 * */
LibraryItemCatalog* UniversalGraphicalComponentPart::GetCatalogItemByPrincipal(
      const std::string& Principal) {
   LibraryItemCatalog* pcatalogitem = NULL;
   std::vector<LibraryItemCatalog*>::const_iterator it = catalog_.begin();
   for (; !pcatalogitem && it != catalog_.end(); ++it) {
      if ((*it)->GetPrincipalAttributeValue().compare(Principal) == 0)
         pcatalogitem = *it;
   }
   return pcatalogitem;
}

/** Actualiza la informacion de un item que fue actualizado desde otro lado*/
void UniversalGraphicalComponentPart::UpdateItemCatalogInformation(
      const LibraryItem* pItem) {
   LibraryItemCatalog* pcatalogitem = pItem->GetCatalogInformation();
   std::string oldprincipalvalue;
   if (ReplaceCatalogItem(pcatalogitem, &oldprincipalvalue)) {
      int position = GetItemComboPosition(pcatalogitem);
      if (position != wxNOT_FOUND) {
         LibraryItemCatalog* pcatalogitem = pItem->GetCatalogInformation();
         pCombo_->Delete(position);
         pCombo_->Insert(pcatalogitem->GetPrincipalAttributeValue().c_str(), position);
      }
   } else {
      delete pcatalogitem;
   }
}

/** */
LibraryItemCatalog* UniversalGraphicalComponentPart::GetCatalogItemById(
      const SuriObject::UuidType& ItemId) {
   LibraryItemCatalog* pcatalogitem = NULL;
   std::vector<LibraryItemCatalog*>::const_iterator it = catalog_.begin();
   for (; !pcatalogitem && it != catalog_.end(); ++it) {
      if ((*it)->GetAssociatedItemId().compare(ItemId) == 0)
         pcatalogitem = *it;
   }
   return pcatalogitem;
}

/**
 * Metodo auxiliar para reemplazar un item de la lista de catalogo del CGU
 * @param[in] pCatalog Referencia al item de catalogo a insertar, tiene el mismo id
 * asociado que el item a reemplazar
 * @param[out] pOldValue valor del item reemplazado
 * @return
 */
bool UniversalGraphicalComponentPart::ReplaceCatalogItem(LibraryItemCatalog* pCatalog,
                                                         std::string* pOldValue) {
   bool deleted = false;
   std::vector<LibraryItemCatalog*>::iterator it = catalog_.begin();
   for (; !deleted && it != catalog_.end(); ++it) {
      std::string oldvalue = (*it)->GetPrincipalAttributeValue();
      if (oldvalue.compare(pCatalog->GetPrincipalAttributeValue()) == 0) {
         deleted = true;
         *pOldValue = oldvalue;
         delete *it;
         catalog_.erase(it);
      }
   }
   if (deleted) {
      catalog_.push_back(pCatalog);
      return true;
   } else {
      return false;
   }
}

/**
 * Retorna el contenido del combo a partir del catalogo del item a representar
 * @param[in] pCatalogItem item de catalogo
 * @return contenido de combo.
 */
std::string UniversalGraphicalComponentPart::GetItemComboValue(
                                             LibraryItemCatalog* pCatalogItem) {
   std::string combocontent = pCatalogItem->GetPrincipalAttributeValue();
   bool extendedcombo = featureCombinationActive_ & EXTENDED_COMBO_FLAG;
   if (extendedcombo)
      combocontent += std::string(ATTRIBUTE_SEPARATOR) + pCatalogItem->GetExtendedAttributeValue();
   // TODO(Ramiro - TCK #2447): Mejorar interfaz grafica para que no
   // sea necesario recortar el contenido del combo.
   // Ver comentario en TCK 2447 para una mejor descripcion.
   return combocontent.substr(0, MAX_COMBO_TEXT_LENGTH);
}

/** Retorna la posicion en el combo del item */
int UniversalGraphicalComponentPart::GetItemComboPosition(
                                                LibraryItemCatalog* pCatalog) {
   std::string combovalue = GetItemComboValue(pCatalog);
   return pCombo_->FindString(combovalue.c_str());
}

/** Indica que se ha actualizado la informacion del item desde afuera. Esto hace
 * que se habilite el boton de guardar
 * @param IsValidData boolean que indica si la informacion es valida */
void UniversalGraphicalComponentPart::ItemInformationChanged(bool IsValidData) {
   wxBitmapButton* psavebutton = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                            wxT("ID_SAVE_ITEM_BUTTON"), wxBitmapButton);
   psavebutton->Enable(IsValidData);
}

/** Devuelve el ultimo estado del check*/
bool UniversalGraphicalComponentPart::GetLastCheckState() const {
   return lastCheckState_;
}


/** Metodo auxiliar para eliminar el item de catalogo correspondiente al valor del
 * atributo Principal que se pasa por parametro*/
SuriObject::UuidType UniversalGraphicalComponentPart::RemoveCatalogByPrincipal(
      const std::string& Principal) {
   bool deleted = false;
   SuriObject::UuidType itemid = SuriObject::NullUuid;
   std::vector<LibraryItemCatalog*>::iterator it = catalog_.begin();
   for (; !deleted && it != catalog_.end(); ++it) {
      if ((*it)->GetPrincipalAttributeValue().compare(Principal) == 0) {
         LibraryItemCatalog* pcatalogitem = *it;
         itemid = pcatalogitem->GetAssociatedItemId();
         catalog_.erase(it);
         delete pcatalogitem;
         deleted = true;
      }
   }
   return itemid;
}

/** Metodo que fuerza la seleccion de un item. En caso de no existir
 *  un item con ese atributo principal, el CGU debera mostrar el atributo
 *  principal pasado por parametro e impedir la seleccion de items*/
void UniversalGraphicalComponentPart::SelectItemByPrincipal(
      const std::string& PrincipalAttribute) {
   lastPrincipalSelected_ = PrincipalAttribute;
   if (!pCombo_)
      return;
   LibraryItemCatalog* pcatalogitem = GetCatalogItemByPrincipal(PrincipalAttribute);
   const Library* plib = cguClient_.GetLibrary(libraryId_);
   if (!pcatalogitem && plib) {
      const LibraryItem* pitem = plib->GetItemByPrincipal(PrincipalAttribute);
      pcatalogitem = pitem ? pitem->GetCatalogInformation() : NULL;
      if (pcatalogitem == NULL) return;
      pcatalogitem->SetFavoriteFlag(plib->IsFavorite(PrincipalAttribute));
   }
   if (featureCombinationActive_ & SINGLE_ITEM) {
      pCombo_->Clear();
      int position = pCombo_->Append(GetItemComboValue(pcatalogitem));
      pCombo_->SetSelection(position);
      pItemSelected_ = pcatalogitem;
      comboPositionSelected_ = position;
      /** Marca el check de favoritos **/
      itemFavoriteChecked_ = pcatalogitem->IsFavorite();
      lastCheckState_ = itemFavoriteChecked_;
      GET_CONTROL(*(this->GetWidget()->GetWindow()),
            "ID_SHOW_FAVORITES_CHECK", wxCheckBox)->SetValue(itemFavoriteChecked_);
      return;
   }
   if (pcatalogitem == NULL) return;
   int position = GetItemComboPosition(pcatalogitem);
   if (position == wxNOT_FOUND)
      position = 0;
   if (position >= 0) {
      pCombo_->SetSelection(position);
      pItemSelected_ = pcatalogitem;
      comboPositionSelected_ = position;
      /** Marca el check de favoritos **/
      itemFavoriteChecked_ = pcatalogitem->IsFavorite();
      lastCheckState_ = itemFavoriteChecked_;
      GET_CONTROL(*(this->GetWidget()->GetWindow()),
            "ID_SHOW_FAVORITES_CHECK", wxCheckBox)->SetValue(itemFavoriteChecked_);
   }
}

/** Indica la biblioteca con cual trabajar. Solo se puede setear una vez **/
bool UniversalGraphicalComponentPart::SetLibrary(const SuriObject::UuidType& LibraryId) {
   if (LibraryId.compare(SuriObject::NullUuid) == 0)
      return false;
   /** Si son distintas es necesario volver a cargar el choice **/
   if (libraryId_.compare(LibraryId) != 0) {
      libraryId_ = LibraryId;
      lastPrincipalSelected_ = "";
      SetInitialValues();
   }
   return true;
}

/** Actualiza la configuracion de botones a desplegar en base a la combinacion de
 *  caracteristicas elegida **/
void UniversalGraphicalComponentPart::UpdateFeaturesLayout(
      FeatureCombinationEnum FeatureCombination) {
   ConfigureFeatures(FeatureCombination);
}

/** Metodo que fuerza la actualizacion de la lista de catalogos que
 *  muestra el cgu
 */
void UniversalGraphicalComponentPart::UpdateCatalogList() {
   catalog_ = cguClient_.GetCatalog(libraryId_, favoriteFilter_);
   if (!pCombo_)
      return;
   pCombo_->Clear();
   std::vector<LibraryItemCatalog*>::iterator it = catalog_.begin();
   for (; it != catalog_.end(); ++it) {
      pCombo_->Append(GetItemComboValue(*it));
   }
   // Una vez reseteado los items, seteo el primero como activo
   if (!catalog_.empty()) {
      pCombo_->SetSelection(0);
   }
}
} /** namespace suri */
