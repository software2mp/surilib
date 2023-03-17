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

#include "suri/PartContainerWidget.h"

// Includes suri
#include "suri/messages.h"
#include "suri/Configuration.h"
#include "logmacros.h"
#include "resources.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/imaglist.h"
#include "wx/html/helpctrl.h"

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(PartContainerWidgetEvent, PartContainerWidget)
   IMPLEMENT_EVENT_CALLBACK(OnButtonOk, OnButtonOk(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonApply, OnButtonApply(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonCancel, OnButtonCancel(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonBackward, OnButtonBackward(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonForward, OnButtonForward(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnHelpButton, OnHelpButton(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnUIUpdate, OnUIUpdate(event), wxUpdateUIEvent)
   IMPLEMENT_EVENT_CALLBACK(OnClose, OnClose(event), wxCloseEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Ctor. default.
 * @param[in] Title titulo de la ventana
 * @param[in] VisibleButtons flag para determinar botones visibles
 * @param[in] EnabledButtons flag para determinar botones activos
 */
PartContainerWidget::PartContainerWidget(const wxString &Title,
                                         const long VisibleButtons,
                                         const long EnabledButtons) :
      partList_(std::list<suri::Part*>()), visibleButtons_(VisibleButtons),
      enabledButtons_(EnabledButtons), NEW_EVENT_OBJECT(PartContainerWidgetEvent)
{
   windowTitle_ = Title;
}

/**
 * Ctor. con un part
 * Recibe un part y lo agrega al container
 * @param[in] pUniquePart part
 * @param[in] Title titulo de la ventana
 * @param[in] VisibleButtons flag para determinar botones visibles
 * @param[in] EnabledButtons flag para determinar botones activos
 */
PartContainerWidget::PartContainerWidget(Part *pUniquePart, const wxString &Title,
                                         const long VisibleButtons,
                                         const long EnabledButtons) :
      partList_(std::list<suri::Part*>()), visibleButtons_(VisibleButtons),
      enabledButtons_(EnabledButtons), NEW_EVENT_OBJECT(PartContainerWidgetEvent)
{
   windowTitle_ = Title;
   CreateTool(NULL);
   InsertPart(pUniquePart);
   // Fija el tamano minimo al del control agregado y centra la ventana
   wxTopLevelWindow *pframe = GetFrame();
   if (pframe) {
      wxSize size = pframe->GetSize();
      pframe->SetMinSize(size);
      pframe->Center();
   }
}

/**
 * Destructor
 */
PartContainerWidget::~PartContainerWidget() {
   if (GetFrame()) {
      GetFrame()->Disconnect(wxEVT_CLOSE_WINDOW,
                             wxCloseEventHandler(PartContainerWidgetEvent::OnClose),
                             NULL, pEventHandler_);
   } else if (pToolWindow_) {
      pToolWindow_->GetParent()->Disconnect(
            wxEVT_CLOSE_WINDOW, wxCloseEventHandler(PartContainerWidgetEvent::OnClose),
            NULL, pEventHandler_);
   }
   DELETE_EVENT_OBJECT;
}

/**
 * Se crea la ventana de herramienta en las clases derivadas
 * Crea la ventana, conecta los botones de control y muestra los requeridos
 * @return bool que indica si pudo crear la ventana (true siempre)
 */
bool PartContainerWidget::CreateToolWindow() {
   pToolWindow_ = GetToolWindow();
   // Esconde los botones seleccionados
   ShowButtons();
   // Conecto los eventos a los controles.
   GET_CONTROL(*pToolWindow_, "wxID_OK", wxButton)
      ->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                wxCommandEventHandler(PartContainerWidgetEvent::OnButtonOk), NULL,
                pEventHandler_);
   CONNECT_TOOL_EVENT(PartContainerWidgetEvent, OnUIUpdate, wxEVT_UPDATE_UI,
                                                              wxUpdateUIEventHandler);
   GET_CONTROL(*pToolWindow_, "wxID_APPLY", wxButton)
      ->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                wxCommandEventHandler(PartContainerWidgetEvent::OnButtonApply), NULL,
                pEventHandler_);
   GET_CONTROL(*pToolWindow_, "wxID_CANCEL", wxButton)
      ->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                wxCommandEventHandler(PartContainerWidgetEvent::OnButtonCancel), NULL,
                pEventHandler_);
   GET_CONTROL(*pToolWindow_, "wxID_BACKWARD", wxButton)
      ->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                wxCommandEventHandler(PartContainerWidgetEvent::OnButtonBackward), NULL,
                pEventHandler_);
   GET_CONTROL(*pToolWindow_, "wxID_FORWARD", wxButton)
      ->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                wxCommandEventHandler(PartContainerWidgetEvent::OnButtonForward), NULL,
                pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_HELP_BUTTON", wxBitmapButton)
      ->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                wxCommandEventHandler(PartContainerWidgetEvent::OnHelpButton), NULL,
                pEventHandler_);

   if (GetFrame()) {
      GetFrame()->Connect(wxEVT_CLOSE_WINDOW,
                          wxCloseEventHandler(PartContainerWidgetEvent::OnClose), NULL,
                          pEventHandler_);
   } else {
      pToolWindow_->GetParent()->Connect(
            wxEVT_CLOSE_WINDOW, wxCloseEventHandler(PartContainerWidgetEvent::OnClose),
            NULL, pEventHandler_);
   }
   return true;
}

/**
 *  Crea el panel principal que se asigna a pToolWindow
 *
 * @return Ventana para usar como panel principal de la ventana. Debe tener
 *         un control de tipo wxPanel cuyo ID sea ID_CONTAINER_WIN
 */
wxWindow *PartContainerWidget::GetToolWindow() {
   return wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                          wxT("ID_PANEL_CONTAINER"));
}

/**
 * Objetivo: Agrega el control principal que contendra a los Part's.
 * @param[in]	pControlWindow: si se esta construyendo un PartContainerWidget con
 *             un unico Part, este parametro es el mismo Part. Si se esta
 *             construyendo un PartContainerWidget con varios Part's, este
 *             parametro es el control que contendra a los Part's.
 * @return bool que indica si se pudo agregar la ventana del part.
 */
bool PartContainerWidget::AddMainControl(wxWindow *pControlWindow) {
   return AddControl(pControlWindow,
                     XRCCTRL(*pToolWindow_, wxT("ID_CONTAINER_WIN"), wxWindow));
}

/**
 * Agrega el control que contendra a los Part's.
 * @param[in] pTool control que contendra los parts
 * @return bool que indica si tuvo exito
 */
bool PartContainerWidget::AddMainControl(Widget *pTool) {
   return AddControl(pTool, wxT("ID_CONTAINER_WIN"));
}

/**
 * Objetivo: Inserta un Part al final de la lista de Part si este no es NULL.
 * @param[in]	pPart: Part.
 * @return inserta_ok? true:false
 * \todo ver por que cuelga a veces en linux
 */
bool PartContainerWidget::InsertPart(suri::Part *pPart) {
   if (!pPart) {
      return false;
   }
   REPORT_DEBUG("D: Insertando %s(%p)",
                 pPart->GetWidget()->GetWindowTitle().c_str(), pPart);

   // Si no pudo crear la herramienta no la agrega a el part.
   if (!pPart->CreatePartTool(GetPartParent())) {
      REPORT_AND_FAIL_VALUE("D:Error al crear la herramienta.", false);
   }

   // Ajusta la ventana cada vez que se agrega un Part
   wxTopLevelWindow *pframe = GetFrame();
   pframe->Freeze();

   bool retval = false;
   if (DoInsert(pPart)) {
      partList_.push_back(pPart);
      // \todo ver por que cuelga a veces en linux
      pPart->SetInitialValues();
      if (pframe) {
         pframe->Layout();
         pframe->Fit();
// Fuerza que se acomoden los controles.
// En MSW no parece darle bola al Fit y/o Layout, por eso le cambio el tamano
// y lo vuelvo al que tenia antes.
// Debuggeando se podria ver que hace SetSize que fuerza que se reacomoden los
// controles en forma correcta.
#ifdef __WXMSW__
         wxSize size = pframe->GetSize();
         pframe->SetSize(wxSize(size.GetWidth()+1, size.GetHeight()+1) );
         pframe->SetSize(wxSize(size.GetWidth(), size.GetHeight() ) );
#endif
      }
      retval = true;
   }
   pframe->Thaw();
   return retval;
}

/**
 * Objetivo: Elimina un Part de la lista de Part's.
 * @param[in]	pPart: puntero al Part que quiere eliminarse.
 * @return elimina_part? true: false;
 */
bool PartContainerWidget::RemovePart(Part *pPart) {
   if (!pPart || CountParts() == 0) {
      return false;
   }

   // busco el Part a eliminar
   std::list<Part *>::iterator it;
   for (it = partList_.begin(); it != partList_.end() && ((*it) != pPart); it++)
      ;

   if (it == partList_.end()) {  // no encuentra el Part
      return false;
   }

   if (!DoRemove(pPart)) {
      return false; /*TODO(06/11/2008 - Alejandro): Ver de emitir msj.*/
   }
   partList_.erase(it);
   if (CountParts() == 0) {
      partList_.clear();
   }

   return true;
}

/**
 * Devuelve la cantidad de Part's que tiene el Container.
 * @return cantidad de parts que tiene el container
 */
int PartContainerWidget::CountParts() {
   return partList_.size();
}

/**
 * Retorna la ventana padre de las partes.
 * @return puntero a la ventana padre de las partes
 */
wxWindow *PartContainerWidget::GetPartParent() {
   return GetWindow();
}

/**
 *  Retorna un bitmap con el icono de la ventana.
 *  @param[out] ToolBitmap icono de la ventana
 *  @param[in] X ancho del icono
 *  @param[in] Y alto del icono
 */
void PartContainerWidget::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(wxT(icon_FRAME), ToolBitmap);
}

/**
 * Objetivo: Devuelve el Part con el indice indicado. No lo elimina de la lista.
 * @param[in]	IndexPart: indice del Part dentro de la lista.
 * @return copia del Part que se encuentra en el indice IndexPart o NULL.
 */
Part *PartContainerWidget::GetPart(const int &IndexPart) {
   if (CountParts() == 0 || IndexPart < 0 || IndexPart > CountParts()) {
      return NULL;
   }

   // Busco el Part solicitado.
   std::list<Part *>::iterator it = partList_.begin();
   for (int i = 0; it != partList_.end() && i != IndexPart; it++, i++)
      ;
   if (it == partList_.end()) {  // no encuentra el Part
      return NULL;
   }

   return (*it);
}

/**
 * Objetivo: Devuelve el indice del Part, recibido por parametro, dentro de la
 * lista.
 * @param[in]	pPart: Part.
 * @return encuentra_Part? >0 : -1
 */
int PartContainerWidget::GetPartIndex(const suri::Part *pPart) {
   if (!pPart) {
      return -1;
   }

   std::list<Part *>::iterator it;
   int i;
   for (i = 0, it = partList_.begin(); it != partList_.end() && ((*it) != pPart);
         it++, i++)
      ;

   if (it == partList_.end()) {  // no encuentra el Part
      return -1;
   }

   return i;
}

/**
 * Aplica los cambios en los Part.
 * @return true si todos los part con cambios pudieron hacer commit, false si
 *             algun part con cambios fallo al hacer commit
 */
bool PartContainerWidget::ApplyChanges() {
   std::list<Part *>::iterator it = partList_.begin();
   for (; it != partList_.end(); it++)
      if ((*it)->HasChanged() && !(*it)->CommitChanges()) {
         return false;
      }
   return true;
}

/**
 * Aplica los cambios en un Part.
 * @param[in] IndexPart indice del part sobre el que se quieren aplicar los cambios
 * @return true si pudo aplicar los cambios en el part
 * @return false si no se encontro el part indicado, o fallo al aplicar cambios
 */
bool PartContainerWidget::ApplyChanges(const int &IndexPart) {
   if (CountParts() == 0 || IndexPart < 0 || IndexPart > CountParts()) {
      return false;
   }

   Part *ppart = GetPart(IndexPart);
   if (!ppart) {
      return false;
   }

   if (!ppart->CommitChanges()) {
      return false;
   }
   return true;
}

/**
 * Descarta los cambios en los Part.
 * @return true si todos los part del container pudieron hacer rollback
 * @return false si algun part fallo al hacer rollback
 */
bool PartContainerWidget::DiscardChanges() {
   std::list<Part *>::iterator it = partList_.begin();
   for (; it != partList_.end(); it++) {
      if (!(*it)->RollbackChanges()) {
         return false;
      }
      // Si no se realiza esta verificacion ocurre segmentation
      // fault al cancelar con cambios en tabla de vector. Ver TCK 611
      if (Widget::IsValid(*it)) {
         (*it)->GetWidget()->GetWindow()->Layout();
      }
   }
   return true;
}

/**
 * Descarta los cambios en un Part.
 * @param[in] IndexPart indice del part del que se quieren descartar cambios
 * @return true si se encontro el Part y pudo hacer rollback, false en otro caso
 */
bool PartContainerWidget::DiscardChanges(const int &IndexPart) {
   if (CountParts() == 0 || IndexPart < 0 || IndexPart > CountParts()) {
      return false;
   }

   Part *ppart = GetPart(IndexPart);
   if (!ppart) {
      return false;
   }

   if (!ppart->RollbackChanges()) {
      return false;
   }

   ppart->GetWidget()->GetWindow()->Layout();
   return true;
}

/**
 * Evento click sobre Boton Ok
 * @param[in] Event evento
 */
void PartContainerWidget::OnButtonOk(wxCommandEvent &Event) {
   Event.Skip(false);
   if (ApplyChanges()) {
      EndModal(wxID_OK);
   }
}

/**
 * Evento click sobre Boton Apply
 * @param[in] Event evento
 */
void PartContainerWidget::OnButtonApply(wxCommandEvent &Event) {
   if (CountParts() == 1) {
      if (!ApplyChanges()) {
         SHOW_ERROR(message_PART_SAVE_ERROR);
         Event.StopPropagation();
      }
   }
}

/**
 * Evento click sobre Boton Cancel.
 *
 * @param[in] Event evento
 */
void PartContainerWidget::OnButtonCancel(wxCommandEvent &Event) {
   // TCK #8274: cambio que partio de un arreglo para Simur.
   if (PartsChanged()) {
      /*Solo preguntar cuando haya cambios en la ventana*/
      VerifyExit(Event);
   } else {
      EndModal(wxID_CANCEL);
   }
}

/**
 * Evento click sobre Boton BackWard
 * @param[in] Event evento
 */
void PartContainerWidget::OnButtonBackward(wxCommandEvent &Event) {
}

/**
 * Evento click sobre Boton ForWard
 * @param[in] Event evento
 */
void PartContainerWidget::OnButtonForward(wxCommandEvent &Event) {
}

/** Evento click sobre Boton de ayuda */
void PartContainerWidget::OnHelpButton(wxCommandEvent &Event) {
   std::string helpdir = suri::Configuration::GetParameter("app_help_dir", wxT("./"));
   helpdir += "ayuda.hhp";
   wxHtmlHelpController* phelpcontroller = new wxHtmlHelpController;
   phelpcontroller->AddBook(wxFileName(helpdir), false);
   phelpcontroller->Display(0);
   phelpcontroller->SetFrameParameters(_("Ayuda: %s"), wxSize(1024, 600));
}

/**
 * Evento update que verifica si hay que habilitar el boton OK y el boton Apply,
 * cuando uno de los Parts tiene cambios.
 * @param[in] Event evento UIUpdate
 * \todo Refactorear para hacer uso correcto del wxUpdateUIEvent
 */
void PartContainerWidget::OnUIUpdate(wxUpdateUIEvent &Event) {
   if (enabledButtons_ & SUR_BTN_OK) {
      GET_CONTROL(*pToolWindow_, "wxID_OK", wxButton)->Enable(true);
   } else {
      GET_CONTROL(*pToolWindow_, "wxID_OK", wxButton)->Enable(PartsValidData());
   }
   if (enabledButtons_ & SUR_BTN_APPLY) {
      GET_CONTROL(*pToolWindow_, "wxID_APPLY", wxButton)->Enable(PartsChanged());
   }

   Event.SetUpdateInterval(100);
}

/**
 * Antes de cerrar el container verifica si los Parts tienen cambios. Pregunta
 * si guardar los cambios y los guarda o descarta.
 * @param[in] Event evento Close
 */
void PartContainerWidget::OnClose(wxCloseEvent &Event) {
   // TCK #8274: cambio que partio de un arreglo para Simur.
   if (PartsChanged()) {
      /*Solo preguntar cuando haya cambios en la ventana*/
      VerifyExit(Event);
   } else {
      EndModal(wxID_CANCEL);
   }
}

/**
 * Decide que botones mostrar segun visibleButtons_
 */
void PartContainerWidget::ShowButtons() {
   // Muestro u Oculto los botones
   GET_CONTROL(*pToolWindow_, "wxID_OK", wxButton)
                                      ->Show(visibleButtons_ & SUR_BTN_OK);
   GET_CONTROL(*pToolWindow_, "wxID_CANCEL", wxButton)
                                      ->Show(visibleButtons_ & SUR_BTN_CANCEL);
   GET_CONTROL(*pToolWindow_, "wxID_APPLY", wxButton)
                                      ->Show(visibleButtons_ & SUR_BTN_APPLY);
   GET_CONTROL(*pToolWindow_, "wxID_BACKWARD", wxButton)
                                      ->Show(visibleButtons_ & SUR_BTN_BACKWARD);
   GET_CONTROL(*pToolWindow_, "wxID_FORWARD", wxButton)
                                      ->Show(visibleButtons_ & SUR_BTN_FORWARD);
   // Habilito o Deshabilito los botones
   GET_CONTROL(*pToolWindow_, "wxID_OK", wxButton)
                                      ->Enable(enabledButtons_ & SUR_BTN_OK);
   GET_CONTROL(*pToolWindow_, "wxID_CANCEL", wxButton)
                                      ->Enable(enabledButtons_ & SUR_BTN_CANCEL);
   GET_CONTROL(*pToolWindow_, "wxID_APPLY", wxButton)
                                      ->Enable(enabledButtons_ & SUR_BTN_APPLY);
   GET_CONTROL(*pToolWindow_, "wxID_BACKWARD", wxButton)
                                      ->Enable(enabledButtons_ & SUR_BTN_BACKWARD);
   GET_CONTROL(*pToolWindow_, "wxID_FORWARD", wxButton)
                                      ->Enable(enabledButtons_ & SUR_BTN_FORWARD);
   pToolWindow_->Layout();
}

// ------------------------------------------------------------------------------
// -----------------------------------PRIVADAS-----------------------------------
// ------------------------------------------------------------------------------
/**
 * Objetivo: Verifica si alguno de los Part que contiene fue modificado.
 * @return al menos 1 Part modificado? true : false.
 */
bool PartContainerWidget::PartsChanged() {
   std::list<Part *>::iterator it;
   for (it = partList_.begin(); it != partList_.end(); it++)
      if ((*it)->HasChanged()) {
         return true;
      }

   return false;
}

/**
 * Objetivo: Verifica si el Part que se encuentra en la posicion IndexPart(base
 * cero) fue modificado.
 * @param[in]	IndexPart: indice del Part a consultar.(base cero).
 * @return Part modificado? true : false.
 */
bool PartContainerWidget::PartsChanged(const int &IndexPart) {
   if (CountParts() == 0 || IndexPart < 0 || IndexPart > CountParts()) {
      return false;
   }

   Part *ppart = GetPart(IndexPart);
   if (!ppart) {
      return false;
   }

   return ppart->HasChanged();
}

/**
 * Implementar en Clases derivadas para agregar Part al Container.
 * @param[in] pPart part que se quiere agregar
 * @return bool que indica si tuvo exito
 */
bool PartContainerWidget::DoInsert(suri::Part *pPart) {
   return AddMainControl(pPart->GetWidget());
}

/**
 * Implementar en Clases derivadas para eliminar Part del Container.
 * @param[in] pPart part que se quiere eliminar del contenedor
 * @return true siempre
 */
bool PartContainerWidget::DoRemove(suri::Part *pPart) {
   return true;
}

/**
 * Verifica que todas las partes tengan datos validos.
 * @return true : si todas las partes tienen datos validos.
 * @return false : cuando encuentra una parte que no tiene datos validos.
 */
bool PartContainerWidget::PartsValidData() {
   std::list<Part *>::iterator it;
   for (it = partList_.begin(); it != partList_.end(); it++)
      if (!(*it)->HasValidData()) {
         return false;
      }

   return true;
}

/**
 * Metodo que recoore las partes y las in/habilita segun el estado del atributo
 * enabled. Las partes inhabilitadas se muestran en gris y no permiten entrada
 * de datos.
 */
void PartContainerWidget::EnableWindows() {
   std::list<Part *>::iterator it;
   for (it = partList_.begin(); it != partList_.end(); it++)
      if ((*it)->GetWidget()->GetWindow()) {
         (*it)->GetWidget()->GetWindow()->Enable((*it)->IsEnabled());
      }
}

/**
 * Verifica si el usuario quiere salir.
 * (boton de cerrar de la ventana y boton cancelar)
 */
void PartContainerWidget::VerifyExit(wxEvent& Event) {
   Event.Skip(false);
   /*Al salir, se pregunta al usuario si quiere guardar los cambios*/
   int res = SHOWQUERY(GetWindow(), _(question_APPLY_PART_CHANGES),
                       wxYES | wxNO | wxCANCEL | wxICON_QUESTION);
   /*En caso afirmativo, se aplican los cambios*/
   if (res == wxID_YES) {
      ApplyChanges();
   }
   /*En caso negativo, se descartan los cambios hechos*/
   if (res == wxID_NO){
      DiscardChanges();
   }
   /*Se cierra la ventana, salvo que se haya apretado "Cancel"*/
   if(res != wxID_CANCEL)
      EndModal (res);
}

}  // namespace suri

/** @mainpage
 *
 * @page "Part y PartContainder" Tutorial PartContainer: HelloWorldContainer
 *
 *  Este tutorial explica como crear una ventana de tipo PartContainer y sus
 * respectivos Part.
 *
 *  Las clases derivadas de suri::Part representan una parte de un dialogo de
 * configuracion o informacion. Pueden ser utilizadas en conjunto o en forma
 * individual. suri::Part provee funcionalidad de verificacion, vuelta atras y
 * aplicacion de cambios.
 *  Las clases derivadas de suri::PartContainer representan el contenedor que
 * soporta a las suri::Part y que posee la logica de como integrar los datos
 * de cada una de las partes.
 *
 *  El tutorial explica como crear un suri::PartContainer de tipo Notebook/Wizard
 * para la configuracion de la aplicacion.
 *
 *  No es necesario crear una clase que herede de suri::PartContainer ya que
 * la logica de configuracion recae en cada uno de los suri::Part.
 *  El primer suri::Part a crear sera el de parametros generales de la aplicacion
 * en particular los siguientes items:
 *  - lib_supported_image_formats : Formatos de imagenes soportados
 *  - app_language : idioma de la aplicacion
 *  - app_max_number_of_context : El numero maximo de contextos permitidos
 *  - tool_always_use_vincenty_distance : Opcion para utilizar el algoritmo
 *   de Vincenty para calculo de distancias en proyecciones geograficas.
 *
 *  La nueva clase debe heredar de suri::Part e implementar los metodos abstractos:
 * @code
 #ifndef APPLICATIONCONFIGPART_H_
 #define APPLICATIONCONFIGPART_H_

 #include "suri/Part.h"

 class wxSpinCtrl;
 class wxChoice;
 class wxCheckBox;

 class ApplicationConfigPart: public suri::Part
 {
 public:
 ApplicationConfigPart();
 virtual ~ApplicationConfigPart();

 virtual bool CreateToolWindow();
 virtual bool HasChanged();
 virtual bool CommitChanges();
 virtual bool RollbackChanges();
 virtual void SetInitialValues();
 virtual bool HasValidData();
 virtual void Update();
 private:
 wxTextCtrl *pImageFormats_;
 wxChoice *pLanguageChoice_;
 wxSpinCtrl *pMaxContexts_;
 wxCheckBox *pUseVincenty_;
 std::string formats_;
 std::string language_;
 long int maxContexts_;
 bool vincentyDistance_;
 };

 #endif
 * @endcode
 *
 *  Ademas de los metodos abstractos se reimplementa suri::Part::HasValidData
 * que permite definir si los datos ingresados son validos.
 *  Las propiedades agregadas funcionan como ayuda para interactuar con los
 * controles graficos y a modo de almacenamiento de datos intermedios.
 *
 *  El paso siguiente es la implementacion de estos metodos, el primero es
 * CreateToolWindow, heredado de suri::Widget y que es en el que se construyen
 * los controles.
 *
 * @code
 bool ApplicationConfigPart::CreateToolWindow()
 {
 wxPanel* ppanel = new wxPanel(pParentWindow_,wxID_ANY);

 wxBoxSizer* psizer = new wxBoxSizer(wxVERTICAL);
 ppanel->SetSizer(psizer);

 wxFlexGridSizer *psubsizer = new wxFlexGridSizer(2,4,4);
 psubsizer->AddGrowableCol(1,1);
 psizer->Add(psubsizer,1,wxEXPAND,0);
 wxString labeltext = _("Formatos Soportados:");
 wxStaticText* ptextctrl = new wxStaticText(ppanel,wxID_STATIC,labeltext);
 psubsizer->Add(ptextctrl,0,wxALIGN_LEFT|wxALL,5);
 pImageFormats_ = new wxTextCtrl(ppanel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_RIGHT);
 psubsizer->Add(pImageFormats_,0,wxEXPAND|wxALL,5);

 labeltext = _("Idioma:");
 ptextctrl = new wxStaticText(ppanel,wxID_STATIC,labeltext);
 psubsizer->Add(ptextctrl,0,wxEXPAND|wxALL,5);
 wxArrayString languages;
 languages.Add(wxT("spanish"));
 languages.Add(wxT("english"));
 languages.Add(wxT("portuguese"));
 pLanguageChoice_ = new wxChoice(ppanel,wxID_ANY,wxDefaultPosition,wxDefaultSize,languages);
 psubsizer->Add(pLanguageChoice_,0,wxEXPAND|wxALL,5);

 labeltext = _("# de contextos maximos:");
 ptextctrl = new wxStaticText(ppanel,wxID_STATIC,labeltext);
 psubsizer->Add(ptextctrl,0,wxALIGN_LEFT|wxALL,5);
 pMaxContexts_ = new wxSpinCtrl(ppanel,wxID_ANY);
 psubsizer->Add(pMaxContexts_,0,wxEXPAND|wxALL,5);

 labeltext = _("Usar distancia de Vincenty:");
 ptextctrl = new wxStaticText(ppanel,wxID_STATIC,labeltext);
 psubsizer->Add(ptextctrl,0,wxALIGN_LEFT|wxALL,5);
 pUseVincenty_ = new wxCheckBox(ppanel,wxID_ANY,wxEmptyString);
 psubsizer->Add(pUseVincenty_,0,wxEXPAND|wxALL,5);

 pToolWindow_ = ppanel;
 return true;
 }
 * @endcode
 *
 *  Este metodo crea un panel y lo llena con los controles adecuados para el tipo
 * de dato que se desea representar.
 *
 *  El siguiente metodo a implementar es suri::Part::SetIninitialValues que
 * debe leer los parametros de la configuracion y volcarlos a los controles
 * graficos.
 *
 * @code
 void ApplicationConfigPart::SetInitialValues()
 {
 formats_ = suri::Configuration::GetParameter("lib_supported_image_formats",formats_);
 language_ = suri::Configuration::GetParameter("app_language",language_);
 maxContexts_ = suri::Configuration::GetParameter("app_max_number_of_context",maxContexts_);
 vincentyDistance_ = suri::Configuration::GetParameter("tool_always_use_vincenty_distance",vincentyDistance_);
 RollbackChanges();
 }
 * @endcode
 *
 *  Como se puede ver, en realidad, el metodo solo carga los valores originales
 * en las propiedades y llama a suri::Part::RollbackChanges que en este caso
 * es el que toma dichas propiedades y llena los controles.
 *
 * @code
 bool ApplicationConfigPart::RollbackChanges()
 {
 pImageFormats_->ChangeValue(wxString(formats_));
 pLanguageChoice_->SetStringSelection(wxString(language_));
 pMaxContexts_->SetValue(wxString::Format("%ld",maxContexts_));
 pUseVincenty_->SetValue(vincentyDistance_);
 return true;
 }
 * @endcode
 *
 *  El metodo suri::Part::HasChanged debe indicar si se ha modificado algun valor
 * de la configuracion.
 *
 * @code
 bool ApplicationConfigPart::HasChanged()
 {
 return pImageFormats_->IsModified() ||
 language_!=pLanguageChoice_->GetStringSelection().c_str() ||
 maxContexts_ != pMaxContexts_->GetValue() ||
 vincentyDistance_ != pUseVincenty_->GetValue();
 }
 * @endcode
 *
 *  La validacion de los datos se realiza en el metodo suri::Part::HasValidData.
 *
 * @code
 bool ApplicationConfigPart::HasValidData()
 {
 int maxcontexts = pMaxContexts_->GetValue();
 return maxcontexts>1 || maxcontexts<8;
 }
 * @endcode
 *
 *  En el caso particular que se esta viendo, la validacion es simple. Queda
 * pendiente validar los formatos y parametrizar los limites.
 *
 *  Si los datos fueron modificados y son validos, el metodo
 * suri::Part::CommitChanges es llamado al presionar el boton OK o APPLY y debe
 * aplicar dichos cambios y hacerlos permanentes.
 *
 * @code
 bool ApplicationConfigPart::CommitChanges()
 {
 if (!HasValidData())
 {
 SHOW_ERROR(_("Datos invalidos"));
 RollbackChanges();
 REPORT_AND_FAIL_VALUE(wxT("Datos invalidos"),false);
 }
 formats_ = pImageFormats_->GetValue();
 suri::Configuration::SetParameter("lib_supported_image_formats",formats_);
 language_ = pLanguageChoice_->GetStringSelection().c_str();
 suri::Configuration::SetParameter("app_language",language_);
 maxContexts_ = pMaxContexts_->GetValue();
 suri::Configuration::SetParameter("app_max_number_of_context",maxContexts_);
 vincentyDistance_ = pUseVincenty_->GetValue();
 suri::Configuration::SetParameter("tool_always_use_vincenty_distance",static_cast<long int>(vincentyDistance_));
 RollbackChanges();
 return true;
 }
 * @endcode
 *
 *  Hasta este momento, se tiene una pagina o "Parte". A continuacion, se muestra
 * el codigo para crear un segundo suri::Part para la configuracion de los
 * parametros de renderizacion 3D.
 *
 * @code
 #ifndef VIEWER3DCONFIGPART_H_
 #define VIEWER3DCONFIGPART_H_

 #include "suri/Part.h"

 class Viewer3DConfigPart: public suri::Part
 {
 public:
 Viewer3DConfigPart();
 virtual ~Viewer3DConfigPart();

 virtual bool CreateToolWindow();
 virtual bool HasChanged();
 virtual bool CommitChanges();
 virtual bool RollbackChanges();
 virtual void SetInitialValues();
 virtual bool HasValidData();
 virtual void Update();
 private:
 wxTextCtrl *pTextureFactor_;
 wxTextCtrl *pTerrainFactor_;
 wxTextCtrl *pCameraVelocity_;
 wxTextCtrl *pExageration_;
 long int textureFactor_;
 long int terrainFactor_;
 long int cameraVelocity_;
 long int exageration_;
 };

 #endif
 * @endcode
 *
 * @code
 #include "Viewer3DConfigPart.h"
 #include "wx/wx.h"
 #include "suri/Configuration.h"

 Viewer3DConfigPart::Viewer3DConfigPart():pTextureFactor_(NULL),
 pTerrainFactor_(NULL),pCameraVelocity_(NULL),
 pExageration_(NULL),
 textureFactor_(1),
 terrainFactor_(1),
 cameraVelocity_(1),
 exageration_(1)
 {
 }

 Viewer3DConfigPart::~Viewer3DConfigPart()
 {
 }

 bool Viewer3DConfigPart::CreateToolWindow()
 {
 wxPanel* ppanel = new wxPanel(pParentWindow_,wxID_ANY);

 wxBoxSizer* psizer = new wxBoxSizer(wxVERTICAL);
 ppanel->SetSizer(psizer);

 wxFlexGridSizer *psubsizer = new wxFlexGridSizer(2,4,4);
 psubsizer->AddGrowableCol(1,1);
 psizer->Add(psubsizer,1,wxEXPAND,0);
 wxString labeltext = _("Factor Textura:");
 wxStaticText* ptextctrl = new wxStaticText(ppanel,wxID_STATIC,labeltext);
 psubsizer->Add(ptextctrl,0,wxALIGN_LEFT|wxALL,5);
 pTextureFactor_ = new wxTextCtrl(ppanel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_RIGHT,wxTextValidator(wxFILTER_NUMERIC));
 psubsizer->Add(pTextureFactor_,0,wxEXPAND|wxALL,5);

 labeltext = _("Factor Terreno:");
 ptextctrl = new wxStaticText(ppanel,wxID_STATIC,labeltext);
 psubsizer->Add(ptextctrl,0,wxALIGN_LEFT|wxALL,5);
 pTerrainFactor_ = new wxTextCtrl(ppanel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_RIGHT,wxTextValidator(wxFILTER_NUMERIC));
 psubsizer->Add(pTerrainFactor_,0,wxEXPAND|wxALL,5);

 labeltext = _("Velocidad camara:");
 ptextctrl = new wxStaticText(ppanel,wxID_STATIC,labeltext);
 psubsizer->Add(ptextctrl,0,wxALIGN_LEFT|wxALL,5);
 pCameraVelocity_ = new wxTextCtrl(ppanel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_RIGHT,wxTextValidator(wxFILTER_NUMERIC));
 psubsizer->Add(pCameraVelocity_,0,wxEXPAND|wxALL,5);

 labeltext = _("Exageracion:");
 ptextctrl = new wxStaticText(ppanel,wxID_STATIC,labeltext);
 psubsizer->Add(ptextctrl,0,wxALIGN_LEFT|wxALL,5);
 pExageration_ = new wxTextCtrl(ppanel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_RIGHT,wxTextValidator(wxFILTER_NUMERIC));
 psubsizer->Add(pExageration_,0,wxEXPAND|wxALL,5);

 pToolWindow_ = ppanel;
 return true;
 }

 bool Viewer3DConfigPart::HasChanged()
 {
 return pTextureFactor_->IsModified() || pTerrainFactor_->IsModified() ||
 pCameraVelocity_->IsModified() || pExageration_->IsModified();
 }

 bool Viewer3DConfigPart::CommitChanges()
 {
 if (!HasValidData())
 {
 SHOW_ERROR(_("Datos invalidos"));
 RollbackChanges();
 REPORT_AND_FAIL_VALUE(wxT("Datos invalidos"),false);
 }
 long int value = 0;
 pTextureFactor_->GetValue().ToLong(&value);
 textureFactor_ = value;
 suri::Configuration::SetParameter("v3d_factor_textura",textureFactor_);
 pTerrainFactor_->GetValue().ToLong(&value);
 terrainFactor_ = value;
 suri::Configuration::SetParameter("v3d_factor_terreno",terrainFactor_);
 pCameraVelocity_->GetValue().ToLong(&value);
 cameraVelocity_ = value;
 suri::Configuration::SetParameter("v3d_velocidad_camara",cameraVelocity_);
 pExageration_->GetValue().ToLong(&value);
 exageration_ = value;
 suri::Configuration::SetParameter("v3d_exageracion",exageration_);
 RollbackChanges();
 return true;
 }

 bool Viewer3DConfigPart::RollbackChanges()
 {
 pTextureFactor_->ChangeValue(wxString::Format("%ld",textureFactor_));
 pTerrainFactor_->ChangeValue(wxString::Format("%ld",terrainFactor_));
 pCameraVelocity_->ChangeValue(wxString::Format("%ld",cameraVelocity_));
 pExageration_->ChangeValue(wxString::Format("%ld",exageration_));
 return true;
 }

 void Viewer3DConfigPart::SetInitialValues()
 {
 textureFactor_ = suri::Configuration::GetParameter("v3d_factor_textura",textureFactor_);
 terrainFactor_ = suri::Configuration::GetParameter("v3d_factor_terreno",terrainFactor_);
 cameraVelocity_ = suri::Configuration::GetParameter("v3d_velocidad_camara",cameraVelocity_);
 exageration_ = suri::Configuration::GetParameter("v3d_exageracion",exageration_);
 RollbackChanges();
 }

 bool Viewer3DConfigPart::HasValidData()
 {
 long int value = 0;
 if (!pTextureFactor_->GetValue().ToLong(&value) || value<1 || value>10)
 return false;
 value = 0;
 if (!pTerrainFactor_->GetValue().ToLong(&value) || value<1 || value>10)
 return false;
 value = 0;
 if (pCameraVelocity_->GetValue().ToLong(&value) || value<1 || value>10)
 return false;
 value = 0;
 if (pExageration_->GetValue().ToLong(&value) || value<1 || value>10)
 return false;
 return true;
 }

 void Viewer3DConfigPart::Update()
 {
 }
 * @endcode
 *
 *  Una vez que se tienen las dos paginas de configuracion, es necesario
 * incorporarlas a la clase contenedora, en este caso, suri::NotebookWidget.
 *  Esto se realiza con el siguiente codigo.
 *
 * @code
 suri::NotebookWidget *pconfig = new suri::NotebookWidget;
 pconfig->CreateTool();
 pconfig->InsertPart(new ApplicationConfigPart);
 pconfig->InsertPart(new Viewer3DConfigPart);
 pconfig->ShowModal();
 * @endcode
 *
 *  Crea un suri::PartContainer que ordena las "Partes" en un Notebook de tipo
 * wizard. Agrega las dos paginas de configuracion y luego lo muestra en forma
 * modal.
 *
 *  De esta manera se construyen las ventanas de configuracion de tipo wizard.
 *
 */
