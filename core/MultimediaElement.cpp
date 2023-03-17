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

#include "MultimediaElement.h"

//Includes standard
#include <map>
#include <sstream>

//Includes Suri
#include "suri/xmlnames.h"
#include "suri/Part.h"
#include "suri/Configuration.h"
#include "suri/LayerInterface.h"
#include "suri/DatasourceInterface.h"

//Includes Wx
#include "wx/xrc/xmlres.h"
#include "wx/image.h"
#include "wx/filename.h"

//Includes App
#include "ParseSuriTemplates.h"
#include "resources.h"
#include "MultimediaPropertiesPart.h"
#include "suri/messages.h"

//Defines
#define MULTIMEDIA_ELEMENT_STR_TYPE "Multimedia"

//Forwards
namespace suri {
AUTO_REGISTER_CLASS(suri::Element, MultimediaElement, 0)

/**
 * Constructor.
 */
MultimediaElement::MultimediaElement() :
		Element(wxT(TYPE_PROPERTY_VALUE_HTML)) {
}

/**
 * Destructor.
 */
MultimediaElement::~MultimediaElement() {

}

/**
 * Devuelve un string con el icono asociado al elemento.
 */
wxString MultimediaElement::GetIcon() const {
   wxFileName fname(GetUrl());
	return fname.GetExt().compare("pdf") ? wxT(icon_PHOTO_ELEMENT) : wxT(icon_DOCUMENT_ELEMENT);
}

/** retorna un string para el icono */
wxString MultimediaElement::DoGetIcon() const {
   wxFileName fname(GetUrl());
	return fname.GetExt().compare("pdf") ? wxT(icon_PHOTO_ELEMENT) : wxT(icon_DOCUMENT_ELEMENT);
}

/**
 * Devuelve el contenido del nodo "caracteristicas|titulo".
 * \todo Utilizar defines del xmlnames.h
 * @param[in]	pElement: puntero al elemento.
 * @return string con el titulo de la Foto
 */
std::string MultimediaElement::GetTitle() const {
	wxString nodepath(FEATURES_NODE);
	nodepath += NODE_SEPARATION_TOKEN;
	nodepath += TITLE_NODE;
	wxXmlNode *pnode = GetNode(nodepath);
	return pnode ? pnode->GetNodeContent().c_str() : "";
}

/**
 * Devuelve el contenido del nodo "caracteristicas|texto".
 * @param[in]	pElement: puntero al elemento.
 * @return string con la descripcion de la Foto
 */
std::string MultimediaElement::GetText() const {
	wxString nodepath(FEATURES_NODE);
	nodepath += NODE_SEPARATION_TOKEN;
	nodepath += TEXT_NODE;
	wxXmlNode *pnode = GetNode(nodepath);
	return pnode ? pnode->GetNodeContent().c_str() : "";
}

/**
 * Devuelve el tipo del elemento.
 * @param[in]	pElement: puntero al elemento.
 * @return string con el tipo del elemento.
 * \todo (17/06/2009 - Alejandro): crear en xmlnames.h define para tipo de elemento.
 */
wxString MultimediaElement::GetTypeAsString() {
	return MULTIMEDIA_ELEMENT_STR_TYPE;
}

/**
 * Devuelve un string con el codigo html del html a insertar en el contexto
 * @param[out]	Html: codigo html.
 * @return true en caso de exito. false C.O.C.
 * \todo Utilizar template obtenido mediante la configuracion
 */
bool MultimediaElement::GetHtmlInfo(wxString &Html) const {
	long photomaxwidth = suri::Configuration::GetParameter(
			std::string("photo_max_width"), 800);
	long photominwidth = suri::Configuration::GetParameter(
			std::string("photo_min_width"), 460);
	long photomargin = suri::Configuration::GetParameter(
			std::string("photo_margin"), 40);

	std::map < std::string, std::string > params;
	wxString title = GetTitle();
	wxString text = GetText();
	wxString copyr = GetCopyRight();
#if defined (__LINUX__)
	title = wxString(wxConvUTF8.cMB2WC(title.c_str()),
			wxCSConv(suri::XmlElement::xmlEncoding_.c_str()));
	text = wxString(wxConvUTF8.cMB2WC(text.c_str()),
			wxCSConv(suri::XmlElement::xmlEncoding_.c_str()));
	copyr = wxString(wxConvUTF8.cMB2WC(copyr.c_str()),
			wxCSConv(suri::XmlElement::xmlEncoding_.c_str()));
#endif
	params.insert(std::make_pair("<suri src=\"titulo\"/>", title.c_str()));
	params.insert(std::make_pair("<suri src=\"descripcion\"/>", text.c_str()));
	params.insert(std::make_pair("<suri src=\"copyright\">", copyr.c_str()));

	//Inserta la foto y corrige el ancho
	std::string tag = "<img src=\"";
   std::string previewurl = GetPreviewImage();
   std::string defaultpreview = suri::Configuration::GetParameter("app_data_dir", "./");
   defaultpreview += "multimedia/default_preview.png";
   previewurl = previewurl.empty() ? defaultpreview : previewurl;
   tag += previewurl;
	tag += "\"";
	wxImage img(previewurl);
	int imagewidth = img.GetWidth();
	int imageheight = img.GetHeight();

	if (img.GetWidth() > photomaxwidth) {
		imagewidth = photomaxwidth;
		imageheight = img.GetHeight() * photomaxwidth / img.GetWidth();

		std::stringstream strstreamw;
		std::stringstream strstreamh;
		strstreamw << imagewidth;
		strstreamh << imageheight;

		tag += " width=\"" + strstreamw.str() + "\" height=\""
				+ strstreamh.str() + "\" ";
	}

	tag += "alt=\"Multimedia\"/>";
	params.insert(
			std::make_pair("<suri src=\"multimedia\" alt=\"Multimedia\"/>",
					tag));

	//Corrige la barra de titulo
	std::stringstream strstreamt;
	strstreamt << std::max(imagewidth + photomargin, photominwidth);
	tag = "<img src=\"Header_imagen.gif\" height=\"15\" width=\""
			+ strstreamt.str() + "\"/>";
	params.insert(std::make_pair("<suri img src=\"Header_imagen.gif\"/>", tag));
   
	std::string pathbasehtml = suri::Configuration::GetParameter(
			wxT("photo_template"), wxT("./"));
   if (GetNode(URL_NODE)->GetNodeContent().compare(GetPreviewImage().c_str()) != 0) {
      params.insert(std::make_pair("<suri src=\"url\"/>", GetNode(URL_NODE)->GetNodeContent().c_str()));
      pathbasehtml = suri::Configuration::GetParameter("multimedia_template", "./");
   }
	ParseSuriTemplates parse(pathbasehtml);
	return parse.GetNewHtml(params, Html);
}

/**
 * Setea el contenido del nodo "caracteristicas|titulo"
 * @param[in]	Title: titulo de la foto.
 */
void MultimediaElement::SetTitle(const wxString &Title) {
	wxXmlNode *pnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(TITLE_NODE));
	pnode->AddChild(
			new wxXmlNode(pnode, wxXML_TEXT_NODE, wxEmptyString, Title));
	AddNode(GetNode(wxT(FEATURES_NODE)), pnode, true);
}

/**
 * Setea el contenido del nodo "caracteristicas|texto"
 * @param[in]	Title: titulo de la foto.
 */
void MultimediaElement::SetText(const wxString &Text) {
	wxXmlNode *pnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(TEXT_NODE));
	pnode->AddChild(new wxXmlNode(pnode, wxXML_TEXT_NODE, wxEmptyString, Text));
	AddNode(GetNode(wxT(FEATURES_NODE)), pnode, true);
}

/**
 * Objetivo: Crea un MultimediaElement.
 * @param[in]	FileName: path absoluto del archivo.
 * @param[in]	Title: titulo del elemento.
 * @param[in]	Text: texto que acompania al elmento.
 * @return un puntero a un objeto del tipo MultimediaElement o null en caso de error.
 * \attention quien invoque a este metodo es responsable del puntero devuelto,
 * por lo tanto debe hacerse cargo de su eliminacion.
 */
MultimediaElement* MultimediaElement::Create(const std::string &FileName,
		const std::string &Title, const std::string &Text) {
	MultimediaElement *pelement = new MultimediaElement();
   if (wxFileName::FileExists(FileName)) {
   	pelement->SetUrl(FileName);
   } else {
      wxXmlNode *pnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(URL_NODE));
	   pnode->AddChild(new wxXmlNode(pnode, wxXML_TEXT_NODE, wxEmptyString, FileName));
	   pelement->AddNode(pelement->GetNode(""), pnode, true);
   }
	
   wxXmlNode *pdepnode = pelement->AddNode(pelement->GetNode(), FEATURES_NODE);
	pelement->AddNode(pdepnode, TITLE_NODE, Title);
	pelement->AddNode(pdepnode, TEXT_NODE, Text);

	wxImage img;
   // Si no es una imagen entonces el archivo multimedia tiene otro tipo de contenido no visualizable
   {
      // se desactiva el logueo de mensajes por este bloque
      wxLogNull logNo;
     	if (img.LoadFile(pelement->GetUrl().c_str())) {
         pelement->AddNode(pdepnode, PREVIEW_IMAGE_NODE, FileName); 
      }
   }
	// Nodo Archivo
	/*TODO(02/12/2008 - Alejandro): Definir el tipo de elemento y de donde
	 * obtenerlo.*/
	pelement->SetFileNode(MULTIMEDIA_ELEMENT_STR_TYPE); //No se traduce, elemento XML
	return pelement;
}

/**
 * Devuelve las paginas especificas de la Anotacion que se van a mostrar en las
 * Propiedades del elemento.
 * @param[in] pDataViewManager administrar general de la aplicacion
 * @param[in] pLayer capa del visualizador activo
 * @param[in] pDatasource fuente de datos  
 * @return vector de Part con los Part de las Propiedades especificas de la
 * Anotacion.
 */
std::vector<suri::Part *> MultimediaElement::DoGetParts(DataViewManager* pDataViewManager,
                         LayerInterface *pLayer, DatasourceInterface *pDatasource) {
	std::vector<suri::Part *> vectorparts;
   if (!pLayer && !pDatasource) {
   	vectorparts.push_back(new MultimediaPropertiesPart(this));
   } else {
      MultimediaElement* pphoto = dynamic_cast<MultimediaElement*>(pLayer ? pLayer->GetElement() :
                              pDatasource->GetElement());
      vectorparts.push_back(new MultimediaPropertiesPart(pphoto));
   }
	return vectorparts;
}

MultimediaElement* MultimediaElement::Initialize(const std::string &FileName) {
	return this;
}

std::string MultimediaElement::GetPreviewImage() const {
 	wxString nodepath(FEATURES_NODE);
	nodepath += NODE_SEPARATION_TOKEN;
	nodepath += PREVIEW_IMAGE_NODE;
	wxXmlNode *pnode = GetNode(nodepath);
   return pnode ? pnode->GetNodeContent().c_str() : "";
}

void MultimediaElement::SetPreviewImage(const std::string& Url) {
	wxXmlNode *pnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(PREVIEW_IMAGE_NODE));
	pnode->AddChild(new wxXmlNode(pnode, wxXML_TEXT_NODE, wxEmptyString, Url));
	AddNode(GetNode(wxT(FEATURES_NODE)), pnode, true);
}

} /** namespace suri **/
