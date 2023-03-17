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

#ifndef SPATIALSUBSETSELECTIONPART_H_
#define SPATIALSUBSETSELECTIONPART_H_

// Includes standard
#include <string>
#include <vector>

// Includes suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/Subset.h"
#include "suri/ObserverInterface.h"
#include "suri/SuriObject.h"
#include "UniversalSubsetInputPart.h"
#include "suri/UseFeatureSelection.h"

namespace suri {

// fordwards
class World;
class DataViewManager;
class LayerTreeModel;
class TreeSelectionManagerInterface;
/** Declara clase ParametricClassificationPartEvent */
DECLARE_EVENT_OBJECT(SpatialSubsetSelectionPartEvent);

namespace ui {
   class HtmlListSelectionWidget;
}


/**
 * Part que permite seleccionar un subset.
 * El subset se puede ingresar directamente o copiar de una capa/visualizador
 * existente.
 */
class SpatialSubsetSelectionPart : public UseFeatureSelection, public Part,
                                   public ProcessAtributeProvider,
                                   public ObserverInterface {
   /** Declara friend la clase de evento ParametricClassificationPartEvent. */
   FRIEND_EVENT_OBJECT(SpatialSubsetSelectionPartEvent);

public:
   /** Ctor */
   SpatialSubsetSelectionPart(DataViewManager *pDataViewManager,
                                                World* pInputWorld);
   /** Dtor */
   virtual ~SpatialSubsetSelectionPart();

   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** retorna la coordenada que ingresaron en srs de trabajo, informa si es valida */
   virtual bool GetSubset(Subset& OutputSubset);

   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification);
   /** Configura el subset en el part a partir del extent de fuente de datos */
   void UpdateSubsetWithDatasource(SuriObject::UuidType DatasouceId);
   /** Configura el subset en el part a partir del extent a traves de una capa */
   void UpdateSubsetWithLayer(SuriObject::UuidType LayerId);

protected:
   // ----------------------------- Eventos -----------------------------
   /** Configura el subset en el part */
   void SetNewSubset(const Subset& NewSubset, const std::string &SubsetSrs);
   /** Evento que se ejecuta al hacer click sobre boton actualizar subset */
   void OnUpdateSubsetButton(wxCommandEvent &Event);
   /** Evento que se ejecuta al presionar el boton -> **/
   void OnRefreshSubsetButtonClick(wxCommandEvent &Event);
   /** Metodo auxiliar que carga la capa de capas **/
   void LoadLayerList();
#ifdef __WXMSW__
   /** evento OnSize */
   void OnSize(wxSizeEvent& Event);
#endif

   /** Parte para seleccion de subset **/
   UniversalSubsetInputPart* pSubsetInputPart_;
   /** Puntero al mundo con subset de la imagen de entrada*/
   World *pInputWorld_;
   /** Administrador con informacion de proyecto */
   DataViewManager *pDataViewManager_;
   /** Clase que administra los nodos seleccion */
   ui::HtmlListSelectionWidget* pTreeSelection_;
   /** Mapa que indica la posicion de cada visualizador en pantalla */
   std::vector<SuriObject::UuidType> viewportListIndex_;

   EVENT_OBJECT_PROPERTY(SpatialSubsetSelectionPartEvent); /*! Objeto que conecta los eventos */
};

} /* namespace suri */
#endif /* SPATIALSUBSETSELECTIONPART_H_ */
