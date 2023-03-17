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

#include "ZoomRenderer.h"

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
#include "Mask.h"
#include "suri/Dimension.h"
#include "suri/Wkt.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

// Defines
/** Utilizado en debug para corregir paso de zoom(evita SIGSEV) */
#ifndef __DEBUG__
#  define __SECURE_ZOOM_STEP_CALCULATION_FIX__
#endif   // __DEBUG__
/** Macro para registrar Renderers en forma automatica */
AUTO_REGISTER_RENDERER(suri::ZoomRenderer);

/** namespace suri */
namespace suri {
/** Template zoom */
/**
 *  Este template se utiliza para realizar zoom de los datos en la dimension X.
 *  Guarda el dato cada Step posiciones de pSrc en pDest hasta el tamanio Size.
 * \pre pDest debe tener Size tamanio reservado
 * \pre pSrc debe tener Size*Step tamanio reservado
 * \post pDest tiene los datos de pSrc cada Step pasos
 * @param[out] pDest puntero a los datos "zoomeados"
 * @param[in] pSrc puntero a los datos a "zoomear"
 * @param[in] Size Tamanio en T de pDest
 * @param[in] Step Cantidad de posiciones de pSrc por cada posicion de pDest
 * @param[in] Offset Offset dentro de pSrc
 * \todo Here be interpolation
 */
template<typename T>
void zoom(void* pDest, void* pSrc, size_t Size, double Step, double Offset) {
   T* pdest = static_cast<T*>(pDest);
   T* psrc = static_cast<T*>(pSrc);
   // \todo Here be interpolation
   for (size_t i = 0; i < Size; i++)
      pdest[i] = psrc[SURI_TRUNC(int, i*Step+Offset)];
}

/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(ZoomRenderer::Parameters::InterpolationFunctionType, zoom);

ZoomRenderer::ZoomRenderer() {
}

ZoomRenderer::~ZoomRenderer() {
}

// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
/** genera un nodo XML a partir de los parametros */
/**
 * @param[in] RenderParameters parametros de ZoomRenderer
 */
wxXmlNode *ZoomRenderer::GetXmlNode(const Parameters &RenderParameters) {
   return NULL;
}

/** genera los parametros a partir de un nodo */
/**
 * @param[in] pNode wxXmlNode en el que se van a buscar los datos
 * @return parametros de ZoomRenderer con datos del elemento
 */
ZoomRenderer::Parameters ZoomRenderer::GetParameters(const wxXmlNode *pNode) {
   if (!pNode) {
      REPORT_ERROR("D:Nodo NULO.");
      return Parameters();
   }
   Parameters params;
   params.interpolation_ = Parameters::None;
   wxXmlNode *pnode = pNode->GetChildren();
   while (pnode != NULL) {
      if (pnode->GetName() == RENDERIZATION_NODE) {
         wxXmlNode *pcnode = pnode->GetChildren();
         while (pcnode != NULL) {
            if (pcnode->GetName() == INTERPOLATION_NODE) {
               wxString interpolation;
               if (!pcnode->GetPropVal(wxT(INTERPOLATION_PROPERTY_ZOOM_IN),
                                       &interpolation)) {
                  break;
               }
               // \todo Incluir soporte para otras interpolaciones
               if (std::string(INTERPOLATION_PROPERTY_VALUE_NEAREST_NEIGHBOUR)
                     == interpolation.c_str()) {
                  params.interpolation_ = Parameters::NearestNeighbour;
               }
            }
            pcnode = pcnode->GetNext();
         }
      }
      if (pnode->GetName() == GEORREFERENCE_NODE) {
         wxXmlNode *pgeonode = pnode->GetChildren();
         while (pgeonode != NULL) {
            if (pgeonode->GetName() == SPATIAL_REFERENCE_NODE) {
               params.spatialModel_ = pgeonode->GetNodeContent().c_str();
            }
            if (pgeonode->GetName() == RASTER_SPATIAL_MODEL_NODE) {
               params.rasterModel_ = pgeonode->GetNodeContent().c_str();
            }
            pgeonode = pgeonode->GetNext();
         }
      }
      pnode = pnode->GetNext();
   }
   return params;
}

// -------------------------- METODO DE CREACION ----------------------------
/** creador + inicializador */
/**
 * Usando el parameters obtenidos del pElement configuro el ZoomRenderer creado.
 * @param[in] pElement Elemento a renderizar.
 * @param[in] pPreviousRenderer Renderizador anterior.
 * @return ZoomRenderer Render creado usando el elemento
 * \attention el ZoomRenderer retornado (y su eliminacion) es responsabilidad
 *            del invocante
 */
ZoomRenderer *ZoomRenderer::Create(Element *pElement,
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
   if (params.interpolation_ != Parameters::NearestNeighbour) {
      REPORT_AND_FAIL_VALUE("D:Interpolacion no soportada", NULL);
   }

   int x, y, b;
   std::string datatype;
   pPreviousRenderer->GetOutputParameters(x, y, b, datatype);
   params.function_ = zoomTypeMap[datatype];
   if (!params.function_) {
      REPORT_AND_FAIL_VALUE("D:Tipo de dato (%s) no manejado", NULL, datatype.c_str());
   }
   ZoomRenderer *pzoom = new ZoomRenderer;
   pzoom->parameters_ = params;
   return pzoom;
}

/** Nombre del renderizador == al nombre del nodo */
std::string ZoomRenderer::CreatedNode() const {
   return INTERPOLATION_NODE;
}

// ----------------------- METODOS DE RENDERIZACION -------------------------
/** Renderiza el elemento dado un World en un Canvas */
/**
 * Usa interpolacion NN(Nearest Neighbour) para generar pixeles intermedios
 * cuando el nroPixeles en pantalla es mayor que en canvas
 * No usa pMask
 * @param[in] pWorldWindow Mundo del viewer asociado al boton
 * @param[in] pCanvas canvas sobre el que se va a renderizar
 * @param[in] pMask mascara que se aplica sobre la imagen
 * \todo Se puede optimizar
 * \todo Ver de calcular el offset del pixel UL
 * \todo No llena todo el viewport (falta un pixel en el canvas)
 * \todo Ver donde deberia estar el assert por la relacion de aspecto
 */
bool ZoomRenderer::Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask) {
   bool prevrenderizationstatus = true;
   if (pPreviousRenderer_) {
      prevrenderizationstatus = pPreviousRenderer_->Render(pWorldWindow, pCanvas,
                                                           pMask);
   }
   if (!pCanvas) {
      REPORT_AND_FAIL_VALUE("D:Canvas no puede ser nulo al realizar zoom.", false);
   }
   int x = 0, y = 0, b = 0;
   std::string dt;
   GetOutputParameters(x, y, b, dt);
   if (pCanvas->GetDataType() != dt) {
      REPORT_AND_FAIL_VALUE(
            "D:No se puede renderizar, inconsistencia entre el tipo de datos del canvas y los de salida del renderer",
            false);
   }
   // tamano del canvas
   int csizex, csizey;
   pCanvas->GetSize(csizex, csizey);
   if (csizex < 1 || csizey < 1) {
      REPORT_AND_FAIL_VALUE("D:Tamano de canvas erroneo", false);
      // es error??
   }
   Subset windowsubset;
   pWorldWindow->GetWindow(windowsubset);

   // \todo : Convertir coordenadas de SR de mundo a SR de imagen
   RasterSpatialModel *prastermodel = RasterSpatialModel::Create(
         parameters_.rasterModel_);
   if (!prastermodel) {
      REPORT_DEBUG("D:No se pudo crear RasterSpatialModel; usando RasterModel");
   } else {
      ParameterCollection params;
      params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                   parameters_.spatialModel_);
      params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                   pWorldWindow->GetSpatialReference());
      TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
      CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                        params);
      TransformationFactoryBuilder::Release(pfactory);
      // UL y LR de la ventana en coordenadas de P,L de la imagen
      // \todo reemplazar por un CoordinatesTransform
      if ((pct && pct->IsIdentity())
            || WktCompare(parameters_.spatialModel_,
                          pWorldWindow->GetSpatialReference())) {
         prastermodel->InverseTransform(windowsubset.ul_);
         prastermodel->InverseTransform(windowsubset.lr_);
      }
      delete pct;
   }
   RasterSpatialModel::Destroy(prastermodel);

   // ancho y alto en pixeles de imagen de la ventana (puede ser mayor o menor)
   // double wwidth = windowsubset.lr_.x_-windowsubset.ul_.x_;
   // double wheigth = windowsubset.lr_.y_-windowsubset.ul_.y_;

   // tamano del viewport
   int vpwidth, vpheight;
   pWorldWindow->GetViewport(vpwidth, vpheight);
   // el canvas es menor que el viewport, debo hacer zoom
   if ((csizex < vpwidth || csizey < vpheight) && (vpwidth > 0 && vpheight > 0)) {
      REPORT_DEBUG("D:Aplicando ZOOM con interpolacion NN");
      // indice de bandas
      std::vector<int> bands(pCanvas->GetBandCount());
      // datos originales
      std::vector<void*> data(pCanvas->GetBandCount());
      // datos zoomeados
      std::vector<void*> zoomdata(pCanvas->GetBandCount());
      // indice de mascara
      std::vector<int> maskbands(1, 0);
      // mascara original
      std::vector<void*> maskdata(pCanvas->GetBandCount());
      // mascara zoomeada
      std::vector<void*> zoommaskdata(pCanvas->GetBandCount());
      for (int b = 0; b < pCanvas->GetBandCount(); b++) {
         bands[b] = b;
         data[b] = new unsigned char[csizex * csizey * pCanvas->GetDataSize()];
         zoomdata[b] = new unsigned char[vpwidth * vpheight * pCanvas->GetDataSize()];
         memset(zoomdata[b], 255, vpwidth * vpheight * pCanvas->GetDataSize());
      }
      if (pMask) {
         maskdata[0] = new unsigned char[csizex * csizey * pMask->GetDataSize()];
         zoommaskdata[0] = new unsigned char[vpwidth * vpheight * pMask->GetDataSize()];
      }
      // me traigo los datos
      pCanvas->Read(bands, data);
      REPORT_DEBUG("D:Tamano del canvas de entrada %d;%d", csizex, csizey);
      REPORT_DEBUG("D:Tamano de viewport %d;%d", vpwidth, vpheight);
      double offsetx = windowsubset.ul_.x_ - SURI_TRUNC(int, windowsubset.ul_.x_),
            offsety = windowsubset.ul_.y_ - SURI_TRUNC(int, windowsubset.ul_.y_);
      REPORT_DEBUG("D:Offset de zoom = %.2f;%.2f", offsetx, offsety);
      // 1+offset esta porque el offset es negativo cuando la imagen empieza
      // despues de 0(1+ esta para tomar el complemento al offset).
      offsetx = offsetx < 0 ? 1 + offsetx : offsetx;
      offsety = offsety < 0 ? 1 + offsety : offsety;
      // calculo el paso en X e Y
      Dimension dim(windowsubset);
      double stepx = dim.GetWidth() / vpwidth, stepy = dim.GetHeight() / vpheight;
      REPORT_DEBUG("D:Factor de zoom = %.2f;%.2f", 1.0/stepx, 1.0/stepy);
      REPORT_DEBUG("D:Offset de zoom = %.2f;%.2f", offsetx, offsety);
#ifdef __ASSERT_ON_ZOOM_ASPECT_CHANGE__
      REPORT_DEBUG("D:assert(%d <= %d)", SURI_TRUNC(int, vpwidth*stepx+offsetx), csizex);
      assert(SURI_TRUNC(int, vpwidth*stepx+offsetx)<=csizex);
      REPORT_DEBUG("D:assert(%d <= %d)", SURI_TRUNC(int, vpheight*stepy+offsety), csizey);
      assert(SURI_TRUNC(int, vpheight*stepy+offsety)<=csizey);
      REPORT_DEBUG("D:assert(%f == %f)", stepx, stepy);
      assert(FLOAT_COMPARE(stepx, stepy) );
#endif
      // Fallback que evita SIGSEV pero puede deformar el tamanio de pixel
#ifdef __SECURE_ZOOM_STEP_CALCULATION_FIX__
      if ( SURI_TRUNC(int, vpwidth*stepx+offsetx)>csizex ||
            SURI_TRUNC(int, vpheight*stepy+offsety)>csizey )
      {
         REPORT_DEBUG("D:Error con los pasos de ZOOM (%.3f;%.3f) vs (%d;%d)",
               vpwidth*stepx+offsetx, vpheight*stepy+offsety, csizex, csizey);
         stepx = (double)(csizex-1)/vpwidth;
         stepy = (double)(csizey-1)/vpheight;
         REPORT_DEBUG("D:Factor de zoom (corregido) = %.2f;%.2f",
               1.0/stepx, 1.0/stepy);
      }
#endif   /* __SECURE_ZOOM_STEP_CALCULATION_FIX__ */
      // recorro las bandas de entrada
      for (int b = 0; b < pCanvas->GetBandCount(); b++) {
         // hago zoom en las filas
         for (int j = 0; j < vpheight; j++) {
            unsigned char *pdata = static_cast<unsigned char*>(data[b])
                  + SURI_TRUNC(int, j * stepy + offsety) * csizex
                        * pCanvas->GetDataSize();
            unsigned char *ptemp = static_cast<unsigned char*>(zoomdata[b])
                  + j * vpwidth * pCanvas->GetDataSize();
            // hago zoom en las columnas
            parameters_.function_(ptemp, pdata, vpwidth, stepx, offsetx);
         }
      }
      // Cambio el las dimensiones del canvas
      pCanvas->SetSize(vpwidth, vpheight);
      if ( pMask ) {
         pMask->Read(maskbands, maskdata);
         for (int j = 0; j < vpheight; j++) {
            unsigned char *pmaskdata = static_cast<unsigned char*>(maskdata[0])
                  + SURI_TRUNC(int, j * stepy + offsety) * csizex
                        * pMask->GetDataSize();
            unsigned char *ptemp = static_cast<unsigned char*>(zoommaskdata[0])
                  + j * vpwidth * pMask->GetDataSize();
            // hago zoom en las columnas
            zoomTypeMap[pMask->GetDataType()](ptemp, pmaskdata, vpwidth, stepx, offsetx);
         }
         // escribo los datos al canvas
         pMask->SetSize(vpwidth, vpheight);
         pMask->Write(maskbands, zoommaskdata);
         pMask->ApplyMask(bands, zoomdata, pCanvas);
      } else
      pCanvas->Write(bands, zoomdata, pMask);
      // libero el temporario
      for (int b = 0; b < pCanvas->GetBandCount(); b++) {
         delete[] static_cast<unsigned char*>(data[b]);
         delete[] static_cast<unsigned char*>(zoomdata[b]);
         delete[] static_cast<unsigned char*>(maskdata[b]);
         delete[] static_cast<unsigned char*>(zoommaskdata[b]);
      }
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
void ZoomRenderer::GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                                  double &Lrx, double &Lry) {
}

/**
 * Obtiene los parametros de entrada del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void ZoomRenderer::GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
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
void ZoomRenderer::GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                       std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------
/**
 * Actualizacion de parametros para representar un elemento
 * @param[in]	pElement: puntero al elemento.
 */
void ZoomRenderer::Update(Element *pElement) {
}
}  // namespace suri
