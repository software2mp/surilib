/*! \file RenderizationManager.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes estandar
#include <vector>

// Includes suri
#include "RenderizationManager.h"
#include "suri/Element.h"
#include "suri/World.h"
#include "suri/Renderer.h"
#include "suri/ListView.h"
#include "suri/WorldExtentManager.h"
#include "RenderPipeline.h"
#include "suri/Canvas.h"
#include "MemoryCanvas.h"
#include "Mask.h"

// Defines

/** namespace suri */
namespace suri {
/*! Definicion del flag estatico para control de renderizacion global */
bool RenderizationManager::rendering_ = false;

/**
 * Constructor
 * @param[in] pList lista que se observa por cambios.
 * @param[in] pWorldWindow mundo que se observa por cambios.
 * \todo considerar que la lista puede traer elementos y que algunos esten activos
 * @return instancia de la clase RenderizationManager
 */
RenderizationManager::RenderizationManager(LayerList *pList, World *pWorldWindow) :
      wUlx_(0), wUly_(0), wLrx_(0), wLry_(0), wwUlx_(0), wwUly_(0), wwLrx_(0),
      wwLry_(0), vpWidth_(0), vpHeight_(0), knownItemsCount_(0), shouldRender_(false),
      pCanvas_(NULL), pMask_(NULL), isRendering_(false), pWorldExtentManager_(NULL),
      renderizationStatus_(true) {
   // Creo y registro los viewers
   pListViewer_ = new ListView<RenderizationManager>(this,
                                                     &RenderizationManager::ListUpdate);
   pList->RegisterViewer(pListViewer_);
   pWorldViewer_ = new WorldView<RenderizationManager>(
         this, &RenderizationManager::WorldUpdate);
   pWorldWindow->RegisterViewer(pWorldViewer_);
   pElementViewer_ = new CallbackView<RenderizationManager>(
         this, &RenderizationManager::ElementChanged,
         &RenderizationManager::ElementDeleted);
}

/**
 *  Se desregistra de la lista y elimina los renderizadores.
 * \todo Hay crear/utilizar una rutina para eliminar los renderizadores en la
 *  factoria (asi puede existir en un .dll)
 */
RenderizationManager::~RenderizationManager() {
   while (!rendererList_.empty())
      DeletePipeline(rendererList_.begin()->first);

   // Desregistro el ElementViewer de todos los elementos antes de eliminarlo
   suri::LayerList *playerlist = pListViewer_->GetList();
   if (Model::IsValid(playerlist)) {
      suri::LayerList::ElementListType::iterator it = playerlist->GetElementIterator();
      for (; it != playerlist->GetElementIterator(false); ++it) {
         suri::Element *pelement = *it;
         pelement->UnregisterViewer(pElementViewer_);
      }
   }
   delete pElementViewer_;
   // Elimino la listviewer. Se desregistra automaticamente
   delete pListViewer_;

   delete pWorldViewer_;
}

/**
 *  Carga el canvas en el renderizador.
 *
 *  En caso de querer eliminar el canvas y no reemplazarlo por nada hacer:
 * @code
 *  delete SetCanvas(NULL);
 * @endcode
 *
 * @param[in] pCanvas Nuevo canvas a utilizar
 * @return el canvas anterior o NULL (para que sea eliminado desde afuera
 * \attention el Canvas anterior retornado(y su eliminacion) es responsabilidad del invocante
 */
Canvas *RenderizationManager::SetCanvas(Canvas *pCanvas) {
   Canvas *ptemp = pCanvas_;
   pCanvas_ = pCanvas;
   return ptemp;
}

/** Carga la mascara a usar */
/**
 *  Permite setear mascara que se utilizara para la renderizacion.
 *
 * \post Se reemplaza la mascara previa, el que llama es responsable de eliminarla
 * @param[in] pMask Mascara nueva
 * @return Mascara anterior (para que el cliente la elimine)
 */
Mask *RenderizationManager::SetMask(Mask *pMask) {
   Mask *ptemp = pMask_;
   pMask_ = pMask;
   return ptemp;
}

/**
 *  Retorna el renderizador asociado al elemento.
 * @param[in] pElement el elemento
 * @return El pipeline que esta renderizando el elemento
 * \attention el RenderPipeline* retornado es responsabilidad de esta clase(no eliminar externamente)
 */
RenderPipeline* RenderizationManager::GetRenderPipeline(Element* pElement) {
   RendererListType::const_iterator renderit = rendererList_.find(pElement);
   // el elemento no tiene renderizador asociado
   if (renderit == rendererList_.end()) {
      // Creo el pipeline
      if (!CreatePipeline(pElement)) {
         REPORT_AND_FAIL_VALUE("D:No se pudo crear el renderizador para el elemento",
                               NULL);
      } else {
         renderit = rendererList_.find(pElement);
         // si dio verdadero CreatePipeline no deberia entrar por aca, pero para asegurarlo...
         if (renderit == rendererList_.end()) {
            REPORT_AND_FAIL_VALUE("D:No se pudo crear el renderizador para el elemento",
                                  NULL);
         }
      }
   }
   return renderit->second;
}

/**
 *  Metodo Update que se ejecuta cuando pList es modificada. Es el callback
 * que se registra en pListViewer_.
 * @param[in] pModel lista que se actualizo
 */
void RenderizationManager::ListUpdate(Model *pModel) {
   LayerList *plist = GetLayerList();
   if (plist != pModel) {
      return;
   }
   // si se agregaron elementos, me registro
   if (pListViewer_->ElementAdded()) {
      LayerList::ElementListType newelements = pListViewer_->GetNewElements();
      ElementAdded(newelements);
   }
   // si se movieron elementos (cambio de orden)
   if (pListViewer_->ElementMoved()) {
      Render(true);
   }
}

/**
 *  Metodo Update que se ejecuta cuando pWorldWindow es modificado. Es el
 * callback que se registra en pWorldViewer_.
 * @param[in] pModel mundo que se actualizo
 */
void RenderizationManager::WorldUpdate(Model *pModel) {
   World *pworld = pWorldViewer_->GetWorld();
   if (!pworld) {
      return;
   }
   if (pWorldViewer_->WorldChanged()) {
      WorldChanged();
   }
   if (pWorldViewer_->WindowChanged()) {
      WorldWindowChanged();
   }
   if (pWorldViewer_->ViewportChanged()) {
      ViewportChanged();
   }

   // Se agrega el render aca para que renderice siempre aunque no cambie nada
   // Con esto se soluciona el bug de renderizacion de las esquinas
   Render(true);
}

/** Metodo que renderiza */
/**
 * Utiliza el WorldExtentManager para calcular el mundo
 * Para cada elemento activo en el LayerList obtiene pipeline y llama al
 * render de la misma.
 * Finalmente ordena al canvas que se dibuje
 * @param[in]	Force fuerza una renderizacion.
 *
 * \todo (danieltesta-#4287): Revisar el uso del elemento asociado (GetAssociatedElement)
 */
void RenderizationManager::Render(bool Force) {
   // Impido renderizacion
   if (!Force && !shouldRender_) {
      return;
   }
   // Si ya esta en el ciclo, salgo
   if (isRendering_) {
      return;
   }
   // protejo el ciclo de renderizacion
   isRendering_ = true;
   rendering_ = true;
   shouldRender_ = false;
   REPORT_DEBUG("D:Renderizando.");
   if (!pCanvas_) {
      isRendering_ = false;
      rendering_ = false;
      REPORT_AND_FAIL("D:No se ha asignado un Canvas de salida.");
   }
   LayerList *plist = GetLayerList();
   if (!Model::IsValid(plist)) {
      isRendering_ = false;
      rendering_ = false;
      return;
   }

   // reinicio lista de elementos no renderizados y estado de la renderizacion
   renderizationStatus_ = false;
   elementsNotRenderized_.clear();
   std::vector<Element*> aux = plist->GetRenderizationOrderList();
   std::vector<Element*> elements;
   std::vector<Element*>::const_iterator it = aux.begin();
   for (; it != aux.end(); ++it) {
      Element* pe = NULL;
      if ((*it)->HasAssociatedElement())
         pe = (*it)->GetAssociatedElement();
      else
         pe = *it;
      elements.push_back(pe);
   }
   World *pworld = pWorldViewer_->GetWorld();
   if (!pworld) {
      isRendering_ = false;
      rendering_ = false;
      REPORT_AND_RETURN("D:Error al obtener el mundo.");
   }
   // Usa WorldExtentManager para calcular extent del mundo
   if (pWorldExtentManager_) {
      pWorldExtentManager_->SetExtent();
   }

   if (!pworld->IsInitialized()) {
      isRendering_ = false;
      rendering_ = false;
      REPORT_AND_RETURN("D:Mundo no inicializado o error en el mundo");
   }
   // aplico el tamano antes del clear por si se necesita dentro del canvas
   int vpx, vpy;
   pworld->GetViewport(vpx, vpy);
   pCanvas_->SetSize(vpx, vpy);
   pCanvas_->Clear();

   Mask *pmask = new Mask;
   pmask->SetSize(vpx, vpy);
   pmask->Clear();
   Canvas *prenderizationcanvas = new MemoryCanvas;
   prenderizationcanvas->InitializeAs(pCanvas_);
   std::vector<Element*>::reverse_iterator rit = elements.rbegin();
   for (; rit != elements.rend(); rit++) {
      Element *pelement = *rit;
      bool active = pelement->IsActive();
      if (active) {
         REPORT_DEBUG("D:Renderizando elemento \"%s\".", pelement->GetName().c_str());
         RenderPipeline *ppipeline = GetRenderPipeline(pelement);

         if (ppipeline) {
            bool status = true;
            PROFILE_CALL(status = ppipeline->Render(pworld, prenderizationcanvas, pmask));
            if (!status) {
               elementsNotRenderized_.push_back(pelement);
               REPORT_DEBUG("D: el elemento %s no pudo ser renderizado. Url: %s",
                            pelement->GetName().c_str(), pelement->GetUrl().c_str());
            }

            // indica que finalizo la renderizacion del elemento
            pelement->SetShouldRender(false);
         } else {
            elementsNotRenderized_.push_back(pelement);
            REPORT_DEBUG("D:No se pudo obtener RenderPipeline para elemento: %s.",
                         pelement->GetName().c_str());
         }
      }
   }
   pmask->Clear();
   delete pmask;
   renderizationStatus_ = (elementsNotRenderized_.empty());

   // debo tomar la cantidad de bandas luego de la renderizacion
   // \todo Javier #2232: Se deben definir estos datos desde afuera
   pCanvas_->InitializeAs(prenderizationcanvas);
   pCanvas_->Clear();
   int x, y;
   pCanvas_->GetSize(x, y);
   if (pMask_ && pMask_->GetSizeX() == x && pMask_->GetSizeY() == y) {
      REPORT_DEBUG("D:Enmascarando");
      pMask_->ApplyMask(prenderizationcanvas, pCanvas_);    // realiza alpha blending
      REPORT_DEBUG("D:Fin Enmascarando");
   } else {
      // No realiza alpha blending
      pCanvas_->Write(prenderizationcanvas, pMask_);
   }
   pCanvas_->Flush(pMask_);
   delete prenderizationcanvas;
   // habilito el ciclo de renderizacion
   isRendering_ = false;
   rendering_ = false;
}

/*!
 *  Retorna el estado global de la renderizacion. Como hay ejecucion de eventos
 * dentro del bucle de renderizado, puede llegar a producirce reentrada a la
 * renderizacion o renderizaciones paralelas. Esto resulta en SIGSEGV desde
 * Viewer2D.
 * @return True Si hay algun RenderizacionManager ejecutando ::Render().
 */
bool RenderizationManager::GetRendering() {
   return rendering_;
}

/**
 *  Actualizacion del tamano del Viewport.
 */
void RenderizationManager::ViewportChanged() {
   REPORT_DEBUG("D:Viewport Changed.");
   // Render(true); se pasa al WorldUpdate
}

/**
 *  Actualizacion del tamano/posicion de la ventana de mundo.
 */
void RenderizationManager::WorldWindowChanged() {
   REPORT_DEBUG("D:WorldWindow Changed.");
   // Render(true);se pasa al WorldUpdate
}

/**
 *  Actualizacion de la extension del mundo.
 */
void RenderizationManager::WorldChanged() {
   REPORT_DEBUG("D:WorldChanged.");
#ifdef __DEBUG__
// Codigo arcaico que genera un archivo de mascara
#  ifdef __UNUSED_CODE__
   REPORT_DEBUG("D:Creando mascara.");
   Subset worldcoord;
   GetWorldWindow()->GetWorld(worldcoord);

   // creo una mascara con un cuadrado en el medio
   Image *pmask = Image::Open("mask.dat", Image::WriteOnly, "envi", 1,
         (int)(wLrx_-wUlx_), (int)(wLry_-wUlx_), "uchar");
   int col, row;
   pmask->GetSize(col, row);
   if ( col <= 0 || row <= 0 ) {
      return;
   }
   unsigned char *pmaskbuff = new unsigned char[col * row];
   for ( int j = 0; j < row; j++ )
   for ( int i = 0; i < col; i++ ) {
      pmaskbuff[i+j*col]=0xFF;
      if ((j > row/4 && j < 3*row/4 && (i > col/4 && i < 3*col/4))) {
         pmaskbuff[i+j*col] = 0x00;
      }
   }
   std::vector<int> idx;
   std::vector<void*> data;
   idx.push_back(0);
   data.push_back(pmaskbuff);
   pmask->Write(idx, data, 0, 0, col, row);
   delete[] pmaskbuff;
   Image::Close(pmask);
#  endif    // __UNUSED_CODE__
#endif   // __DEBUG__
}

/**
 *  Se agregaron elementos de la lista
 *  @param[in] NewElements lista con los elementos que se agregaron
 */
void RenderizationManager::ElementAdded(LayerList::ElementListType &NewElements) {
   LayerList *plist = GetLayerList();
   if (!plist) {
      return;
   }
   // Agrego renderizadores para los elementos nuevos
   LayerList::ElementListType::iterator it = NewElements.begin();
   for (; it != NewElements.end(); ++it) {
      // Creo el pipeline
      if (!CreatePipeline(*it)) {
         REPORT_DEBUG("D:No se creo renderizador para %s.", (*it)->GetName().c_str());
      }
   }
}

/**
 * Se eliminaron elementos de la lista
 * @param[in] pModel modelo del elemento eliminado
 */
void RenderizationManager::ElementDeleted(Model *pModel) {
   // Elimino el pipeline del elemento cerrado
   DeletePipeline(static_cast<Element*>(pModel));
   // des-registro el elemento
   if (Model::IsValid(pModel)) {
      pModel->UnregisterViewer(pElementViewer_);
   }
   // Renderizo
   Render(true);
}

/**
 * Se modifico un elemento de la lista
 * @param[in] pModel modelo del elemento modificado
 *
 * \todo (danieltesta-#4287): Revisar el uso del elemento asociado (GetAssociatedElement)
 */
void RenderizationManager::ElementChanged(Model *pModel) {
   REPORT_DEBUG("D:Elemento modificado.");
   LayerList *plist = GetLayerList();
   if (!plist) {
      return;
   }
   LayerList::ElementListType::iterator it = plist->GetElementIterator();
   for (; it != plist->GetElementIterator(false); ++it) {
      if ((*it)->HasChanged()) {
         Element* pe = ((*it)->HasAssociatedElement()) ? (*it)->GetAssociatedElement() : *it;
         shouldRender_ = true;
         RenderPipeline *ppipeline = GetRenderPipeline(pe);
         if (ppipeline) {
            REPORT_DEBUG("D:Actualizando pipeline.");
            ppipeline->Update(pe);
         }
      }
   }

   if (shouldRender_) {
      Render();
   }
}

/**
 *  Crea y agrega el pipeline de renderizacion del elemento a la lista de
 * pipelines (cada pipeline representa un elemento de la lista).
 * @param[in] pElement Elemento para el cual hay que crear el pipeline de
 *            renderizadores
 * @return True si se pudo crear el pipeline con exito.
 */
bool RenderizationManager::CreatePipeline(Element *pElement) {
   RenderPipeline *ppipeline = RenderPipeline::Create(pElement, pCanvas_);
   if (!ppipeline) {
      return false;
   }
   // registro el elemento
   pElement->RegisterViewer(pElementViewer_);
   rendererList_.insert(std::make_pair(pElement, ppipeline));
   return true;
}

/** Elimina el pipeline asociado al elemento */
/**
 *  Elimina el pipeline que renderiza el elemento.
 * @param[in] pElement Punero al elemento.
 * @return true si pudo eliminar con exito
 * \warning Este metodo puede ser llamado desde el destructor del pElement
 *        como Model, entonces cualquier metodo o propiedad del mismo pueden
 *        no existir. NO acceder al mismo.
 */
bool RenderizationManager::DeletePipeline(Element *pElement) {
   RendererListType::iterator renderit = rendererList_.find(pElement);
   if (renderit == rendererList_.end()) {
      REPORT_AND_RETURN_VALUE("D:No se elimino ningun elemento.", false);
   }REPORT_DEBUG("D:Eliminando pipeline");
   // si existe en la lista quiere decir que me puedo desregistrar
   Element *pelement = GetLayerList() ? GetLayerList()->GetElement(pElement) : NULL;
   if (Model::IsValid(pelement)) {
      pelement->UnregisterViewer(pElementViewer_);
   }
   RenderPipeline::Destroy(renderit->second);
   rendererList_.erase(renderit);
   return true;
}
}
