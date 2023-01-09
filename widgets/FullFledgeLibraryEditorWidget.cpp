/*! \file FullFledgeLibraryEditorWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <algorithm>
#include <cmath>
#include <list>
#include <iterator>
#include <string>

// Includes suri
#include "suri/messages.h"
#include "suri/Configuration.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/FullFledgeLibraryEditorWidget.h"
#include "LibraryItemEditorDebugPart.h"
#include "suri/LibraryItemOrigin.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/imaglist.h"

// Defines
#define ID_ITEM_EDITOR_PANEL "ID_ITEM_EDITOR_PANEL"
#define MIN_LIBRARY_WIDTH 600
#define MIN_LIBRARY_HEIGHT 400
#define NEW_ITEM "Nuevo item"
#define ITEM_NAME_SIZE 20

namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(FullFledgeLibraryEditorWidgetEvent, FullFledgeLibraryEditorWidget)
      IMPLEMENT_EVENT_CALLBACK(OnAddClick, OnAddClick(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnRemoveClick, OnRemoveClick(event), wxCommandEvent)
// Separo las llamadas en la conexion de eventos
END_IMPLEMENT_EVENT_OBJECT/** \endcond */


/** Constructor **/
FullFledgeLibraryEditorWidget::FullFledgeLibraryEditorWidget(
      LibraryManager* pManager, const std::string& LibraryCode, const std::string& WindowTitle) :
      PartContainerWidget(_(caption_LIBRARY), SUR_BTN_APPLY | SUR_BTN_OK | SUR_BTN_CANCEL,
                          SUR_BTN_APPLY | SUR_BTN_OK | SUR_BTN_CANCEL),
      NEW_EVENT_OBJECT(FullFledgeLibraryEditorWidgetEvent),
      notificator_(this), libraryCode_(LibraryCode),
      pItemSelector_(new LibraryItemListViewWidget(pManager, &notificator_, libraryCode_)),
      pManager_(pManager), pCurrentEditor_(NULL), itemRemoved_(false),
      pLibrary_(NULL) {
   windowTitle_ = _(WindowTitle.c_str());
}


/** Destructor **/
FullFledgeLibraryEditorWidget::~FullFledgeLibraryEditorWidget() {
   DELETE_EVENT_OBJECT;
}

/**
 * Objetivo: Crea el frame contenedor(PartContainerWidget) y el objeto que contendra a
 * los Parts. Realiza los connect que correspondan segun tipo de Widget
 * @return bool que indica si se pudo crear la ventana
 */
bool FullFledgeLibraryEditorWidget::CreateToolWindow() {
   PartContainerWidget::CreateToolWindow();

   wxWindow *pcontainer = XRCCTRL(*pToolWindow_, wxT("ID_CONTAINER_WIN"), wxWindow);

   wxWindow* pwin = wxXmlResource::Get()->LoadPanel(
         pcontainer, "ID_UNIVERSAL_LIBRARY_EDITOR_PANEL");

   AddControl(pItemSelector_, "ID_ITEM_SELECTOR_PANEL");

   AddMainControl(pwin);

   if (GetFrame()) {
      GetFrame()->SetMinSize(wxSize(MIN_LIBRARY_WIDTH, MIN_LIBRARY_HEIGHT));
   }

   GET_CONTROL(*pwin, wxT("ID_REMOVE_ITEM_BTN"), wxBitmapButton)->Connect(
            wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(FullFledgeLibraryEditorWidgetEvent::OnRemoveClick),
            NULL, pEventHandler_);
   GET_CONTROL(*pwin, wxT("ID_ADD_ITEM_BTN"), wxBitmapButton)->Connect(
            wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(FullFledgeLibraryEditorWidgetEvent::OnAddClick), NULL,
            pEventHandler_);
   return true;
}

/**
 * Retorna la ventana padre de las partes.
 * @return puntero a la ventana padre de las partes
 */
wxWindow * FullFledgeLibraryEditorWidget::GetPartParent() {
   wxWindow *pcontainer = XRCCTRL(*pToolWindow_, wxT("ID_ITEM_EDITOR_PANEL"), wxWindow);
   return pcontainer;
}

/**
 * Objetivo: Agrega un Part al Notebook.
 * @param[in]	pPart: Part a agregar.
 * @return agrega_ok? true:false.
 */
bool FullFledgeLibraryEditorWidget::DoInsert(suri::Part *pPart) {
   bool retVal = false;

   if (pPart != NULL) {
      retVal = AddControl(pPart->GetWidget(), "ID_ITEM_EDITOR_PANEL");
   }

   return retVal;
}

/**
 * Objetivo: Elimina el Part del Notebook.
 * @param[in]	pPart: Part a eliminar.
 * @return elimina_ok? true:false.
 */
bool FullFledgeLibraryEditorWidget::DoRemove(suri::Part *pPart) {
   bool retVal = false;

   if (pPart) {
      retVal = true;
   }
   return retVal;
}

/**
 *  Metodo encargado de actualizar el editor asociado al item seleccionado
 *  @param pItem item al cual debe ajustar el editor del full-fledge
 * */
void FullFledgeLibraryEditorWidget::UpdateEditor(const LibraryItem* pItem) {
   bool modifiable = true;
   if (!pLibrary_) {
      LibraryClient client(pManager_, LibraryManager::ALL);
      pLibrary_ = client.GetLibraryByCode(libraryCode_);
   }
   modifiable = pLibrary_->HasOriginCapability(pItem->GetOriginId(),
                                          LibraryItemOrigin::WRITE);
   if (pCurrentEditor_) {
      CommitChangesFromActiveItem();
      pCurrentEditor_->SetActiveItem(pItem, false, modifiable);
   } else {
      pCurrentEditor_ =
            LibraryItemEditorPart::Create(
                  pManager_,
                  pItem,
                  pLibrary_->GetLibraryCode(),
                  UniversalGraphicalComponentPartInterface::SingleFavoritesAndNoLabel); //ExtendedNoDropDown
      partList_.push_back(pCurrentEditor_);
      AddControl(pCurrentEditor_, wxT(ID_ITEM_EDITOR_PANEL));

      wxScrolledWindow* pcontainer = XRCCTRL(*pToolWindow_, wxT(ID_ITEM_EDITOR_PANEL),
                                             wxScrolledWindow);
      if (pcontainer != NULL) {
         pcontainer->FitInside();
         pcontainer->Layout();
      }

      pCurrentEditor_->SetActiveItem(pItem, false, modifiable);
   }
}

/** Metodo que se llama cuando se presiona el boton de agregar*/
void FullFledgeLibraryEditorWidget::OnAddClick(wxCommandEvent& Event) {
   // TODO(Gabriel - TCK #3933): Ver forma de que las condiciones para agregar
   // o eliminar sean mas intuitivas para el usuario (ej. habilitar/deshabilitar
   // los botones en funcion de la capabilitie del origen
   SuriObject::UuidType origid = pItemSelector_->GetOriginIdSelected();
   if (origid.compare(SuriObject::NullUuid) == 0) {
      SHOW_ERROR("Para agregar un item se debe posicionar sobre un origen");
      return;
   }
   LibraryClient editorclient(pManager_, LibraryManager::ALL);
   if (!pLibrary_) {
      pLibrary_ = editorclient.GetLibraryByCode(libraryCode_);
   }
   // si origen tiene capability write entonces sigo sino salgo
   if (!pLibrary_->HasOriginCapability(origid, LibraryItemOrigin::WRITE)) {
      SHOW_ERROR("No es posible agregar el item en el destino seleccionado");
      return;
   }
   CommitChangesFromActiveItem();
   // se muestra dialogo para permitir el ingreso del nombre del item a agregar
   wxDialog dialog(NULL, wxID_ANY, _(NEW_ITEM), wxDefaultPosition, wxDefaultSize,
                   wxDEFAULT_DIALOG_STYLE);
   dialog.SetSizer(new wxBoxSizer(wxVERTICAL));
   static int addcount = 0;
   std::string addstr = NEW_ITEM;
   addstr.append(NumberToString<int>(addcount));
   wxTextCtrl *ptxtctrl = new wxTextCtrl(&dialog, wxID_ANY, _(NEW_ITEM));
   ptxtctrl->SetMaxLength(ITEM_NAME_SIZE);
   ++addcount;
   dialog.GetSizer()->Add(ptxtctrl);
   dialog.GetSizer()->Add(dialog.CreateButtonSizer(wxOK | wxCANCEL), 0,
                          wxEXPAND | wxALIGN_BOTTOM);
   dialog.Fit();
   dialog.Update();
   // muestro el dialogo
   if (dialog.ShowModal() == wxID_OK) {
      dialog.TransferDataFromWindow();
      std::string itemname = ptxtctrl->GetValue().c_str();
      if (pLibrary_) {
         LibraryItem* pitem = new LibraryItem(pLibrary_->GetId(), itemname);
         if (!pLibrary_->GetItemByPrincipal(itemname) &&
               editorclient.AddItemToLibraryOrigin(pLibrary_->GetId(), origid, pitem)) {
            UpdateEditor(pitem);
            notificator_.NotifyItemAdded(origid, pitem);
         } else {
            SHOW_ERROR(_(message_CANT_ADD_ITEM),
                       pitem->GetPrincipalAttribute()->GetValue().c_str());
            delete pitem;
            return;
         }
      }
   }
}

/** Metodo que se llama cuando se presiona el boton de eliminar*/
void FullFledgeLibraryEditorWidget::OnRemoveClick(wxCommandEvent& Event) {
   if (!pCurrentEditor_)
      return;
   switch (SHOWQUERY(GetWindow(), _(question_DELETE_ITEM),
                      wxYES|wxNO|wxICON_QUESTION)) {
            case wxID_YES:
            {
               LibraryClient editorclient(pManager_, LibraryManager::ALL);
               if (!pLibrary_)
                  pLibrary_ = editorclient.GetLibraryByCode(libraryCode_);
               const LibraryItem* pitem = pCurrentEditor_->GetActiveItem();

               if (pitem) {
                  itemRemoved_ = true;
                  if (editorclient.RemoveItemFromLibrary(pLibrary_->GetId(), pitem->GetId())) {
                     notificator_.NotifyActiveItemRemoved();
                     pCurrentEditor_->SetActiveItem(NULL);
                  } else {
                     SHOW_ERROR(_(message_CANT_DELETE_ITEM),
                                pitem->GetPrincipalAttribute()->GetValue().c_str());
                     return;
                  }
                  itemRemoved_ = false;
               }
               break;
            }
            case wxID_NO:
            default:
            {
               break;
            }
   }
}

/** Metodo encargado de verficar los cambios sobre el item activo en el editor**/
void FullFledgeLibraryEditorWidget::CommitChangesFromActiveItem() {
   if (!itemRemoved_ && pCurrentEditor_ && pCurrentEditor_->HasChanged()) {
      switch (SHOWQUERY(GetWindow(), _(question_SAVE_PART_CHANGES),
                      wxYES|wxNO|wxCANCEL|wxICON_QUESTION)) {
            case wxID_YES:
            {
               if (!pCurrentEditor_->CommitChanges()) {
                  SHOW_ERROR(message_PART_SAVE_ERROR);
               }
               break;
            }
            case wxID_NO:
            default:
            {
               pCurrentEditor_->RollbackChanges();
               break;
            }
      }
   }
}
}  /** namespace suri */

