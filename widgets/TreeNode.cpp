/*! \file TreeNode.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "TreeNode.h"

// Includes standard

// Includes Suri

// Includes Wx
#include "wx/dcmemory.h"

// Includes App

// Defines

namespace suri {
/**
 * Constructor de nodo
 * @param[in] NodeId id de nodo
 */
TreeNode::TreeNode(HtmlConfigurationData* pHtmlData, long VisualizationFlags) :
      nodeId_(""), pHtmlConfigurationData_(pHtmlData),
      visualizationFlags_(VisualizationFlags) {
}

/** Dtor */
TreeNode::~TreeNode() {
}

/**
 * Retorna id del nodo
 * @return ItemId identificador del nodo
 */
TreeNodeId TreeNode::GetNodeId() {
   return nodeId_;
}

/**
 * Modifica id del nodo
 * @param[in] Id nuevo identificador del nodo
 */
void TreeNode::SetNodeId(const TreeNodeId &Id) {
   nodeId_ = Id;
}

/**
 * Retorna clase que se usa para configurar html del item
 * @return HtmlConfigurationData* puntero a clase que se usa para configurar html
 */
HtmlConfigurationData* TreeNode::GetHtmlConfigurationData() {
   return pHtmlConfigurationData_;
}

/**
 * Configura clase que se usa para configurar html del item
 * @param[in] pHtmlData puntero a clase que se usa para configurar html
 * \attention La clase no toma posecion del puntero, antes de eliminar instancia
 * de pHtmlData llamar a SetHtmlConfigurationData(NULL)
 */
void TreeNode::SetHtmlConfigurationData(HtmlConfigurationData* pHtmlData) {
   pHtmlConfigurationData_ = pHtmlData;
}

/**
 * Configura o agrega un DecoratedHtmlItem en vector con Html
 * @param[in] VectorIndex posicion donde quiero agregar HtmlItem
 * @param[in] HtmlItem item que quiero agregar a vector
 * @param[in] HtmlVector vector que estoy configurado
 * @param[out] HtmlVector vector con HtmlVector configurado en la posicion
 * deseada
 * @return bool que indica si pudo configurar HtmlItem en HtmlVector.
 */
bool TreeNode::SetHtmlItemToVector(int VectorIndex, const DecoratedHtmlItem &HtmlItem,
                                   std::vector<DecoratedHtmlItem> &HtmlVector) {
   if (VectorIndex > (int) HtmlVector.size() || VectorIndex < 0) {
      return false;
   }

   if (VectorIndex < (int) HtmlVector.size()) {
      HtmlVector[VectorIndex] = HtmlItem;
   } else {
      HtmlVector.push_back(HtmlItem);
   }
   return true;
}

/**
 * Modifica Text para que entre en ventana de longitud WindowWidth
 * @param[in] Text cadena de caracteres que quiero graficar
 * @param[in] WindowWidth Ancho en pixels de la ventana donde quiero graficar
 * @return cadena de texto que entra en WindowWidth
 */
std::string TreeNode::GetSizedText(std::string Text, int WindowWidth) {
   if (WindowWidth <= 0) {
      return "";
   }

   wxMemoryDC *pdc = new wxMemoryDC();

   int index = 1;
   std::string sizedtext = Text;
   int sizedtextlenght = pdc->GetTextExtent(sizedtext).GetWidth();
   while (sizedtextlenght > WindowWidth && index < (int) Text.size()) {
      sizedtext = "...";
      sizedtext += Text.substr(index);
      sizedtextlenght = pdc->GetTextExtent(sizedtext).GetWidth();
      index++;
   }

   delete pdc;
   return sizedtext;
}
}
