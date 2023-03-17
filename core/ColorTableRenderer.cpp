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

#include "ColorTableRenderer.h"

// Includes standard
#include <limits>

// Includes Suri
#include "suri/xmlnames.h"
#include "suri/DataTypes.h"
#include "suri/Element.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Canvas.h"
// Includes Wx
#include "wx/xml/xml.h"
// Includes App
// Defines

/** Macro para registrar Renderers en forma automatica */
AUTO_REGISTER_RENDERER(suri::ColorTableRenderer);

/** namespace suri */
namespace suri {
/** Ctor */
ColorTableRenderer::ColorTableRenderer() {
}

/** Dtor */
ColorTableRenderer::~ColorTableRenderer() {
}

/**
 * genera un nodo colortable a partir de los parametros
 * @param[in] RenderParameters con los que se crea nodo colortable
 * @return nodo generado
 */
wxXmlNode *ColorTableRenderer::GetXmlNode(const Parameters &RenderParameters) {
   wxXmlNode *pcolortablenode = NULL;
   ColorTable::ToXml(RenderParameters.colorTable_, pcolortablenode);
   return pcolortablenode;
}

/**
 * genera los parametros a partir de un nodo elemento con nodo colortable
 * dentro de renderizadores.
 * @param[in] pNode nodo elemento con nodo colortable interno
 * @return parametros obtenidos
 */
ColorTableRenderer::Parameters ColorTableRenderer::GetParameters(
      const wxXmlNode *pNode) {
   ColorTableRenderer::Parameters params;
   params.colorTable_.Activate(false);
   if (!pNode) {
      return params;
   }
   // Obtengo nodo de renderizacion
   wxXmlNode *pdnode = pNode->GetChildren();
   wxXmlNode *pcnode = NULL;
   while (pdnode != NULL && pcnode == NULL) {
      if (pdnode->GetName() == wxT(RENDERIZATION_NODE)) {
         pcnode = pdnode->GetChildren();
      }
      pdnode = pdnode->GetNext();
   }

   // Obtengo nodo color table
   bool found = false;
   while (pcnode != NULL && !found) {
      found = ColorTable::FromXml(pcnode, params.colorTable_);
      pcnode = pcnode->GetNext();
   }
   if (!found) {
      REPORT_DEBUG("D: Se intento obtener parametros de LUT de elemento sin este nodo");
   }
   return params;
}

/**
 * Crea un renderizador a partir de un pElement
 * @param[in] pElement con el que se quiere crear renderizador
 * @param[in] pPreviousRenderer renderizador anterior en cadena de
 * renderizadores
 * @return ColorTableRenderer creado. NULL si no se pudo
 * \attention el LutRenderer retornado(y su eliminacion) es responsabilidad
 * del invocante
 */
Renderer *ColorTableRenderer::Create(Element *pElement,
                                     Renderer *pPreviousRenderer) const {
   if (!pPreviousRenderer) {
      return NULL;
   }

   // Si no existe nodo LUT con una tabla en elemento retorno false
   wxString nodepath = wxString(wxT(RENDERIZATION_NODE)) + wxT(NODE_SEPARATION_TOKEN)
         + wxString(CreatedNode().c_str()) + wxT(NODE_SEPARATION_TOKEN)
         + wxString(wxT(TABLE_NODE));

   wxXmlNode *pnode = pElement->GetNode(nodepath);
   if (!pnode) {
      REPORT_ERROR(
            "D:No se pudo encontrar el nodo XML en el elemento \"%s\"", pElement->GetName().c_str());
      return NULL;
   }

   // Creo pcolortable
   ColorTableRenderer *pcolortable = new ColorTableRenderer;
   // Leo parametros de nodo
   ColorTableRenderer::Parameters colortableparams = ColorTableRenderer::GetParameters(
         pElement->GetNode(wxT("")));

   if (!ColorTableToLut(colortableparams, pcolortable->parameters_)
         || !LoadFunction(pcolortable->parameters_, pElement, pPreviousRenderer)
         || !ValidateTable(pcolortable->parameters_, pPreviousRenderer)) {
      delete pcolortable;
      return NULL;
   }

   return pcolortable;
}

/**
 * Nombre del renderizador == al nombre del nodo
 * @return string con el nombre del renderer
 */
std::string ColorTableRenderer::CreatedNode() const {
   return wxT(COLOR_TABLE);
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------
/**
 * Actualizacion de parametros para representar un elemento
 * No modifica los datos de salida
 * @param[in] pElement puntero al elemento
 */
void ColorTableRenderer::Update(Element * pElement) {
   // Leo parametros de nodo
   ColorTableRenderer::Parameters colortableparams = ColorTableRenderer::GetParameters(
         pElement->GetNode(wxT("")));

   if (!ColorTableToLut(colortableparams, parameters_)
         || !ValidateTable(parameters_, pPreviousRenderer_)
         || !LoadFunction(parameters_, pElement, pPreviousRenderer_)) {
      REPORT_DEBUG("D:Error en los parametros");
      parameters_.lut_.active_ = false;
   }
}

/** Adapta Parameters de ColorTable a Lut */
bool ColorTableRenderer::ColorTableToLut(ColorTableRenderer::Parameters InParameters,
                                         LutRenderer::Parameters &OutParameters) {
   wxXmlNode* pcolortablenode = GetXmlNode(InParameters);
   pcolortablenode->SetName(wxT(LUT_NODE));
   return LutArray::FromXml(pcolortablenode, OutParameters.lut_);
}

/**
 * Adapta Parameters de Lut a ColorTable
 * @param[in] InParameters parametros Lut de entrada
 * @param[out] OutParameters parametros ColorTable de salida
 * @return true si pudo realizar la transformacion, false en otro caso
 */
bool ColorTableRenderer::LutToColorTable(
      LutRenderer::Parameters &InParameters,
      ColorTableRenderer::Parameters &OutParameters) {
   wxXmlNode* plutnode = LutRenderer::GetXmlNode(InParameters);
   plutnode->SetName(wxT(COLOR_TABLE));
   return ColorTable::FromXml(plutnode, OutParameters.colorTable_);
}
}

