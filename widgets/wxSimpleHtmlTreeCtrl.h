/*! \file wxSimpleHtmlTreeCtrl.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXSIMPLEHTMLTREECTRL_H_
#define WXSIMPLEHTMLTREECTRL_H_

// Includes standard

// Includes Suri
#include "wxNoModelTreeEventHandler.h"
#include "wxDecoratedHtmlListCtrl.h"
#include "HtmlConfigurationData.h"
#include "TreeInnerNode.h"

// Includes Wx
#include "wx/wx.h"

// Includes App

// Defines
#define wxContextMenu 2
#define wxCheckableItems 4
#define wxNoAutoState 8

namespace suri {
/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(wxSimpleHtmlTreeCtrlEvent);

/**
 * Presenta API de arbol y configura el html de los items en
 * wxDecoratedHtmlListCtrl para que la lista parezca un arbol.
 */
class wxSimpleHtmlTreeCtrl : public wxDecoratedHtmlListCtrl {
public:
   //! Declara el objeto de eventos como clase friend
   FRIEND_EVENT_OBJECT(wxSimpleHtmlTreeCtrlEvent);

   /**
    * TODO(Javier - Sin TCK): Agregar metodo a interfaz de wxSimpleHtmlTreeCtrl que permita
    * mover grupos enteros. Luego corregir wxNoModelTreeEventHandler para que use este metodo
    * y no tenga que acceder a root_
    */
   friend class wxNoModelTreeEventHandler;

   //! Ctor
   wxSimpleHtmlTreeCtrl(wxWindow* pParent, long Style = 0, long BehaviorFlags =
         wxNoBehaviour);
   //! Dtor
   ~wxSimpleHtmlTreeCtrl();

   //! Retorna raiz del arbol
   ItemId GetRoot();
   //! Busca el nodo con id 'NodeId' y retorna el id del primer hijo.
   ItemId GetFirstChild(const ItemId &NodeId);
   //! Busca el nodo con id 'NodeId' y retorna el siguiente hijo del mismo padre.
   ItemId GetNextSibling(const ItemId &NodeId);
   //! Busca el nodo con id 'NodeId' y retorna el padre.
   ItemId GetParentNode(const ItemId &NodeId);
   //! Busca el nodo con id 'NodeId' y retorna su HtmlConfigurationData.
   HtmlConfigurationData* GetConfigurationData(const ItemId &NodeId);
   //! Busca el nodo con id 'NodeId' y agrega 'pNode' a continuacion.
   ItemId AddNodeAfter(const ItemId &NodeId, HtmlConfigurationData* pHtmlNode);
   //! Busca el nodo con id 'NodeId' y agrega 'Node' antes del nodo encontrado.
   ItemId AddNodeBefore(const ItemId &NodeId, HtmlConfigurationData* pHtmlNode);
   //! Busca el nodo con id 'NodeId'. Agrega 'pNode' como su ultimo hijo.
   ItemId AppendNode(const ItemId &NodeId, HtmlConfigurationData* pHtmlNode);
   //! Busca el nodo con id 'NodeId' y lo saca del arbol.
   HtmlConfigurationData* RemoveNode(const ItemId &NodeId);
   //! Busca el nodo con id 'NodeId', si es nodo interno lo expande/contrae.
   void Expand(const ItemId &NodeId, bool Expand);
   //! Busca el nodo con id 'NodeId', informa si esta expandido
   bool IsExpanded(const ItemId &NodeId);
   //! Informa si el nodo es visible en la lista
   bool IsVisible(const ItemId &NodeId);

   //! Valida si un ItemId sigue siendo valido en la estructura del arbol.
   bool ValidateItemId(const ItemId &NodeId);
   //! Clase que se usa para definir comportamiento ante eventos de usuario
   void SetTreeEventHandler(wxHtmlTreeEventHandler* pTreeEventHandler);
   //! Incrementa en 1 el contador que bloquea la actualizacion del arbol
   void BeginBatch();
   //! Decrementa en 1 el contador que bloquea la actualizacion del arbol
   void EndBatch();

protected:

   //! Crea TreeNode configurado con pHtmlNode
   TreeNode* CreateTreeNode(HtmlConfigurationData* pHtmlNode);
   //! Actualiza wxDecoratedHtmlListCtrl con el nuevo contenido del arbol.
   void UpdateDecoratedHtmlList();
   //! Se ejecuta cuando se invierte estado de check desde la interfaz
   void InvertItemCheckFromGui(ItemId Id);

   // ---- Redefinen comportamiento de wxDecoratedHtmlListCtrl para que
   // se comporte como un arbol ----

   //! Se ejecuta cuando se mueve un item despues de otro
   bool DoOnMoveAfter(const ItemId &Id, const ItemId &DestinationId);
   //! Se ejecuta cuando se mueve un item antes que otro
   bool DoOnMoveBefore(const ItemId &Id, const ItemId &DestinationId);
   //! Se ejecuta cuando se mueve un item despues del ultimo elemento
   bool DoOnMoveEnd(const ItemId &Id);
   //! Se ejecuta cuando se modifica la seleccion
   void DoOnSelectionChanged();

private:
   // ---- Eventos de wx ----
   //! Se ejecuta cuando se hace click sobre link de un nodo
   void OnLink(wxHtmlLinkEvent &Event);
   //! Se ejecuta con evento de click izq.(sin soltar el boton).
   void OnLeftDown(wxMouseEvent &Event);
   //! Se ejecuta cuando se hace doble click sobre item en lista.
   void OnDoubleClick(wxMouseEvent &Event);
   //! Lanzado cuando cambia el tamanio de la ventana
   void OnResize(wxSizeEvent &Event);
   //! Se ejecuta cuando se suelta click derecho sobre la lista.
   void OnRightUp(wxMouseEvent &Event);
   //! Se ejecuta cuando se selecciona item en menu sobre nodo
   void OnContextMenu(wxMenuEvent &Event);
   //! Evento para dibujar la posicion del elemento en el Drag&Drop
   void OnUIUpdate(wxUpdateUIEvent &Event);

   // Atributos internos de la clase
   TreeInnerNode* root_; /*! Nombre del nodo */
   wxHtmlTreeEventHandler* pTreeEventHandler_; /*! Define comportamiento */
   /*! de arbol ante los comandos del usuario */
   int batchCount_; /*! int que bloquea la actualizacion de la iterfaz */
   /*! grafica con el contenido del arbol cuando >0 */
   bool dirty_; /*! Flag que le indica a OnUIUpdate que debe llamar */
   /*! a UpdateDecoratedHtmlList */
   bool validDoubleClick_; /*! bool que se usa para evitar que se ejecute */
   /*! evento doble click cuando se hizo click sobre link */
   EVENT_OBJECT_PROPERTY(wxSimpleHtmlTreeCtrlEvent);
};
}  // namespace

#endif /* WXSIMPLEHTMLTREECTRL_H_ */
