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

#ifndef RENDERIZATIONMANAGER_H_
#define RENDERIZATIONMANAGER_H_

// Includes standard
#include <map>
#include <deque>

// Includes suri
#include "suri/WorldView.h"
#include "suri/LayerList.h"
#include "logmacros.h"
#include "suri/World.h"
#include "suri/ListView.h"
#include "suri/Element.h"
#include "Model.h"

class wxXmlNode;

/** namespace suri */
namespace suri {
class Renderer;
class Canvas;
class Mask;
class RenderPipeline;
class WorldExtentManager;

/** Clase que realiza la renderizacion de los elementos */
/**
 *  Esta clase renderiza los elementos de la lista.
 *
 *  Recibe una lista de elementos y un mundo, registra los eventos de los mismos
 * y reacciona creando y eliminando pipelines de renderizacion o renderizando
 * los elementos en el canvas.
 */
class RenderizationManager {
public:
   /** Constructor */
   /**
    * @param[in] pList Lista de elementos a renderizar.
    * @param[in] pWorldWindow Mundo renderizado.
    */
   RenderizationManager(LayerList *pList, World *pWorldWindow);
   /** Destructor */
   virtual ~RenderizationManager();

   /** Carga el canvas a usar */
   Canvas *SetCanvas(Canvas *pCanvas);

   /** Carga la mascara a usar */
   Mask *SetMask(Mask *pMask);

   /** Metodo de acceso a la lista */
   /**
    *  Obtiene la lista. La lista se registra en la propiedad pListViewer_ y se
    * obtiene de manera similar al mundo.
    * @return Lista renderizada.
    */
   LayerList *GetLayerList() const {
      LayerList *plist = pListViewer_->GetModel<LayerList>();
      if (!plist) {
         REPORT_DEBUG("D:Error al obtener lista.");
      }
      return plist;
   }
   /** Metodo de actualizacion de la lista */
   void ListUpdate(Model *pModel);
   /** Metodo de actualizacion del mundo */
   void WorldUpdate(Model *pModel);
   /** Setea el WorldExtentManager */
   void SetWorldExtentManager(WorldExtentManager *pExtentManager) {
      pWorldExtentManager_ = pExtentManager;
   }
   /** Retorna el WorldExtentManager */
   WorldExtentManager* GetWorldExtentManager() {
      return pWorldExtentManager_;
   }
   /** Metodo que renderiza */
   virtual void Render(bool Force = false);
   /** Indica si hay algun proceso de renderizacion activo */
   static bool GetRendering();
   /** retorna el estado de la renderizacion. */
   /**
    * retorna el valor de la propiedad renderizationStatus_.
    * @return true en caso de que la renderizacion haya tenido exito.
    * @return false en caso de que la renderizacion no haya tenido exito.
    */
   bool GetRenderizationStatus() {
      return renderizationStatus_;
   }

   /** retorna los nombres de los elementos no renderizados. */
   std::vector<Element*> GetNotRenderizedElements() {
      return elementsNotRenderized_;
   }

protected:
   /** tipo de la lista de renderizadores */
   typedef std::map<Element*, RenderPipeline*> RendererListType;
   /** Acceso a los renderizadores */
   RenderPipeline* GetRenderPipeline(Element* pElement);
   /** Modificacion del tamanio del viewport */
   virtual void ViewportChanged();
   /** Modificacion de la ventana */
   virtual void WorldWindowChanged();
   /** Cambio el mundo */
   virtual void WorldChanged();
   /** Se agregaron elementos de la lista */
   virtual void ElementAdded(LayerList::ElementListType &NewElements);
   /** Se eliminaron elementos de la lista */
   virtual void ElementDeleted(Model *pModel);
   /** Se modifico un elemento de la lista */
   virtual void ElementChanged(Model *pModel);
   /** Crea un pipeline para renderizar un elemento */
   virtual bool CreatePipeline(Element *pElement);
   /** Elimina un pipeline asociado a un elemento */
   virtual bool DeletePipeline(Element *pElement);
private:
   double wUlx_; /*! mundo, coord uper-left x */
   double wUly_; /*! mundo, coord uper-left y */
   double wLrx_; /*! mundo, coord lower-right x */
   double wLry_; /*! mundo, coord lower-right y */
   double wwUlx_; /*! ventana, coord uper-left x */
   double wwUly_; /*! ventana, coord uper-left y */
   double wwLrx_; /*! ventana, coord lower-right x */
   double wwLry_; /*! ventana, coord lower-right x */
   int vpWidth_; /*! Ancho de viewport */
   int vpHeight_; /*! Alto de viewport */
   size_t knownItemsCount_; /*! cantidad de items conocidos de la lista */
   RendererListType rendererList_; /*! Lista con los renderizadores */
   ListView<RenderizationManager> *pListViewer_; /*! View de la lista */
   WorldView<RenderizationManager> *pWorldViewer_; /*! View del mundo */
   CallbackView<RenderizationManager> *pElementViewer_; /*! View de elementos */
   bool shouldRender_; /*! flag de renderizacion */
   Canvas *pCanvas_; /*! Canvas de renderizacion */
   Mask *pMask_; /*! Mascara utilizada durante la renderizacion */
   bool isRendering_; /*! flag para evitar reentrar */
   WorldExtentManager* pWorldExtentManager_; /*! Clase que calcula extent */
   /* de mundo al renderizar */
   bool renderizationStatus_; /*! indica si la renderizacion fue o no exitosa */
   std::vector<Element *> elementsNotRenderized_; /*! nombres de elementos que */
   /* no pudieron ser */
   /* renderizados */
   static bool rendering_; /*! flag global para indicar renderizacion */
};
}

#endif /*RENDERIZATIONMANAGER_H_*/
