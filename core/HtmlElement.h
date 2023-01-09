//<COPYRIGHT> a reemplazar a futuro

#ifndef HTMLELEMENT_H_
#define HTMLELEMENT_H_

//Includes standard
#include <vector>
#include <string>

//Includes Suri
#include "suri/Element.h"
#include "suri/Part.h"

//Includes Wx

//Includes App

//Defines

//Forwards
namespace suri {
//! Clase que representa un elemento Html
/**
 * Representa elementos html asociados a un archivo html.
 */
class HtmlElement: public suri::Element {
	//! Ctor. de Copia.
	HtmlElement(const HtmlElement &HtmlElement);

public:
	//! Ctor.
	HtmlElement();
	//! Dtor.
	virtual ~HtmlElement();
	//! retorna un string para el icono
	virtual wxString GetIcon() const;
   /** retorna un string para el icono */
   virtual wxString DoGetIcon() const;
	//! retorna los detalles del elemento
	virtual wxString GetDetails() const;
	//! Muestra la herramienta de propiedades
	virtual void ShowPropertyTool();
	//! Funcion estatica para crear un HtmlElement
	static HtmlElement* Create(const std::string &FileName);
	//! Devuelve los Part que se van a mostrar en las Propiedades.
	virtual std::vector<suri::Part *> DoGetParts() {
		return std::vector<suri::Part *>();
	}
protected:
	//! Obtiene los url de los archivos de imagen de un html.
	wxArrayString GetUrlImages(const wxString &FileName);
private:
	virtual HtmlElement* Initialize(const std::string &FileName) {
		return this;
	}
};
} /** namespace suri */

#endif /*HTMLELEMENT_H_*/
