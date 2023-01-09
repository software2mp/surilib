/*! \file NotebookWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <algorithm>
#include <cmath>

// Includes suri
#include "suri/NotebookWidget.h"
#include "suri/messages.h"
#include "suri/Configuration.h"
#include "suri/AuxiliaryFunctions.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/imaglist.h"

// defines
#ifdef __WXMSW__
/** Tamano extra para el ancho de la ventana en funcion del tamano minimo de las partes */
#define NOTEBOOK_CONTAINER_PLUS_WIDTH 150
/** Tamano extra para el alto de la ventana en funcion del tamano minimo de las partes */
#define NOTEBOOK_CONTAINER_PLUS_HEIGHT 50
#else
/** Tamano extra para el ancho de la ventana en funcion del tamano minimo de las partes */
#define NOTEBOOK_CONTAINER_PLUS_WIDTH 125
/** Tamano extra para el alto de la ventana en funcion del tamano minimo de las partes */
#define NOTEBOOK_CONTAINER_PLUS_HEIGHT 75
#endif

/** Relacion del tamano del espacio frente al resto de los caracteres para centrar texto */
#define WHITESPACE_SIZE_MULTIPLIER 1.5
#ifdef __WXGTK__
/** En wxGTK el Listbook no agrega lineas a los titulos */
#  define __WXLISTBOOK_DOESNT_WRAP_TITLES__
#endif

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(NotebookWidgetEvent, NotebookWidget)
// Separo las llamadas en la conexion de eventos
#ifdef __UNUSED_CODE__
   IMPLEMENT_EVENT_CALLBACK(OnNotebookChange, OnNotebookChange(event), wxNotebookEvent)
   IMPLEMENT_EVENT_CALLBACK(OnListbookChange, OnListbookChange(event), wxListbookEvent)
#endif // __UNUSED_CODE__
   IMPLEMENT_EVENT_CALLBACK(OnListPageChanged, OnPageChanged(event), wxListbookEvent)
   IMPLEMENT_EVENT_CALLBACK(OnListPageChanging, OnPageChanging(event), wxListbookEvent)
   IMPLEMENT_EVENT_CALLBACK(OnNotePageChanged, OnPageChanged(event), wxNotebookEvent)
   IMPLEMENT_EVENT_CALLBACK(OnNotePageChanging, OnPageChanging(event), wxNotebookEvent)
   IMPLEMENT_EVENT_CALLBACK(OnChoicePageChanged, OnPageChanged(event), wxChoicebookEvent)
   IMPLEMENT_EVENT_CALLBACK(OnChoicePageChanging, OnPageChanging(event), wxChoicebookEvent)
   IMPLEMENT_EVENT_CALLBACK(OnTreePageChanged, OnPageChanged(event), wxTreebookEvent)
   IMPLEMENT_EVENT_CALLBACK(OnTreePageChanging, OnPageChanging(event), wxTreebookEvent)
   IMPLEMENT_EVENT_CALLBACK(OnToolPageChanged, OnPageChanged(event), wxToolbookEvent)
   IMPLEMENT_EVENT_CALLBACK(OnToolPageChanging, OnPageChanging(event), wxToolbookEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonApply, OnButtonApply(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnChildFocus, OnChildFocus(event), wxChildFocusEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Ctor.
 * @param[in]  Title: titulo de la ventana
 * @param[in]	Style: indica el estilo del Notebook a crear.\n
 * @param[in]  Type: tipo de book.
 * - Style puede ser uno de:
 * .
 * - wxNB_TOP, wxNB_LEFT, wxNB_RIGHT, wxNB_BOTTOM.
 */
NotebookWidget::NotebookWidget(const wxString &Title, const long &Style, BookType Type,
                               const long VisibleButtons, const long EnabledButtons) :
      PartContainerWidget(Title, VisibleButtons, EnabledButtons), pBookCtrl_(NULL),
      notebookStyle_(Style), type_(Type), NEW_EVENT_OBJECT(NotebookWidgetEvent) {
   windowTitle_ = Title;
#ifdef PREVENT_EVENT_REPETITION
   eventVetoedInApplyChangesQuestion_ = false;
   eventVetoedInInvalidDataQuestion_ = false;
   buttonEvent_ = false;
#endif
}

/**
 * Dtor.
 */
NotebookWidget::~NotebookWidget() {
#ifndef __WINDOWS__
   /**
    * Comento este codigo porque en windows se genera SIGSEG al destruir
    * el notebook de un proceso de clasificacion. Linea
    * pBookCtrl_->Disconnect(
    *          wxEVT_COMMAND_LISTBOOK_PAGE_CHANGING,
    *          wxListbookEventHandler(NotebookWidgetEvent::OnListPageChanging),
    *          NULL, pEventHandler_);
    */
   switch (type_) {
      case Listbook:
         pBookCtrl_->Disconnect(
               wxEVT_COMMAND_LISTBOOK_PAGE_CHANGING,
               wxListbookEventHandler(NotebookWidgetEvent::OnListPageChanging), NULL,
               pEventHandler_);
         pBookCtrl_->Disconnect(
               wxEVT_COMMAND_LISTBOOK_PAGE_CHANGED,
               wxListbookEventHandler(NotebookWidgetEvent::OnListPageChanged), NULL,
               pEventHandler_);
         break;
      case Toolbook:
         pBookCtrl_->Disconnect(
               wxEVT_COMMAND_TOOLBOOK_PAGE_CHANGING,
               wxToolbookEventHandler(NotebookWidgetEvent::OnToolPageChanging), NULL,
               pEventHandler_);
         pBookCtrl_->Disconnect(
               wxEVT_COMMAND_TOOLBOOK_PAGE_CHANGED,
               wxToolbookEventHandler(NotebookWidgetEvent::OnToolPageChanged), NULL,
               pEventHandler_);
         break;
      default:
      case Notebook:
         pBookCtrl_->Disconnect(
               wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,
               wxNotebookEventHandler(NotebookWidgetEvent::OnNotePageChanging), NULL,
               pEventHandler_);
         pBookCtrl_->Disconnect(
               wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
               wxNotebookEventHandler(NotebookWidgetEvent::OnNotePageChanged), NULL,
               pEventHandler_);
         break;
      case Choicebook:
         pBookCtrl_->Disconnect(
               wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGING,
               wxChoicebookEventHandler(NotebookWidgetEvent::OnChoicePageChanging),
               NULL, pEventHandler_);
         pBookCtrl_->Disconnect(
               wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED,
               wxChoicebookEventHandler(NotebookWidgetEvent::OnChoicePageChanged), NULL,
               pEventHandler_);
         break;
      case Treebook:
         pBookCtrl_->Disconnect(
               wxEVT_COMMAND_TREEBOOK_PAGE_CHANGING,
               wxTreebookEventHandler(NotebookWidgetEvent::OnTreePageChanging), NULL,
               pEventHandler_);
         pBookCtrl_->Disconnect(
               wxEVT_COMMAND_TREEBOOK_PAGE_CHANGED,
               wxTreebookEventHandler(NotebookWidgetEvent::OnTreePageChanged), NULL,
               pEventHandler_);
         break;
   }
#endif  // __WINDOWS__
   DELETE_EVENT_OBJECT;
}

/**
 * Objetivo: Crea el frame contenedor(PartContainerWidget) y el objeto que contendra a
 * los Part's. Realiza los connect que correspondan segun tipo de Widget
 * @return bool que indica si se pudo crear la ventana
 */
bool NotebookWidget::CreateToolWindow() {
   PartContainerWidget::CreateToolWindow();
   if (notebookStyle_ != wxNB_TOP && notebookStyle_ != wxNB_LEFT
         && notebookStyle_ != wxNB_RIGHT && notebookStyle_ != wxNB_BOTTOM) {
      notebookStyle_ = wxNB_TOP;
   }
   switch (type_) {
      case Listbook:
         pBookCtrl_ = new wxListbook(pToolWindow_, wxID_ANY, wxDefaultPosition,
                                     wxDefaultSize, notebookStyle_, "");
         pBookCtrl_->Connect(
               wxEVT_COMMAND_LISTBOOK_PAGE_CHANGING,
               wxListbookEventHandler(NotebookWidgetEvent::OnListPageChanging), NULL,
               pEventHandler_);
         pBookCtrl_->Connect(
               wxEVT_COMMAND_LISTBOOK_PAGE_CHANGED,
               wxListbookEventHandler(NotebookWidgetEvent::OnListPageChanged), NULL,
               pEventHandler_);
         break;
      case Toolbook:
         pBookCtrl_ = new wxToolbook(pToolWindow_, wxID_ANY, wxDefaultPosition,
                                     wxDefaultSize, notebookStyle_, "");
         pBookCtrl_->Connect(
               wxEVT_COMMAND_TOOLBOOK_PAGE_CHANGING,
               wxToolbookEventHandler(NotebookWidgetEvent::OnToolPageChanging), NULL,
               pEventHandler_);
         pBookCtrl_->Connect(
               wxEVT_COMMAND_TOOLBOOK_PAGE_CHANGED,
               wxToolbookEventHandler(NotebookWidgetEvent::OnToolPageChanged), NULL,
               pEventHandler_);
         break;
      default:
      case Notebook:
         pBookCtrl_ = new wxNotebook(pToolWindow_, wxID_ANY, wxDefaultPosition,
                                     wxDefaultSize, notebookStyle_, "");
         pBookCtrl_->Connect(
               wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,
               wxNotebookEventHandler(NotebookWidgetEvent::OnNotePageChanging), NULL,
               pEventHandler_);
         pBookCtrl_->Connect(
               wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
               wxNotebookEventHandler(NotebookWidgetEvent::OnNotePageChanged), NULL,
               pEventHandler_);
         break;
      case Choicebook:
         pBookCtrl_ = new wxChoicebook(pToolWindow_, wxID_ANY, wxDefaultPosition,
                                       wxDefaultSize, notebookStyle_, "");
         pBookCtrl_->Connect(
               wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGING,
               wxChoicebookEventHandler(NotebookWidgetEvent::OnChoicePageChanging),
               NULL, pEventHandler_);
         pBookCtrl_->Connect(
               wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED,
               wxChoicebookEventHandler(NotebookWidgetEvent::OnChoicePageChanged), NULL,
               pEventHandler_);
         break;
      case Treebook:
         pBookCtrl_ = new wxTreebook(pToolWindow_, wxID_ANY, wxDefaultPosition,
                                     wxDefaultSize, notebookStyle_, "");
         pBookCtrl_->Connect(
               wxEVT_COMMAND_TREEBOOK_PAGE_CHANGING,
               wxTreebookEventHandler(NotebookWidgetEvent::OnTreePageChanging), NULL,
               pEventHandler_);
         pBookCtrl_->Connect(
               wxEVT_COMMAND_TREEBOOK_PAGE_CHANGED,
               wxTreebookEventHandler(NotebookWidgetEvent::OnTreePageChanged), NULL,
               pEventHandler_);
         break;
   }
   if (!AddMainControl(pBookCtrl_)) {
      return false;
   }
   // Evita assert en debug porque notebook no implementa SetPageSize
   if (type_ != Notebook)
      pBookCtrl_->SetPageSize(wxSize(MINIMUM_PART_WIDTH, MINIMUM_PART_HEIGHT));
   if (GetFrame()) {
      GetFrame()->SetMinSize(
            wxSize(MINIMUM_PART_WIDTH + NOTEBOOK_CONTAINER_PLUS_WIDTH,
                   MINIMUM_PART_HEIGHT + NOTEBOOK_CONTAINER_PLUS_HEIGHT));
   }

   GET_CONTROL(*pToolWindow_, "wxID_APPLY", wxButton)
      ->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                wxCommandEventHandler(NotebookWidgetEvent::OnButtonApply), NULL,
                pEventHandler_);

   return true;
}

/**
 * Evento click sobre Boton Aplicar
 * @param[in] Event evento
 * \todo porque se ejecuta 2 veces (ver de donde llega este error)
 * \todo : no se esta teniendo en cuenta el valor de retorno de un apply,
 *  si bien los errores se manejar en cada part, se deberia contemplar el caso
 *  de que lo ultimo no pase
 */
void NotebookWidget::OnButtonApply(wxCommandEvent &Event) {

   ApplyChanges(pBookCtrl_->GetSelection());
   // porque se ejecuta 2 veces (\todo ver de donde llega este error)
   Event.Skip(false);
}

/**
 * Evento click sobre Boton BackWard
 * @param[in] Event evento
 */
void NotebookWidget::OnButtonBackward(wxCommandEvent &Event) {
#ifdef PREVENT_EVENT_REPETITION
   buttonEvent_ = true;
#endif
   pBookCtrl_->AdvanceSelection(false);
}

/**
 * Evento click sobre Boton ForWard
 * @param[in] Event evento
 */
void NotebookWidget::OnButtonForward(wxCommandEvent &Event) {
#ifdef PREVENT_EVENT_REPETITION
   buttonEvent_ = true;
#endif
   pBookCtrl_->AdvanceSelection();
}

/**
 * Evento para checkear el estado de la app. y habilitar o no los botones y los part
 * @param[in] Event evento UIUpdate
 */
void NotebookWidget::OnUIUpdate(wxUpdateUIEvent &Event) {
   int sel = pBookCtrl_->GetSelection();
   if (sel == 0) {
      GET_CONTROL(*pToolWindow_, "wxID_BACKWARD", wxButton)->Enable(false);
      GET_CONTROL(*pToolWindow_, "wxID_FORWARD", wxButton)->Enable(true);
   }
   if (sel > 0 && (sel + 1) < static_cast<int>(pBookCtrl_->GetPageCount())) {
      GET_CONTROL(*pToolWindow_, "wxID_BACKWARD", wxButton)->Enable(true);
      GET_CONTROL(*pToolWindow_, "wxID_FORWARD", wxButton)->Enable(true);
   }
   if ((sel + 1) == static_cast<int>(pBookCtrl_->GetPageCount())) {
      GET_CONTROL(*pToolWindow_, "wxID_BACKWARD", wxButton)->Enable(true);
      GET_CONTROL(*pToolWindow_, "wxID_FORWARD", wxButton)->Enable(false);
   }

   GET_CONTROL(*pToolWindow_, "wxID_OK", wxButton)->Enable(PartsValidData());
   GET_CONTROL(*pToolWindow_, "wxID_APPLY", wxButton)->Enable(PartsChanged());

   EnableWindows();
   SetPartsIconsState();

   Event.SetUpdateInterval(250);
}

// Separo las llamadas en la conexion de eventos
#ifdef __UNUSED_CODE__
/** Evento generico para los book */
void NotebookWidget::OnBookCtrl(wxBookCtrlBaseEvent &Event) {
   /*Declaro y cargo Vector de estructuras EventInfo, con los eventos de los
    * book. Solo cargo los eventos que se corresponden con el book utilizado.
    */
   static const struct EventInfo {
      wxEventType typeChanged, typeChanging;
   }events[] =
   {
      {
         wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
         wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,
      }, {
         wxEVT_COMMAND_LISTBOOK_PAGE_CHANGED,
         wxEVT_COMMAND_LISTBOOK_PAGE_CHANGING,
      }
   };

   // Recorro el vector buscando que evento se disparo.
   const wxEventType eventType = Event.GetEventType();
   for ( size_t n = 0; n < WXSIZEOF(events); n++ )
   {
      const EventInfo& ei = events[n];
      // eventType es una prop. de Event
      if ( eventType == ei.typeChanged )// ya cambio la pagina del book.
      {
         OnPageChanged(Event);
         break;
      }
      if ( eventType == ei.typeChanging )  // antes de cambiar la pagina del book.
      {
         OnPageChanging(Event);
         break;
      }
   }
}
#endif   // __UNUSED_CODE__
/**
 * Evento que se dispara cuando se esta cambiando de pagina del book
 * Si hay cambio pregunta que hacer y los aplica o descarta
 * Si hay cambios pero no son validos, pregunta si volver a los datos anteriores
 * o editar las modificaciones.
 * \note hay dos situaciones en las que se genera un veto al evento de
 * PAGE_CHANGING, en algunos casos el veto hace que el evento se dispare
 * nuevamente. Para evitar esta situacion, si esta el define PREVENT_EVENT_REPETITION
 * se agregan los flags que permiten obviar el segundo evento.
 * \todo ver si se puede evitar que el evento se dispare dos veces.
 * @param[in] Event evento
 */
void NotebookWidget::OnPageChanging(wxBookCtrlBaseEvent &Event) {
   Event.Skip(false);
   // ignora el evento con 1 sola pagina presente
   if (!(pBookCtrl_->GetPageCount() > 1)) {
      return;
   }
   int pageselected = Event.GetOldSelection();
   Part *ppart = GetPart(pageselected);

#ifdef PREVENT_EVENT_REPETITION
   if ( eventVetoedInApplyChangesQuestion_ || eventVetoedInInvalidDataQuestion_ )
   {
      Event.Veto();
      eventVetoedInApplyChangesQuestion_ = false;
      eventVetoedInInvalidDataQuestion_ = false;
      return;
   }
#endif

   if (!ppart) {
      REPORT_DEBUG("D: Parte invalida");
      return;
   }

   if (ppart->HasChanged()) {
      if (ppart->HasValidData()) {
         switch(SHOWQUERY(GetWindow(), _(question_APPLY_PART_CHANGES),
                            wxYES|wxNO|wxCANCEL|wxICON_QUESTION))
               {
                  case wxID_YES:
                  ppart->CommitChanges();
                  break;
                  case wxID_NO:
                  ppart->RollbackChanges();
                  ppart->GetWidget()->GetWindow()->Layout();
                  break;
                  case wxID_CANCEL:
                  Event.Veto();
#ifdef PREVENT_EVENT_REPETITION
                  if ( !buttonEvent_ ) {
                     eventVetoedInApplyChangesQuestion_= true;
                  }
#endif
                  break;
                  default:
                  break;
               }
            } else
            {
               switch ( SHOWQUERY(GetWindow(), _(question_INVALID_DATA),
                           wxYES|wxNO|wxICON_QUESTION) )
               {
                  case wxID_YES:
                  ppart->RollbackChanges();
                  ppart->GetWidget()->GetWindow()->Layout();
                  break;
                  case wxID_NO:
                  Event.Veto();
#ifdef PREVENT_EVENT_REPETITION
                  if ( !buttonEvent_ ) {
                     eventVetoedInInvalidDataQuestion_= true;
                  }
#endif
                  break;
                  default:
                  break;
               }
            }
         }
#ifdef PREVENT_EVENT_REPETITION
         buttonEvent_=false;
#endif
      }

/**
 * Evento que se dispara cuando se cambio de pagina del book
 * \attention : este metodo se cambia para que cuando se vuelve una pagina
 * no se carguen los valores iniciales sino los ultimos datos validos.
 * Las partes anteriores no se perjudican, porque su implementacion del
 * metodo RollbackChanges llama a SetInitialValues, y SetInitialValues carga
 * en el control los ultimos datos validos.
 * @param[in] Event evento
 */
void NotebookWidget::OnPageChanged(wxBookCtrlBaseEvent &Event) {
   Part *ppart = GetPart(Event.GetSelection());
   if (ppart) {
      ppart->Update();
      /*
       * La llamada parece no tener sentido ya que los controles
       * quedan en memoria. El estado de los objetos no se pierde.
       * En algunos casos esta llamada puede hacer que se re-creen
       * todos los controles de una ventana devuelta.
       * \todo (danieltesta-#5600): verificar en general NotebookWidget
       * para ver si todos los metodos, estados y llamados estan bien.
       * Chequear si hay llamadas de mas, la logica de las llamadas,
       * etc, etc.
       */
      //ppart->RollbackChanges();
      ppart->GetWidget()->GetWindow()->Layout();
   }
   pBookCtrl_->GetPage(Event.GetSelection())->SetFocus();
}

void NotebookWidget::OnChildFocus(wxChildFocusEvent &Event) {
   Event.Skip(false);
   return;
}

/**
 * Retorna la ventana padre de las partes.
 * @return puntero a la ventana padre de las partes
 */
wxWindow * NotebookWidget::GetPartParent() {
   return pBookCtrl_;
}

// ------------------------------------------------------------------------------
// -----------------------------------PRIVADAS-----------------------------------
// ------------------------------------------------------------------------------
/**
 * Objetivo: Agrega un Part al Notebook.
 * @param[in]	pPart: Part a agregar.
 * @return agrega_ok? true:false.
 */
bool NotebookWidget::DoInsert(suri::Part *pPart) {
   if (!pPart) {
      return false;
   }

   int iconsize = 32;
   if (type_ == Notebook) {
      iconsize = 16;
   }

   wxImageList *pimlist = pBookCtrl_->GetImageList();
   if (!pimlist) {
      /* Nota: si el tamanio de la lista, o sea, el ancho y alto de las imagenes
       * que se van a guardar en la lista, es mas grande que la imagen, el
       * metodo Add no agrega la imagen.
       */
      pimlist = new wxImageList(iconsize, iconsize);
      pBookCtrl_->AssignImageList(pimlist);
   }
   wxBitmap bmp = wxNullBitmap;

   pPart->GetWidget()->GetWindowIcon(bmp, iconsize, iconsize);
   pimlist->Add(bmp);
   // Agrega una version del icono en escala de grises.
   wxImage imgingrey = bmp.ConvertToImage();
   imgingrey = imgingrey.ConvertToGreyscale();
   pimlist->Add(imgingrey);
// agregarlo asi hace que no se respete bien el tamano del Part
#ifndef __UNUSED_CODE__
   wxScrolledWindow *ptemppanel = new wxScrolledWindow(pBookCtrl_);
   ptemppanel->SetSizer(new wxBoxSizer(wxVERTICAL));
   ptemppanel->SetScrollRate(5, 5);
   pBookCtrl_->AddPage(ptemppanel, wxEmptyString, false, pimlist->GetImageCount() - 1);
   AddControl(pPart->GetWidget(), ptemppanel);
   pBookCtrl_->SetPageText(pBookCtrl_->GetPageCount() - 1,
                           WrapTitle(pPart->GetWidget()->GetWindowTitle()));
   ptemppanel->Connect(wxEVT_CHILD_FOCUS,
                       wxChildFocusEventHandler(NotebookWidgetEvent::OnChildFocus),
                       NULL, pEventHandler_);
   return true;
#else
   return pBookCtrl_->AddPage(pPart->GetWidget()->GetWindow(), WrapTitle(pPart->GetWindowTitle() ),
         false, pimlist->GetImageCount()-1);
#endif
}

/**
 * Objetivo: Elimina el Part del Notebook.
 * @param[in]	pPart: Part a eliminar.
 * @return elimina_ok? true:false.
 */
bool NotebookWidget::DoRemove(suri::Part *pPart) {
   if (!pPart) {
      return false;
   }
   return pBookCtrl_->DeletePage(GetPartIndex(pPart));
}

/**
 * Metodo que cambia el icono de la parte segun este habilitada o no.
 * Muestra el icono en color si la parte esta habilitada y en escala de grises
 * si no esta habilitada.
 */
void NotebookWidget::SetPartsIconsState() {
   /* se hace este return porque el Choicebook, a pesar que permite asignarle
    * una lista de imagenes, no tiene implementado el metodo SetPageImage =>
    * falla la ejecucion.
    */
   if (type_ == Choicebook) {
      return;
   }

   if (pBookCtrl_->GetImageList()) {
      bool partenable;
      int partindex, imageindex;

      std::list<Part *>::iterator it;
      for (it = partList_.begin(); it != partList_.end(); it++) {
         partenable = (*it)->IsEnabled();
         partindex = GetPartIndex(*it);
         if (partindex != -1) {
            imageindex = partindex * 2;
            if (!partenable) {
               imageindex += 1;
            }
            if (imageindex < pBookCtrl_->GetImageList()->GetImageCount()) {
               pBookCtrl_->SetPageImage(partindex, imageindex);
            }
         }
      }
   }
}

/**
 * Acomoda los textos de las partes para que haya newlines
 * @param[in] Title titulo
 * @return string con el titulo con newlines
 */
wxString NotebookWidget::WrapTitle(const wxString &Title) const {
   if (Title.empty()) {
      return Title;
   }
#ifdef __WXLISTBOOK_DOESNT_WRAP_TITLES__
   std::vector<std::string> words = split<std::string>(Title.c_str());
   size_t maxchars =
         (*std::max_element(words.begin(), words.end(), LessThanSize)).size();
   std::vector<std::string> joinedwords;
   std::vector<std::string>::iterator it = words.begin();
   std::string accum;
   do {
      accum.clear();
      while (it != words.end() && accum.length() + (*it).length() <= maxchars)
         accum += *(it++) + " ";
      joinedwords.push_back(trim(accum, " "));
   } while (it != words.end());
   maxchars =
         (*std::max_element(joinedwords.begin(), joinedwords.end(), LessThanSize)).size();
   it = joinedwords.begin();
   while (it != joinedwords.end()) {
      double extraspaces = std::abs(static_cast<int>(maxchars - (*it).length()))
            / 2.0 * WHITESPACE_SIZE_MULTIPLIER;
      *it = std::string(SURI_ROUND(int, extraspaces), ' ') + *it;
      it++;
   }
   return join(joinedwords.begin(), joinedwords.end(), "\n").c_str();
#else
   return Title;
#endif   // __WXLISTBOOK_DOESNT_WRAP_TITLES__
}
}  // namespace suri
// ------------------------------------------------------------------------------
// ------------------------FIN CLASE NotebookWidget---------------------------
// ------------------------------------------------------------------------------
