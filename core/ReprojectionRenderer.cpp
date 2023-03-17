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

// Includes Estandar
#include <map>
#include <string>
#include <vector>

// Includes Suri
#include "suri/ReprojectionRenderer.h"
#include "suri/RasterSpatialModel.h"
#include "suri/World.h"
#include "suri/Canvas.h"
#include "Mask.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/xmlnames.h"
#include "suri/Element.h"
#include "suri/Uri.h"
#include "suri/ParameterCollection.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/GroundControlPoint.h"
#include "suri/TextFileGcpLoader.h"
#include "suri/GcpList.h"
#include "suri/Dimension.h"

// Includes Wx
#include "wx/xml/xml.h"

// Defines
#define URI_FORMAT_KEY "format"
#define URI_DATASET_KEY "dataset"
#define URI_HDF5_FORMAT_VALUE "hdf5"

#define DELETE_PCT(x) delete x; x = NULL;

/**
 * Parche hasta que se solucione bug:
 * El SRS de los puntos en extent de mundo y ventana
 * de pWorldIn no esta en srs de imagen de salida.
 */
#define __EXTENT_SRS_BUG_FIX__

// forwards

/** Macro para registrar el renderer */
AUTO_REGISTER_RENDERER(suri::ReprojectionRenderer);

namespace suri {

/** ctor **/
ReprojectionRenderer::ReprojectionRenderer() :
      pImage_(NULL), pMask_(NULL), changed_(true) {
   parameters_.pGcpList_ = NULL;
}

/** Destructor
 *  Se encarga de cerrar la imagen **/
ReprojectionRenderer::~ReprojectionRenderer() {
   Image::Close(pImage_);
   if (parameters_.pGcpList_)
      delete parameters_.pGcpList_;
   parameters_.pGcpList_ = NULL;
}

/** Crea un renderizador a partir de un pElement
 * Metodo  abstracto puro utilizado para crear con el metodo
 * estatico Create() instancias de clases derivadas registradas con el
 * metodo RegisterViewer().
 * @param[in] pElement Elemento a renderizar.
 * @param[in] pPreviousRenderer Renderizador anterior.
 */
Renderer *ReprojectionRenderer::Create(Element *pElement,
                                       Renderer *pPreviousRenderer) const {
   ReprojectionRenderer *prenderer = new ReprojectionRenderer;
   LoadParameters(pElement, &prenderer->parameters_);
   return prenderer;
}

/** Nombre del renderizador == al nombre del nodo
 *  Retorna el nombre del nodo que maneja el renderizador.
 * @return Nombre del nodo manejado por la clase derivada.
 */
std::string ReprojectionRenderer::CreatedNode() const {
   return REPROJECTION_NODE;
}

/**
 * Renderiza el elemento dado un World en un Canvas.
 *
 * @param[in] pWorldWindow Mundo de llegada.
 * @param[out] pCanvas canvas donde se guarda el resultado de la renderizacion.
 * @param[out] pMask mascara resultado de la renderizacion.
 *
 * \todo (danieltesta-#4043): En este metodo hay una serie de parches para que
 * funcionen los procesos de corregistro y georreferenciacion sin interferir con
 * el actual funcionamiento del proceso de reproyeccion. Por una cuestion de tiempo
 * y de como estan encarados los procesos se tuvo que recurrir a una solucion como
 * esta. Ver a futuro la posibilidad de contemplar una mejora gral a estos procesos
 * para que el codigo sea mas limpio.
 */
bool ReprojectionRenderer::Render(const World *pWorldWindow, Canvas* pCanvas,
                                  Mask* pMask) {

   suri::CoordinatesTransformation* pct = GetTransformation(pWorldWindow);

   bool returnvalue = true;
   if (pPreviousRenderer_ && pct && pct->IsIdentity()) {
      delete pct;
      returnvalue = pPreviousRenderer_->Render(pWorldWindow, pCanvas, pMask);
   } else if (pPreviousRenderer_) {
      UpdateImage();

      // Parche #1 para que funcionen los procesos de corregistro y georreferenciacion
      // sin inteferir con el proceso de reproyeccion.
      World* psrcworld = new World();
      World* pdestworld = new World(*pWorldWindow);

      InitializeInputWorld(pdestworld, psrcworld, pct);

      Canvas* ppreviouscanvas = new MemoryCanvas;
      ppreviouscanvas->InitializeAs(pCanvas);
      Mask* ppreviousmask = new Mask;
      ppreviousmask->InitializeAs(pMask);

      // Parche #2 para que funcionen los procesos de corregistro y georreferenciacion
      // sin inteferir con el proceso de reproyeccion.
      if (!pPreviousRenderer_->Render((parameters_.pGcpList_) ? pdestworld : psrcworld,
                                      ppreviouscanvas, ppreviousmask)) {
         delete pct;
         return false;
      }

      pCanvas->SetBandCount(ppreviouscanvas->GetBandCount());
      pCanvas->SetDataType(ppreviouscanvas->GetDataType());
      pCanvas->Clear();
      pMask->SetBandCount(ppreviousmask->GetBandCount());
      pMask->Clear();

      int vpwidth, vpheight;
      pdestworld->GetViewport(vpwidth, vpheight);
      pCanvas->SetSize(vpwidth, vpheight);
      pCanvas->SetDataType(pImage_->GetDataType());
      pMask->InitializeAs(pCanvas);
      int bandcount = ppreviouscanvas->GetBandCount();
      pCanvas->SetBandCount(bandcount);

      // ----------- inicializo punteros de datos de imagen y de viewport ------//
      std::vector<void*> previousdata(bandcount), viewportdata(bandcount);
      std::vector<int> canvasindex(bandcount);
      // Inicializo los datos del canvas en funcion de lo generado por el render anterior
      InitializeDataFromCanvas(vpwidth, vpheight, ppreviouscanvas, canvasindex,
                               previousdata, viewportdata);
      // Leo la imagen
      ppreviouscanvas->Read(canvasindex, previousdata);

      std::vector<void*> maskdata(1), previousmaskdata(1);
      // Inicializo los datos de la mascara a partir de la mascara genereda en el
      // render anterior
      InitializeDataFromMask(vpwidth, vpheight, ppreviouscanvas, ppreviousmask,
                             previousmaskdata, maskdata);

      // Se recorre el viewport
      // Parche #3 para que funcionen los procesos de corregistro y georreferenciacion
      // sin inteferir con el proceso de reproyeccion.
      WalkThroughViewport((parameters_.pGcpList_) ? psrcworld : pdestworld,
                          (parameters_.pGcpList_) ? pdestworld : psrcworld,
                          ppreviouscanvas, ppreviousmask, pCanvas, pMask, previousdata,
                          viewportdata, previousmaskdata, maskdata, pct);

      if (pMask) {
         std::vector<int> idx(1, 0);
         pMask->Write(idx, maskdata);
         pMask->ApplyMask(canvasindex, viewportdata, pCanvas);
      } else {
         // escribo los datos en el canvas
         pCanvas->Write(canvasindex, viewportdata, pMask);
      }
      // elimino los datos
      for (int b = 0; b < bandcount; b++) {
         delete[] static_cast<unsigned char*>(previousdata.at(b));
         delete[] static_cast<unsigned char*>(viewportdata.at(b));
      }
      delete[] static_cast<unsigned char*>(maskdata.at(0));
      delete[] static_cast<unsigned char*>(previousmaskdata.at(0));
      delete psrcworld;
      delete pdestworld;
      delete ppreviouscanvas;
      delete ppreviousmask;
      delete pct;
      REPORT_DEBUG("D:ReprojectionRenderer::Render()::end");
      changed_ = false;
      returnvalue = true;
   } else {
      delete pct;
   }
   return returnvalue;
}

/**
 *
 * @param pWorldWindow
 * @param pTransformWorld
 * @param pPreviousCanvas
 * @param pPreviousMask
 * @param pCanvas
 * @param pMask
 * @param PreviousData
 * @param ViewportData
 * @param PreviousMaskData
 * @param MaskData
 */
void ReprojectionRenderer::WalkThroughViewport(const World* pWorldWindow,
                                               World* pTransformWorld,
                                               Canvas* pPreviousCanvas,
                                               Mask* pPreviousMask, Canvas* pCanvas,
                                               Mask* pMask,
                                               std::vector<void*>& PreviousData,
                                               std::vector<void*>& ViewportData,
                                               std::vector<void*>& PreviousMaskData,
                                               std::vector<void*>& MaskData,
                                               suri::CoordinatesTransformation* pct) {

   if (pct) {
      int bandcount = pPreviousCanvas->GetBandCount();
      int vpwidth, vpheight;
      pWorldWindow->GetViewport(vpwidth, vpheight);
      int prevx = 0, prevy = 0;
      pPreviousCanvas->GetSize(prevx, prevy);
      // recorro el viewport
      // copio el dato de cada pixel de la matriz de la imagen en un pixel de la
      // matriz del viewport utilizando las transformaciones de coordenadas
      // esto se encarga de ajustar el offset y hacer zoom y decimado
      for (int j = 0; j < vpheight; j++) {
         for (int i = 0; i < vpwidth; i++) {
            Coordinates temp(i, j);
            Coordinates out(0, 0);
            // de coordenadas de viewport a coordenadas de mundo de llegada
            pWorldWindow->Transform(temp, out);
            // de coordenadas de mundo de llegada a coordenadas de mundo de salida (previous)
            pct->Transform(out);
            // de sistema de salida a pixel - linea de matriz del canvas previous
            pTransformWorld->InverseTransform(out, temp);
            out = temp;
            // ----------- Este codigo hace interpolacion NN implicita para ZOOM y DECIMANDO
            // coordenadas truncadas de matriz (interpolacion NN implicita)
            int imgi = SURI_TRUNC(int, out.x_), imgj = SURI_TRUNC(int, out.y_);
            // Si cae dentro de la matriz de imagen, copio el dato
            // -1 porque utiliza double y pregunta por <=
            if (Subset(Coordinates(0, 0), Coordinates(prevx - 1, prevy - 1)).IsInside(
                  Coordinates(imgi, imgj))) {
               for (int b = 0; b < bandcount; b++) {
                  // punteros temporales para la copia
                  unsigned char *pdest = static_cast<unsigned char*>(ViewportData.at(b))
                        + (i + j * vpwidth) * pCanvas->GetDataSize();
                  unsigned char *psrc = static_cast<unsigned char*>(PreviousData.at(b))
                        + (imgi + imgj * prevx) * pPreviousCanvas->GetDataSize();
                  memcpy(static_cast<void*>(pdest), static_cast<void*>(psrc),
                         pCanvas->GetDataSize());
               }
               // punteros temporales para la copia de la mascara
               unsigned char *pmaskdest = static_cast<unsigned char*>(MaskData.at(0))
                     + (i + j * vpwidth) * pMask->GetDataSize();
               unsigned char *pmasksrc =
                     static_cast<unsigned char*>(PreviousMaskData.at(0))
                           + (imgi + imgj * prevx) * pPreviousMask->GetDataSize();
               memcpy(static_cast<void*>(pmaskdest), static_cast<void*>(pmasksrc),
                      pMask->GetDataSize());
            } else {
               // set data as NODATA
               // punteros temporales para la copia de la mascara
               /*unsigned char *pmaskdest = static_cast<unsigned char*>(maskdata.at(0))
                + (i + j * vpwidth) * pMask->GetDataSize();
                pmaskdest[0] = Mask::Invalid;*/
            }
            // ----------- Fin -------------------------------------------------------------
         }
      }
   }
}

/**
 * Inicializa la informacion en CanvasIndex, PreviousData y ViewPortData en funcion
 * del canvas y el tam. del viewport que se pasa por parametro
 * @param[in] VpWidth ancho del viewport
 * @param[in] VpHeight alto del viewport
 * @param[in] pPreviousCanvas canvas generado por el renderizador anterior
 * @param[out] CanvasIndex vector con los indices de los canvas
 * @param[out] PreviousData data del canvas que genero el renderizador anterior
 * @param[out] ViewportData data del viewport que genero el renderizador anterior
 */
void ReprojectionRenderer::InitializeDataFromCanvas(int VpWidth, int VpHeight,
                                                           Canvas* pPreviousCanvas,
                                                           std::vector<int>& CanvasIndex,
                                                           std::vector<void*>& PreviousData,
                                                           std::vector<void*>& ViewportData) {
   int prevx = 0, prevy = 0;
   pPreviousCanvas->GetSize(prevx, prevy);
   int bandcount = pPreviousCanvas->GetBandCount();
   REPORT_DEBUG("D: Tamanio del canvas -> %d ; %d", VpWidth, VpHeight);
   REPORT_DEBUG("D: Tamanio de la imagen -> %d ; %d", prevx, prevy);
   REPORT_DEBUG("D: Cantidad de Bandas -> %d", bandcount);
   for (int b = 0; b < bandcount; b++) {
      PreviousData.at(b) = new unsigned char[prevx * prevy * pImage_->GetDataSize()];
      unsigned char *ptemp = new unsigned char[VpWidth * VpHeight
            * pPreviousCanvas->GetDataSize()];
      ViewportData.at(b) = ptemp;
      memset(ptemp, 0, VpWidth * VpHeight * pPreviousCanvas->GetDataSize());
      CanvasIndex.at(b) = b;
   }
}

/**
 * Inicializa la informacion en MaskData y ViewPortData en funcion
 * del canvas y el tam. del viewport que se pasa por parametro
 * @param[in] VpWidth
 * @param[in] VpHeight
 * @param[in] pPreviousCanvas canvas generado por el renderizador anterior
 * @param[in] pPreviousMask mascara generada por el renderizador anterior
 * @param[out] PreviousMaskData informacion de la mascara generada por
 * el renderizador anterior
 * @param[out] MaskData
 */
void ReprojectionRenderer::InitializeDataFromMask(int VpWidth, int VpHeight,
                                                  Canvas* pPreviousCanvas,
                                                  Mask* pPreviousMask,
                                                  std::vector<void*>& PreviousMaskData,
                                                  std::vector<void*>& MaskData) {
   int prevx = 0, prevy = 0;
   pPreviousCanvas->GetSize(prevx, prevy);
   unsigned char *ptemp = new unsigned char[VpWidth * VpHeight
         * pPreviousMask->GetDataSize()];
   MaskData.at(0) = ptemp;
   memset(ptemp, Mask::Invalid, VpWidth * VpHeight * pPreviousMask->GetDataSize());
   unsigned char *pmasktemp = new unsigned char[prevx * prevy
         * pPreviousMask->GetDataSize()];
   PreviousMaskData.at(0) = pmasktemp;
   std::vector<int> maskindex(1, 0);
   pPreviousMask->Read(maskindex, PreviousMaskData);
}

/**
 * Devuelve el objeto para realizar la transformacion dado los parametros
 * especificados.
 *
 * @param[in] pWorldIn Mundo del cual se toman los parametros a transformar.
 * @return Objeto para transformacion de coordenadas.
 */
suri::CoordinatesTransformation* ReprojectionRenderer::GetTransformation(
      const World* pWorldIn) const {

   std::string algorithm = parameters_.transformationType_;

   suri::ParameterCollection params;
   params.AddValue<std::string>(suri::TransformationFactory::kParamWktIn,
                                pWorldIn->GetSpatialReference());
   params.AddValue<std::string>(suri::TransformationFactory::kParamWktOut,
                                parameters_.spatialReference_);

   suri::TransformationFactory* pfactory = suri::TransformationFactoryBuilder::Build();

   std::vector<GroundControlPoint> vgcps;

   // Si es una transformacion polinomica primero hay que conseguir
   // un conjunto de GCPs para poder hacer el calculo. Los GCPs se
   // consiguen haciendo una transformacion exacta sobre una cantidad
   // determinada de puntos.
   if (algorithm.compare(suri::TransformationFactory::kPolynomial) == 0 && !parameters_.pGcpList_) {

      suri::CoordinatesTransformation* pexact = pfactory->Create(
            suri::TransformationFactory::kExact, params);

      Subset extent;

      pWorldIn->GetWorld(extent);
#ifdef __EXTENT_SRS_BUG_FIX__
      extent = parameters_.imageExtent_;
#endif
      std::vector<Coordinates> coords = Subset::ExtractCoordinates(
            extent, DEFAULT_CALCULATION_POINTS, parameters_.delta_);

      for (int ix = 0, len = coords.size(); ix < len; ++ix) {
         Coordinates src = coords[ix];
         Coordinates dest = coords[ix];

         pexact->Transform(src
#ifdef __EXTENT_SRS_BUG_FIX__     
         , true
#endif
         );

         vgcps.push_back(GroundControlPoint(src, dest));
      }

      delete pexact;
      pexact = NULL;
   } else if (algorithm.compare(suri::TransformationFactory::kPolynomial) == 0
                                                      && parameters_.pGcpList_) {
      RasterSpatialModel* psourcerastermodel = RasterSpatialModel::Create(
                              parameters_.pGcpList_->GetRasterModelSource());
      RasterSpatialModel* pdestinationrastermodel = RasterSpatialModel::Create(
                              parameters_.pGcpList_->GetRasterModelDestination());
      // Los puntos de control quedan en el srs y no pixel-linea. Ver TCK 3680.
      std::vector<GroundControlPoint> originalgcps = parameters_.pGcpList_->CreateAdaptedGcps();
      std::vector<GroundControlPoint>::iterator it = originalgcps.begin();
      for (; it != originalgcps.end(); ++it) {
         Coordinates src = it->GetSource();
         Coordinates dest = it->GetDestination();
         if (psourcerastermodel)
            psourcerastermodel->Transform(src);
         if (pdestinationrastermodel)
            pdestinationrastermodel->Transform(dest);
         vgcps.push_back(GroundControlPoint(src, dest));
      }

      RasterSpatialModel::Destroy(psourcerastermodel);
      RasterSpatialModel::Destroy(pdestinationrastermodel);
   }

   params.AddValue<std::vector<GroundControlPoint> >(suri::TransformationFactory::kParamGcps, vgcps);
   params.AddValue<int>(suri::TransformationFactory::kParamOrder, parameters_.transformationOrder_);

   suri::CoordinatesTransformation* pct = pfactory->Create(
         parameters_.transformationType_, params);
   suri::TransformationFactoryBuilder::Release(pfactory);
   return pct;
}

/**
 *  Metodo auxiliar que dado el mundo de salida y la transformacion
 *  calcula el mundo de entrada.
 *  @param[in] pWorldIn mundo de la imagen de salida
 *  @param[out] pTransformWorld Mundo que configura la parte de la imagen
 *  de entrada a leer
 */
void ReprojectionRenderer::InitializeInputWorld(
      const World* pWorldIn, World* &pTransformWorld,
      suri::CoordinatesTransformation* pct) const {

   int x = 0, y = 0;
   this->pImage_->GetSize(x, y);
   pTransformWorld->SetViewport(x, y);

   // Parche para que corregistro y georreferenciacion funcionen.
   if (parameters_.pGcpList_) {
      Subset world(0, 0, x, y);
      RasterSpatialModel* pdestinationrastermodel = RasterSpatialModel::Create(
                              parameters_.pGcpList_->GetRasterModelSource());
      if (pdestinationrastermodel) {
         pdestinationrastermodel->Transform(world.lr_);
         pdestinationrastermodel->Transform(world.ul_);
         RasterSpatialModel::Destroy(pdestinationrastermodel);
      }

      pTransformWorld->SetSpatialReference(
            parameters_.pGcpList_->GetSpatialReferenceSource());
      pTransformWorld->SetWorld(world);
      pTransformWorld->SetWindow(world);
   } else if (pct) {
      pTransformWorld->SetSpatialReference(parameters_.spatialReference_);
      Subset transfwindow, extent;
      // TODO(javier):  optimizar para leer solo la ventana transformada. Requiere estimar la
      // cantidad de pixels y lineas de ptransfworld
#ifndef __EXTENT_SRS_BUG_FIX__
      pWorldIn->GetWorld(extent);
      pct->Transform(extent);
#endif
      extent = parameters_.imageExtent_;
      pTransformWorld->SetWorld(extent);
      pTransformWorld->SetWindow(extent);

#ifndef __EXTENT_SRS_BUG_FIX__
      pWorldIn->GetWindow(transfwindow);
      pct->Transform(transfwindow);
      pTransformWorld->SetWindow(transfwindow);

      RasterSpatialModel *prastermodel = RasterSpatialModel::Create(
                                                   parameters_.rasterModel_);
      if (prastermodel) {
         prastermodel->InverseTransform(transfwindow.lr_);
         prastermodel->InverseTransform(transfwindow.ul_);
         Dimension dim(transfwindow);
         pTransformWorld->SetViewport(dim.GetWidth(), dim.GetHeight());
      }
#endif
   }
}

/** Obtiene el "bounding box" del elemento renderizado
 * \pre el subset debe ser seteado antes de llamar a esta funcion con un
 *      valor invalido.
 * \post Se refleja el subset de mundo que renderiza.
 * \post No se modifican los parametros en caso de falla o subset nulo
 * @param[in] pWorld Mundo sobre el cual se debe devolver el BB
 * @param[out] Ulx ULx de mundo renderizado.
 * @param[out] Uly ULy de mundo renderizado.
 * @param[out] Lrx LRx de mundo renderizado.
 * @param[out] Lry LRy de mundo renderizado.
 */
void ReprojectionRenderer::GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                                          double &Lrx, double &Lry) {
   // TODO(Gabriel - TCK #2416): Actualizar con la informacion de renderizacion
   // por ahora es una copia textual de RasterRenderer::GetBoundingBox
   if (!pImage_)
      REPORT_DEBUG("D:No hay imagen asociada");

   suri::CoordinatesTransformation* pct = GetTransformation(pWorld);

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
 *  Obtiene los parametros de entrada del renderer
 *  Permite obtener los parametros de entrada que requiere el renderizador,
 *  es decir, los datos que espera de pPreviousRenderer.
 *  En caso de que no tenga algun requerimiento, el parametro tomara los
 *  siguientes valores.
 *  @param[out] SizeX Tamanio en X requerido por el renderizador.
 *  @param[out] SizeY Tamanio en Y requerido por el renderizador.
 *  @param[out] BandCount Bandas requeridas.
 *  @param[out] DataType Tipo de dato.
 */
void ReprojectionRenderer::GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                              std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
}

/**
 * Obtiene los parametros de salida del renderer
 * Permite obtener los parametros de salida del renderizador.
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void ReprojectionRenderer::GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                               std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
}

/** Actualizacion de parametros para representar un elemento
 *  Metodo abstracto de actualizacion del renderizador. Examina el elemento
 * y actualiza sus parametros de manera de reflejar los cambios que hayan
 * sido aplicados.
 * @param[in] pElement Elemento a analizar por cambios.
 */
void ReprojectionRenderer::Update(Element *pElement) {
   Parameters parameters;
   parameters.pGcpList_ = NULL;
   LoadParameters(pElement, &parameters);
   UpdateImage();
   if (IsValidParameters(parameters)) {
      CopyParameters(&parameters, &parameters_);
   }
}

/** Metodo auxiliar que carga los parametros necesarios para la renderizacion
 *  que posee el elemento que se pasa por parametro
 *  @param[in] pElement elemento del cual se toma la informacion para configurar
 *  los parametros
 *  @param[out] pParameters parametros que poseen la configuracion de la renderizacion
 */
void ReprojectionRenderer::LoadParameters(Element* pElement, Parameters* pParameters) const {
   wxXmlNode* prootnode = pElement->GetNode(wxT(""));
   pElement->GetElementExtent(pParameters->imageExtent_);
   if (!prootnode) {
      REPORT_ERROR("D:Nodo NULO.");
      return;
   }
   wxXmlNode *pnode = prootnode->GetChildren();
   while (pnode != NULL) {
      if (pnode->GetName().CompareTo(RENDERIZATION_NODE) == 0) {
         LoadRenderizationParameters(pnode, pParameters);
      } else if (pnode->GetName().CompareTo(URL_NODE) == 0) {
         pParameters->imageUrl_ = pnode->GetNodeContent().c_str();
      } else if (pnode->GetName().CompareTo(GEORREFERENCE_NODE) == 0) {
         LoadGeorreferenceParameters(pnode, pParameters);
      }
      pnode = pnode->GetNext();
   }
}

/** Metodo auxiliars encargado de la carga de parametros bajo el nodo renderizacion
 *  @param[in] pRenderNode nodo de renderizacion
 *  @param[out] pParameters parametros a configurar en base al contenido del nodo render
 */
void ReprojectionRenderer::LoadRenderizationParameters(wxXmlNode* pRenderNode,
                                                       Parameters* pParameters) const {
   wxXmlNode *pnode = pRenderNode->GetChildren();
   std::string bandcombination;
   while (pnode != NULL) {
      if (pnode->GetName().CompareTo(BAND_COMBINATION_NODE) == 0) {
         bandcombination = pnode->GetNodeContent().c_str();
      } else if (pnode->GetName().CompareTo(REPROJECTION_NODE) == 0) {
         wxXmlNode *preprojnode = pnode->GetChildren();
         while (preprojnode != NULL) {
            if (preprojnode->GetName().CompareTo(REPROJECTION_TYPE_NODE) == 0) {
               pParameters->transformationType_ = preprojnode->GetNodeContent().c_str();
            } else if (preprojnode->GetName().CompareTo(
                                             REPROJECTION_TYPE_ALGORITHM_NODE) == 0) {
               pParameters->transformationAlgorithm_ =
                     preprojnode->GetNodeContent().c_str();
            } else if (preprojnode->GetName().CompareTo(
                                             REPROJECTION_TYPE_ORDER_NODE) == 0) {
               pParameters->transformationOrder_ = StringToNumber<int>(
                     preprojnode->GetNodeContent().c_str());
            } else if (preprojnode->GetName().CompareTo(REPROJECTION_TYPE_DELTA_NODE) == 0) {
                  pParameters->delta_ = StringToNumber<int>(
                        preprojnode->GetNodeContent().c_str());
            } else if (preprojnode->GetName().CompareTo(
                                             REPROJECTION_TYPE_COEFFICIENT_NODE) == 0) {
            } else if (preprojnode->GetName().CompareTo(
                                             REPROJECTION_TYPE_GCPLIST_NODE) == 0) {
               LoadGcpListFromXml(preprojnode, pParameters);
            }
            preprojnode = preprojnode->GetNext();
         }
      }
      pnode = pnode->GetNext();
   }
   pParameters->bandCombination_ = split<int>(bandcombination);
}

/** Metodo auxiliar que se encarga de cargar la lista de puntos de control
 *  a partir de la definicion en el nodo XML que se pasa por parametro
 * @param[in] pGcpNode nodo que contiene la definicion de los puntos de control a cargar
 * @param[out] pParameters puntero a la clase Parameters que se quiere configurar
 */
void ReprojectionRenderer::LoadGcpListFromXml(wxXmlNode* pGcpNode,
                                              Parameters* &pParameters) const {
   std::string gcplisturl;
   wxXmlNode* pchild = pGcpNode->GetChildren();
   while (pchild != NULL) {
      if (pchild->GetName().CompareTo(REPROJECTION_TYPE_GCPLIST_URL_NODE) == 0) {
         gcplisturl = pchild->GetNodeContent().c_str();
         break;
      }
      pchild = pchild->GetNext();
   }
   if (pParameters && !gcplisturl.empty()) {
      if (pParameters->pGcpList_ == NULL)
         pParameters->pGcpList_ = new GcpList();
      // pParameters->pGcpList_->Clear();
      TextFileGcpLoader pgcpLoader;
      pgcpLoader.Load(gcplisturl, *pParameters->pGcpList_);
   }
}

/** Metodo auxiliar encargado de parametros de georreferencia **/
void ReprojectionRenderer::LoadGeorreferenceParameters(wxXmlNode* pGeorreferenceNode,
                                                       Parameters* pParameters) const {
   wxXmlNode *pgeonode = pGeorreferenceNode->GetChildren();
   while (pgeonode != NULL) {
      if (pgeonode->GetName().CompareTo(SPATIAL_REFERENCE_NODE) == 0) {
         pParameters->spatialReference_ = pgeonode->GetNodeContent().c_str();
      }
      if (pgeonode->GetName().CompareTo(RASTER_SPATIAL_MODEL_NODE) == 0) {
         pParameters->rasterModel_ = pgeonode->GetNodeContent().c_str();
      }
      pgeonode = pgeonode->GetNext();
   }
}

/** Metodo que determina si la clase Parameters pasada por parametro tiene datos validos
 *  @param ParametersToCheck parametros a chequear
 *  @return true en caso de que los parametros posean informacion valida
 *  @return false en caso contrario
 */
bool ReprojectionRenderer::IsValidParameters(const Parameters& ParametersToCheck) const {
   // TODO(Gabriel - TCK #2416): Actualizar con la informacion de renderizacion
   // por ahora es una copia textual de RasterRenderer::GetOutputParameters
   if (pImage_ == NULL || ParametersToCheck.bandCombination_.empty())
      return false;

   // Valido que existan las bandas a renderizar
   bool validparameters = true;
   int maxbandrequested = *std::max_element(ParametersToCheck.bandCombination_.begin(),
                                            ParametersToCheck.bandCombination_.end());
   if (maxbandrequested >= pImage_->GetBandCount())
      validparameters = false;

   return validparameters;
}

/**
 * Metodo auxiliar que realiza la copia de datos de una instancia Parameters a otra
 * @param[in] pSourceParameters Parameters desde donde se copian los datos
 * @param[out] pDestinationParameters Parameters sobre el cual se deben copiar los datos
 */
void ReprojectionRenderer::CopyParameters(Parameters* pSourceParameters,
                                          Parameters* pDestinationParameters) const {
   pDestinationParameters->bandCombination_ = pSourceParameters->bandCombination_;
   pDestinationParameters->pGcpList_ = pSourceParameters->pGcpList_;
   pDestinationParameters->generateMask_ = pSourceParameters->generateMask_;
   pDestinationParameters->imageUrl_ = pSourceParameters->imageUrl_;
   pDestinationParameters->noDataValue_ = pSourceParameters->noDataValue_;
   pDestinationParameters->rasterModel_ = pSourceParameters->rasterModel_;
   pDestinationParameters->readFunction_ = pSourceParameters->readFunction_;
   pDestinationParameters->spatialReference_ = pSourceParameters->spatialReference_;
   pDestinationParameters->transformationAlgorithm_ =
         pSourceParameters->transformationAlgorithm_;
   pDestinationParameters->transformationOrder_ =
         pSourceParameters->transformationOrder_;
   pDestinationParameters->transformationType_ = pSourceParameters->transformationType_;
}

/**
 * Tranforma el Url del elemento a string que usa Image para abrir la imagen.
 * Este string depende del driver de Gdal que se use.
 * @param[in] ImageId de la imagen que se utiliza como base para generar el id
 * @return string que se le pasa a Image.
 */
std::string ReprojectionRenderer::GenerateImageId(const std::string &ImageId) {
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

/** Metodo auxiliar que dada la url de la imagen en los parametros de renderizacion,
 *  carga la imagen en caso de que no se encuentre cargada */
void ReprojectionRenderer::UpdateImage() {
   if (!pImage_)
      pImage_ = Image::Open(GenerateImageId(parameters_.imageUrl_));
   else
      REPORT_DEBUG("D: RasterRenderer ya tiene una imagen abierta");
}
} /** namespace suri */


