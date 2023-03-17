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

// Includes suri
#include "FiltredVectorRenderer.h"
#include "suri/XmlFunctions.h"
#include "suri/xmlnames.h"
#include "suri/Element.h"
#include "FilteredVector.h"
#include "suri/Vector.h"

// Includes otros
#include "ogrsf_frmts.h"

/** Macro para registrar Renderers en forma automatica */
AUTO_REGISTER_RENDERER(suri::FiltredVectorRenderer);

namespace suri {

/** Ctor */
FiltredVectorRenderer::FiltredVectorRenderer() : pFilter_(NULL) {
}

/** Dtor */
FiltredVectorRenderer::~FiltredVectorRenderer() {
}

/**
 * Configura un filtro que se aplica sobre el vector.
 * En numero de capa se obtiene de analizar pFilter->GetFilteredName
 * @param[in] pFilter filtro que se debe aplicar.
 * @param[out] pNode nodo de la capa.
 */
bool FiltredVectorRenderer::SetFilter(const Filter* pFilter, wxXmlNode *pNode) {
   // Busco el indice sobre hay que agregar el filtro
   std::string filtredname = pFilter->GetFilteredName();
   Parameters params = VectorRenderer::GetParameters(pNode);
   Vector* pvector = Vector::Open(params.vectorUrl_);
   int layercount = pvector->GetLayerCount();
   int filterindex = -1;
   for (int i = 0; i < layercount && filterindex < 0; i++) {
      OGRLayer* player = pvector->GetLayer(i);
      const char* pname = player->GetName();
      if (filtredname.compare(pname) == 0)
         filterindex = i;
   }
   Vector::Close(pvector);
   if (filterindex < 0)
      return false;

   wxXmlNode* prendernode = GetNodeByName(RENDERIZATION_NODE, pNode);
   if (!prendernode)
      return false;
   // Actualizo el nodo de renderizacion de la capa correspondiente.
   wxXmlNode *pflnode = GetNodeByName(std::string(wxT(RENDERIZATION_NODE))
                                           + wxT("|") + FILTER_NODE, pNode);
   if (!pflnode) {
      wxXmlNode* pfnodenew = new wxXmlNode(prendernode, wxXML_ELEMENT_NODE,
                                           FILTER_NODE);
      new wxXmlNode(pfnodenew, wxXML_TEXT_NODE, wxT(""),
                                           pFilter->GenerateQueryAsString());
   } else {
      pflnode->GetChildren()->SetContent(pFilter->GenerateQueryAsString());
   }

   return true;
}

/** 
 * Retorna el filtro que se aplican sobre una capa del vector 
 * @param[in] pNode nodo del elemento
 * @param[in] LayerIndex numero de capa
 * @param[out] pFilter puntero al filtro de la capa
 * @return bool que indica si habia un filtro configurado en la capa.
 */
bool FiltredVectorRenderer::GetFilter(const wxXmlNode *pNode, int LayerIndex,
                                                            Filter* &pFilter) {
   const wxXmlNode* pfnode = GetNodeByName(
         std::string(wxT(RENDERIZATION_NODE)) + wxT("|") + FILTER_NODE, pNode);

   if (!pfnode) return false;

   pFilter = new Filter(pfnode->GetNodeContent().c_str());

   return true;
}

/**
 * Crea vector renderer, le carga los parametros del elemento y controla que
 * el vector sea valido(caso contrario falla).
 * @param[in] pElement elemento del vector a renderizar
 * @param[in] pLastRenderer renderizador que esta antes en pipeline de
 * renderizacion
 * \attention el VectorRenderer retornado(y su eliminacion) son responsabilidad
 * del invocante
 * @return puntero a VectorRenderer
 * @return NULL si no se pudo inicializar vector o si no hay un filtro configurado
 * para la capa activa.
 */
VectorRenderer *FiltredVectorRenderer::Create(Element *pElement,
                                             Renderer *pLastRenderer) const {
   VectorRenderer* prenderer = VectorRenderer::Create(pElement, pLastRenderer);
   if (!prenderer)
      return NULL;
   delete prenderer;

   FiltredVectorRenderer* pfiltredrenderer = new FiltredVectorRenderer;
   pfiltredrenderer->parameters_ = GetParameters(pElement->GetNode(wxT("")));
   if (!GetFilter(pElement->GetNode(wxT("")),
                      pfiltredrenderer->parameters_.activelayer_,
                                              pfiltredrenderer->pFilter_)) {
      delete pfiltredrenderer;
      pfiltredrenderer = NULL;
   }

   return pfiltredrenderer;

}

/** 
 * Retorna vector de la capa a renderizar
 * @return vector configurado con filtro
 */
Vector* FiltredVectorRenderer::OpenVector() {
   return new FilteredVector(VectorRenderer::OpenVector(), pFilter_);
}

/** Nombre del renderizador == al nombre del nodo */
std::string FiltredVectorRenderer::CreatedNode() const {
   return FILTER_NODE;
}

} /* namespace suri */
