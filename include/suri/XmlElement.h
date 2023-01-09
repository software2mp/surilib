/*! \file XmlElement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
