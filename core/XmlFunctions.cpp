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

#include "suri/XmlFunctions.h"

// Includes standard
#include <sstream>
#include <limits>
#include <algorithm>
#include <set>

// Includes Suri
#include "logmacros.h"
#include "suri/messages.h"
// Includes Wx
#include "wx/sstream.h"
// Includes App
// Defines

/** namespace suri */
namespace suri {
/**
 * Funcion auxiliar que crea un nodo xml con el nombre y valor que se indica por parametro.
 * El objetivo de esta funcion es el de uniformizar la creacion de un nodo y su contenido. 
 * @param[in] NodeName nombre del nodo a crear
 * @param[in] NodeValue valor del nodo xml. El contenido se guarda en un nodo hijo del mismo (de
 * tipo texto)
 * @param[in] pParentNode padre del nodo a crear.
 * @return nueva instancia de nodo xml con el nombre y valor deseado
 * @return NULL en caso contrario    
**/
wxXmlNode* CreateKeyValueNode(const std::string& NodeName, const std::string& NodeValue,
                              wxXmlNode* pParentNode) {
   wxXmlNode* pnewnode = new wxXmlNode(pParentNode, wxXML_ELEMENT_NODE, NodeName.c_str());
   pnewnode->AddChild(new wxXmlNode(pnewnode, wxXML_TEXT_NODE, wxEmptyString, NodeValue));
   return pnewnode;
}

/**
 * Crea nodo tabla de tamanio indicado
 * @param[in] pNode nodo que se quiere limpiar
 * @param[in] NodeName nombre del nodo
 * @param[in] Rows cantidad de filas
 * @param[in] Columns cantidad de columnas
 */
void CreateTable(wxXmlNode* &pNode, const std::string &NodeName, int Rows,
                 int Columns) {
   // Creo una tabla inicializada con 0s
   std::vector<std::vector<std::string> > defaultvalues(
         Rows, std::vector<std::string>(Columns, "0"));
   CreateTable(pNode, NodeName, defaultvalues);
}

/**
 * Retorna la dimansion del nodo
 * @param[in] pNode nodo que se quiere editar/usar
 * @param[out] Rows cantidad de filas
 * @param[out] Columns cantidad de columnas
 */
void GetDimension(const wxXmlNode* pNode, int &Rows, int &Columns) {
   Rows = GetRows(pNode);
   Columns = GetColumns(pNode);
}

/**
 * Retorna la cantidad de filas del nodo tabla
 * @param[in] pNode nodo que se quiere editar/usar
 * @return la cantidad minima entre el valor de la propiedad filas del nodo y la
 * cantidad real de nodos fila del nodo.
 */
int GetRows(const wxXmlNode *pNode) {
   if (!pNode) {
      return 0;
   }
   // Guardo las bandas que afecta la tabla
   wxString property;
   long int tempint = 0;

   // obtiene las columnas de la propiedad
   pNode->GetPropVal(wxT(ROWS_PROPERTY), &property);
   if (!property.ToLong(&tempint)) {
      tempint = 0;
   }

   // verifico que haya suficientes filas y columnas reales
   wxXmlNode *pnode = pNode->GetChildren();
   int rowcount = 0;
   while (pnode != NULL) {
      rowcount++;
      pnode = pnode->GetNext();
   }
   // me quedo con lo minimo de filas que encontre
   return std::min(tempint, static_cast<long int>(rowcount));
}

/**
 * Retorna la cantidad de columnas del nodo tabla
 * @param[in] pNode nodo que se quiere editar/usar
 * @return la cantidad minima entre el valor de la propiedad columnas del nodo
 * y la cantidad real de columnas del nodo.
 */
int GetColumns(const wxXmlNode *pNode) {
   if (!pNode) {
      return 0;
   }
   // Guardo las bandas que afecta la tabla
   wxString property;
   long int tempint = 0;

   // obtiene las columnas de la propiedad
   pNode->GetPropVal(wxT(COLUMNS_PROPERTY), &property);
   if (!property.ToLong(&tempint)) {
      tempint = 0;
   }

   // verifico que haya suficientes columnas reales
   int rowcount = GetRows(pNode), colcount = std::numeric_limits<int>::max();
   for (int i = 0; i < rowcount; i++)
      colcount = std::min(colcount,
                          static_cast<int>(GetRow<std::string>(pNode, i).size()));
   // me quedo con lo minimo de filas que encontre
   return std::min(tempint, static_cast<long int>(colcount));
}

/* ++++++++++++++++++++ Fin de funciones para tablas ++++++++++++++++++++++++ */

/**
 * Compara depth-first. Respetando el orden de los nodos.
 * @param[in] pLhs puntero a un nodo del tipo NodeType
 * @param[in] pRhs puntero a un nodo del tipo NodeType
 * @return true si coinciden en sus valores, y en los valores de sus hijos.
 * @return false si no coinciden en alguno de sus valores.
 */
template<class ComparePolicy, class NodeType>
bool CompareOrdered(NodeType *pLhs, NodeType *pRhs) {
   // Compara respetando el orden
   while (pLhs && pRhs && pLhs->GetName() == pRhs->GetName()
         && ComparePolicy::CompareValue(pLhs, pRhs)
         && ComparePolicy::CompareChildren(pLhs, pRhs)) {
      pLhs = pLhs->GetNext();
      pRhs = pRhs->GetNext();
   }
   // si ambos son NULL quiere decir que salio del bucle por llegar al final de
   // uno y no del otro
   return !pLhs && !pRhs;
}

/**
 * Compara Un-Ordered.
 * @param[in] pLhs puntero a un nodo del tipo NodeType
 * @param[in] pRhs puntero a un nodo del tipo NodeType
 * @return true si los nodos coinciden en sus valores, y en los valores de sus hijos.
 * @return false si los nodos no coinciden en sus valores, y en los valores de sus hijos.
 */
template<class ComparePolicy, class NodeType>
bool CompareUnOrdered(NodeType *pLhs, NodeType *pRhs) {
   if (!pLhs && !pRhs) {
      return true;
   }
   if ((pLhs && !pRhs) || (!pLhs && pRhs)) {
      return false;
   }
   int foundcount = 0;
   int searchedcount = 0;
   static int recursions = 0;
   recursions++;
   std::set<NodeType*> visitednodes;
   NodeType *prhs = NULL;
#ifdef __DEBUG__
   NodeType *plhs = pLhs;
#endif
   while (pLhs) {
      searchedcount++;
      prhs = pRhs;
      // Compara
      while (prhs) {
         REPORT_DEBUG(
               "D:(%d)Verificando \"%s\"(%p) contra \"%s\"(%p)",
               recursions, pLhs->GetName().c_str(), pLhs, prhs->GetName().c_str(), prhs);
         if (visitednodes.find(prhs) == visitednodes.end()) {
            if (pLhs->GetName() == prhs->GetName()
                  && ComparePolicy::CompareValue(pLhs, prhs)
                  && ComparePolicy::CompareChildren(pLhs, prhs)) {
               foundcount++;
               REPORT_DEBUG("D:(%d)Encontrado", recursions);
               visitednodes.insert(prhs);
               break;
            }
         } else {
            REPORT_DEBUG("D:(%d)Nodo visitado \"%s\"(%p)",
                         recursions, prhs->GetName().c_str(), prhs);
         }
         prhs = prhs->GetNext();
      }
      pLhs = pLhs->GetNext();
   }
   // si ambos son NULL quiere decir que salio del bucle por llegar al final de
   // uno y no del otro
   REPORT_DEBUG(
         "D:(%d) %s::searchedcount(%d) ; %s::foundcount(%d)",
         recursions, plhs && plhs->GetParent() ? plhs->GetParent()->GetName().c_str() : "Root", searchedcount, pRhs && pRhs->GetParent() ? pRhs->GetParent()->GetName().c_str() : "Root", foundcount);
   recursions--;
   return searchedcount == foundcount;
}

/** Estructura que permite comparar propiedades de nodos del tipo wxXmlNode. */
struct PropertyPolicy {
   /**
    * Compara el valor contenido en las propiedades de los nodos.
    * @param[in] pLhs propiedad del nodo XML.
    * @param[in] pRhs propiedad del nodo XML.
    * @return true en caso de que contengan valores iguales.
    * @return false en caso de que no contengan valores iguales.
    */
   static bool CompareValue(wxXmlProperty *pLhs, wxXmlProperty *pRhs) {
      if (pLhs) {
         REPORT_DEBUG("D:pLhs(%p)->Propiedad(%s) = %s",
                      pLhs, pLhs->GetName().c_str(), pLhs->GetValue().c_str());
      }
      if (pRhs) {
         REPORT_DEBUG("D:pRhs(%p)->Propiedad(%s) = %s",
                      pRhs, pRhs->GetName().c_str(), pRhs->GetValue().c_str());
      }

      // condicion de salida
      return pLhs && pRhs && pLhs->GetName() && pRhs->GetName()
            && pLhs->GetValue() == pRhs->GetValue();
   }

   /**
    * Devuelve true en caso de que las propiedades de los nodos no sean NULL.
    * False en cualquier otro caso
    * @param[in] pLhs propiedad del nodo XML.
    * @param[in] pRhs propiedad del nodo XML.
    * @return true en caso de que las propiedades de los nodos no sean NULL
    * @return false en caso de que alguna de las propiedades de los nodos sea NULL
    */
   static bool CompareChildren(wxXmlProperty *pLhs, wxXmlProperty *pRhs) {
      return pLhs && pRhs;
   }
};

/** Estructura con metodos que permiten comparar nodos del tipo wxXmlNode. */
struct NodePolicy {
   /**
    * Compara nodos. Compara tipo de nodo y su contenido.
    * @param[in] pLhs nodo XML
    * @param[in] pRhs nodo XML
    * @return true si los nodos coinciden en tipo y contenido.
    * @return false si los nodos difieren en tipo o contenido.
    */
   static bool CompareValue(wxXmlNode *pLhs, wxXmlNode *pRhs) {
      if (pLhs->GetType() == wxXML_ELEMENT_NODE
            && pRhs->GetType() == wxXML_ELEMENT_NODE) {
         return true;
      }
      if (pLhs) {
         REPORT_DEBUG("D:pLhs(%p)->Contenido(%s) = %s",
                      pLhs, pLhs->GetName().c_str(), pLhs->GetContent().c_str());
      }
      if (pRhs) {
         REPORT_DEBUG("D:pRhs(%p)->Contenido(%s) = %s",
                      pRhs, pRhs->GetName().c_str(), pRhs->GetContent().c_str());
      }

      // condicion de salida
      return pLhs && pRhs && pLhs->GetContent() == pRhs->GetContent()/* &&
       CompareUnOrdered<PropertyPolicy,wxXmlProperty>(pLhs->GetProperties(),pRhs->GetProperties())*/;
   }

   /**
    * Compara nodos hijos de los nodos recibidos por parametro. Invoca a
    * ::CompareUnOrdered
    * @param[in] pLhs nodo XML
    * @param[in] pRhs nodo XML
    * @return true en caso de que los nodos hijos sean iguales.
    * @return false en caso de que los nodos hijos no sean iguales.
    */
   static bool CompareChildren(wxXmlNode *pLhs, wxXmlNode *pRhs) {
      return CompareUnOrdered<NodePolicy, wxXmlNode>(pLhs->GetChildren(),
                                                     pRhs->GetChildren());
   }
};

/**
 * Compara dos nodos convirtiendolos a string
 * @param[in] Lhs primer nodo a comparar
 * @param[in] Rhs segundo nodo a comparar
 * @return resultado de comparar los nodos como string
 */
bool operator==(const wxXmlNode &Lhs, const wxXmlNode &Rhs) {
#ifdef __UNUSED_CODE__
   wxXmlNode *plhs = const_cast<wxXmlNode*>(&lhs),
   *prhs = const_cast<wxXmlNode*>(&rhs);
   CompareUnOrdered<NodePolicy, wxXmlNode>(plhs, prhs);
   return false;
#endif
   return Xml2String(&Lhs) == Xml2String(&Rhs);
}

/** Retorna el nodo XML en forma de string */
/**
 * guarda en wxString el xml del elemento
 * @param[in] pNode nodo que se quiere convertir a string
 * @return string con datos de nodo
 */
std::string Xml2String(const wxXmlNode *pNode) {
   if (!pNode) {
      return message_NOT_AVAILABLE;
   }
   wxXmlDocument doc;
   doc.SetRoot(new wxXmlNode(*pNode));
   wxString temp;
   wxStringOutputStream ss(&temp);
   doc.Save(ss);
   //Como wx agrega el encoding, se lo quito a mano
   int encodingInit = temp.find(_("<?"));
   int encodingEnd = temp.find(_("?>"));
   //Muevo al final del encoding
   encodingEnd += 3;
   temp = temp.erase(encodingInit, encodingEnd);
   return temp.mb_str();
}

/**
 *  Accede al nodo de nombre Name, si name contiene | se busca el segundo
 * string en el nodo del primero, el tercer string en el nodo del segundo, ect.
 * Retorna
 * @param[in] Name es el nombre del nodo buscado, en caso de ser "" retorna
 *  el nodo pSeed
 * @param[in] pSeed se usa para poder iterar por los hijos, si es != NULL
 *  busca a partir de ese nodo y sino busca en los hijos de pXmlProperties_
 * @return Un nodo wxXmlNode, el root, o el hijo de root o pSeed con nombre
 *  Name
 * \todo hace que funcione correctamente en caso de usar | en Name
 * \attention el wxXmlNode* retornado es responsabilidad de esta clase(no eliminar externamente)
 */
wxXmlNode* GetNodeByName(const wxString &Name, wxXmlNode *pSeed) {
   return const_cast<wxXmlNode*>(GetNodeByName(Name,
                                               const_cast<const wxXmlNode*>(pSeed)));
}

/**
 *  Accede al nodo de nombre Name, si name contiene | se busca el segundo
 * string en el nodo del primero, el tercer string en el nodo del segundo, ect.
 * @param[in] Name es el nombre del nodo buscado, en caso de ser "" retorna
 *  el nodo pSeed
 * @param[in] pSeed se usa para poder iterar por los hijos, si es != NULL
 *  busca a partir de ese nodo y sino busca en los hijos de pXmlProperties_
 * @return Un nodo wxXmlNode, el root, o el hijo de root o pSeed con nombre
 *  Name. Es un const.
 * \todo hace que funcione correctamente en caso de usar | en Name
 * \attention el wxXmlNode* retornado es responsabilidad de esta clase(no eliminar externamente)
 */
const wxXmlNode* GetNodeByName(const wxString &Name, const wxXmlNode *pSeed) {
   const wxXmlNode* pnodeold;
   const wxXmlNode* pnodeaux;
   if (Name.IsEmpty() || !pSeed) {
      return pSeed;
   }
   std::vector<std::string> nodes = tokenizer(Name.c_str(), "|");
   if (nodes.size() != 1) {
      std::vector<std::string>::iterator it = nodes.begin();
      const wxXmlNode *pnode = pSeed;
      while (it != nodes.end()) {
         pnodeold = pnode;
         pnode = GetNodeByName(*it, pnode);
         if (!pnode) {
            // Busca si hay otros nombres repetidos en este nodo o anteriores
            while ((it != nodes.begin()) && (!pnode)) {
               it--;
               pnodeaux = pnodeold;
               pnodeold = pnodeold->GetNext();
               while ((pnodeold) && (!pnode)) {
                  if (pnodeold->GetName() == *it) {
                     it++;
                     pnode = GetNodeByName(*it, pnodeold);
                     if (!pnode) {
                        it--;
                     }
                  }
                  pnodeold = pnodeold->GetNext();
               }
               pnodeold = pnodeaux->GetParent();
            }
            if (!pnode) {
               REPORT_DEBUG("D:No se encontro el nodo %s.", Name.c_str());
               return NULL;
            }
         }
         it++;
      }
      return pnode;
   }
   const wxXmlNode *pnode = pSeed;
   if (pnode) {
      pnode = pnode->GetChildren();
   }
   while (pnode) {
      if (pnode->GetName() == Name) {
         return pnode;
      }
      pnode = pnode->GetNext();
   }REPORT_DEBUG("D:No se encontro el nodo %s.", Name.c_str());
   return NULL;
}

}
