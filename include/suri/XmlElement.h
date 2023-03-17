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

#ifndef XMLELEMENT_H_
#define XMLELEMENT_H_

// Includes standard
#include <vector>

// Includes wx
#include "wx/wx.h"
#include "wx/xml/xml.h"

/** namespace suri */
namespace suri {
/** Clase elemento con las propiedades en un nodo XML */
/**
 *  Esta clase tiene como objetivo facilitar la entrada/salida de los parametros
 * de los distintos elementos de los contextos. Utiliza XML.
 *  Cada elemnto que derive de esta clase debera implementar GetProperties() a
 * fin de devolver un nodo xml con las propiedades de si mismo y todos los
 * sub-elementos que contenga.
 */
class XmlElement {
   /** Ctor. de Copia. */
   XmlElement(const XmlElement &XmlElement);

public:
   /*! guarda la codificacion de los archivos XML */
   static const std::string xmlEncoding_;
   /** encoding para xml en utf8 **/
   static const std::string utf8XmlEncoding_;
   /** Ctor */
   XmlElement(const wxString& ElementName, const wxString& ElementType = wxEmptyString);
   /** Dtor */
   virtual ~XmlElement();
   /** Permite acceder a un nodo XML basado en nombre */
   virtual wxXmlNode *GetNode(const wxString &Name = wxT(""),
                              wxXmlNode *pSeed = NULL) const;
   /** Permite agregar un nodo con contenido y una propiedad */
   virtual wxXmlNode *AddNode(wxXmlNode *pParentNode, const wxString &Name,
                              const wxString &Content = wxT(""),
                              const wxString &PropertyName = wxT(""),
                              const wxString &PropertyValue = wxT(""), bool Replace =
                                    true);
   /** Agrega el nuevo nodo, si ya existe uno con ese nombre lo reemplaza */
   virtual void AddNode(wxXmlNode *pParentNode, wxXmlNode *pNewNode,
                        bool Replace = true);
   /** Agrega pNewNode a pParentNode antes o despues de pReferenceNode. */
   virtual void AddNode(wxXmlNode *pParentNode, wxXmlNode *pNewNode,
                        wxXmlNode *pReferenceNode, bool Next = true);
   /** Reemplaza pOldNode en pParentNode con pNewNode. */
   virtual bool ReplaceNode(wxXmlNode *pParentNode, wxXmlNode *pNewNode,
                            wxXmlNode* &pOldNode);
   /** Elimina el nodo pNode de pParenNode. */
   bool RemoveNode(wxXmlNode *pParentNode, wxXmlNode* &pNode);

   /** Retorna el nodo XML padre de las propiedades del elemento */
   /**
    * @return una copia del nodo xml con las propiedades, responsabilidad de
    *         el que llama a liberar (el que recibe debe hacer delete)
    */
   virtual wxXmlNode *GetProperties() {
      return new wxXmlNode(*pXmlProperties_);
   }
   /** Retorna el nodo XML en forma de string */
   wxString GetXmlAsString() const;
   /** Retorna el tipo de elemento */
   virtual wxString GetTypeAsString();
   /** Retorna la descripcion del tipo de formato del elemento. */
   virtual wxString GetFormatDescription();

#ifdef __DEBUG__
   /** Imprime las propiedades en log */
   void PrintProperties() const;
#endif
protected:
   wxXmlNode *pXmlProperties_; /*! Las propiedades XML del elemento */
};
}

#endif /*XMLELEMENT_H_*/
