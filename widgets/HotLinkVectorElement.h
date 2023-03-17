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

#ifndef HOTLINKVECTORELEMENT_H_
#define HOTLINKVECTORELEMENT_H_

//Includes standard

//Includes Suri
#include "suri/xmlnames.h"
#include "suri/VectorElement.h"

//Includes Wx

//Includes App

//Defines
//define para el nombre del campo
#define SUR_HOTLINK_FIELD "SURHotLink"


//Forwards
namespace suri {
class Part;
}

//! Clase que representa un elemento vectorial con HotLink.
/**
 * Esta clase hereda de VectorElement.
 * Es un vector de puntos que tiene la particularidad de tener un hipervinculo
 * asociado.
 * Este hipervinculo consiste en un html(titulo, foto, descripcion) que se
 * genera en la ventana de propiedades del elemento hotlink.
 * Permite:
 * 	- obtener su pagina de propiedades.
 * 	- setear y obtener el nombre del campo de la base de datos utilizado para
 * guardar el url del html.
 * 	- setear y obtener el valor del campo de la base de datos utilizado para
 * guardar el url del html.
 * \note en las dependencias del elemento no se incluyen ni la imagen ni el html
 * que conforman el HotLink, ya que esto generaria que se dupliquen dichos
 * archivos dentro de la carpeta del elemento.
 * \see Ver nota en HotLinkPropertiesPart::CommitChanges.
 * \warning El nombre del campo HotLink (y de cualquier campo de un shp) no
 *  debe exeder los 10 caracteres ASCII-127 y sera truncado a esa longitud.
 */
class HotLinkVectorElement : public suri::VectorElement {
   //! Ctor. de Copia.
   HotLinkVectorElement(const HotLinkVectorElement &HotLinkVectorElement);

public:
   //! Ctor. default.
   HotLinkVectorElement();
   //! Dtor.
   virtual ~HotLinkVectorElement();
   //! retorna un string para el icono
   virtual wxString DoGetIcon() const;
   //! Funcion estatica para crear un VectorElement
   static HotLinkVectorElement* Create(const std::string &FileName);
   //! Devuelve los Part que se van a mostrar en las Propiedades.
   virtual std::vector<suri::Part*> DoGetParts();

   //! Devuelve el nodo caracteristicas
   virtual wxXmlNode *GetFeaturesNode();
   //! Devuelve los valores del nodo caracteristicas
   bool GetFeaturesValues(wxString &Title, wxString &Description, wxString &ImgUrl,
                          wxString &HtmlUrl);
   //! Agrega el nodo caracteristicas
   virtual void SetFeaturesNode(wxXmlNode *pNode);
   //! Agrega el nodo caracteristicas
   virtual void SetFeaturesNode(const wxString &Title, const wxString &Description,
                                const wxString &ImgUrl, const wxString &HtmlUrl);
   //! Devuelve el nombre del campo que contiene los url de los hotlinks
   wxString GetHotLinkFieldName(const int Layer, wxString &FieldName);
   //! Setea el nombre del campo que contiene los url de los hotlinks
   void SetHotLinkFieldName(const int Layer, const wxString &FieldName);
   //! Setea el nombre del campo que contiene los url de los hotlinks
   void SetHotLinkFieldName(const wxString &FieldName);
   //! Guarda las dependencias en el nodo
   void SetDependencies(const std::string &Path);
   //! Obtiene el contenido del campo SurHotLink del dbf
   wxString GetHotLinkFieldValue() const;
   //! Establece el contenido del campo SurHotLink del dbf
   void SetHotLinkFieldValue(const wxString &Value);
   //! Copia el vector al directorio temporal de la app.
   bool CopyVectorToTemp(const std::string &PathHotLink);
   //! Modifico el url del elemento y de las dependencias
   void ModifyUrls(const std::string &BasePath);
private:
   virtual HotLinkVectorElement* InitializeClass(const std::string &FileName);
};

#endif /* HOTLINKVECTORELEMENT_H_ */
