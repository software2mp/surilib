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

#ifndef HTMLTREEWIDGET_H_
#define HTMLTREEWIDGET_H_

// Includes standard
#include <string>
#include <vector>

// Includes suri
#include "suri/Widget.h"
#include "suri/ObserverInterface.h"
#include "DecoratedHtmlItem.h"
#include "WorkGroupInterface.h"
#include "suri/Subject.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/htmllbox.h"

namespace suri {

// fordwards
class wxDecoratedHtmlListCtrl;
class GuiTreeModel;
class HtmlTreeNodeHandlerInterface;

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(HtmlTreeWidgetEvent);

/**
 * Adapta wxDecoratedHtmlListCtrl para que se comporte como arbol
 */
class HtmlTreeWidget : public suri::Widget, public ObserverInterface {
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(HtmlTreeWidgetEvent);

public:
   /** Ctor */
   HtmlTreeWidget(wxWindow *pParent, GuiTreeModel* pGuiTreeModel,
                                 HtmlTreeNodeHandlerInterface* pHtmlTreeNodes,
                                 Subject* pChangesNotifier,
                                 long ConfigurationFlags = DefaultBehaviour);
   /** Dtor */
   virtual ~HtmlTreeWidget();

   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Actualiza control grafico con datos en modelo */
   void UpgradeControlContent();
   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification);
   /** Fuerza la actualizacion del widget **/
   void ForceRefresh();

   /** Flags que definen comportamiento del arbol */
   static const long DefaultBehaviour = 0;
   static const long DraggableItems = 1;
   static const long MultipleSelection = 2;

   /** Setea el minimo de size para los items mostrados en la lista */
   void SetMinItemSize(int Min);
   /** Setea el maximo de size para los items mostrados en la lista */
   void SetMaxItemSize(int Max);

   /** Devuelve el mayor entre MinItemSize y MaxItemSize */
   int GetManualWindowsSize();
   /** Devuelve el size promedio de la relacion pixeles-ventana */
   int GetFitToWindowsSize();

   /** */
   void SetViewcontextId(const SuriObject::UuidType& ViewcontextId);

   /** */
   SuriObject::UuidType GetViewcontextId() { return viewcontextId_; }

protected:
   /** Retorna el html de un nodo segun sea su tipo */
   std::string GetNodeHtml(const NodePath &Path, int WindowWidth);
   /** Le agrega identacion a un nodo para que parezca dentro de un arbol. */
   std::string AddIdentantion(const NodePath &Node, const std::string &ItemHtml);
   /** Calcula el ancho que ocupa la identacion */
   int CalculateNodeAvailableWith(const NodePath &Node, int WindowWidth);
   /** Actualiza vector html con los elementos visibles */
   void UpdateHtmlVectorWithGroupNodes(int WindowWidth,
                        std::vector<DecoratedHtmlItem>& HtmlVector);
private:
   // ---- Eventos de wx ----
   /** Se ejecuta cuando se hace click sobre link de un nodo */
   void OnLink(wxHtmlLinkEvent &Event);
   /** Se ejecuta con evento de click izq.(sin soltar el boton). */
   void OnLeftDown(wxMouseEvent &Event);
   /** Se ejecuta cuando se hace doble click sobre item en lista. */
   void OnDoubleClick(wxMouseEvent &Event);
   /** Lanzado cuando cambia el tamanio de la ventana */
   void OnResize(wxSizeEvent &Event);
   /** Se ejecuta cuando se suelta click derecho sobre la lista. */
   void OnRightUp(wxMouseEvent &Event);
   /** Evento para dibujar la posicion del elemento en el Drag&Drop */
   void OnUiUpdate(wxUpdateUIEvent &Event);

   /** Retorna el path al nodo asociado al elemento en 'Position' de la lista */
   bool GetListItemNodePath(int Position, NodePath &Path);



   /** Control grafico con arbol en pantalla */
   wxDecoratedHtmlListCtrl* pWxTree_;

   /** Modelo de grupos que represento por pantalla */
   GuiTreeModel* pGuiTreeModel_;
   /** Clase que meneja los eventos y la renderizacion de los nodos html. */
   HtmlTreeNodeHandlerInterface* pHtmlTreeNodeHandler_;
   /** Notifica cambios en el arbol */
   Subject* pChangesNotifier_;

   /** Id de observer en pGroupManager_ */
   int observerId_;
   /** Evitar que se ejecute evento doble click cuando se hizo click sobre link */
   bool validDoubleClick_;

   /** Flag configurado por el usuario */
   long configurationFlags_;

   /** Indica que se debe recalcular el html del arbol */
   bool reloadHtml_;

   /**  Size minimo */
   int minSize_;
   /** Size maximo */
   int maxSize_;

   /** evento doble click cuando se hizo click sobre link */
   EVENT_OBJECT_PROPERTY(HtmlTreeWidgetEvent);
   /** Id del viewcontext al que pertenece el htmltreewidget **/
   SuriObject::UuidType viewcontextId_;
};

} /* namespace suri */
#endif /* HTMLTREEWIDGET_H_ */
