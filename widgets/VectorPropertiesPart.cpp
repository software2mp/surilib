/*! \file VectorPropertiesPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes suri
#include "VectorPropertiesPart.h"
#include "resources.h"
#include "VectorLayer.h"
#include "suri/Configuration.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/LibraryClient.h"
#include "suri/DataViewManager.h"

// Includes wx
#include "wx/choicebk.h"

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(VectorPropertiesEventHandler, VectorPropertiesPart)
      IMPLEMENT_EVENT_CALLBACK(OnStyleChoiceChange, OnStyleChoiceChange(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Nombre del XRC que representa la ventana. */
const std::string VectorPropertiesPart::VECTOR_STYLE_EDITION_XRC_NAME = wxT(
		"ID_PANEL_VECTOR_PROP");
/**
 * Ctor. default.
 * @param[in] pVectorLayer capa vectorial que quiero editar
 * @return instancia de la clase VectorPropertiesPart
 */
VectorPropertiesPart::VectorPropertiesPart(DataViewManager* pDataViewManager,
                                           VectorLayer* pLayer) :
      pVectorLayer_(pLayer), pDataViewManager_(pDataViewManager),
      pSimpleEditor_(NULL), pCategorizedEditor_(NULL),
		pEventHandler_(new VectorPropertiesEventHandler(this)) {
   windowTitle_ =
         (Configuration::GetParameter("app_short_name", "") == wxT("SoPI")) ?
         _(caption_VECTOR_PROPERTIES) : _(caption_VECTOR_PROPERTIES_SIMUR);
}

/** dtor **/
VectorPropertiesPart::~VectorPropertiesPart() {
	delete pEventHandler_;
}

/** Inicializa los controles */
void VectorPropertiesPart::SetInitialValues() {
}

/** Se crea la ventana de herramienta en las clases derivadas */
bool VectorPropertiesPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
         wxT(VECTOR_STYLE_EDITION_XRC_NAME.c_str()));
   if (!pToolWindow_)
      return false;

   LibraryClient client(pDataViewManager_->GetLibraryManager());
   const Library* plib = client.GetLibraryByCode(
         LibraryManagerFactory::VectorstyleLibraryCode);

   pSimpleEditor_ = new ui::SimpleVectorStylePart(
         plib ? plib->GetId() : SuriObject::NullUuid,
         pDataViewManager_->GetLibraryManager(), pVectorLayer_);
   AddControl(pSimpleEditor_->GetWidget(), wxT("ID_SIMPLE_STYLE"));

   pSimpleEditor_->SetInitialValues();

   pCategorizedEditor_ = new ui::CategorizedVectorStylePart(pDataViewManager_,
                                                            pVectorLayer_);
   AddControl(pCategorizedEditor_->GetWidget(), wxT("ID_CATEGORIZED_PANEL"));

   pCategorizedEditor_->SetInitialValues();
	
	wxChoice* pstylechoice = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_STYLE_CHOICE", wxChoice);
	int page = 1;
   if (pVectorLayer_->GetElement()->GetNode(RENDERIZATION_NODE NODE_SEPARATION_TOKEN LAYERS_NODE) != NULL)
      page = 0;
   
	if (page == 0) {
		pCategorizedEditor_->Hide();
		pSimpleEditor_->Show();
	} else {
		pSimpleEditor_->Hide();
		pCategorizedEditor_->Show();
	}
	pstylechoice->SetSelection(page);

   pstylechoice->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(VectorPropertiesEventHandler::OnStyleChoiceChange),
         NULL,
         pEventHandler_);

	return true;
}
/** Indica si el Part tiene cambios para salvar. */
/**
 * Para cada tipo de propiedad verifica si cambiaron sus valores
 * @return true si se modifican las propiedades del Elemento.
 * @return false si no se modifican las propiedades del Elemento.
 */
bool VectorPropertiesPart::HasChanged() {
	wxChoice* pstylechoice = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_STYLE_CHOICE", wxChoice);
	int page = pstylechoice->GetSelection();
   if (page > 0) {
      return pCategorizedEditor_ ? pCategorizedEditor_->HasChanged() : false;
   } else {
      return pSimpleEditor_ ? pSimpleEditor_->HasChanged() : false;
   }
}

/** Salva los cambios realizados en el Part.
 * @return true en caso de poder salvar los cambios realizados
 * @return false en caso de no poder salvar los cambios realizados
 */

bool VectorPropertiesPart::CommitChanges() {
  	wxChoice* pstylechoice = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_STYLE_CHOICE", wxChoice);
	int page = pstylechoice->GetSelection();
	if (page > 0)
      return pCategorizedEditor_->CommitChanges();
   else
      return pSimpleEditor_->CommitChanges();
}

/** Restaura los valores originales del Part.
 * @return true.
 */
bool VectorPropertiesPart::RollbackChanges() {
	SetInitialValues();
	return true;
}

/**
 * Retorna el icono de la ventana
 * @param[out] ToolBitmap bitmap que se asocia a la ventana
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 */
void VectorPropertiesPart::GetWindowIcon(wxBitmap& ToolBitmap, int X,
		int Y) const {
	GET_BITMAP_RESOURCE(icon_ELEMENT_VECTOR_PROPERTIES, ToolBitmap);
}

/** Metodo que se llama cuando se lanza el evento de cambio en el choice de seleccion
  * de estilos **/
void VectorPropertiesPart::OnStyleChoiceChange(wxCommandEvent& Event) {
  	wxChoice* pstylechoice = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_STYLE_CHOICE", wxChoice);
	int page = pstylechoice->GetSelection();
	if (page > 0) {
		pCategorizedEditor_->Show();
		pSimpleEditor_->Hide();
	} else {
		pCategorizedEditor_->Hide();
		pSimpleEditor_->Show();
	}
	pToolWindow_->Fit();
}

} // namespace suri
