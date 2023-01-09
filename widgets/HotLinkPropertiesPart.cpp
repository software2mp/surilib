/** \file HotLinkPropertiesPart.cpp
 *
 * @file HotLinkPropertiesPart.h
 *  Software de Procesamiento de Imagenes Satelitales de CONAE.
 *  Copyright 2013-2007 CONAE.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "HotLinkPropertiesPart.h"

//Includes standard

//Includes Suri
#include "suri/Part.h"
#include "suri/Configuration.h"
#include "suri/xmlnames.h"
#include "suri/Vector.h"
#include "suri/FileManagementFunctions.h"
#include "suri/DatasourceInterface.h"
#include "suri/VectorEditor.h"
#include "HotLinkVectorElement.h"

//Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/ffile.h"
#include "wx/dir.h"

//Includes App
#include "resources.h"
#include "suri/messages.h"

//Includes ORG
#include "ogr_geometry.h"
#include "ogr_feature.h"
#include "ogrsf_frmts.h"

//Defines

//Forwards

START_IMPLEMENT_EVENT_OBJECT(HotLinkPropertiesPartEvent, HotLinkPropertiesPart)IMPLEMENT_EVENT_CALLBACK(OnAddPreviewCommandButtonClick,OnAddPreviewCommandButtonClick(event), wxCommandEvent)
IMPLEMENT_EVENT_CALLBACK(OnUIUpdate, OnUIUpdate(event), wxUpdateUIEvent)
END_IMPLEMENT_EVENT_OBJECT

//------------------------------------------------------------------------------

//! Ctor. a partir de una fuente de datos
HotLinkPropertiesPart::HotLinkPropertiesPart(
		suri::DatasourceInterface *pDatasource, suri::LayerInterface* pLayer) :
		NEW_EVENT_OBJECT(HotLinkPropertiesPartEvent),
		pElement_(NULL), pDatasource_(pDatasource), pLayer_(pLayer) {
	pElement_ = HotLinkVectorElement::Create(pDatasource->GetUrl());
	windowTitle_ = _(caption_HOTLINK_PROPERTIES);
}

/**
 * Constructor.
 * \todo El elemento recibido debe ser de clase HotLinkVectorElement.
 * @param[in]  pElement: puntero al elemento.
 */
HotLinkPropertiesPart::HotLinkPropertiesPart(HotLinkVectorElement *pElement) :
		NEW_EVENT_OBJECT(HotLinkPropertiesPartEvent),
		pElement_(pElement)
#ifdef __SAVE_URLHTML_CODE__
, urlHtml_(wxT(""))
#endif

{
	windowTitle_ = _(caption_HOTLINK_PROPERTIES);
}
//------------------------------------------------------------------------------

/**
 * Destructor.
 */
HotLinkPropertiesPart::~HotLinkPropertiesPart() {
	DELETE_EVENT_OBJECT;
	delete pElement_;
}

//------------------------------------------------------------------------------
/**
 * Carga la ventana diseniada. Si tuvo exito, asocia evento UIUpdate a las
 * textctrl de titulo, descripcion e imagen; y asocia evento de click al boton
 * para cargar imagen.
 * \todo No verifica que se haya podido cargar el Panel del XRC.
 * @return true Siempre
 */
bool HotLinkPropertiesPart::CreateToolWindow() {
	pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
			wxT("ID_PANEL_HOTLINK_PROP"));

	// Conecto los controles con sus eventos.
	GET_CONTROL(*pToolWindow_, "ID_BMPBTN_IMG", wxBitmapButton)->Connect(
				wxEVT_COMMAND_BUTTON_CLICKED,
				wxCommandEventHandler(
						HotLinkPropertiesPartEvent::OnAddPreviewCommandButtonClick),
				NULL, pEventHandler_);

	GET_CONTROL(*pToolWindow_, "ID_TXT_TITLE", wxTextCtrl)->Connect(
				wxEVT_UPDATE_UI,
				wxUpdateUIEventHandler(HotLinkPropertiesPartEvent::OnUIUpdate),
				NULL, pEventHandler_);
	GET_CONTROL(*pToolWindow_, "ID_TXT_DESC", wxTextCtrl)->Connect(
				wxEVT_UPDATE_UI,
				wxUpdateUIEventHandler(HotLinkPropertiesPartEvent::OnUIUpdate),
				NULL, pEventHandler_);
	GET_CONTROL(*pToolWindow_, "ID_TXT_IMG", wxTextCtrl)->Connect(
				wxEVT_UPDATE_UI,
				wxUpdateUIEventHandler(HotLinkPropertiesPartEvent::OnUIUpdate),
				NULL, pEventHandler_);
	return true;
}

//------------------------------------------------------------------------------
/**
 * Inicializa los controles con los valores del nodo caracteristicas
 */
void HotLinkPropertiesPart::SetInitialValues() {
#ifdef __SAVE_URLHTML_CODE__
	wxString title, description, urlimg;
	if(!GetFeaturesValues(title, description, urlimg, urlHtml_))
#else

	wxXmlNode* pfeanode = pDatasource_->GetElement()->GetNode(FEATURES_NODE);
	wxXmlNode* phlnode =
			pDatasource_->GetElement()->GetNode(
					LAYERS_NODE NODE_SEPARATION_TOKEN LAYER_NODE NODE_SEPARATION_TOKEN HOTLINK_NODE);
	if (pfeanode && phlnode) {
		pElement_->SetFeaturesNode(new wxXmlNode(*pfeanode));
		//pElement_->SetHotLinkFieldValue(phlnode);
		// Copio hotlink al pElement
		wxXmlNode* playernode = pElement_->GetNode(
				LAYERS_NODE NODE_SEPARATION_TOKEN LAYER_NODE);
		wxXmlNode *phlnode =
				pDatasource_->GetElement()->GetNode(
						LAYERS_NODE NODE_SEPARATION_TOKEN LAYER_NODE NODE_SEPARATION_TOKEN HOTLINK_NODE);
		wxXmlNode* pnewhlnode = new wxXmlNode(*phlnode);
		pElement_->AddNode(playernode, pnewhlnode);
	}
	// Cuando se inicializa la ventana de hotlink si se encuentra configurado
	// el hotlink se copia esta configuracion al elemento copia
	wxString title, description, urlimg, urlhtml;
	if (!GetFeaturesValues(title, description, urlimg, urlhtml))
#endif
		REPORT_AND_FAIL(
				"D:No se pudieron obtener valores del nodo caracteristicas");

	GET_CONTROL(*pToolWindow_, "ID_TXT_TITLE", wxTextCtrl)->SetValue(title);
	GET_CONTROL(*pToolWindow_, "ID_TXT_DESC", wxTextCtrl)->SetValue(
				description);
	GET_CONTROL(*pToolWindow_, "ID_TXT_IMG", wxTextCtrl)->SetValue(urlimg);
}

//------------------------------------------------------------------------------
/**
 * Devuelve el estado de activacion del Part.
 * @return true Siempre esta activo.
 */
bool HotLinkPropertiesPart::IsEnabled() {
	return true;
}

//------------------------------------------------------------------------------
/**
 * Indica si el Part tiene cambios para salvar.
 * Verifica si los datos que tenia guardados son distintos de los que hay
 * cargados en los controles.
 * @return true si los datos cambiaron. false C.O.C.
 */
bool HotLinkPropertiesPart::HasChanged() {
	wxString title, description, urlimg, urlhtml, value = wxT("");
	if (!GetFeaturesValues(title, description, urlimg, urlhtml))
		REPORT_DEBUG(
				"D:No se pudieron obtener valores del nodo caracteristicas");

	value =
			USE_CONTROL(*pToolWindow_, "ID_TXT_TITLE", wxTextCtrl, GetValue(), "");
	if (value.Cmp(title) != 0 && modified_)
		return true;

	value =
			USE_CONTROL(*pToolWindow_, "ID_TXT_DESC", wxTextCtrl, GetValue(), "");
	if (value.Cmp(description) != 0 && modified_)
		return true;

	value = USE_CONTROL(*pToolWindow_, "ID_TXT_IMG", wxTextCtrl, GetValue(), "");
	if (value.Cmp(urlimg) != 0 && modified_)
		return true;

	return false;
}

//------------------------------------------------------------------------------
/**
 * Salva los cambios realizados en el Part.
 * Genera y salva el html e imagen que conforman el hotlink en la carpeta
 * temporal de la App.
 * Modifica el xml del elemento agregando nodo caracteristicas y nodo hotlink
 * (si es un vector de puntos)
 * @return true si pudo salvar el html e imagen que conforman el hotlink y
 * escribir la BD del vector con el url del html.
 * @return false si fallo al intentar salvar algun archivo.
 * \note el html e imagen que conforman el hotlink, sus url no se agregan en el
 * nodo dependencias del elemento ya que esto generaria que se dupliquen dichos
 * archivos al momento de generar el PAT comprimido. Se estan agregando en el
 * nodo caracteristicas.
 */
bool HotLinkPropertiesPart::CommitChanges() {
	//obtengo los valores de los controles.
	wxString title, desc, urlimg;
	title =
			USE_CONTROL(*pToolWindow_, wxT("ID_TXT_TITLE"), wxTextCtrl, GetValue(), "");
	desc =
			USE_CONTROL(*pToolWindow_, wxT("ID_TXT_DESC"), wxTextCtrl, GetValue(), "");
	urlimg =
			USE_CONTROL(*pToolWindow_, wxT("ID_TXT_IMG"), wxTextCtrl, GetValue(), "");

#if defined (__LINUX__)
	title = wxString(wxConvUTF8.cMB2WC(title.c_str()),
			wxCSConv(suri::XmlElement::xmlEncoding_.c_str()));
	desc = wxString(wxConvUTF8.cMB2WC(desc.c_str()),
			wxCSConv(suri::XmlElement::xmlEncoding_.c_str()));
#endif

	if (!pElement_)
		REPORT_AND_FAIL_VALUE(
				"D:Error al grabar cambios. Elemento no es de tipo Vector",
				false);
	std::string pathtempdir = suri::Configuration::GetParameter(
			"app_hotlink_base_dir_volatile", wxT("./")), urlhtml = wxT("");

#ifdef __SAVE_URLHTML_CODE__
	//si no tengo el url del html, lo genero.
	if(urlHtml_.IsEmpty())
	{
		urlhtml = pathtempdir;
		urlhtml = GetNextPath(urlhtml);
		wxFileName fn(pElement_->GetName());
		urlhtml += fn.GetPathSeparator(wxPATH_UNIX);
		if(!fn.HasExt())
		urlhtml += wxT("elemento"); //No se traduce, elemento XML
		else
		urlhtml += pElement_->GetName().c_str();

		urlhtml += wxT(".html");
		urlHtml_ = urlhtml;
	}
	else
	{
#ifdef __CODIGO_ANTERIOR__
		if(pathtempdir.rfind("temp", pathtempdir.length())!=std::string::npos)
		{
			urlhtml = GetNextPath(urlhtml);
			wxFileName fn(pElement_->GetName());
			urlhtml += fn.GetPathSeparator(wxPATH_UNIX);
			if(!fn.HasExt())
			urlhtml += wxT("elemento"); //No se traduce, elemento XML
			else
			urlhtml += pElement_->GetName().c_str();
			urlhtml += wxT(".html");
			urlHtml_ = urlhtml;
		}
		else
		urlhtml = urlHtml_.c_str();
#endif
		urlhtml = urlHtml_;
	}
#else
	// genero el url del html
	urlhtml = suri::GetNextPath(pathtempdir);
	if (urlhtml.empty()) {
		SHOW_ERROR(message_SAVE_CHANGES_ERROR);
		REPORT_AND_FAIL_VALUE(
				"D:No se pudo crear directorio para HotLink en el directorio temporal.",
				false);
	}
	wxFileName fn(pElement_->GetName());
	if (!fn.HasExt())
		urlhtml += wxT(CONTEXT_ELEMENT_NODE);
	else
		urlhtml += pElement_->GetName().c_str();

	urlhtml += wxT(".html");
#endif

	// creo el directorio donde voy a crear el hotlink
	wxFileName fnhtml(urlhtml);
	if (!wxFileName::DirExists(fnhtml.GetPath()))
		if (!fnhtml.Mkdir(fnhtml.GetPath(), 0755, wxPATH_MKDIR_FULL))
			REPORT_AND_FAIL_VALUE(
					"D:No se pudo crear directorio para HotLink en el directorio temporal.",
					false);

	//creo la imagen que lleva el html en el dir donde va a estar el html.
	wxString newurlimg = wxT("");
	if (!urlimg.IsEmpty()) {
		if (!wxFileName::FileExists(urlimg)) {
			SHOW_ERROR(message_FILE_NOT_FOUND_s, urlimg.c_str());
			return false;
		}
		wxImage img(urlimg);
		if (!img.IsOk()) {
			SHOW_ERROR(message_INVALID_IMAGE);
			return false;
		}
		newurlimg = fnhtml.GetVolume()
				+ (fnhtml.HasVolume() ? fnhtml.GetVolumeSeparator() : wxT(""))
				+ fnhtml.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR,
						wxPATH_UNIX) + wxFileName(urlimg).GetFullName();
		if (!wxCopyFile(urlimg, newurlimg, true))
			REPORT_AND_FAIL_VALUE("D:No se pudo salvar imagen: %s", false,
					newurlimg.c_str());
	}

	//Agrego nodo caracteristicas
	SetFeaturesValues(title, desc, newurlimg, urlhtml);

	pElement_->SetHotLinkFieldName(wxT(SUR_HOTLINK_FIELD));

	//Genero Html
	CreateHtml(title, desc, newurlimg, urlhtml);

#ifdef __UNUSED_CODE__
	/* NOTA: no agrego los archivos como dependencias ya que al momento de generar
	 * el PAT comprimido, las dependencias se hacen relativas al elemento. Como
	 * resultado de esto no se generaria nunca el directorio:
	 *                PAT - XXX/comun/hotlinks/...
	 * Los HL se agregan al PAT procesando el nodo "caracteristicas".
	 * Ver PatFile::CreatePatFile().
	 */
//Agrego el html y la imagen que utiliza como dependencias
	pElement_->SetDependencies(urlhtml);
	if(!newurlimg.IsEmpty())
	pElement_->SetDependencies(newurlimg.c_str());
#endif

#ifdef __UNUSED_CODE__
	/* si el url no tiene temp => copio el vector al temp.
	 * Significa que es un vector perteneciente a un PAT instalado o de usuario.
	 */
	if (pElement_->GetUrl().Find(wxT("temp")) == wxNOT_FOUND)
	if (!pElement_->CopyVectorToTemp(urlhtml)) {
		SHOW_ERROR(message_SAVE_CHANGES_ERROR);
		REPORT_AND_FAIL_VALUE(
				"D:No se pudo copiar vector a directorio temporal", false);
	}
#endif
	/*TODO(17/04/2009 - Alejandro): para no modificar un vector perteneciente a
	 * un PAT que no sea el actual, si se cumple el if anterior hay que cerrar
	 * el vector y abrir el nuevo y recien cambiar el contenido del hotlink.*/

	//Modifico el campo SURHotLink con el nuevo url del hotlink.
	// si no existe el campo lo genero
	suri::VectorEditor editor;
	if (editor.GetFieldPosition(SUR_HOTLINK_FIELD) < 0) {
		if (!editor.OpenVector(pElement_->GetUrl().c_str())
				|| !editor.OpenLayer(0)
				|| !editor.CreateField(SUR_HOTLINK_FIELD,
						suri::DataInfo<unsigned char>::Name)) {
			SHOW_ERROR(
					"No se pudo insertar el campo %s, en el dbf y/o en el archivo xml",
					SUR_HOTLINK_FIELD);
		}
	}

	pElement_->SetHotLinkFieldValue(urlhtml);

	GET_CONTROL(*pToolWindow_, wxT("ID_TXT_TITLE"), wxTextCtrl)->DiscardEdits();
	GET_CONTROL(*pToolWindow_, wxT("ID_TXT_DESC"), wxTextCtrl)->DiscardEdits();
	GET_CONTROL(*pToolWindow_, wxT("ID_TXT_IMG"), wxTextCtrl)->DiscardEdits();
	// Envio update para que refresque el icono de la lista

	// Una vez finalizada la modificacion sobre pElement replico los cambios
	// en las fuente de datos. No es necesario replicar el contenido fisico
	// porque la copia apunta al mismo archivo fisico que el original
	if (pDatasource_) {
		// Copio hotlink
		wxXmlNode *phlnode =
				pElement_->GetNode(
						LAYERS_NODE NODE_SEPARATION_TOKEN LAYER_NODE NODE_SEPARATION_TOKEN HOTLINK_NODE);

		wxXmlNode* playernode = pLayer_->GetElement()->GetNode(
				LAYERS_NODE NODE_SEPARATION_TOKEN LAYER_NODE);
		wxXmlNode* playernewhlnode = new wxXmlNode(*phlnode);
		pLayer_->GetElement()->AddNode(playernode, playernewhlnode);

		wxXmlNode* pdslayernode = pDatasource_->GetElement()->GetNode(
				LAYERS_NODE NODE_SEPARATION_TOKEN LAYER_NODE);
		wxXmlNode* pnewhlnode = new wxXmlNode(*phlnode);
		pDatasource_->GetElement()->AddNode(pdslayernode, pnewhlnode, NULL, FALSE);

		// Copio nodo caracteristicas
		wxXmlNode *pfeanode = pElement_->GetNode(FEATURES_NODE);
		wxXmlNode* playernewfeanode = new wxXmlNode(*pfeanode);
		pLayer_->GetElement()->AddNode(pLayer_->GetElement()->GetNode(),
				playernewfeanode);
		wxXmlNode* pnewfeanode = new wxXmlNode(*pfeanode);
		pDatasource_->GetElement()->AddNode(
				pDatasource_->GetElement()->GetNode(), pnewfeanode);
	}
	pElement_->SetChanged();
	pElement_->SendViewerUpdate();
	return true;
}

//------------------------------------------------------------------------------
/**
 * Restaura los ultimos valores guardados en los controles del Part.
 */
bool HotLinkPropertiesPart::RollbackChanges() {
	SetInitialValues();
	return true;
}

//------------------------------------------------------------------------------
/**
 * Presenta un dialogo de apertura de archivos para seleccionar la imagen
 * que se despliega como hotlink.
 * \todo Reemplazar por el #define para extensiones de imagenes.
 * @param[in]  Event: evento
 */
void HotLinkPropertiesPart::OnAddPreviewCommandButtonClick(
		wxCommandEvent &Event) {
	wxFileDialog filedlg(pToolWindow_, _(caption_SELECT_GENERIC_IMAGE), wxT(""),
			wxT(""), _((filter_COMMON_IMAGE_FORMATS_LOAD).c_str()));
	if (filedlg.ShowModal() == wxID_CANCEL)
		return;
	wxFileName fnpreview(filedlg.GetPath().c_str());
	wxString pathpreview = fnpreview.GetVolume().c_str()
			+ (fnpreview.HasVolume() ? fnpreview.GetVolumeSeparator() : wxT(""))
			+ fnpreview.GetFullPath(wxPATH_UNIX);
	GET_CONTROL(*pToolWindow_, wxT("ID_TXT_IMG"), wxTextCtrl)->ChangeValue(
				pathpreview);
	GET_CONTROL(*pToolWindow_, wxT("ID_TXT_IMG"), wxTextCtrl)->SetModified(
				true);
	Event.Skip();
}
//------------------------------------------------------------------------------
/**
 * Este metodo responde a los eventos UIUpdate(que se disparan cuando no hay
 * eventos de usuario en la cola de eventos de wx).
 * Setea flag de Part modificado verificando el estado de las textctrl.
 * \todo No deberia usarse un UiUpdate para setear datos, el UiUpdate solo debe
 *  usarse para actualizar la interfaz de usuario!
 * @param[in]  Event: evento
 */
void HotLinkPropertiesPart::OnUIUpdate(wxUpdateUIEvent &Event) {
	modified_ =
			(USE_CONTROL(*pToolWindow_, wxT("ID_TXT_TITLE"), wxTextCtrl, IsModified(),
					false)
					|| USE_CONTROL(*pToolWindow_, wxT("ID_TXT_DESC"), wxTextCtrl, IsModified(),
							false)
					|| USE_CONTROL(*pToolWindow_, wxT("ID_TXT_IMG"), wxTextCtrl, IsModified(),
							false));
	Event.Skip();
}

//------------------------------------------------------------------------------
//! Devuelve el icono de la ventana.
void HotLinkPropertiesPart::GetWindowIcon(wxBitmap& ToolBitmap, int X,
		int Y) const {
	GET_BITMAP_RESOURCE(icon_ELEMENT_HOTLINK_PROPERTIES, ToolBitmap);
}
//------------------------------------------------------------------------------

/**
 * Objetivo: Obtiene los valores del nodo Caracteristicas
 * @param[out] Title: contenido del nodo titulo.
 * @param[out] Description: contenido del nodo descripcion.
 * @param[out] ImgUrl: contenido del nodo url.
 * @param[out] htmlUrl: contenido del nodo html|url
 * @return obtiene datos? true : false.
 */
bool HotLinkPropertiesPart::GetFeaturesValues(wxString &Title,
		wxString &Description, wxString &ImgUrl, wxString &HtmlUrl) {
	Title = wxT(""), Description = wxT(""), ImgUrl = wxT(""), HtmlUrl = wxT("");
	//obtengo nodo Caracteristicas
	if (!pElement_)
		return false;

	return pElement_->GetFeaturesValues(Title, Description, ImgUrl, HtmlUrl);
}

/**
 * Setea los valores del nodo Caracteristicas
 * @param[in]  Title: contenido del nodo titulo.
 * @param[in]  Description: contenido del nodo descripcion.
 * @param[in]  ImgUrl: contenido del nodo url.
 * @param[in]  htmlUrl: contenido del nodo html|url
 */
void HotLinkPropertiesPart::SetFeaturesValues(const wxString &Title,
		const wxString &Description, const wxString &ImgUrl,
		const wxString &HtmlUrl) {
	if (pElement_)
		pElement_->SetFeaturesNode(Title, Description, ImgUrl, HtmlUrl);
	else
		REPORT_AND_FAIL("D:Error al setear nodo caracteristicas.");
}

/**
 * Crea y salva un html con los parametros pasados, basandose en un template
 * @param[in]  Title: contenido del nodo titulo.
 * @param[in]  Description: contenido del nodo descripcion.
 * @param[in]  ImgUrl: contenido del nodo url.
 * @param[in]  htmlUrl: contenido del nodo html|url
 * @return true si pudo crear el html. false C.O.C
 */
bool HotLinkPropertiesPart::CreateHtml(const wxString &Title,
		const wxString &Desc, const wxString &UrlImg,
		const wxString &HtmlPath) {
	std::string htmltemplate = suri::Configuration::GetParameter(
			"photo_template", wxT("./"));
	wxFFile templatehtml(htmltemplate.c_str(), "r");
	if (!templatehtml.IsOpened())
		REPORT_AND_FAIL_VALUE("D:No se pudo obtener template", false);

	//levanto todo el template en memoria para reemplazar los tokens
	wxString templatesrc = wxT("");
	templatehtml.ReadAll(&templatesrc);
	templatehtml.Close();

	templatesrc.Replace("<suri src=\"titulo\"/>", Title, false);
	wxString aux = wxT("");
	if (!UrlImg.IsEmpty()) {
		aux = "<img src=\"";
		aux += wxFileName(UrlImg).GetFullName().c_str();
		aux += "\">";
	}
	templatesrc.Replace("<suri src=\"fotografia\" alt=\"Fotografia\"/>", aux,
			false);
	templatesrc.Replace("<suri src=\"descripcion\"/>", Desc, false);

	wxFileName fn(HtmlPath);
	wxFFile file(fn.GetFullPath(), "w");
	if (!file.IsOpened())
		REPORT_AND_FAIL_VALUE("D:No se pudo generar html", false);

	if (!file.Write(templatesrc)) {
		file.Close();
		REPORT_AND_FAIL_VALUE("D:Falla escritura de html", false);
	}

	file.Flush();
	file.Close();
	return true;
}

//------------------------------------------------------------------------------
//-------------------------------HOTLINK PROPERTIES-----------------------------
//------------------------------------------------------------------------------
