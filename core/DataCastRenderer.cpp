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

#include "DataCastRenderer.h"

// Includes suri
#include "suri/messages.h"
#include "suri/xmlnames.h"
#include "logmacros.h"
#include "suri/Element.h"
#include "suri/DataTypes.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/RasterSpatialModel.h"
#include "suri/World.h"
#include "suri/Canvas.h"
#include "suri/Dimension.h"
#include "MemoryCanvas.h"

/** Macro para registrar Renderers en forma automatica */
AUTO_REGISTER_RENDERER(suri::DataCastRenderer);

/** namespace suri */
namespace suri {
// Defines

/** Template cast */
/**
 * Este template se utiliza paracambiar los datos del canvas.
 * Guarda los datos de pSrc en pDest haciendo el casteo correspondiente
 * \pre pDest debe tener Size*tamanioTipoDatoDest tamanio reservado
 * \pre pSrc debe tener Size*tamanioTipoDatoSrc tamanio reservado
 * \post pDest tiene los datos de pSrc con tipo destino
 * @param[out] pDest puntero a los datos con nuevo tipo
 * @param[in] pSrc puntero a los datos originales
 * @param[in] Size cantidad de elementos a castear
 */
template<typename T1, typename T2>
void datacast(void* pDest, void* pSrc, size_t Size) {
   REPORT_DEBUG("D: Tipo de dato destino: %s", DataInfo<T1>::Name);
   REPORT_DEBUG("D: Tipo de dato origen: %s", DataInfo<T2>::Name);
   T1* pdest = static_cast<T1*>(pDest);
   T2* psrc = static_cast<T2*>(pSrc);
   // \todo Here be interpolation
   for (size_t i = 0; i < Size; i++) {
      T1 aux = static_cast<T1>(psrc[i]);
      pdest[i] = aux;
   }
}
/** Genera un mapa doble (matriz) indexado por tipos a la funcion */
INITIALIZE_DUAL_DATATYPE_MAP(DataCastRenderer::Parameters::CastFunctionType, datacast);

/**
 * Ctor.
 */
DataCastRenderer::DataCastRenderer() {
}

/**
 * Dtor.
 */
DataCastRenderer::~DataCastRenderer() {
}

// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
/**
 * genera el nodo a partir de los parametros.
 * @param[in]	RenderParameters: parametros.
 * @return Nodo con tipo de dato de salida
 */
wxXmlNode *DataCastRenderer::GetXmlNode(const Parameters &RenderParameters) {
   wxXmlNode *pdatacastnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(DATA_CAST_NODE));
   pdatacastnode->AddProperty(OUTPUT_PROPERTY,
                              RenderParameters.destinationDataType_.c_str());
   return pdatacastnode;
}

/**
 * Genera los parametros a partir de un nodo
 * @param[in] pNode wxXmlNode en el que se van a buscar los datos
 * @return parametros de ZoomRenderer con datos del elemento
 */
DataCastRenderer::Parameters DataCastRenderer::GetParameters(const wxXmlNode *pNode) {
   if (!pNode) {
      REPORT_ERROR("D:Nodo NULO.");
      return Parameters();
   }
   Parameters params;

   wxXmlNode *pnode = pNode->GetChildren();
   while (pnode != NULL) {
      if (pnode->GetName() == RENDERIZATION_NODE) {
         wxXmlNode *pcnode = pnode->GetChildren();
         while (pcnode != NULL) {
            if (pcnode->GetName() == DATA_CAST_NODE) {
               wxString output_type;
               if (!pcnode->GetPropVal(wxT(OUTPUT_PROPERTY), &output_type)) {
                  break;
               }
               params.destinationDataType_ = output_type;
            }
            pcnode = pcnode->GetNext();
         }
      }
      pnode = pnode->GetNext();
   }
   return params;
}

// -------------------------- METODO DE CREACION ----------------------------
/** creador + inicializador */
/**
 * Usando los parametros obtenidos configuro el DataCastRenderer. El tipo de
 * dato de entrada surge del pPreviousRenderer y el de salida del nodo contenido
 * en pElement
 * @param[in] pElement Elemento a renderizar.
 * @param[in] pPreviousRenderer Renderizador anterior.
 * @return DataCastRenderer Render creado usando el elemento
 * \attention el DataCastRenderer retornado(y su eliminacion) es responsabilidad
 *            del invocante
 */
DataCastRenderer *DataCastRenderer::Create(Element *pElement,
                                           Renderer *pPreviousRenderer) const {
   if (!pPreviousRenderer) {
      return NULL;
   }

   wxXmlNode *pnode = pElement->GetNode(wxT(""));
   if (!pnode) {
      REPORT_ERROR(
            "D:No se pudo encontrar el nodo XML en el elemento \"%s\"", pElement->GetName().c_str());
      return NULL;
   }
   Parameters params = GetParameters(pnode);
   Parameters params2 = GetParameters(pnode);
   int x, y, b;
   std::string datatype;
   pPreviousRenderer->GetOutputParameters(x, y, b, datatype);

   // Generar funcion segun tipo de salida
   TRY
   {
      params.function_ = datacastTypeMatrix[params.destinationDataType_][datatype];
      params2.function_ = datacastTypeMatrix[datatype][params.destinationDataType_];
   }
   CATCH {
      params.function_ = NULL;
      params2.function_ = NULL;
   }
   if (!params.function_) {
      REPORT_AND_FAIL_VALUE("D:Tipo de dato (%s) no manejado", NULL, datatype.c_str());
   }
   DataCastRenderer *pcastrenderer = new DataCastRenderer;
   pcastrenderer->parameters_ = params;
   pcastrenderer->parametersToChar_ = params2;
   return pcastrenderer;
}

/**
 * Nombre del renderizador == al nombre del nodo
 * @return string con el nombre del renderer
 * \todo pasar por macro de traduccion
 */
std::string DataCastRenderer::CreatedNode() const {
   return DATA_CAST_NODE;
}

// ----------------------- METODOS DE RENDERIZACION -------------------------

/**
 * Obtiene los datos del canvas, cambia el tipo de dato del mismo, utiliza la
 * funcion obtenida en create para transformar los datos al tipo indicado por
 * el nodo xml y finalmente actualiza los datos del canvas
 * En caso de que no se pueda cambiar el tipo de dato del canvas al tipo de dato
 *  de salida del pPreviousRenderer_, se genera un MemoryCanvas intermedio.
 * @attention RasterRenderer Reinicializa el Canvas que recibe. En funcion de
 *  este detalle es que se puede hacer la prueba del cambio de tipo de dato.
 *  Para una version del sistema de renderizacion que no relice este ajuste
 *  (para permitir mosaico) es necesario modificar este checkeo para no destruir
 *  los datos del canvas.
 * @param[in] pWorldWindow World con parte del mundo que se va a mostrarse por
 *             pantalla
 * @param[out] pCanvas Canvas al que hay que aplicarle la transformacion
 * @param[in] pMask mascara que se le debe aplicar al cambas
 * @param[out] pMask mascara generada si necesario
 */
bool DataCastRenderer::Render(const World *pWorldWindow, Canvas* pCanvas,
                              Mask* pMask) {
   bool prevrenderizationstatus = true;
   REPORT_DEBUG("D:Casteando datos");
   Canvas *ptempcanvas = pCanvas;
   if (pPreviousRenderer_ && pCanvas) {
      int x, y, b;
      std::string datatype;
      pPreviousRenderer_->GetOutputParameters(x, y, b, datatype);
      std::vector<int> bands;
      std::vector<void*> originalData;
      std::vector<void*> castedData;
      for (int x = 0; x < pCanvas->GetBandCount(); x++) {
         bands.push_back(x);
         unsigned char* data = new unsigned char [pCanvas->GetSizeX() * pCanvas->GetSizeY() *
                                                  pCanvas->GetDataSize()];
         originalData.push_back(data);
      }
      pCanvas->Read(bands, originalData);
      pCanvas->SetDataType(datatype);
      // TODO matiaslafroce: usar un vector y referencias
      //                     así se borra el delete de abajo
      for (int x = 0; x < pCanvas->GetBandCount(); x++) {
         unsigned char* data = new unsigned char [pCanvas->GetSizeX() * pCanvas->GetSizeY() *
                                                           pCanvas->GetDataSize()];
         /* datacast<unsigned short, unsigned char>*/
         parametersToChar_.function_(data, originalData[x], pCanvas->GetSizeX() *
                                                                            pCanvas->GetSizeY());
         castedData.push_back(data);
      }
      pCanvas->Write(bands, castedData);
      for (int x = 0; x < pCanvas->GetBandCount(); x++) {
         delete[] static_cast<unsigned char*>(originalData[x]);
         delete[] static_cast<unsigned char*>(castedData[x]);
      }
      // no puede cambiar el tipo de canvas
      if (pCanvas->GetDataType() != datatype) {
         ptempcanvas = new MemoryCanvas;
         REPORT_DEBUG("D:Se creo un canvas intermedio");
      }
      prevrenderizationstatus = pPreviousRenderer_->Render(pWorldWindow, ptempcanvas,
                                                           pMask);
      // Reinicializo con los tamanos que tiene el canvas temporario a la salida
      // de la renderizacion
      if (ptempcanvas != pCanvas) {
         pCanvas->SetBandCount(ptempcanvas->GetBandCount());
         ptempcanvas->GetSize(x, y);
         pCanvas->SetSize(x, y);
      }
   }
   if (!ptempcanvas) {
      REPORT_AND_FAIL_VALUE("D:Canvas no puede ser nulo al realizar data cast.", false);
   }

   // Si el tipo de dato que obtengo del canvas es void informo error
   if (ptempcanvas->GetDataType() == DataInfo<void>::Name) {
      REPORT_AND_FAIL_VALUE("D:Tipos de datos invalidos.", false);
   }

   if (ptempcanvas->GetDataType() != parameters_.destinationDataType_) {
      REPORT_DEBUG("D:Aplicando transformacion de tipo de dato");
      // tamano del canvas
      int csizex, csizey;
      ptempcanvas->GetSize(csizex, csizey);
      if (csizex < 1 || csizey < 1) {
         REPORT_AND_FAIL_VALUE("D:Tamano de canvas erroneo", false);
      }
      // indice de bandas
      std::vector<int> bands(ptempcanvas->GetBandCount());
      // datos originales
      std::vector<void*> data(ptempcanvas->GetBandCount());
      for (int b = 0; b < ptempcanvas->GetBandCount(); b++) {
         bands[b] = b;
         data[b] = new unsigned char[csizex * csizey * ptempcanvas->GetDataSize()];
      }
      int bandcount = ptempcanvas->GetBandCount();
      // me traigo los datos
      ptempcanvas->Read(bands, data);
      // Cambio tipo de dato
      ptempcanvas->SetDataType(parameters_.destinationDataType_);
      // Transformo datos
      std::vector<void*> newdata(bandcount);
      for (int b = 0; b < bandcount; b++) {
         newdata[b] = new unsigned char[csizex * csizey * ptempcanvas->GetDataSize()];
         memset(newdata[b], 0, csizex * csizey * ptempcanvas->GetDataSize());
         parameters_.function_(newdata[b], data[b], csizex * csizey);
      }
      ptempcanvas->SetBandCount(bandcount);
      ptempcanvas->SetSize(csizex, csizey);

      // Guardo Bandas
      ptempcanvas->Write(bands, newdata);
      for (int b = 0; b < bandcount; b++) {
         delete[] static_cast<unsigned char*>(data[b]);
         delete[] static_cast<unsigned char*>(newdata[b]);
      }
   }
   if (pCanvas != ptempcanvas) {
      pCanvas->Write(ptempcanvas, pMask);
      delete ptempcanvas;
   }
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
void DataCastRenderer::GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                                      double &Lrx, double &Lry) {
}

/**
 * Obtiene los parametros de entrada del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void DataCastRenderer::GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                          std::string &DataType) const {
   // porque requiere lo mismo de entrada que lo que da a la salida
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
void DataCastRenderer::GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                           std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }

   DataType = parameters_.destinationDataType_;
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------
/**
 * Actualizacion de parametros para representar un elemento
 * @param[in]	pElement: puntero al elemento.
 */
void DataCastRenderer::Update(Element *pElement) {
}
}  // namespace suri
