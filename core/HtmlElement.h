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
