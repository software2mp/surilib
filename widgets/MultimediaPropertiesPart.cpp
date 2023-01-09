//<COPYRIGHT> a reemplazar a futuro

#include "MultimediaPropertiesPart.h"

//Includes standard

//Includes Suri
#include "suri/xmlnames.h"

//Includes Wx
#include "wx/xrc/xmlres.h"
#include "wx/filepicker.h"

//Includes App
#include "suri/messages.h"
#include "resources.h"

//Defines

//Forwards

START_IMPLEMENT_EVENT_OBJECT(MultimediaPropertiesPartEvent, MultimediaPropertiesPart)
	IMPLEMENT_EVENT_CALLBACK(OnUIUpdate, OnUIUpdate(event), wxUpdateUIEvent)
END_IMPLEMENT_EVENT_OBJECT

/**
 * @param[in]	pElement: puntero al Elemento.
 */
MultimediaPropertiesPart::MultimediaPropertiesPart(suri::MultimediaElement *pElement,
                                             bool ShowPreviewSelector) :
      pElement_(pElement), pTextTitle_(NULL), pTextDescription_(NULL),
      NEW_EVENT_OBJECT(MultimediaPropertiesPartEvent), showImageSelector_(ShowPreviewSelector) {
   windowTitle_ = _(caption_MULTIMEDIA_PROPERTIES);
}

/**
 * Destructor.
 */
MultimediaPropertiesPart::~MultimediaPropertiesPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Carga la ventana diseniada. En caso de exito.
 * \post asocia evento UIUpdate a las textctrl para poder verificar su estado.
 * @return true en caso de poder cargar la ventana, false C.O.C.
 */
bool MultimediaPropertiesPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_, wxT("ID_PHOTO_PROPERTIES"));
   if (!pToolWindow_)
      return false;
   pTextTitle_ = XRCCTRL(*pToolWindow_, wxT("ID_PHOTO_TITLE"), wxTextCtrl);
   pTextDescription_ = XRCCTRL(*pToolWindow_,wxT("ID_PHOTO_DESCRIPTION"),
         wxTextCtrl);
   if (pTextTitle_)
      pTextTitle_->Connect(wxEVT_UPDATE_UI,
                           wxUpdateUIEventHandler(MultimediaPropertiesPartEvent::OnUIUpdate),
                           NULL, pEventHandler_);
   if (pTextDescription_)
      pTextDescription_->Connect(
            wxEVT_UPDATE_UI,
            wxUpdateUIEventHandler(MultimediaPropertiesPartEvent::OnUIUpdate), NULL,
            pEventHandler_);
   /** Si la vista previa es igual al archivo fisico de la capa entonces es una capa de tipo foto**/
   bool showimagepicker = showImageSelector_ &&
                        pElement_->GetUrl().compare(pElement_->GetPreviewImage().c_str()) != 0;
   XRCCTRL(*pToolWindow_,wxT("ID_PREVIEW_IMG_PANEL"), wxPanel)->Show(showimagepicker);
   return true;
}

/**
 * Inicializa los controles.
 * Carga en las textctrl el contenido del nodo caracteristicas.
 */
void MultimediaPropertiesPart::SetInitialValues() {
   pTextTitle_->SetValue(pElement_->GetTitle());
   pTextDescription_->SetValue(pElement_->GetText());
   XRCCTRL(*pToolWindow_,wxT("ID_PREVIEW_IMG_FILECTRL"),
            wxFilePickerCtrl)->SetPath(pElement_->GetPreviewImage());
}

//! Devuelve el estado del Part.
bool MultimediaPropertiesPart::IsEnabled() {
   return true;
}

/**
 * Verifica el estado de las textctrl y si su contenido es distinto al contenido
 * del nodo caracteristicas.
 * @return true en caso de que haya cambios para salvar. false C.O.C.
 */
bool MultimediaPropertiesPart::HasChanged() {
   wxString value = wxT("");
   value = pTextTitle_->GetValue();
   if (value.Cmp(pElement_->GetTitle()) != 0 && modified_) return true;

   value = pTextDescription_->GetValue();
   if (value.Cmp(pElement_->GetText()) != 0 && modified_) return true;
   return false;
}

/**
 * Guarda en el nodo caracteristicas el contenido de las textctrl.
 * @return true.
 */
bool MultimediaPropertiesPart::CommitChanges() {
   pElement_->SetTitle(pTextTitle_->GetValue());
   pElement_->SetText(pTextDescription_->GetValue());
   pElement_->SetChanged();
   pElement_->SendViewerUpdate();
   std::string preview = XRCCTRL(*pToolWindow_,wxT("ID_PREVIEW_IMG_FILECTRL"), 
                        wxFilePickerCtrl)->GetPath().c_str();
   if (!preview.empty())
      pElement_->SetPreviewImage(preview);
   modified_ = false;
   pTextTitle_->DiscardEdits();
   pTextDescription_->DiscardEdits();
   return true;
}

/**
 * Restaura los ultimos valores guardados en los controles del Part.
 */
bool MultimediaPropertiesPart::RollbackChanges() {
   return true;
}

/**
 * Este metodo se ejecuta cuando se dispara el evento UIUpdate.
 * Tiene por objetivo, determinar si las textctrl cambiaron su contenido.
 * \todo El evento UIupdate debe actualizar los objetos de UI y no determinar
 *      un estado, esto se debe hacer en el evento correspondiente de los
 *      wxTextCtrls o con Validators o con TransferDataTo/FromWindow.
 * @param[in]	Event: evento.
 */
void MultimediaPropertiesPart::OnUIUpdate(wxUpdateUIEvent &Event) {
   modified_ = (pTextTitle_->IsModified() || pTextDescription_->IsModified());
   Event.SetUpdateInterval(100);
   Event.Skip();
}

/**
 * Objetivo: Devuelve el icono de propiedades
 * Por ahora no tiene en cuenta el X e Y dado que devuelve el de 32x32 pixels
 */
void MultimediaPropertiesPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_PHOTO_PROPERTIES, ToolBitmap);
}

//----------------------------Fin de MultimediaPropertiesPart----------------------------
