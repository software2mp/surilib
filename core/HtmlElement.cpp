//<COPYRIGHT> a reemplazar a futuro

#include "HtmlElement.h"

//Includes standard
#include <map>

//Includes Suri
#include "suri/xmlnames.h"

//Includes Wx
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/ffile.h>

//Includes App
#include "resources.h"
#include "ImageParser.h"

//Defines

//Forwards

namespace suri {
/**
 * Macro para registrar el tipo de elemento
 */
AUTO_REGISTER_CLASS(Element, HtmlElement, 0)

/**
 * Constructor.
 */
HtmlElement::HtmlElement() :
		Element(wxT(TYPE_PROPERTY_VALUE_HTML)) {
}

/**
 * Destructor.
 */
HtmlElement::~HtmlElement() {
}

/**
 * Devuelve un string con el icono del elemento.
 * @return wxString con el icono del elemento.
 */
wxString HtmlElement::GetIcon() const {
	return wxT(icon_DOCUMENT_ELEMENT);
}

/** retorna un string para el icono */
wxString HtmlElement::DoGetIcon() const {
	return wxT(icon_DOCUMENT_ELEMENT);
}

/**
 * Devuelve los detalles del elemento.
 * @return wxString con el xml del elemento.
 */
wxString HtmlElement::GetDetails() const {
	return wxT("");
}

//! Muestra la herramienta de propiedades
void HtmlElement::ShowPropertyTool() {
//	ElementPropertiesWidget *ptool = new ElementPropertiesWidget;
//	if (ptool)
//		ptool->GetWindow()->GetParent()->Show(true);
}

/**
 * Funcion estatica para crear un HtmlElement
 * @param[in]	UrlElement: url del html
 * @return puntero a un HtmlElement o null en caso de error.
 * \attention quien invoque a este metodo es responsable del puntero devuelto,
 * por lo tanto debe hacerse cargo de su eliminacion.
 */
HtmlElement* HtmlElement::Create(const std::string &UrlElement) {
	/*! \todo:(22/08/2008 - Alejandro): Se esta haciendo la verificacion con la ext.
	 * del archivo. Deberia poder hacerse a traves del Parser.(ver valor de
	 * retorno del metodo GetProduct().*/
	/*TODO(22/08/2008 - Alejandro): Se esta haciendo la verificacion con la ext.
	 * del archivo. Deberia poder hacerse a traves del Parser.(ver valor de
	 * retorno del metodo GetProduct().*/
	wxFileName fn(UrlElement);
	if (fn.GetExt().MakeUpper().Cmp("HTML") != 0
			&& fn.GetExt().MakeUpper().Cmp("HTM") != 0)
		return NULL;

	HtmlElement *pelement = new HtmlElement;
	pelement->SetUrl(UrlElement);

	/* NOTA: dejo bloque try-catch ya que no tengo forma de verificar si el
	 * archivo(FileName) tiene un path valido o no. La unica forma es tratar de
	 * abrirlo como sucede en GetUrlImages ya wxFileName::FileExists devuelve true*/
	wxArrayString urlvec;
	TRY
	{
		urlvec = pelement->GetUrlImages(pelement->GetUrl());
	}
	CATCH {
		delete pelement;
		return NULL;
	}
	for (size_t i = 0; i < urlvec.Count(); i++)
		pelement->AddDependency(
				(fn.GetPath() + fn.GetPathSeparator() + urlvec.Item(i)).c_str());

	//Nodo Archivo
	/*TODO(02/12/2008 - Alejandro): definir el tipo y de donde obtenerlo*/
	pelement->SetFileNode(wxT("HTML")); //No se traduce, es elemento de XML

	return pelement;
}

/**
 * Objetivo: Obtiene los url de los archivos de imagen de un html.
 * @param[in]	FileName: Nombre del archivo html.
 * @return	Devuelve un vector con los url de las imagenes del html.
 */
wxArrayString HtmlElement::GetUrlImages(const wxString &FileName) {
	wxFFile file(FileName);
	if (!file.IsOpened())
		REPORT_AND_FAIL_VALUE("D:Error al abrir archivo.", wxArrayString());

	wxString strfile;
	wxArrayString urlvec;
	bool readflag = false;
	while (!readflag) {
		readflag = file.ReadAll(&strfile);
		ImageParser *pimgparser = new ImageParser(strfile);
		urlvec = pimgparser->GetSources();
		strfile.clear();
		delete pimgparser;
	}
	if (!file.Close())
		REPORT_AND_FAIL_VALUE("D:Error al cerrar archivo.", wxArrayString());

	return urlvec;
}
} /** namespace suri **/
