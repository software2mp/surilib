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

#ifndef MULTIMEDIAELEMENT_H_
#define MULTIMEDIAELEMENT_H_

//Includes standard

//Includes Suri
#include "suri/Element.h"
#include "suri/Part.h"

//Includes Wx

//Includes App

//Defines

//Forwards
namespace suri {
//! Clase que permite generar elementos con contenido multimedia (Fotografia, PDF, audio, video)
/**
 * Permite:
 * 	- generar elementos Fotografia a partir de un archivo de imagen.
 * 	- obtner un string con el codigo html del html que se despliega en el
 * visualizador del contexto.
 * 	- obtener la ventana de propiedades especificas del elemento multimedia.
 */
class MultimediaElement: public suri::Element {
	//! Ctor. de Copia.
	MultimediaElement(const MultimediaElement &MultimediaElement);

public:
	//! Constructor
	MultimediaElement();
	//!Destructor
	virtual ~MultimediaElement();
	//! retorna un string para el icono
	virtual wxString GetIcon() const;
	   /** retorna un string para el icono */
	virtual wxString DoGetIcon() const;
	//! Devuelve el contenido del nodo titulo
   std::string GetTitle() const;
	//! retorna los detalles del elemento
	wxString GetDetails() const {
		return wxT("");
	}
   std::string GetPreviewImage() const;
	//! Devuelve el contenido del nodo texto
	std::string GetText() const;
	//! Devuelve el Tipo de Elemento
	virtual wxString GetTypeAsString();
	//! Devuelve un string con el html a insertar en el contexto
	bool GetHtmlInfo(wxString &Html) const;
	//! Establece el contenido del nodo titulo
	void SetTitle(const wxString &Title);
	//! Establece el contenido del nodo texto
	void SetText(const wxString &Text);
   void SetPreviewImage(const std::string& Url);
	//! Muestra la herramienta de propiedades
	virtual void ShowPropertyTool() {}
	//! Funcion estatica para crear un MultimediaElement
	static MultimediaElement* Create(const std::string &FileName,
			const std::string &Title = wxT(""),
			const std::string &Text = wxT(""));
   /** Devuelve vector con Parts especificos del elemento. */
   virtual std::vector<suri::Part*> DoGetParts(
         DataViewManager* pDataViewManager = NULL, LayerInterface *pLayer = NULL,
         DatasourceInterface *pDatasource = NULL);

protected:
private:
	virtual MultimediaElement* Initialize(const std::string &FileName);
};
} /** namespace suri **/
#endif /*MULTIMEDIAELEMENT_H_*/
