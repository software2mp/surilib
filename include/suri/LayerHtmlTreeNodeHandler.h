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

#ifndef LAYERHTMLTREENODEHANDLER_H_
#define LAYERHTMLTREENODEHANDLER_H_

// Includes standard
#include <set>
#include <map>
#include <string>

// Includes suri
#include "suri/HtmlTreeNodeHandlerInterface.h"
#include "GuiTreeModel.h"
#include "suri/Subject.h"
#include "suri/DatasourceInterface.h"
#include "suri/ViewcontextInterface.h"

namespace suri {
// forwards
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(LayerHtmlTreeNodeHandlerEvent);

class ViewcontextManagerInterface;
class DatasourceManagerInterface;
class GroupManagerInterface;
class LayerInterface;
class DataViewManager;
class RasterLayer;
class VectorLayer;

/**
 * Implementa HtmlTreeNodeHandlerInterface para que los nodos del arbol sean capas.
 */
class LayerHtmlTreeNodeHandler : public HtmlTreeNodeHandlerInterface,
                                                           public Subject {
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(LayerHtmlTreeNodeHandlerEvent);

public:
   enum NodeState {
      ON, OFF, BOTH
   };

   /** Ctor */
   LayerHtmlTreeNodeHandler(DataViewManager* pDataViewManager,
                           GroupManagerInterface* pGroupManager,
                           GuiTreeModel* pGuiTreeModel,
                           long Flags = SimpleHtmlNode);
   /** Dtor */
   virtual ~LayerHtmlTreeNodeHandler();

   /** Retorna el html de un nodo */
   virtual std::string GetHtml(const NodePath &Node, int Width);

   /** Genera menu que se debera mostrar sobre el arbol. */
   virtual void CreateContextMenu(const NodePath &Node,
                              Widget* pWidget, int X, int Y);
   /** Se llama cuando el usuario hace doble click sobre un nodo. */
   virtual void ExecuteDoubleClick(const NodePath &Node);
   /** Se llama cuando el usuario hace doble click sobre un nodo. */
   virtual void ExecuteHtmlLink(const NodePath &Node,
                              const std::string &LinkId);
   /** Reformatea el string del nombre del elemento dependiendo de
    * el tamaño de la ventana de la lista (izquierda).*/
   std::string Reformat( std::string str, unsigned int charsbyLine );

   /** Retorna listado de nodos hoja activos. */
   std::set<SuriObject::UuidType> GetActiveLeafs();
   /** Retorna el estado del nodo hoja. */
   NodeState GetState(const NodePath &Path);

   /** Configura el estado del nodo hoja. */
   void SetLeafState(const NodePath &Path, NodeState NewState);

   /** Configura el estado del nodo hoja. */
   void SetLeafStateStandard(const NodePath &Path, NodeState NewState);

   /** Configura el estado del nodo hoja. */
   void SetLeafStateForSimur3D(const NodePath &Path, NodeState NewState);


   static const long SimpleHtmlNode = 0;
   static const long CheckableHtmlNode = 1;
   static const long ContextMenu = 2;
   static const long IndependentState = 4;
   static const long ShowPreviews = 8;
   static const long CheckableHtmlNodeExclusive = 16;

protected:

  /** 
   *  Metodo que obtiene el html asociado a la informacion de una capa 
   *  que se desea mostrar de forma visual en el arbol de capas  
   *  @param[in] pLayer capa sobre la cual se consultara la informacion a 
   *  mostrar en el arbol de capas
   *  @return string html que representa la informacion de la capa.
   */
   std::string AddSubItemInformation(LayerInterface*& pLayer);
   /** Se llama cuando se selecciona item del menu contextual */
   void OnContextMenu(wxMenuEvent &Event);
   /** Activa el item asociado al path en el viewcontext seleccionado */
   void InvertItemState(const NodePath &Node);
   /** Retorna el estado del grupo en el visualizador activo */
   NodeState GetGroupState(const NodePath &Node);
   /** Retorna el icono del grupo indicado. */
   std::string GetGroupIconSrc(const NodePath &Node);
   /** Retorna el icono del nodo hoja indicado */
   std::string GetLeafIconSrc(const NodePath &Node);
   /** Retorna la capa asociada al nodo. */
   LayerInterface* GetNodeLayer(const NodePath &Node);
   /** Retorna el estado del nodo hoja. */
   NodeState GetLeafState(const NodePath &Path);
   /** administrador de informacion de la aplicacion */
   DataViewManager* pDataViewManager_;
   /** Manager de capas. */
   ViewcontextManagerInterface* pViewcontextManager_;
   /** Manager de datasources. */
   DatasourceManagerInterface* pDatasourceManager_;
   /** Manager de grupos. */
   GroupManagerInterface* pGroupManager_;

   /*! Se usa para configurar html */
   long flags_;

   /** Modelo del arbol grafico. */
   GuiTreeModel* pGuiTreeModel_;

   /** Path ultimo item del que se desplego el menu */
   NodePath lastMenuItemId_;

   /** Conjunto que guarda las hojas activas */
   std::set<SuriObject::UuidType> activeNodes_;

   /** Iconos con los previews de las capas */
   std::map<SuriObject::UuidType, std::string> layerPreviews_;

   /*! Objeto que conecta los eventos */
   EVENT_OBJECT_PROPERTY(LayerHtmlTreeNodeHandlerEvent);

   /** Copia un elemento a otro context */
   void SendToOtherContext(suri::ViewcontextInterface::ViewcontextType Type);
};

} /* namespace suri */
#endif /* LAYERHTMLTREENODEHANDLER_H_ */
