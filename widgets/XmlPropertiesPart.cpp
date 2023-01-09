/*! \file XmlPropertiesPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */



// Includes wx
#include "XmlPropertiesPart.h"
#include "wx/sstream.h"
#include "wx/xrc/xmlres.h"
#include "suri/DatasourceInterface.h"
#include "suri/LayerInterface.h"

/** namespace suri */
namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(XmlPropertiesPartEvent, XmlPropertiesPart)
      IMPLEMENT_EVENT_CALLBACK(OnUIUpdate, OnUIUpdate(event), wxUpdateUIEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor
 * @param[in] pElement puntero al Elemento.
 * @return instancia de la clase XmlPropertiesPart
 */
XmlPropertiesPart::XmlPropertiesPart(Element *pElement) :
      pElement_(pElement), NEW_EVENT_OBJECT(XmlPropertiesPartEvent), pDatasource_(NULL),
      pLayer_(NULL) {
   windowTitle_ = _(caption_XML_PROPERTIES);
}

/**
 * Constructor
 * @param[in] pDatasource referencia a la fuente de datos
 * @param[in] pLayer referencia al layer que posee el nodo render correcto.
 * @return instancia de la clase XmlPropertiesPart
 */
XmlPropertiesPart::XmlPropertiesPart(DatasourceInterface* pDatasource, LayerInterface* pLayer) :
      pElement_(NULL), NEW_EVENT_OBJECT(XmlPropertiesPartEvent), pDatasource_(pDatasource),
      pLayer_(pLayer) {
   // piso el nodo render de datasource para mostrar el xml de ds + layer
   Element* pelementfrom = pLayer->GetElement();
   Element* pelementdest = pDatasource->GetElement();
   ReplaceElementRenderizationNodes(pelementfrom, pelementdest);
   windowTitle_ = _(caption_XML_PROPERTIES);
}

/** Dtor. */
XmlPropertiesPart::~XmlPropertiesPart() {
   /*
    * Comento "delete pTextXml_;" ya que con la modificacion hecha en
    * DialogWidget::EndModal provoca un SIGSEGV. Tampoco funciona utilizando
    * wxWindow::Destroy.
    */
#ifdef __UNUSED_CODE__
   delete pTextXml_;
#endif
   DELETE_EVENT_OBJECT
   ;
}
/**
 *  Crea la ventana con un elemento de edicion de texto (para el XML).
 * Conecta a este ultimo el evento OnUIUpdate.
 */
bool XmlPropertiesPart::CreateToolWindow() {
   pToolWindow_ = new wxPanel(pParentWindow_, wxID_ANY);
   wxBoxSizer *psizer = new wxBoxSizer(wxVERTICAL);
   pTextXml_ = new wxTextCtrl(pToolWindow_, wxID_ANY, "", wxDefaultPosition,
                              wxDefaultSize,
                              wxTE_MULTILINE | wxTE_PROCESS_TAB | wxHSCROLL);
   psizer->Add(pTextXml_, 1, wxEXPAND, 5);
   pToolWindow_->SetSizer(psizer);

   pTextXml_->Connect(wxEVT_UPDATE_UI,
                      wxUpdateUIEventHandler(XmlPropertiesPartEvent::OnUIUpdate), NULL,
                      pEventHandler_);
   return true;
}

/** Inicializa el valor del text con el Xml del elemento */
void XmlPropertiesPart::SetInitialValues() {
   Element* pelement = (pDatasource_)? pDatasource_->GetElement(): pElement_;
   if (pelement) {
      pTextXml_->SetValue(pelement->GetXmlAsString());
   }
}

/** Devuelve el estado del Part. */
bool XmlPropertiesPart::IsEnabled() {
#ifdef __DEBUG__
   return Part::IsEnabled();
#endif
   return false;  // solo por precaucion.
}

/** Indica si el Part tiene cambios para salvar. */
bool XmlPropertiesPart::HasChanged() {
   Element* pelement = (pDatasource_)? pDatasource_->GetElement(): pElement_;
   return ((pTextXml_->GetValue().Cmp(pelement->GetXmlAsString()) != 0) && modified_);
}

/** Salva los cambios realizados en el Part. */
bool XmlPropertiesPart::CommitChanges() {
   if (!HasChanged()) {
      SetInitialValues();
   }

   wxString temp = pTextXml_->GetValue().c_str();
   if (temp.IsEmpty()) {
      REPORT_AND_FAIL_VALUE("D:Error al obtener Propiedades Xml.", false);
   }

   wxStringInputStream ss(temp);
   wxXmlDocument doc(ss, suri::XmlElement::xmlEncoding_);
   Element* pelement = (pDatasource_)? pDatasource_->GetElement(): pElement_;
   pelement->AddNode(NULL, doc.DetachRoot());
   Element* pelementdest = pLayer_->GetElement();
   // reemplazo el nodo del elemento del visualizador activo.
   pelementdest->AddNode(NULL, new wxXmlNode(*pelement->GetNode()));
   pelement->SetChanged();
   pelement->SendViewerUpdate();
   // resetea el flag de que cambio la text.
   pTextXml_->DiscardEdits();
   modified_ = false;
   return true;
}

/** Restaura los valores originales del Part. */
bool XmlPropertiesPart::RollbackChanges() {
   SetInitialValues();
   return true;
}

/**
 * Objetivo: Se implementa este Evento para que al modificar algun dato que se
 * muestra en este Part, desde otro Part, el Boton de Aplicar no quede
 * habilitado aun despues de haber sido presionado.(Ver HasChanged)
 * NOTA: debe implemetarse de la misma forma en todos los Part que compartan
 * informacion y la expongan mediante algun control.
 * @param[in] Event: Evento.
 */
void XmlPropertiesPart::OnUIUpdate(wxUpdateUIEvent &Event) {
   modified_ = pTextXml_->IsModified();
   Event.SetUpdateInterval(250);
   Event.Skip();
}

/**
 * Reemplaza el nodo de renderizacion
 * @param[in] pElementTo elemento que posee el nodo que se quiere usar para reemplazar
 * @param[out] pElementDest elemento que posee el nodo a ser reemplazado
 * @return true si pudo reemplazar el nodo, false en caso contrario
 */
bool XmlPropertiesPart::ReplaceElementRenderizationNodes(
      Element *pElementFrom, Element* pElementDest) {
   wxXmlNode *prendernodefrom = pElementFrom->GetNode(RENDERIZATION_NODE);
   wxXmlNode *prendernodedest = pElementDest->GetNode(RENDERIZATION_NODE);
   if (!prendernodefrom) {
	   return false;
   }
   wxXmlNode *pnewrendernodefrom = new wxXmlNode(*prendernodefrom);
   return pElementDest->ReplaceNode(prendernodedest->GetParent(),
                                pnewrendernodefrom, prendernodedest);
}

/*void XmlPropertiesPart::GetWindowIcon(wxBitmap& ToolBitmap,int X,int Y) const
 {
 GET_BITMAP_RESOURCE(icon_RASTER_ELEMENT,ToolBitmap);
 }*/
}  // namespace suri
// ------------------------------------------------------------------------------
// ------------------------------Fin XmlPropertiesPart-------------------------------
// ------------------------------------------------------------------------------
