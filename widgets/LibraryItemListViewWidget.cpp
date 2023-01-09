/*! \file LibraryItemListViewWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <vector>
#include <string>
#include <set>
#include <map>
#include <utility>

// Includes Suri
#include "LibraryItemListViewWidget.h"
#include "suri/LibraryItemCatalog.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
#include "wx/gdicmn.h"
#include "wx/toplevel.h"

// Defines
#define ID_TREECTRL "ID_TREECTRL"
#define ID_ITEM_SELECTOR_PANEL "ID_ITEM_SELECTOR_PANEL"
#define MIN_LIBRARY_WIDTH 600
#define MIN_LIBRARY_HEIGHT 400
#define SYSTEM_CATEGORY_NAME "Sistema"
#define NONE_CATEGORY_NAME "Ninguno"
#define USER_CATEGORY_NAME "Usuario"
#define PROYECT_CATEGORY_NAME "Proyecto"

// forwards

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(TreeSelectionChangeEvent, LibraryItemListViewWidget)
      IMPLEMENT_EVENT_CALLBACK(OnItemSelectionChange,
                               OnItemSelectionChange(event), wxTreeEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor
 * @param pManager
 * @param pNotificator
 * @param LibraryId
 * @param GroupBy
 */
LibraryItemListViewWidget::LibraryItemListViewWidget(
      LibraryManager* pManager, LibraryItemListNotificator* pNotificator,
      const std::string& LibraryCode, const GroupByType& GroupBy) :
      NEW_EVENT_OBJECT(TreeSelectionChangeEvent), client_(pManager,
                                                          LibraryManager::ALL),
      pManager_(pManager), libraryCode_(LibraryCode), pNotificator_(pNotificator),
      itemRemove_(false), pLibrary_(NULL) {
   if (pNotificator_)
      pNotificator_->SetLibraryItemList(this);
}

/** Destructor **/
LibraryItemListViewWidget::~LibraryItemListViewWidget() {
   DELETE_EVENT_OBJECT;
}


/** Se crea la ventana de herramienta en las clases derivadas */
bool LibraryItemListViewWidget::CreateToolWindow() {
   if (pToolWindow_)
      return false;
   pTreeCtrl_ = new wxTreeCtrl(pParentWindow_, wxID_ANY);
   pTreeCtrl_->Connect(wxEVT_COMMAND_TREE_SEL_CHANGED,
         wxTreeEventHandler(TreeSelectionChangeEvent::OnItemSelectionChange),
         NULL, pEventHandler_);
   pTreeCtrl_->SetWindowStyle(wxTR_HIDE_ROOT);
   SetInitialValues();
   if (GetFrame()) {
      GetFrame()->SetMinSize(wxSize(MIN_LIBRARY_WIDTH, MIN_LIBRARY_HEIGHT));
   }
   pToolWindow_ = pTreeCtrl_;
   return true;
}

/**
 *  Configura los valores del wxTree a partir de el contenido de la biblioteca
 */
void LibraryItemListViewWidget::SetInitialValues() {
   pTreeCtrl_->DeleteAllItems();
   wxTreeItemId rootid = pTreeCtrl_->AddRoot(libraryCode_.c_str());
   pLibrary_ = client_.GetLibraryByCode(libraryCode_);
   if (!pLibrary_)
      return;
   std::set<LibraryItemOrigin::OriginCategoryType> categories =
         pLibrary_->GetOriginCategory();
   std::set<LibraryItemOrigin::OriginCategoryType>::const_iterator catit =
         categories.begin();
   for (; catit != categories.end(); ++catit) {
      std::vector<const LibraryItemOrigin*> origins =
            pLibrary_->GetOriginsByCategory(*catit);
      std::vector<const LibraryItemOrigin*>::const_iterator origit = origins.begin();

      wxTreeItemId categitemid = pTreeCtrl_->AppendItem(rootid,
                                                        GetCategoryName(*catit).c_str());
      for (; origit != origins.end(); ++origit) {

         std::vector<LibraryItemCatalog*> catalog = (*origit)->GetCatalog();
         std::vector<LibraryItemCatalog*>::const_iterator it = catalog.begin();
         wxTreeItemId originitemid =
               pTreeCtrl_->AppendItem(categitemid, (*origit)->GetName().c_str());
         originMap_.insert(std::make_pair((*origit)->GetId(), originitemid));
         originItemIds_.push_back(originitemid);
         for (; it != catalog.end(); ++it) {
            pTreeCtrl_->AppendItem(originitemid,
                                   (*it)->GetPrincipalAttributeValue().c_str());
         }
         delete_all(catalog);
      }
   }
}

/** Devuelve el nombre asociado a la categoria de origen **/
std::string LibraryItemListViewWidget::GetCategoryName(
      LibraryItemOrigin::OriginCategoryType Category) const {
   std::string categoryname;
   switch (Category) {
      case LibraryItemOrigin::NONE:
         categoryname = NONE_CATEGORY_NAME;
         break;
      case LibraryItemOrigin::SYSTEM:
         categoryname = SYSTEM_CATEGORY_NAME;
         break;
      case LibraryItemOrigin::USER:
         categoryname = USER_CATEGORY_NAME;
         break;
      case LibraryItemOrigin::PROJECT:
         categoryname = PROYECT_CATEGORY_NAME;
         break;
   }
   return categoryname;
}

/** Metodo auxiliar que captura el evento de cambio de seleccion **/
void LibraryItemListViewWidget::OnItemSelectionChange(wxTreeEvent& Event) {
   if (itemRemove_)
      return;
   // validacion sobre la seleccion para que sea sobre un item de biblioteca
   wxTreeItemId id = pTreeCtrl_->GetSelection();
   if (!IsLibraryItem(id))
      return;
   std::string principal  = pTreeCtrl_->GetItemText(pTreeCtrl_->GetSelection()).c_str();
   const LibraryItem* pitem = pLibrary_->GetItemByPrincipal(principal);
   if (pitem)
      pNotificator_->NotifyLibraryItemSelectionChanged(pitem);
}

/** Agrega un item a la lista
 *  Precondicion: El item ya fue agregado a la biblioteca, lo que quiere decir
 *  es que existe un origen con permisos para agregar este item*/
void LibraryItemListViewWidget::AddItem(const SuriObject::UuidType& OriginId,
                                        const LibraryItem* pItem) {
#ifdef __OLD_SELECTION_SYSTEM__
   wxTreeItemId itemid = pTreeCtrl_->GetSelection();
   // se fija si la seleccion es un item de biblioteca agrega el nuevo item bajo
   // el mismo padre
   wxTreeItemId parentid;
   if (IsLibraryItem(itemid)) {
      parentid = pTreeCtrl_->GetItemParent(itemid);
   } else {
      // TODO(Gabriel - TCK #3411): Ver forma de mejorar esta implementacion
      // en caso contrario lo agregar al primer origen disponible
     if (originItemIds_.size() >0)
        parentid = originItemIds_.at(0);
   }
#endif
   OriginItemAssociationMap::iterator findit = originMap_.find(OriginId);
   if (findit != originMap_.end()) {
      wxTreeItemId itemid = pTreeCtrl_->AppendItem(
            findit->second, pItem->GetPrincipalAttribute()->GetValue().c_str());
      pTreeCtrl_->SelectItem(itemid);
   }
}

/** Remueve un item de la lista */
void LibraryItemListViewWidget::RemoveActiveItem() {
   if (pTreeCtrl_) {
      itemRemove_ = true;
      wxTreeItemId itemid = pTreeCtrl_->GetSelection();
      if (IsLibraryItem(itemid)) {
         pTreeCtrl_->Unselect();
         pTreeCtrl_->Delete(itemid);
         itemRemove_ = false;
      }
   }
}

/**
 * Metodo que verifica que el id pasado por parametro pertenezca a un item de biblioteca
 * valido. Se verifica que el padre del item corresponda a un origen valido
 * Precondicion: pTreeCtrl_ es != NULL
 * @param[in] ItemId id del item
 * @return true en caso de que corresponda a un item, false en caso contrario
 */
bool LibraryItemListViewWidget::IsLibraryItem(wxTreeItemId& ItemId) {
   ItemIdVector::iterator it = originItemIds_.begin();
   bool founditem = false;
   wxTreeItemId parentid = pTreeCtrl_->GetItemParent(ItemId);
   // Se verifica que el padre del item corresponda a un origen valido
   for (; !founditem && it != originItemIds_.end(); ++it) {
      founditem = parentid == *it;
   }
   return founditem;
}

/** Obtiene el id del origen seleccionado en el arbol.
 * @return id del origen seleccionado
 * @return SuriObject::NullUuid en caso de que la seleccion no sea un origen
 */
SuriObject::UuidType LibraryItemListViewWidget::GetOriginIdSelected() const {
   wxTreeItemId selectid = pTreeCtrl_->GetSelection();
   OriginItemAssociationMap::const_iterator it = originMap_.begin();
   SuriObject::UuidType id = SuriObject::NullUuid;
   for (; it != originMap_.end(); ++it) {
      if (it->second == selectid) {
         id = it->first;
         break;
      }
   }
   return id;
}
} /** namespace suri */
