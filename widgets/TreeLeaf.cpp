/*! \file TreeLeaf.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "TreeLeaf.h"

// Includes standard

// Includes Suri
#include "logmacros.h"
#include "resources.h"

// Includes Wx
#include "wx/html/htmprint.h"
#include "wx/dcmemory.h"
#include "wx/html/htmlwin.h"

// Includes App

// Defines

#define LEAFNODE \
   "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">	\
<tr>\
  <td>\
    <table border=\"0\"  cellspacing=\"0\" cellpadding=\"0\">\
      <tr>\
        <td>\
			<img width=\"16\" height=\"16\" src=\"__ICON__\">\
		</td>\
      </tr>\
    </table>\
      </td>\
		<td width=\"100%\">\
			<table border=\"0\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\">	\
				<tr>\
					<td>\
					  &nbsp;&nbsp;__NAME__\
					</td>\
				</tr>\
			</table>\
		</td>\
		__CHECKBOXHTML__\
	</tr>\
</table>\
"

#define CHECKBOXHTML \
   "\
		<td>\
			<table align= \"left\" width=\"16px\">\
				<tr>\
					<td align=\"right\">\
						<a href=\"checkbox\">\
							<img src=\"__CHOICE__\">\
						</a>\
					</td>\
				</tr>\
			</table>\
		</td>"

#define NAME_FLAG "__NAME__"
#define CHOICE_FLAG "__CHOICE__"
#define ICON_FLAG "__ICON__"
#define CHECKBOXHTML_FLAG "__CHECKBOXHTML__"

#if defined(__LINUX__)
#define LEAFOFFSET 32
#else
#define LEAFOFFSET 28
#endif
#define CHECKOFFSET 20

namespace suri {
/**
 * Constructor de nodo
 * del arbol.
 */
TreeLeaf::TreeLeaf(HtmlConfigurationData* pHtmlData, long VisualizationFlags) :
      TreeNode(pHtmlData, VisualizationFlags) {
}

TreeLeaf::~TreeLeaf() {
}

/**
 * Retorna la cantidad de nodos visibles debajo del nodo.
 * @return int cantidad de nodos visibles debajo del nodo
 */
int TreeLeaf::GetVisibleNodeCount() {
   return 1;
}

/**
 * Actualiza el html de todos los items visibles. El html lo guarda en el vector
 * que recibe por parametro, si faltan elementos los agrega, si hay elementos
 * de mas los elimina.
 * @param[in] WindowWidth Ancho en pixels de la ventana donde quiero graficarlo
 * @param[in] VectorIndex posicion del proximo item a modificar del vector.
 * @param[in] HtmlVector vector en blanco o no cuyo contenido se quiere actualizar.
 * El contenido previo del vector se perdera.
 * @param[out] HtmlVector vector configurado con el html de todos los elementos
 * visibles. Si se configuran los items en el orden del vector la lista parecera un
 * arbol.
 */
void TreeLeaf::UpdateHtmlVectorWithVisibleItems(
      int WindowWidth, int &VectorIndex, std::vector<DecoratedHtmlItem> &HtmlVector) {
   // Agrego html de hoja
   DecoratedHtmlItem decoratedhtml(GetNodeId(), GetLeafHtml(WindowWidth));
   SetHtmlItemToVector(VectorIndex, decoratedhtml, HtmlVector);
   VectorIndex++;
}

/**
 * Retorna el estado de activacion del nodo
 * @return estado de activacion del nodo
 */
HtmlConfigurationData::State TreeLeaf::UpdateStateUsingLeafs() {
   return pHtmlConfigurationData_->GetState();
}

/**
 * Actualiza el estado de activacion del nodo y de todos sus hijos
 * @param[in] NodeId id del nodo que quiero actualizar
 * @param[in] NewState nuevo estado del nodo
 */
void TreeLeaf::UpdateState(const ItemId &NodeId,
                           HtmlConfigurationData::State NewState) {
   if (nodeId_.Compare(NodeId) == 0) {
      pHtmlConfigurationData_->SetState(NewState);
   } else {
      REPORT_DEBUG(
            "Debug: Error en arbol. Llamada a TreeLeaf::UpdateState con id erroneo");
   }
}

/**
 * Configura id del nodo y actualiza el de todos los subnodos
 * @param[in] Id nuevo id del nodo.
 */
void TreeLeaf::UpdateNodeId(const TreeNodeId &Id) {
   SetNodeId(Id);
}

/**
 * Imprime error por debug porque es nodo hoja
 * @param[in] NodeId id de nodo interno que se quire expander/contraer
 * @param[in] Expand indica si el nodo deberia estar expandido o no
 */
void TreeLeaf::Expand(const ItemId &NodeId, bool Expand) {
   REPORT_DEBUG("Debug: Error en arbol. Llamada a TreeLeaf::Expand");
}

/**
 * Retorna siempre falso porque es nodo hoja
 * @param[in] NodeId id de nodo interno que se quire expander/contraer
 * @return false
 */
bool TreeLeaf::IsExpanded(const ItemId &NodeId) {
   REPORT_DEBUG("Debug: Error en arbol. Llamada a TreeLeaf::IsExpanded");
   return false;
}

/**
 * Informa si el nodo es visible o esta oculto dentro de
 * un nodo colapsado.
 * Retorna siempre true.
 * @param[in] NodeId id de nodo
 */
bool TreeLeaf::IsVisible(const ItemId &NodeId) {
   return true;
}

/**
 * Retorna HtmlConfigurationData de nodo si identificador coincide con NodeId.
 * @param[in] NodeId id de nodo
 * @return HtmlConfigurationData* de nodo si identificador coincide con NodeId.
 * @return NULL si no coincide
 */
HtmlConfigurationData* TreeLeaf::GetHtmlConfigurationData(const ItemId &NodeId) {
   if (nodeId_.Compare(NodeId) == 0) {
      return pHtmlConfigurationData_;
   }
   return NULL;
}

/**
 * Retorna html con datos de nodo interno. El html encabeza el nivel dentro del
 * arbol
 * @param[in] WindowWidth Ancho en pixels de la ventana donde quiero graficarlo
 * @return html de nodo interno
 */
std::string TreeLeaf::GetLeafHtml(int WindowWidth) {
   std::string innernode = LEAFNODE;

   // Configuro nombre del nodo
   int offset =
         ((visualizationFlags_ & CheckableNode) != 0) ? LEAFOFFSET + CHECKOFFSET :
                                                        LEAFOFFSET;
   int width = std::max(WindowWidth - offset, 0);
   size_t position = innernode.find(NAME_FLAG);
   innernode.replace(position, sizeof(NAME_FLAG) - 1,
                     GetSizedText(pHtmlConfigurationData_->GetName(), width));

   // Configuro icono cargado en HtmlConfigurationData_
   std::string src = pHtmlConfigurationData_->GetIcon();
   position = innernode.find(ICON_FLAG);
   innernode.replace(position, sizeof(ICON_FLAG) - 1, src);

   // Configuro check
   std::string checkhtml = "";
   if ((visualizationFlags_ & CheckableNode) != 0) {
      checkhtml = CHECKBOXHTML;
      src = button_CHECK_UNCHECKED;
      if (pHtmlConfigurationData_->GetState() == HtmlConfigurationData::ON) {
         src = button_CHECK_CHECKED;
      }
      position = checkhtml.find(CHOICE_FLAG);
      checkhtml.replace(position, sizeof(CHOICE_FLAG) - 1, src);
   }
   position = innernode.find(CHECKBOXHTML_FLAG);
   innernode.replace(position, sizeof(CHECKBOXHTML_FLAG) - 1, checkhtml);

   return innernode;
}

/**
 * Busca el nodo con id 'NodeId' y retorna el id del primer hijo.
 * @param[in] NodeId id de nodo
 * @return ItemId id del primer hijo del nodo con id 'NodeId'
 * @return INVALIDTREEID si 'NodeId' es nodo hoja o no encontro NodeId
 */
ItemId TreeLeaf::GetFirstChild(const ItemId &NodeId) {
   REPORT_DEBUG("Debug: Error en arbol. Llamada a TreeLeaf::GetFirstChild");
   return INVALIDTREEID;
}

/**
 * Busca el nodo con id 'NodeId' y retorna el siguiente hijo del mismo padre.
 * @param[in] NodeId id de nodo
 * @return ItemId id del proximo nodo del mismo padre
 * @return INVALIDTREEID si no tiene proximo hermano o no encontro !NodeId
 */
ItemId TreeLeaf::GetNextSibling(const ItemId &NodeId) {
   REPORT_DEBUG("Debug: Error en arbol. Llamada a TreeLeaf::GetNextSibling");
   return INVALIDTREEID;
}

/**
 * Busca el nodo con id 'NodeId' y retorna el padre.
 * @param[in] NodeId id de nodo existente en arbol
 * @return ItemId id del padre
 * @return INVALIDID si NodeId es root.
 */
ItemId TreeLeaf::GetParent(const ItemId &NodeId) {
   REPORT_DEBUG("Debug: Error en arbol. Llamada a TreeLeaf::GetParent");
   return INVALIDTREEID;
}

/**
 * Busca el nodo con id 'NodeId' y agrega Node al mismo nivel, despues del
 * nodo encontrado.
 * @param[in] NodeId id de nodo
 * @param[in] pNode pNode nodo que quiero agregar
 * \attention pNode pasa a ser responsabilidad de este nodo. No
 * eliminar fuera de la clase
 * @return ItemId id del nodo que se agrego
 * @return INVALIDID si no encontro NodeId en el arbol.
 */
ItemId TreeLeaf::AddNodeAfter(const ItemId &NodeId, TreeNode* pNode) {
   REPORT_DEBUG("Debug: Error en arbol. Llamada a TreeLeaf::AddNodeAfter");
   return INVALIDTREEID;
}

/**
 * Busca el nodo con id 'NodeId' y agrega 'Node' al mismo nivel, antes del
 * nodo encontrado.
 * @param[in] NodeId id de nodo
 * @param[in] pNode pNode nodo que quiero agregar
 * \attention pNode pasa a ser responsabilidad de este nodo. No
 * eliminar fuera de la clase
 * @return ItemId id del nodo que se agrego
 * @return INVALIDID si no encontro NodeId en el arbol.
 */
ItemId TreeLeaf::AddNodeBefore(const ItemId &NodeId, TreeNode* pNode) {
   REPORT_DEBUG("Debug: Error en arbol. Llamada a TreeLeaf::AddNodeBefore");
   return INVALIDTREEID;
}

/**
 * Busca el nodo con id 'NodeId' y agrega Node al mismo nivel, antes o despues
 * del nodo encontrado dependiendo del valor de After.
 * @param[in] NodeId id de nodo
 * @param[in] pNode pNode nodo que quiero agregar
 * \attention pNode pasa a ser responsabilidad de este nodo. No
 * eliminar fuera de la clase
 * @param[in] After Si es true agrega nodo despues de pNode, si es false se
 * agrega nodo antes de pNode.
 * @return ItemId id del nodo que se agrego
 * @return INVALIDID si no encontro NodeId en el arbol.
 */
ItemId TreeLeaf::AddNode(const ItemId &NodeId, TreeNode* pNode, bool After) {
   REPORT_DEBUG("Debug: Error en arbol. Llamada a TreeLeaf::AddNode");
   return INVALIDTREEID;
}

/**
 * Busca el nodo con id 'NodeId'. Agrega 'pNode' como ultimo nodo debajo
 * de nodo encontrado.
 * @param[in] NodeId id de nodo interno
 * @param[in] pNode pNode nodo que quiero agregar al arbol
 * \attention pNode pasa a ser responsabilidad de este nodo. No
 * eliminar fuera de la clase.
 * @return ItemId id del nodo que se agrego
 * @return INVALIDID si no encontro NodeId en el arbol.
 */
ItemId TreeLeaf::AppendNode(const ItemId &NodeId, TreeNode* pNode) {
   REPORT_DEBUG("Debug: Error en arbol. Llamada a TreeLeaf::AppendNode");
   return INVALIDTREEID;
}

/**
 * Busca el nodo con id 'NodeId' y lo saca del nodo. Si es un nodo
 * interno, solo se puede remover si esta vacio.
 * @param[in] NodeId id de nodo
 * @return Node nodo que se saco del arbol
 * \attention pNode deja de ser responsabilidad de este nodo.
 * Eliminar puntero en codigo cliente. La instancia es la misma que se
 * agrego con AddNodeBefore, AddNodeAfter o AppendNode.
 * @return NULL si no encontro NodeId en el arbol.
 */
TreeNode* TreeLeaf::RemoveNode(const ItemId &NodeId) {
   REPORT_DEBUG("Debug: Error en arbol. Llamada a TreeLeaf::RemoveNode");
   return NULL;
}
}
