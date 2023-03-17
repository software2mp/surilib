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

// Include standard
#include <sstream>
#include <algorithm>
#include <map>
#include <string>

// Includes suri
#include "RasterRenderer.h"
#include "MemoryCanvas.h"
#include "suri/World.h"
#include "Mask.h"
#include "suri/xmlnames.h"
#include "suri/Element.h"
#include "suri/Image.h"
#include "suri/DataTypes.h"
#include "Decimate.h"
#include "suri/RasterSpatialModel.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Dimension.h"
#include "suri/Uri.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
// Define
#define URI_FORMAT_KEY "format"
#define URI_DATASET_KEY "dataset"
#define URI_HDF5_FORMAT_VALUE "hdf5"
#ifndef __DEBUG__
#  define __CUSTOM_CANVAS_OFFSET_FIX__
#endif
#define NOT_VALID_DATA_VALUE "valor_no_valido"
/** Macro para registrar el renderer */
AUTO_REGISTER_RENDERER(suri::RasterRenderer);

/** namespace suri */
namespace suri {
/**
 * Constructor
 */
RasterRenderer::RasterRenderer() :
      pImage_(NULL), pMask_(NULL), changed_(true) {
}

/**
 * Destructor
 */
RasterRenderer::~RasterRenderer() {
   Image::Close(pImage_);
}

// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
/**
 * Genera un nodo XML a partir de los parametros
 * @param[in]	RenderParameters: parametros.
 * @return Nodo con los parametros del renderer. NULL si fallo.
 */
wxXmlNode *RasterRenderer::GetXmlNode(const Parameters &RenderParameters) {
   return NULL;
}


/** obtiene los parametros del nodo de Geo referencia*/
/**
 * @param[in] params Parameters donde se van a copiar los datos encontrados (pasado por parametros)
 * @param[in] parentNode wxXmlNode puntero al padre del nodo padre
 * @return no se devuelve nada
 */

void RasterRenderer::GetReferenceNode(Parameters &params, wxXmlNode *pElementNode) {
   wxXmlNode *pRenderNode = pElementNode->GetChildren();
   while (pRenderNode != NULL) {
      if (pRenderNode->GetName() == SPATIAL_REFERENCE_NODE) {
         params.spatialModel_ = pRenderNode->GetNodeContent().c_str();
      }
      if (pRenderNode->GetName() == RASTER_SPATIAL_MODEL_NODE) {
         params.rasterModel_ = pRenderNode->GetNodeContent().c_str();
      }
      pRenderNode = pRenderNode->GetNext();
   }
   return;
}


/** obtiene los parametros del nodo de datos DN no validos*/
/**
 * @param[in] params Parameters donde se van a copiar los datos encontrados (pasado por parametros)
 * @param[in] parentNode wxXmlNode puntero al padre del nodo padre
 * @return no se devuelve nada
 */
void RasterRenderer::GetNoValidDnValue(Parameters& params, wxXmlNode *pElementNode) {
   wxXmlNode *pfileNode = pElementNode->GetChildren();

   while ((pfileNode != NULL) && (pfileNode->GetName() != FORMAT_NODE)) {
      pfileNode = pfileNode->GetNext();
   }

   if (pfileNode)
       pfileNode= pfileNode->GetChildren();

   while (pfileNode != NULL && pfileNode->GetName() != PROPERTIES_NODE) {
      pfileNode = pfileNode->GetNext();
   }

   if (pfileNode)
       pfileNode= pfileNode->GetChildren();

   while (pfileNode != NULL) {
      if (pfileNode->GetName() == NOT_VALID_DATA_VALUE) {
         params.generateMask_ = true;
         params.noDataValue_ = StringToNumber<double>(pfileNode->GetNodeContent().c_str());
      }
      pfileNode = pfileNode->GetNext();
   }
}

/** genera los parametros a partir de un nodo */
/**
 * @param[in] pNode wxXmlNode en el que se van a buscar los datos
 * @return parametros de RasterRenderer con datos del elemento
 * \todo(Javier - Sin TCK): Arreglar porque puede quedarse en un bucle infinito
 */
RasterRenderer::Parameters RasterRenderer::GetParameters(const wxXmlNode *pNode) {
   if (!pNode) {
      REPORT_ERROR("D:Nodo NULO.");
      return Parameters();
   }
   Parameters params;
   std::string bandcombination;
   wxXmlNode *pnode = pNode->GetChildren();
   while (pnode != NULL) {
      if (pnode->GetName() == RENDERIZATION_NODE) {
         wxXmlNode *pcnode = pnode->GetChildren();
         while (pcnode != NULL) {
            if (pcnode->GetName() == BAND_COMBINATION_NODE) {
               bandcombination = pcnode->GetNodeContent().c_str();
               break;
            }
            pcnode = pcnode->GetNext();
         }
      } else if (pnode->GetName() == URL_NODE) {
         params.imageUrl_ = pnode->GetNodeContent().c_str();
      } else if (pnode->GetName() == GEORREFERENCE_NODE) {
         RasterRenderer::GetReferenceNode(params, pnode);
      } else if (pnode->GetName() == FILE_NODE) {
         RasterRenderer::GetNoValidDnValue(params, pnode);
         wxXmlNode* prawmeta = pnode->GetChildren();
         while (prawmeta && prawmeta->GetName().compare(RAW_METADATA))
            prawmeta = prawmeta->GetNext();
         GetRasterMetadata(prawmeta, params);
      }
      pnode = pnode->GetNext();
   }
   params.bandCombination_ = split<int>(bandcombination);
   // Siempre usa decimacion (NN) para la lectura
   params.readFunction_ = RasterRenderer::Parameters::Decimate;
   return params;
}

/** Metodo auxiliar para extraer informacion de metadata en el xml **/
void RasterRenderer::GetRasterMetadata(wxXmlNode* pMetadataNode,
                                       Parameters& Param) {
   wxXmlNode* pcurrent = pMetadataNode ? pMetadataNode->GetChildren() : NULL;
   while (pcurrent) {
      Param.rawMetadata_.SetOption(pcurrent->GetName().c_str(),
                                   pcurrent->GetNodeContent().c_str());
      pcurrent = pcurrent->GetNext();
   }
}

// -------------------------- METODO DE CREACION ----------------------------
/** creador + inicializador */
/**
 * Usando el parameters obtenidos del pElement configuro el RasterRenderer creado.
 * @param[in] pElement Elemento a renderizar.
 * @param[in] pPreviousRenderer Renderizador anterior(no se usa).
 * @return RasterRenderer Render creado usando el elemento
 * \attention el RasterRenderer retornado(y su eliminacion) es responsabilidad del invocante
 */
RasterRenderer *RasterRenderer::Create(Element *pElement,
                                       Renderer *pPreviousRenderer) const {
   wxXmlNode *pnode = pElement->GetNode(wxT(""));
   if (!pnode) {
      REPORT_ERROR(
            "D:No se pudo encontrar el nodo XML en el elemento \"%s\"",
                                                  pElement->GetName().c_str());
      return NULL;
   }
   RasterRenderer *prenderer = new RasterRenderer;
   prenderer->parameters_ = GetParameters(pnode);

   if (!prenderer->UpdateImage(NULL))
   REPORT_AND_FAIL_VALUE("D:RasterRenderer, error al abrir la imagen.", NULL);

   return prenderer;
}

/** Nombre del renderizador == al nombre del nodo */
std::string RasterRenderer::CreatedNode() const {
   return BAND_COMBINATION_NODE;
}

// ----------------------- METODOS DE RENDERIZACION -------------------------

/** Renderiza el elemento dado un World en un Canvas */
/**
 * Crea RasterSpatialModel a partir del rasterModel_ en parameters del renderer,
 * Crea una transformacin entre el modelo espacial de la imagen y el del pWorldWindow para
 * obtener las coordenads de los extremos del pWordWindow en las coord espaciales de la imagen
 * Uso estas coordenada para encontrar los pixeles de los extremos en la imagen y en
 * el viewport. Analizo si tengo que decimar y agrego decimate a image de ser necesario.
 * Inicializo el canvas y guardo la imagen leida(relleno si el cambas es mayor que el subset leido)
 * Finalmente genero la mascara
 * @param[in] pWorldWindow World con parte del mundo que se va a mostrarse por pantalla
 * @param[out] pCanvas Canvas al que se le carga el raster de la imagen del elemento
 * @param[in] pMask mascara que se le debe aplicar al cambas
 * @param[out] pMask mascara generada si tiene una guardada
 * \todo implementar el llenado de la mascara
 * \todo reimplementar usando transformaciones estandarizadas (World,
 *       Coordinates, Subset, Intersect, etc)
 * \todo (danieltesta-#4287): Revisar todos los comentarios que dicen que hay codigo
 * que no deberia ir o codigo que es temporal.
 */
bool RasterRenderer::Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask) {
   if (!pCanvas) {
      REPORT_AND_FAIL_VALUE("D:Canvas nulo", false);
   }

   /*
    CODIGO TEMPORAL: Solo existe mientras Image no lea WXSs en forma eficiente.
    Para esto deberia leer solo la parte del mundo que muestra la ventana en la
    minima resolucion posible, caso contrario se usa demasiado ancho de banda.
    Temporalmente se creo WxsRenderer que reimplementa UpdateImage y
    crea Image con la parte de interes
    */
   UpdateImage(pWorldWindow);
   /* Fin CODIGO TEMPORAL */

   REPORT_DEBUG("D:RasterRenderer::Render()::begin");
   Subset windowsubset;
   pWorldWindow->GetWindow(windowsubset);

   int vpwidth, vpheight;
   pWorldWindow->GetViewport(vpwidth, vpheight);
   RasterSpatialModel *prastermodel = RasterSpatialModel::Create(
         parameters_.rasterModel_);
   CoordinatesTransformation *pct = NULL;
   if (!prastermodel) {
      REPORT_AND_FAIL_VALUE("D:No se pudo crear RasterSpatialModel", false);
   } else {
      // UL y LR de la ventana en coordenadas de P,L de la imagen
      ParameterCollection params;
      params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                   parameters_.spatialModel_);
      params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                   pWorldWindow->GetSpatialReference());
      TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
      CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                        params);
      TransformationFactoryBuilder::Release(pfactory);
      if (pct) {
         REPORT_DEBUG("D:Referencia espacial del mundo coincide con la del raster");
         // \todo reemplazar por un CoordinatesTransform
         prastermodel->InverseTransform(windowsubset.ul_);
         prastermodel->InverseTransform(windowsubset.lr_);
      } else { // si fallo CoordinatesTransform::Create salgo
         RasterSpatialModel::Destroy(prastermodel);
         REPORT_AND_FAIL_VALUE(
               "D:No se creo un CoordinateTransform, del raster al mundo", false);
      }
   }

#ifndef __USE_NEW_RASTER_RENDERER__
   delete pct;
   RasterSpatialModel::Destroy(prastermodel);

   // windowsubset debe estar en PixelLinea
   // ancho y alto en pixeles de imagen de la ventana (puede ser mayor o menor)
   double wwidth = windowsubset.lr_.x_ - windowsubset.ul_.x_;
   double wheigth = windowsubset.lr_.y_ - windowsubset.ul_.y_;

   int imgx, imgy;
   pImage_->GetSize(imgx, imgy);

   double ulx = std::max(static_cast<double>(0), windowsubset.ul_.x_),
           uly = std::max(static_cast<double>(0), windowsubset.ul_.y_),
           lrx = std::min(static_cast<double>(imgx), windowsubset.lr_.x_),
           lry = std::min(static_cast<double>(imgy), windowsubset.lr_.y_);

   double offsetx = -std::min(static_cast<double>(0), windowsubset.ul_.x_),
           offsety = -std::min(static_cast<double>(0), windowsubset.ul_.y_);

   // me aseguro un subset UL-LR correcto
   ulx = std::min(ulx, lrx);
   uly = std::min(uly, lry);
   lrx = std::max(lrx, ulx);
   lry = std::max(lry, uly);
   // coordenadas de subset
   int imgulx = SURI_TRUNC(int, ulx), imguly = SURI_TRUNC(int, uly), imglrx =
         SURI_TRUNC(int, SURI_CEIL(lrx)), imglry = SURI_TRUNC(int, SURI_CEIL(lry));
   int imgoffsetx = SURI_TRUNC(int, SURI_CEIL(offsetx)), imgoffsety =
         SURI_TRUNC(int, SURI_CEIL(offsety));
   // tamanio del subset de imagen leido
   int subsetw = (imglrx - imgulx), subseth = (imglry - imguly);
   bool isdecimating = false;
   // Calculo la dimension de la imagen (considerando la posibilidad de
   // window > world, en coordenadas de PL de la imagen)
   // Se hace el min antes de la conversion para que no se exeda el limite
   // de int cuando hay mucho zoom
   int destx = SURI_TRUNC(int, std::min(static_cast<double>(imgx),
               SURI_CEIL(imgx/wwidth*vpwidth)));
   int desty = SURI_TRUNC(int, std::min(static_cast<double>(imgy),
               SURI_CEIL(imgy/wheigth*vpheight)));
   REPORT_DEBUG("D:Dimension de la imagen destino = (%d;%d)", destx, desty);
   // decimador al tamano decimado segun la relacion entre window y viewport
   Decimate decimate(destx, desty);
   // diferencia (en px) entre el subset a leer y el subset de la imagen (real)
   Dimension dim(windowsubset);
   // Correccion esta para que el calculo del plusx(y) considere el offset que se
   // elimina al hacer trunc del offsetx(y).
   double correccionx = windowsubset.ul_.x_ - SURI_TRUNC(int, windowsubset.ul_.x_),
         correcciony = windowsubset.ul_.y_ - SURI_TRUNC(int, windowsubset.ul_.y_);
   correccionx = correccionx < 0 ? 1 + correccionx : correccionx;
   correcciony = correcciony < 0 ? 1 + correcciony : correcciony;
   int plusx = static_cast<int>(
              dim.GetWidth() + correccionx - subsetw - imgoffsetx < 0 ? 0 :
                 SURI_CEIL(dim.GetWidth()+correccionx-subsetw-imgoffsetx)),
         plusy = static_cast<int>(
              dim.GetHeight() + correcciony - subseth - imgoffsety < 0 ? 0 :
                 SURI_CEIL(dim.GetHeight()+correcciony-subseth-imgoffsety));

   // veo si tengo que decimar en alguna dimension
   // if (subsetw>vpwidth||subseth>vpheight) esto falla si la imagen es chica
   // Las condicion 3 y 4 estan por problemas de redondeo cuando el tamano del
   // canvas es similar al de la imagen contenida en el mundo.
   if (destx < imgx || desty < imgy || (imgx + 1 >= SURI_CEIL(imgx/wwidth*vpwidth))
         || (imgy + 1 >= SURI_CEIL(imgy/wheigth*vpheight))) {         // esto anda si la
                                                                      // imagen es chica o
                                                                      // grande
      REPORT_DEBUG("D:Decimando la imagen");
      isdecimating = true;
      pImage_->PushSource(&decimate);
      // El offset se redondea para reducir el error en el comienzo de la imagen
      imgoffsetx = SURI_ROUND(int, offsetx/wwidth*vpwidth);
      imgoffsety = SURI_ROUND(int, offsety/wheigth*vpheight);
#ifdef __UNUSED_CODE__
      // Calcula el offset como la posicion del UL de la ventana o el Raster
      // en coordenadas del viewport.
      // Elimina la dependencia para este calculo del factor de decimado de la
      // imagen y previene el error numerico
      Subset window;
      pWorldWindow->GetWindow(window);
      Dimension dim(window);
      {
         double ulx = 0, uly = 0, lrx = 0, lry = 0;
         GetBoundingBox(pWorldWindow, ulx, uly, lrx, lry);
         Subset win(ulx, uly, lrx, lry);
         Subset intersection = Intersect(window, win);
         pWorldWindow->W2VTransform(intersection.ul_.x_, intersection.ul_.y_,
               imgoffsetx, imgoffsety);
      }
#endif
      // calculo el subset de imagen que debo leer
      // busco el UL y LR de la imagen, en coordenadas de VP-Img(decimada)
      decimate.Real2Resized(ulx, uly, ulx, uly);
      decimate.Real2Resized(lrx, lry, lrx, lry);
      // El comienzo de la imagen se redondea para disminuir el error al comienzo de la imagen
      imgulx = SURI_ROUND(int, ulx);
      imguly = SURI_ROUND(int, uly);
      // El final de la imagen se redondea al limite superior para que no quede linea negra al final
      imglrx = SURI_ROUND(int, SURI_CEIL(lrx) );
      imglry = SURI_ROUND(int, SURI_CEIL(lry) );
      // me aseguro que no haya plus negativos (subset>canvas) al decimar
      subsetw = std::min(imglrx - imgulx, vpwidth);
      subseth = std::min(imglry - imguly, vpheight);
      // evita que plus<0 cuando imagen aumenta uno o dos pixeles al hacer round de imagen u offset
      // hacia arriva
      if (vpwidth - subsetw - imgoffsetx < 0) {
         subsetw = vpwidth - imgoffsetx;
      }
      if (vpheight - subseth - imgoffsety < 0) {
         subseth = vpheight - imgoffsety;
      }

      // fijo el subset a leer con el ancho/alto modificado
      imglrx = imgulx + subsetw;
      imglry = imguly + subseth;

      plusx = vpwidth - subsetw - imgoffsetx;
      plusy = vpheight - subseth - imgoffsety;
   }REPORT_DEBUG("D:Pluses %d;%d", plusx, plusy);
   assert(plusx >= 0 && plusy >= 0);

   REPORT_DEBUG("D:Subset a leer (%d;%d , %d;%d) %dx%d",
                imgulx, imguly, imglrx, imglry, subsetw, subseth);
   REPORT_DEBUG("D:Subset real (%.2f; %.2f , %.2f; %.2f) %.2fx%.2f",
                ulx, uly, lrx, lry, wwidth, wheigth);
   REPORT_DEBUG("D:Offset en el canvas %d;%d", imgoffsetx, imgoffsety);

   // -------------------------------------------------------------------------
   // a partir de aca pImage_ se encuentra en coordenadas del sistema del ViewPort
   // -------------------------------------------------------------------------
   // inicializo el canvas (debe tener la dimension del viewport)
   TRY
   {
      if (pCanvas->GetDataType() != pImage_->GetDataType() ||
            static_cast<size_t>(pCanvas->GetBandCount()) !=
                           parameters_.bandCombination_.size() ||
            pCanvas->GetSizeX() != subsetw + imgoffsetx + plusx ||
            pCanvas->GetSizeY() != subseth + imgoffsety + plusy) {
         pCanvas->SetDataType(pImage_->GetDataType());
         pCanvas->SetBandCount(parameters_.bandCombination_.size());
         pCanvas->SetSize(subsetw + imgoffsetx + plusx, subseth + imgoffsety + plusy);
      }
   } CATCH {
      REPORT_DEBUG("D:No se pudo inicializar el canvas en forma correcta");
      return false;
   }

   if (!ReadImageData(pImage_, pCanvas, pMask, imgulx, imguly, imglrx, imglry, subsetw, subseth,
                 imgoffsetx, imgoffsety))
      return false;
   // Controla que el canvas sea compatible en lugar de modificarlo
#ifdef __UNUSED_CODE__
   if (pCanvas->GetDataType() != pImage_->GetDataType() ||
                     static_cast<size_t>(pCanvas->GetBandCount())
                                       != parameters_.bandCombination_.size()) {
      REPORT_DEBUG("D: Canvas configurado en forma incorrecta");
      return false;
   }
   int cx, cy;
   pCanvas->GetSize(cx, cy);
   int wantedx = subsetw + imgoffsetx + plusx,
         wantedy = subseth + imgoffsety + plusy;
   if (cx != wantedx || cy != wantedy) {
      REPORT_DEBUG("D: Canvas configurado en forma incorrecta");
      return false;
   }
#endif
   // si tiene el decimador
   if (isdecimating) {
      pImage_->PopSource();
   }

#else
   // configuro el canvas
   pCanvas->SetSize(vpwidth, vpheight);
   pCanvas->SetDataType(pImage_->GetDataType() );
   pMask->InitializeAs(pCanvas);
   int bandcount = parameters_.bandCombination_.size();
   pCanvas->SetBandCount(bandcount);
   // inicializo punteros de datos de imagen y de viewport
   std::vector<void*> imagedata(bandcount), viewportdata(bandcount), maskdata(1);
   std::vector<int> canvasindex(bandcount);
   int imgx = 0,
   imgy = 0;
   pImage_->GetSize(imgx, imgy);
   REPORT_DEBUG("D: Tamanio del canvas -> %d ; %d", vpwidth, vpheight);
   REPORT_DEBUG("D: Tamanio de la imagen -> %d ; %d", imgx, imgy);
   for ( int b=0; b<bandcount; b++ ) {
      imagedata.at(b) = new unsigned char[imgx*imgy*pImage_->GetDataSize()];
      unsigned char *ptemp = new unsigned char[vpwidth*vpheight*pCanvas->GetDataSize()];
      viewportdata.at(b) = ptemp;
      memset(ptemp, 0, vpwidth*vpheight*pCanvas->GetDataSize() );
      canvasindex.at(b) = b;
   }
   unsigned char *ptemp = new unsigned char [vpwidth*vpheight*pMask->GetDataSize()];
   maskdata.at(0) = ptemp;
   memset(ptemp, Mask::Opaque, vpwidth*vpheight*pMask->GetDataSize());
   // leo toda la imagen
   pImage_->Read(parameters_.bandCombination_, imagedata, 0, 0, imgx, imgy);
   // recorro el viewport
   // copio el dato de cada pixel de la matriz de la imagen en un pixel de la
   // matriz del viewport utilizando las transformaciones de coordenadas
   // esto se encarga de ajustar el offset y hacer zoom y decimado
   for ( int b=0; b < bandcount; b++ )
   for ( int j=0; j < vpheight; j++ )
   for ( int i=0; i < vpwidth; i++ ) {
      Coordinates temp(i, j);
      Coordinates out(0, 0);
      // de coordenadas de viewport a coordenadas de mundo
      pWorldWindow->Transform(temp, out);
      // de sistema de mundo, a sistema de imagen
      pct->TransformCoordinates(out);
      // de sistema de imagen a pixel - linea de matriz
      prastermodel->InverseTransform(out);
// ----------- Este codigo hace interpolacion NN implicita para ZOOM y DECIMANDO
      // coordenadas truncadas de matriz (interpolacion NN implicita)
      int imgi = SURI_TRUNC(int, out.x_),
      imgj = SURI_TRUNC(int, out.y_);
      // Si cae dentro de la matriz de imagen, copio el dato
      // -1 porque utiliza double y pregunta por <=
      if ( Subset(Coordinates(0, 0), Coordinates(imgx-1, imgy-1) ).
            IsInside(Coordinates(imgi, imgj) ) ) {
         // punteros temporales para la copia
         unsigned char *pdest = static_cast<unsigned char*>(
               viewportdata.at(b) )+(i+j*vpwidth)*pCanvas->GetDataSize(),
         *psrc = static_cast<unsigned char*>(
               imagedata.at(b) )+(imgi+imgj*imgx)*pCanvas->GetDataSize();
         memccpy(static_cast<void*>(pdest), static_cast<void*>(psrc),
               pCanvas->GetDataSize(), 1);
      }
// ----------- Fin -------------------------------------------------------------
   }
   if (pMask) {
      std::vector<int> idx(1,0);
      dynamic_cast<Canvas*>(pMask)->Write(idx, maskdata);
      pMask->ApplyMask(canvasindex, viewportdata, pCanvas);
   } else {
      // escribo los datos en el canvas
      pCanvas->Write(canvasindex, viewportdata, pMask);
   // elimino los datos
   }
   for ( int b=0; b<bandcount; b++ )
   {
      delete[] static_cast<unsigned char*>(imagedata.at(b) );
      delete[] static_cast<unsigned char*>(viewportdata.at(b) );
   }
   delete[] static_cast<unsigned char*>(maskdata.at(0));
   // elimino las transformaciones
   RasterSpatialModel::Destroy(prastermodel);
   CoordinatesTransformInterface::Destroy(pct);
#endif   // __USE_OLD_RASTER_RENDERER__
   REPORT_DEBUG("D:RasterRenderer::Render()::end");
   changed_ = false;
   return true;
}

/**
 * Obtiene el "bounding box" del elemento renderizado
 * @param[in]	pWorld: puntero a la ventana del mundo.
 * @param[in]	Ulx: uper-left x
 * @param[in]	Uly: uper-left y
 * @param[in]	Lrx: lower-right x
 * @param[in]	Lry: lower-right y
 */
void RasterRenderer::GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                                    double &Lrx, double &Lry) {
   if (!pImage_)
   REPORT_DEBUG("D:No hay imagen asociada");
   ParameterCollection params;
     params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                  parameters_.spatialModel_);
     params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                  pWorld->GetSpatialReference());
     TransformationFactory* pfactory =
           TransformationFactoryBuilder::Build();
     CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                              params);
     TransformationFactoryBuilder::Release(pfactory);
   if (pct && !pct->IsIdentity()) {
      REPORT_DEBUG("D:Referencia espacial del mundo != de la del raster");
      delete pct;
      return;
   }
   delete pct;

   // asume que parameters_.rasterModel_ es equivalente a pWorld->RasterModel()
   RasterSpatialModel *prastermodel = RasterSpatialModel::Create(
         parameters_.rasterModel_);
   if (!prastermodel) {
      REPORT_AND_RETURN("D:No se pudo crear RasterSpatialModel");
   }
   Ulx = 0;
   Uly = 0;
   Lrx = pImage_->GetSizeX();
   Lry = pImage_->GetSizeY();
   Coordinates ul(Ulx, Uly), lr(Lrx, Lry);
   prastermodel->Transform(ul);
   prastermodel->Transform(lr);

   // Transformar usando coordinate transform a un nuevo BB (reproyectado)
   // se deberia recorrer los bordes quedandose con los max-min
   RasterSpatialModel::Destroy(prastermodel);
   Ulx = ul.x_;
   Uly = ul.y_;
   Lrx = lr.x_;
   Lry = lr.y_;
   REPORT_DEBUG("D:Extent de elemento cargado con exito %.2f;%.2f %.2f;%.2f",
                Ulx, Uly, Lrx, Lry);
}

/**
 * Obtiene los parametros de entrada del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void RasterRenderer::GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                        std::string &DataType) const {
   SizeX = -1;
   SizeY = -1;
   BandCount = -1;
   DataType = DataInfo<void>::Name;
}

/**
 * Obtiene los parametros de salida del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void RasterRenderer::GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                         std::string &DataType) const {
   BandCount = parameters_.bandCombination_.size();
   if (pImage_) DataType = pImage_->GetDataType();
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------
/**
 * Actualizacion de parametros para representar un elemento
 * @param[in]	pElement: puntero al elemento.
 */
void RasterRenderer::Update(Element *pElement) {
   Parameters param = GetParameters(pElement->GetNode(wxT("")));
   if (param.imageUrl_ != parameters_.imageUrl_) {
      /*REPORT_ERROR("D:No se trata del mismo URL, se esperaba \"%s\" se recibio \"%s\".",
       parameters_.imageUrl_.c_str(),param.imageUrl_.c_str());*/
      changed_ = true;
      return;
   }

   if (parameters_.bandCombination_ != param.bandCombination_) {
      changed_ = true;
   }
   if (parameters_.generateMask_ != param.generateMask_ ||
         !AreEqual(parameters_.noDataValue_, param.noDataValue_)) {
      changed_ = true;
   }
   // \TODO(Javier #2232: Cargar desde la fuente de datos)

   if (ValidateParameters(param))
      parameters_ = param;
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------
/** Indica si el renderizador tiene cambios para aplicar */
bool RasterRenderer::IsDirty() {
   return changed_;
}

/** Lee los datos de la imagen */
bool RasterRenderer::ReadImageData(Image* pImage, Canvas* pCanvas, Mask* pMask,
                                   int Ulx, int Uly, int Lrx, int Lry,
                                   int SubsetWidth, int SubsetHeight,
                                   int ImgOffsetX, int ImgOffsetY) {

   int csizex = 0, csizey = 0;
   pCanvas->GetSize(csizex, csizey);
   REPORT_DEBUG("D:Tamano de Canvas %d;%d", csizex, csizey);
   int datasize = pImage->GetDataSize();
   // \todo: Se podria optimizar la escritura, haciendola en forma directa
   //      a los buffers del MemoryCanvas usando GetBlock (\todo: ver como
   //      lo seteo en dirty y si es necesario)
   // reservo memoria para la lectura de la imagen
   int buffersize = SubsetWidth * SubsetHeight * datasize;

   if (buffersize < 0) {
      REPORT_DEBUG("Se calculo un valor negativo para reservar memoria! (MODO GIS)");
      buffersize *= -1;
   }

   std::vector<void*> imagedata(parameters_.bandCombination_.size());
   for (size_t i = 0; i < imagedata.size(); i++) {
      imagedata[i] = new unsigned char[buffersize];
      REPORT_DEBUG("D:imagedata[%d] = %x",
                   i, static_cast<unsigned char*>(imagedata[i]));
   }

   // Leo los datos de la imagen
   if (!pImage->Read(parameters_.bandCombination_, imagedata, Ulx, Uly, Lrx,
                     Lry)) {
      for (size_t i = 0; i < imagedata.size(); i++)
         delete[] static_cast<unsigned char*>(imagedata[i]);

      return false;
   }
   std::vector<void*> auxData(pCanvas->GetBandCount());
   for (int x= 0; x < pCanvas->GetBandCount(); x++) {
      auxData[x] = new unsigned char[csizex * csizey * datasize];
   }
   // genero un vector de las bandas para la escritura en el canvas
   std::vector<int> bands(parameters_.bandCombination_.size());
   for (size_t i = 0; i < bands.size(); i++)
      bands[i] = i;
   pCanvas->Read(bands, auxData);
   // si el canvas es mas grande que el subset leido
   // debo copiar por linea
   // \todo enmascarar lo que se rellena
   if (csizex > SubsetWidth || csizey > SubsetHeight) {
      REPORT_DEBUG("D:Rellenando canvas con valores nulos");
      for (size_t b = 0; b < imagedata.size(); b++) {
#ifdef __CUSTOM_CANVAS_OFFSET_FIX__
         if ( SubsetWidth+ImgOffsetX>csizex )
         {
            ImgOffsetX = csizex - SubsetWidth - 1;
            ImgOffsetX = ImgOffsetX<0 ? 0 : ImgOffsetX;
         }
         if ( SubsetHeight+ImgOffsetY>csizey )
         {
            ImgOffsetY = csizey - SubsetHeight - 1;
            ImgOffsetY = ImgOffsetY<0 ? 0 : ImgOffsetY;
         }
#endif   // uso los datos sobre el canvas por si hay algo
         unsigned char *pcanvasdata = static_cast<unsigned char*> (auxData[b]);
         REPORT_DEBUG("D:pcanvasdata[%d] = %x", b, pcanvasdata);
         unsigned char *ptemp = pcanvasdata
               + (ImgOffsetY * csizex + ImgOffsetX) * datasize;
         unsigned char *pdata = static_cast<unsigned char*>(imagedata[b]);
         for (int j = 0; j < SubsetHeight; j++) {
            // todo (danieltesta-#4308): esto es un workaround para evitar que
            // la aplicacion pinche. La proyeccion esta bien hecha pero cuando
            // se la quiere visualizar en modo GIS pincha. Pasa solo con una
            // imagen con un SRS proyectado en particular.
            if (SubsetWidth < 0)
               SubsetWidth *= -1;
            memcpy(ptemp, pdata, SubsetWidth * datasize);
            ptemp += (csizex * datasize);
            pdata += (SubsetWidth * datasize);
         }
         delete[] static_cast<unsigned char*>(imagedata[b]);
         imagedata[b] = pcanvasdata;
      }
   }
#ifdef __DEBUG__
   unsigned char **pverifypointers = new unsigned char*[imagedata.size()];
   {
      size_t size = imagedata.size();
      for (size_t i = 0; i < size; i++)
         pverifypointers[i] = static_cast<unsigned char*>(imagedata[i]);
   }
#endif   /* __DEUBG__ */
   if (pMask)
      pMask->InitializeAs(pCanvas);
   FillMaskFromRasterData(imagedata, pImage->GetDataType(), pMask);
   // si no existe la mascara o pide no generarla, copio directamente los datos
   if (pMask && parameters_.generateMask_)
      pMask->ApplyMask(bands, imagedata, pCanvas);
   else
      pCanvas->Write(bands, imagedata, NULL);
   // borro los datos temporales
   size_t size = imagedata.size();
   for (size_t i = 0; i < size; i++) {
#ifdef   __DEBUG__
      if (pverifypointers[i] != static_cast<unsigned char*>(imagedata[i])) {
         REPORT_DEBUG("D:Error en los punteros anotado = %x, usado = %x",
                      pverifypointers[i], static_cast<unsigned char*>(imagedata[i]));
         REPORT_DEBUG("D:Evitando SIGSEV, generando LEAK");
      } else
#endif   /* __DEBUG__ */
      {
         REPORT_DEBUG("D:imagedata[%d] = %x",
                      i, static_cast<unsigned char*>(imagedata[i]));
         FLUSH();
         delete[] static_cast<unsigned char*>(imagedata[i]);
      }
   }
#ifdef __DEBUG__
   delete[] pverifypointers;
#endif
   return true;
}

/**
 * Template para generar una mascara en funcion de los datos de imagen.
 * Esta mascara verifica el contenido de todas las bandas y en el caso
 * de que TODAS las bandas contengan el valor no valido se enmascara.
 * @param Source array de punteros a los datos
 * @param SourceMaskedValue array de punteros al dato a considerar como invalido
 * @param pMask puntero a la mascara
 * @param Size tamano de los punteros
 */
template<class T>
void genmaskvector(std::vector<void*>& Source, std::vector<void*>& SourceMaskedValue,
                   unsigned char *pMask, int Size) {
   for (int i = 0; i < Size; i++) {
      bool maskpixel = true;
      for (unsigned int b = 0; maskpixel && b < Source.size(); ++b) {
         T* psource = static_cast<T*>(Source.at(b));
         T source = psource[i];
         double maskedvalue = *(static_cast<double*>(SourceMaskedValue.at(b)));
         // Si alguna banda tiene un valor != al valor no valido
         // entonces no se enmascara el pixel
         if (!SURI_ISNAN(source) && !AreEqual(source, maskedvalue)) {
            maskpixel = false;
         }
      }
      // si se enmascaro copio en todas las bandas el valor no valido
      if (maskpixel) {
         pMask[i] = Mask::Transparent;
      }
   }
}

/**
 * Template para generar una mascara en funcion de los datos de imagen.
 *
 * @param pSource puntero a los datos
 * @param pSourceMaskedValue puntero al dato a considerar como invalido
 * @param pMask puntero a la mascara
 * @param Size tamano de los punteros
 */
template<class T>
void genmask(void* pSource, void* pSourceMaskedValue, unsigned char *pMask, int Size) {
   T* psource = static_cast<T*>(pSource);
   double maskedvalue = *(static_cast<double*>(pSourceMaskedValue));
   for (int i = 0; i < Size; i++) {
      T source = psource[i];
      // Donde el valor es el que se debe enmascarar, cargo transparente en la
      // mascara
      if (SURI_ISNAN(source) || AreEqual(source, maskedvalue))
         pMask[i] = Mask::Transparent;
   }
}

/** Inicializa mapa de tipos de datos. */
//INITIALIZE_DATATYPE_MAP(void (*)(void*, void*, unsigned char*, int), genmask);
INITIALIZE_DATATYPE_MAP(void (*)(std::vector<void*>&,
      std::vector<void*>&, unsigned char*, int), genmaskvector);

/**
 *  Carga en pMask los pixels que deben ser considerados como invalidos para
 * la lectura del raster.
 *  Los datos son considerados invalidos si son iguales al dato definido como
 * No-Data en la fuente de datos o si se encuentran fuera de la matriz de la
 * imagen leida.
 *
 * @param pImage pImage Imagen que tiene los datos.
 * @param[in] pMask Mascara donde se guardan los datos.
 * @param[in] ValidSubset Subset en coordenadas de pMask (Debe truncarse).
 */
void RasterRenderer::FillMaskFromRasterData(std::vector<void*> &RasterData,
                                             const std::string &RasterDataType,
                                             Mask *pMask) const {
   if (!pMask || pMask->GetBandCount() == 0)
      return;
   std::vector<int> maskbands;
   maskbands.push_back(0);
   std::vector<void*> maskdata;
   pMask->GetInternalData(maskbands, maskdata);
   unsigned char *pmaskdata = static_cast<unsigned char*>(maskdata.at(0));
   int x, y;
   pMask->GetSize(x, y);
   // fuerza mascara completamente opaca
   memset(pmaskdata, Mask::Opaque, x * y);
   // si no debe generar mascara, sale con todo opaco
   if (!parameters_.generateMask_)
      return;
   double masked = parameters_.noDataValue_;
   void *p = &masked;
   std::vector<void*> nodatavaluearray;
   for (size_t b = 0; b < RasterData.size(); ++b) {
      nodatavaluearray.push_back(p);
   }
   genmaskvectorTypeMap[RasterDataType](RasterData, nodatavaluearray, pmaskdata, x * y);
}


/**
 * El comportamiento default es abrir la imagen completa.
 * Si pImage_==NULL crea una imagen usando url en los parametros del renderizador.
 * Valida que los parametros sean validos para la imagen abierta
 * \post pImage_ no debe ser NULL.
 * @param[in]  pWorldWindow: parte del mundo que se quiere renderizar,
 *                en caso de ser NULL se abre la imagen completa.
 */
bool RasterRenderer::UpdateImage(const World* pWorldWindow) {
   if (!pImage_) {
      Image::ImageAccessType access = Image::ReadOnly;
      std::string writer = "null";
      std::string datatype = parameters_.rawMetadata_.GetOption("Datatype");
      std::string dataname = datatype.empty() ? "void" : datatype;
      int bandcount = 0, sizex = 0, sizey = 0;
      std::string pixels = parameters_.rawMetadata_.GetOption("Pixels");
      if (!pixels.empty()) {
         sizex = StringToNumber<int>(pixels);
      }
      std::string lines = parameters_.rawMetadata_.GetOption("Lines");
      if (!lines.empty()) {
         sizey = StringToNumber<int>(lines);
      }

      std::string bcountstr = parameters_.rawMetadata_.GetOption("Bandcount");
      if (!bcountstr.empty()) {
         bandcount = StringToNumber<int>(bcountstr);
      }
      pImage_ = Image::Open(GenerateImageId(parameters_.imageUrl_), access, writer,
                            bandcount, sizex, sizey, dataname, parameters_.rawMetadata_);
   } else {
      REPORT_DEBUG("D: RasterRenderer ya tiene una imagen abierta");
   }

   return ValidateParameters(parameters_);
}

/**
 * Tranforma el Url del elemento a string que usa Image para abrir la imagen.
 * Este string depende del driver de Gdal que se use.
 * @return string que se le pasa a Image.
 */
std::string RasterRenderer::GenerateImageId(const std::string &ImageId) {
   std::string gdalid = ImageId;

   // Si el path es un uri lo transformo a string soportado por gdal.
   Uri uri(ImageId);
   if (CaseInsensitiveCompare(uri.GetScheme(), "file")) {
      gdalid = uri.GetPath();
      std::map<std::string, std::string> queryparameters = suri::StringToMap(
            uri.GetQuery(), Uri::QUERY_SEPARATOR);

      if (queryparameters.count(URI_FORMAT_KEY) != 0) {
         if (CaseInsensitiveCompare(queryparameters[URI_FORMAT_KEY],
                                    URI_HDF5_FORMAT_VALUE)
               && queryparameters.count(URI_DATASET_KEY) == 1) {
            gdalid = wxT("HDF5:") + uri.GetPath() + wxT("://")
                  + queryparameters[URI_DATASET_KEY];
         }
      }
   }

   return gdalid;
}

/**
 * Valida que los datos en clase Parameters sean congruentes con la imagen
 * que se quiere renderizar.
 * @param[in]  Parameters: parametros que quiero validar.
 */
bool RasterRenderer::ValidateParameters(const Parameters &Parameters) {
   if (pImage_ == NULL || Parameters.bandCombination_.empty()) return false;

   // Valido que existan las bandas a renderizar
   bool validparameters = true;
   int maxbandrequested = *std::max_element(Parameters.bandCombination_.begin(),
                                            Parameters.bandCombination_.end());
   if (maxbandrequested >= pImage_->GetBandCount()) validparameters = false;

   return validparameters;
}
}

