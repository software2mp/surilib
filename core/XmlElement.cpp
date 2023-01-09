/*! \file XmlElement.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/XmlElement.h"

// Includes standard
#include <vector>

// Includes suri
#include "suri/messages.h"
#include "logmacros.h"
#include "suri/xmlnames.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/XmlFunctions.h"

// Includes wx
#include "wx/sstream.h"
#ifdef __DEBUG__
// Include para escribir a stdout
#include "wx/wfstream.h"
#endif

/** namespace suri */
namespace suri {
/** codificacion de los XML */
const std::string XmlElement::xmlEncoding_ = XML_FILE_ENCODING;
const std::string XmlElement::utf8XmlEncoding_ = "UTF-8";

/** ctor */
/**
 * @param[in] ElementName Nombre del nodo del elemento
 * @param[in] ElementType Propiedad TIPO del nodo, de estar vacia, no se agrega
 * @return instancia de la clase XmlElement
 */
XmlElement::XmlElement(const wxString& ElementName, const wxString& ElementType) {
   pXmlProperties_ = new wxXmlNode(wxXML_ELEMENT_NODE, ElementName);
   if (ElementType != wxEmptyString) {
      pXmlProperties_->AddProperty(TYPE_PROPERTY, ElementType);
   }
}

/**
 * Destructor.
 */
XmlElement::~XmlElement() {
   delete pXmlProperties_;
}

/**
 *  Accede al nodo de nombre Name, si name contiene | se busca el segundo
 * string en el nodo del primero, el tercer string en el nodo del segundo, ect.
 * Retorna
 * @param[in] Name es el nombre del nodo buscado, en caso de ser "" retorna
 *  el nodo root (pXmlProperties_)
 * @param[in] pSeed se usa para poder iterar por los hijos, si es != NULL
 *  busca a partir de ese nodo y sino busca en los hijos de pXmlProperties_
 * @return Un nodo wxXmlNode, el root, o el hijo de root o pSeed con nombre
 *  Name
 * \todo hace que funcione correctamente en caso de usar | en Name
 * \attention el wxXmlNode* retornado es responsabilidad de esta clase(no eliminar externamente)
 */
wxXmlNode* XmlElement::GetNode(const wxString &Name, wxXmlNode *pSeed) const {
   return GetNodeByName(Name, pSeed ? pSeed : pXmlProperties_);
}

/** Agrega el nuevo nodo, si ya existe uno con ese nombre lo reemplaza */
/**
 * Agrega a pParentNode un nodo wxXML_ELEMENT_NODE con nombre Name,
 * un hijo tipo wxXML_TEXT_NODE de nombre Content(texto "") y una
 * propiedad de nombre PropertyName y valor PropertyValue.
 * Si existe usa Replace para decidir que hacer.
 * Si en name hay varios nombres separados por | agrega nodos uno
 * dentro del otro con los nombres respectivos. Al ultimo nodo
 * le agrega el contenido y propiedades de otros parametros
 * @param[in] pParentNode nodo dentro del que se quiere agregar un nodo
 * @param[in] Name Nombre del nodo que se quiere agregar
 * @param[in] Content Contenido del nodo que se quiere agregar. Si esta en
 * blanco no se agrega contenido
 * @param[in] PropertyName nombre de la primer propiedad del nodo. Si esta en
 * blanco no se agrega nada.
 * @param[in] PropertyValue valor que toma la propiedad del nodo. El
 * nombre de la propiedad no debe estar en blanco para que se configure
 * correctamente
 * @param[in] Replace Si el nodo existe este flag decide si se remplaza o no
 * @return Nodo generado
 * \todo eliminar if (__UNUSED_CODE__) por codigo correcto
 * \todo propagar el replace, si no afecta la funcionalidad acutal
 * \attention el nodo retornado(y su eliminacion) es responsabilidad de esta clase. No eliminar externamente
 */
wxXmlNode* XmlElement::AddNode(wxXmlNode *pParentNode, const wxString &Name,
                               const wxString &Content, const wxString &PropertyName,
                               const wxString &PropertyValue, bool Replace) {
   wxString name = Name;
   std::vector<std::string> nodes = tokenizer(name.c_str(), "|");
   if (nodes.size() > 1) {
// El codigo comentado sirve para completar la funcionalidad en caso de que se
// envie Content || PropertyName || PropertyValue con un Name compuesto con tokens (|)
// ya que en esa condicion se saltea el utlimo nodo
#if __UNUSED_CODE__
      name = nodes.back().c_str();
      nodes.pop_back();

      std::vector<std::string>::iterator it = nodes.begin();
      wxXmlNode *pnode = pParentNode;
      while ( it!=nodes.end() )
      {
         // \todo propagar el replace, si no afecta la funcionalidad acutal
         pnode = AddNode(pnode, (*it).c_str() );
         if ( !pnode ) {
            REPORT_AND_FAIL_VALUE("D:No se encontro el nodo %s.",
                  NULL, name.c_str() );
         }
         it++;
      }

      return AddNode(pnode, name, Content, PropertyName, PropertyValue, Replace);
#else // __UNUSED_CODE__
      if (!Content.empty() || !PropertyName.empty() || !PropertyValue.empty()) {
         name = nodes.back().c_str();
         nodes.pop_back();
         REPORT_WARNING(
               "D:Advertencia, el ultimo (%s) nodo no sera agregado", name.c_str());
      }
      std::vector<std::string>::iterator it = nodes.begin();
      wxXmlNode *pnode = pParentNode;
      while (it != nodes.end()) {
         pnode = AddNode(pnode, (*it).c_str());
         if (!pnode) {
            REPORT_AND_FAIL_VALUE("D:No se encontro el nodo %s.", NULL, name.c_str());
         }
         it++;
      }
      return pnode;
#endif   // __UNUSED_CODE__
   }
   // Crea un nodo con el nombre dado
   wxXmlNode *pnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, name);
   // Si tiene contenido se lo agrega
   if (!Content.IsEmpty()) {
      pnode->AddChild(new wxXmlNode(pnode, wxXML_TEXT_NODE, wxEmptyString, Content));
   }
   if (!PropertyName.IsEmpty()) {
      pnode->AddProperty(PropertyName, PropertyValue);
   }
   // Agrega el nodo
   AddNode(pParentNode, pnode, Replace);
   // regresa el nodo creado
   return pnode;
}

/**
 *  Agrega el nuevo nodo, si existe uno con el mismo nombre lo reemplaza.
 * @param[in] pParentNode es el nodo padre
 * @param[in] pNewNode es el nuevo nodo
 * @param[in] Replace flag que indica si debe ser reemplazado
 * \post Se agrega el nodo como uno de los hijos de pParentNode. Si exitiera un
 *  hijo con el mismo nombre pNewNode->GetName() y Replace==true, lo reemplaza
 * (destruyendo el nodo existente)
 */
void XmlElement::AddNode(wxXmlNode *pParentNode, wxXmlNode *pNewNode, bool Replace) {
   if (!pNewNode) {
      REPORT_ERROR("D:NewNode == NULL");
      return;
   }
   // lo agrego como raiz
   if (pParentNode == NULL) {
      delete pXmlProperties_;
      pXmlProperties_ = pNewNode;
      return;
   }
   // Si no tiene hijos, lo agrego primero
   if (!pParentNode->GetChildren()) {
      pParentNode->AddChild(pNewNode);
      return;
   }
   // Busca un nodo con el mismo nombre
   wxXmlNode *psearchednode;
   if (Replace && (psearchednode = GetNode(pNewNode->GetName(), pParentNode))) {
      // Lo reemplaza
      pParentNode->InsertChild(pNewNode, psearchednode->GetNext());
      pParentNode->RemoveChild(psearchednode);
      delete psearchednode;
   } else  // si no existe
   {
      // busco el ultimo nodo
      wxXmlNode *iterator = pParentNode->GetChildren();
      if (iterator) {
         while (iterator->GetNext())
            iterator = iterator->GetNext();
         // Lo agrego
         pNewNode->SetParent(pParentNode);
         iterator->SetNext(pNewNode);
      }
   }
}

/**
 * Agrega un nodo al elemento. Si parametro Next=true agrega pNewNode despues
 * de pReferenceNode, sino guarda pNewNode antes de pReferenceNode.
 * Si pReferenceNode==NULL se agrega como primer(Next==false) o
 * ultimo(Next==true) nodo de pParentNode
 * @param[in] pParentNode Nodo donde quiero agregar el nodo
 * @param[in] pNewNode Nodo que quiero agregar
 * @param[in] pReferenceNode Nodo que uso de referencia para agregar pNewNode.
 * Puede ser NULL
 * @param[in] Next bool que indica si hay que agregar pNewNode antes(false) o
 * despues(true) del nodo de referencia.(Default true)
 * @return indica si pudo agregar el nodo
 */
void XmlElement::AddNode(wxXmlNode *pParentNode, wxXmlNode *pNewNode,
                         wxXmlNode *pReferenceNode, bool Next) {
   if (pNewNode == NULL) {
      REPORT_DEBUG("D: pNewNode no esta definido");
      return;
   }

   // Si pParentNode==NULL busco en raiz
   wxXmlNode* pparentnode = pParentNode;
   if (pParentNode == NULL) {
      pparentnode = pXmlProperties_;
   }

   // Busco el nodo de referencia.
   wxXmlNode* preferencenode = pparentnode->GetChildren();
   wxXmlNode* ppreviewsnode = NULL;
   while (preferencenode != NULL && preferencenode != pReferenceNode) {
      ppreviewsnode = preferencenode;
      preferencenode = preferencenode->GetNext();
   }
   if (preferencenode == NULL and pReferenceNode != NULL) {
      REPORT_DEBUG("D: pReferenceNode no esta en el elemento");
      return;
   }

   // Si pReferenceNode==NULL y Next==true agrego pNewNode como ultimo
   // nodo de pParentNode
   if ((pReferenceNode == NULL && Next == true)) {
      preferencenode = ppreviewsnode;
   }

   // Agrego pNewNode a pParentNode
   if (Next) {
      // Lo agrego despues de la referencia o al final(preferencenode=ppreviewsnode)
      pNewNode->SetParent(pparentnode);
      preferencenode->SetNext(pNewNode);
   } else {
      // Lo agrego antes de la referencia o al comienzo(preferencenode=NULL)
      pparentnode->InsertChild(pNewNode, preferencenode);
   }
}

/**
 * Reemplaza pOldNode con pNewNode en pParenNode.
 * @param[in] pParentNode Nodo donde quiero reemplazar el nodo.
 * Si es NULL se usa el nodo del elemento
 * @param[in] pNewNode Nodo que quiero agregar.
 * Si es NULL solo se elimina pOldNode
 * @param[in] pOldNode Nodo que quiero reemplazar.
 * Si es NULL o no lo encuentra falla.
 * @param[out] pOldNode Si tiene exito en reemplazar el nodo, el puntero
 * pOldNode se elimina y se deja en NULL. Si existen copias de este puntero
 * se hacen invalidas y fallaran cuando se usen.
 * @return indica si pudo reemplazar el nodo
 */
bool XmlElement::ReplaceNode(wxXmlNode *pParentNode, wxXmlNode* pNewNode,
                             wxXmlNode* &pOldNode) {
   if (pOldNode == NULL) {
      REPORT_DEBUG("D: el nodo no esta definido");
      return false;
   }

   // Si pParentNode==NULL busco en raiz
   wxXmlNode* pparentnode = pParentNode;
   if (pParentNode == NULL) {
      pparentnode = pXmlProperties_;
   }

   // Busco el nodo que quiero eliminar.
   wxXmlNode* poldnode = pparentnode->GetChildren();
   while (poldnode != NULL && poldnode != pOldNode)
      poldnode = poldnode->GetNext();

   if (poldnode == NULL) {
      REPORT_DEBUG("D: pOldNode no esta en el elemento");
      return false;
   }

   // Agrego pNewNode despues del nodo
   if (pNewNode) {
      pNewNode->SetParent(pparentnode);
      pNewNode->SetNext(poldnode->GetNext());
      poldnode->SetNext(pNewNode);
   }

   // Elimino pOldNode(wxXmlNode::RemoveChild) de pParentNode
   if (!RemoveNode(pparentnode, poldnode)) {
      // Elimino el nodo que agregue recien
      if (pNewNode) {
         RemoveNode(pparentnode, pNewNode);
      }
      return false;
   }

   return true;
}

/**
 * Elimina el nodo pNode de pParenNode.
 * @param[in] pParentNode Nodo donde esta el nodo que quiero eliminar.
 * Si NULL usa nodo del elemento
 * @param[in] pNode Nodo que quiero agregar. Si es NULL falla
 * @param[out] pNode Si tiene exito en eliminar el nodo, el puntero pNode
 * se elimina y se deja en NULL. Si existen copias de este puntero se hacen
 * invalidas y fallaran cuando se usen.
 * @return indica si pudo eliminar el nodo
 */
bool XmlElement::RemoveNode(wxXmlNode *pParentNode, wxXmlNode* &pNode) {
   if (pNode == NULL) {
      REPORT_DEBUG("D: el nodo a eliminar no esta definido");
      return false;
   }

   // Si pParentNode==NULL busco en raiz
   wxXmlNode* pparentnode = pParentNode;
   if (pParentNode == NULL) {
      pparentnode = pXmlProperties_;
   }

   // Elimino pOldNode(wxXmlNode::RemoveChild) de pParentNode
   if (!pparentnode->RemoveChild(pNode)) {
      REPORT_DEBUG("D: el nodo a eliminar no esta en parent node");
      return false;
   }

   delete pNode;
   pNode = NULL;
   return true;
}

/**
 * guarda en wxString el xml del elemento
 * @return nodo XML en forma de string
 */
wxString XmlElement::GetXmlAsString() const {
   wxXmlDocument doc;
   doc.SetRoot(GetNode(wxT("")));
   doc.SetEncoding(suri::XmlElement::xmlEncoding_);
   doc.SetFileEncoding(suri::XmlElement::xmlEncoding_);
   wxString temp;
   wxStringOutputStream ss(&temp);
   doc.Save(ss);
   doc.DetachRoot();
   return temp;
}

/**
 * Devuelve el valor de la propiedad tipo del nodo elemento.
 * @return	cadena con el tipo de elemento en caso de exito.
 * @return	"N/A" en caso de que no se encuentre el nodo elemento o la propiedad
 * tipo (del nodo elemento).
 */
wxString XmlElement::GetTypeAsString() {
   wxXmlNode *pcontextnode = GetNode(wxT(""));
   wxString type = _(message_NOT_APPLICABLE);
   if (pcontextnode) {
      type = pcontextnode->GetPropVal(TYPE_PROPERTY, _(message_NOT_APPLICABLE));
   }
   return type;
}

/**
 * Devuelve la descripcion del tipo de formato de un elemento. Invoca a
 * XmlElement::GetTypeAsString().
 * @return std::string con la descripcion del tipo de formato de un elemento.
 */
wxString XmlElement::GetFormatDescription() {
   return GetTypeAsString();
}

#ifdef __DEBUG__
void XmlElement::PrintProperties() const {
   wxXmlDocument *pdoc = new wxXmlDocument;
   pdoc->SetFileEncoding(suri::XmlElement::xmlEncoding_);
   pdoc->SetRoot(pXmlProperties_);
   // Si se define el simbolo con el nombre de un archivo
   // entonces salva el XML a dicho archivo, sino
   // lo saca por el log
#if !defined(__PRINTPROPERTIES_OUTPUT_FILE__)
   wxFFile ffile(stdout);
#else
   wxFFile ffile(__PRINTPROPERTIES_OUTPUT_FILE__, "w");
#endif
   if (ffile.IsOpened()) {
      wxFFileOutputStream output(ffile);
      pdoc->Save(output);
      ffile.Close();
      ffile.Detach();
   } else {
      REPORT_DEBUG("D:No se pudo imprimir la salida del XML");
   }
   pdoc->DetachRoot();
   delete pdoc;
}
#endif
}  // namespace suri
