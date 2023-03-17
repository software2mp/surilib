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

#include <algorithm>

#include "ClassificationRenderer.h"
#include "MemoryCanvas.h"
#include "SpectralAngleMapperAlgorithm.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Canvas.h"
#include "suri/DataTypes.h"
#include "suri/Element.h"
#include "suri/StatisticsFunctions.h"
#include "suri/xmlnames.h"
#include "suri/XmlFunctions.h"

#include "wx/wx.h"

/** Cantidad de bandas de la imagen salida de la clasificacion */
#define CLASIFICATOR_OUTPUT_BANDS 1
/** Tipo de dato de la imagen salida de la clasificacion */
#define CLASSIFICATOR_OUTPUT_DATA_TYPE int
/** Numero de banda de la imagen salida de la clasificacion */
#define DEFAULT_OUTPUT_BAND 0

/** Macro para registrar Renderers en forma automatica */
AUTO_REGISTER_RENDERER(suri::ClassificationRenderer);

/** namespace suri */
namespace suri {
/**
 * Constructor
 */
ClassificationRenderer::ClassificationRenderer() {
}

/**
 * Destructor
 */
ClassificationRenderer::~ClassificationRenderer() {
}

// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
/** genera un nodo XML a partir de los parametros */
/**
 * Genera el nodo de Classificacion. Para hacerlo llama a GetNode del
 * algorimo contenido en Parameters.
 * @param[in] RenderParameters parametros con los que se genera el nodo
 * @return nodo del renderizador configurado con datos en parameters
 * \attention el nodo devuelto es responsabilidad del solicitante
 */
wxXmlNode *ClassificationRenderer::GetXmlNode(const Parameters &RenderParameters) {
   // Creo una copia de los parametros para poder ordenar las clases
   Parameters rendererparameters = RenderParameters;

   if (!rendererparameters.pAlgorithm_ || !rendererparameters.pAlgorithm_->Check()) {
      return NULL;
   }

   // Crea el nodo clasificacion
   wxXmlNode *prenderernode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                            wxT(CLASSIFICATION_NODE));

   prenderernode->AddChild(rendererparameters.pAlgorithm_->GetNode());

   return prenderernode;
}

/** Genera los parametros a partir de un nodo */
/**
 * Genera un parameters usando los datos en el nodo.
 * @param[in] pNode nodo clasificacion
 * @return parameters del clasificador configurado con los datos del algoritmo
 */
ClassificationRenderer::Parameters ClassificationRenderer::GetParameters(
      const wxXmlNode *pNode) {
   Parameters parameters;

   wxXmlNode *pnode = NULL;
   // Obtengo nodo de renderizacion
   wxXmlNode *pdnode = pNode->GetChildren();
   wxXmlNode *pcnode = NULL;
   while (pdnode != NULL) {
      // Obtengo nodo renderizacion
      if (pdnode->GetName() == wxT(RENDERIZATION_NODE)) {
         pcnode = pdnode->GetChildren();
      }
      pdnode = pdnode->GetNext();
   }
   // Obtengo nodo lut
   while (pcnode != NULL) {
      if (pcnode->GetName() == wxT(CLASSIFICATION_NODE)) {
         pnode = pcnode;
         pcnode = NULL;
      } else {
         pcnode = pcnode->GetNext();
      }
   }

   if (!pnode) {
      return parameters;
   }

   // Busco el nodo del algoritmo
   wxString nodepath = wxString(wxT(CLASSIFICATION_ALGORITHM_NODE)) +
         wxT(NODE_SEPARATION_TOKEN) + wxString(wxT(ALGORITHM_NAME_NODE));
   const wxXmlNode *pdatanode = GetNodeByName(nodepath, pnode);
   // creo algoritmo con el nombre del nodo
   if (!pdatanode)
      return parameters;

   ClassificationAlgorithmInterface* palgorithm = ClassificationAlgorithmInterface::Create(
                                                pdatanode->GetNodeContent().c_str());
   // Si no pudo crear algo, o no pudo configurar nodo o nodo invalido falla
   if (!palgorithm || !palgorithm->Update(pnode->GetChildren())
         || !palgorithm->Check()) {
      delete palgorithm;
      return parameters;
   }

   parameters.pAlgorithm_ = palgorithm;

   return parameters;
}

// -------------------------- METODO DE CREACION ----------------------------
/**
 * Crea un ClassificationRenderer a partir de un Elemento
 * @param[in]	pElement: puntero a un elemento.
 * @param[in]	pPreviousRenderer: puntero al renderer anterior en la cadena.
 * @return puntero a un objeto de tipo MapRenderer.
 * \attention quien invoque a este metodo es responsable del puntero devuelto,
 * por lo tanto debe hacerse cargo de su eliminacion.
 */
ClassificationRenderer *ClassificationRenderer::Create(
      Element *pElement, Renderer *pPreviousRenderer) const {
   if (!pPreviousRenderer) {
      return NULL;
   }

   // Si no hay un nodo algebra con al menos una ecuacion falla
   wxString nodepath = wxString(wxT(RENDERIZATION_NODE)) + wxT(NODE_SEPARATION_TOKEN)
         + wxString(CreatedNode().c_str());

   wxXmlNode *pnode = pElement->GetNode(nodepath);
   if (!pnode) {
      REPORT_ERROR(
            "D:No se pudo encontrar el nodo XML de \"%s\"", pElement->GetName().c_str());
      return NULL;
   }

   // Obtengo el tipo de dato a la entrada
   int sizex, sizey, bandcount;
   std::string datatype;
   if (pPreviousRenderer) {
      pPreviousRenderer->GetOutputParameters(sizex, sizey, bandcount, datatype);
   }

   // Leo parametros de nodo
   Parameters params;
   params = GetParameters(pElement->GetNode(wxT("")));

   // Configuro el tipo de dato de entrada
   if (!params.pAlgorithm_ || !params.pAlgorithm_->Check()) {
      REPORT_ERROR("D:error en los datos del nodo");
      return NULL;
   }

   // Creo nuevo renderizador
   ClassificationRenderer *pclasif = new ClassificationRenderer;
   pclasif->parameters_ = params;

   // Valor no valido
   bool ndvavailable = false;
   double nodatavalue = 0.0;
   pElement->GetNoDataValue(ndvavailable, nodatavalue);

   pclasif->SetNoDataValue(nodatavalue);
   pclasif->SetNoDataValueAvailable(ndvavailable);

   return pclasif;
}

/** Nombre del renderizador == al nombre del nodo */
std::string ClassificationRenderer::CreatedNode() const {
   return wxT(CLASSIFICATION_NODE);
}

// ----------------------- METODOS DE RENDERIZACION -------------------------
/**
 * Renderiza el elemento dado un World en un Canvas
 * @param[in]	pWorldWindow: puntero a la ventana del mundo.
 * @param[in]	pCanvas: puntero al Canvas.
 * @param[in]	pMask: puntero a la mascara raster binaria.
 */
bool ClassificationRenderer::Render(const World *pWorldWindow, Canvas* pCanvas,
                                    Mask* pMask) {
   bool prevrenderizationstatus = true;
   // Cambio tipo de dato en canvas y llamo a Render
   Canvas *ptempcanvas = pCanvas;
   int canvassizex, canvassizey, originalbandcount = 0;
   std::string datatype;
   if (pPreviousRenderer_ && pCanvas) {
      pPreviousRenderer_->GetOutputParameters(canvassizex, canvassizey,
                                              originalbandcount, datatype);

      pCanvas->SetDataType(datatype);
      // no puede cambiar el tipo de canvas
      if (pCanvas->GetDataType() != datatype) {
         ptempcanvas = new MemoryCanvas;
         REPORT_DEBUG("D:Se creo un canvas intermedio");
      }
      prevrenderizationstatus = pPreviousRenderer_->Render(pWorldWindow, ptempcanvas,
                                                           pMask);
   } else {
      REPORT_DEBUG("D:Canvas no puede ser nulo al aplicar ecuacion.");
      return false;
   }
   // Leo el tamanio del canvas retornado
   pCanvas->GetSize(canvassizex, canvassizey);
   if (canvassizex < 1 || canvassizey < 1) {
      REPORT_DEBUG("D:Tamano de canvas erroneo");
      return false;
   }

   // leo el canvas retornado
   std::vector<int> bands(originalbandcount);
   std::vector<void*> data(originalbandcount);
   for (int b = 0; b < originalbandcount; b++) {
      bands[b] = b;
      data[b] = new unsigned char[canvassizex * canvassizey * pCanvas->GetDataSize()];
   }
   pCanvas->Read(bands, data);

   // Si el canvas es el original cambio el tipo de dato al de la salida y el numero de bandas
   if (ptempcanvas == pCanvas) {
      /**
       * TODO(Gabriel TCK #6482): Aca se hardcodea el tipo de dato de salida. Esto
       * no permite que el proceso de clasificacion pueda dar como resultado
       * por ejemplo el angulo espectral asociado al pixel
       */
       pCanvas->SetDataType(DataInfo<CLASSIFICATOR_OUTPUT_DATA_TYPE>::Name);
   }
   pCanvas->SetBandCount(CLASIFICATOR_OUTPUT_BANDS);
   pCanvas->SetSize(canvassizex, canvassizey);
   int size = canvassizex * canvassizey;

   // Creo vector donde se guardan datos de salida temporales
   void* outdata = new unsigned char[canvassizex * canvassizey * pCanvas->GetDataSize()];

   // Aplico las operaciones a las bandas
   parameters_.pAlgorithm_->SetNoDataValue(GetNoDataValue());
   parameters_.pAlgorithm_->SetNoDataValueAvailable(IsNoDataValueAvailable());

   bool returnvalue = parameters_.pAlgorithm_->Classify(
         (CLASSIFICATOR_OUTPUT_DATA_TYPE*) outdata, data, size, datatype);

   if (returnvalue) {
      // Guardo cambios
      std::vector<void*> outvector;
      outvector.push_back(outdata);
      std::vector<int> outputbands;
      outputbands.push_back(DEFAULT_OUTPUT_BAND);
      pCanvas->Write(outputbands, outvector);
   }

   // libero el temporario
   for (int b = 0; b < originalbandcount; b++)
      delete[] static_cast<unsigned char*>(data[b]);
   delete[] static_cast<unsigned char*>(outdata);

   return true && prevrenderizationstatus;
}

/**
 * Obtiene el "bounding box" del elemento renderizado
 * @param[in]	pWorld: puntero a la ventana del mundo.
 * @param[in]	Ulx: uper-left x
 * @param[in]	Uly: uper-left y
 * @param[in]	Lrx: lower-right x
 * @param[in]	Lry: lower-right y
 */
void ClassificationRenderer::GetBoundingBox(const World *pWorld, double &Ulx,
                                            double &Uly, double &Lrx, double &Lry) {
}

/**
 * Obtiene los parametros de entrada del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void ClassificationRenderer::GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                                std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
}

/**
 * Obtiene los parametros de salida del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void ClassificationRenderer::GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                                 std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
   DataType = DataInfo<CLASSIFICATOR_OUTPUT_DATA_TYPE>::Name;
   BandCount = CLASIFICATOR_OUTPUT_BANDS;
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------
/**
 * Actualizacion de parametros para representar un elemento
 * @param[in]	pElement: puntero al elemento.
 */
void ClassificationRenderer::Update(Element *pElement) {
   // Leo parametros de nodo
   Parameters params;
   params = GetParameters(pElement->GetNode(wxT("")));
   if (params.pAlgorithm_) {
      delete parameters_.pAlgorithm_;
      parameters_.pAlgorithm_ = params.pAlgorithm_;
   }
}
}
