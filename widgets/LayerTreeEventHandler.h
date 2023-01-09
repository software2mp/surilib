/*! \file LayerTreeEventHandler.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LAYERTREEEVENTHANDLER_H_
#define LAYERTREEEVENTHANDLER_H_

// Includes standard
#include <map>

// Includes Suri
#include "suri/LayerTreeWidget.h"
#include "ItemId.h"
#include "suri/DatasourceInterface.h"

// Includes Wx

// Includes App

// Defines

// Forwards

namespace suri {

// Forwards Suri
class LayerList;
class LayerTreeWidget;
class Element;
class DataViewManager;
class ViewcontextInterface;

/**
 *
 */
class LayerTreeEventHandler : public wxHtmlTreeEventHandler {
public:
   /** Ctor */
   LayerTreeEventHandler(DataViewManager *pDataViewManager,
                         ViewcontextInterface *pViewcontext);
   /** Dtor */
   ~LayerTreeEventHandler();

   /** Configura el widget que despliega la LayerList */
   void SetTreeWidget(LayerTreeWidget* pTreeWidget);
   /** Retorna el wxSimpleHtmlTreeCtrl donde se despliega la LayerList */
   wxSimpleHtmlTreeCtrl* GetHtmlTreeCtrl();

   /** Se ejecuta cuando se modifican los elementos seleccionados */
   virtual void DoOnSelectionChanged();
   /** Modifica el estado del nodo y el de sus hijos. */
   virtual void DoOnStateChanged(const ItemId &Id,
                                 HtmlConfigurationData::State NewState);

   /** Valida si se puede mover nodo Id debajo de ParentId */
   virtual bool ValidateNodeNewParent(const ItemId &Id, const ItemId &ParentId);
   /** Mueve un nodo despues de otro. */
   virtual void DoOnMoveNodeAfter(const ItemId &Id, const ItemId &DestinationId);
   /** Mueve un nodo antes de otro. */
   virtual void DoOnMoveNodeBefore(const ItemId &Id, const ItemId &DestinationId);
   /** Mueve un nodo debajo de otro. */
   virtual void DoOnMoveNodeUnder(const ItemId &Id, const ItemId &DestinationId);
   /** Genera menu que se debera mostrar sobre la lista */
   virtual wxMenu* CreateContextMenu(const ItemId &Id);
   /** Se llama cuando se selecciona item del menu contextual */
   virtual void DoOnContextMenu(wxMenuEvent &Event);

private:
   /** Mueve un grupo de elementos antes o despues de un nodo */
   bool MoveGroupContent(const ItemId &GroupId, const ItemId &NewGroupId);
   /** */
   DatasourceInterface* GetAssociatedDatasource(Element *pElement);
   /** */
   void ReplaceELementTextureNode(Element *pElementTo, Element *pElementFrom);
   /** Muestra el nombre de los elementos en la layer list ordenados */
   void ShowLayerListContent();
   /*! ItemId del ultimo menu desplegado */
   ItemId lastMenuItemId_;
   /** Administrador que posee las fuentes de datos*/
   DataViewManager *pDataViewManager_;
   /** Viewcontext activo que posee la lista de elementos que se configura en el arbol*/
   ViewcontextInterface *pViewcontext_;
   /*! Control grafico del arbol */
   LayerTreeWidget* pTreeWidget_;
};

}   // namespace suri

#endif /* LAYERTREEEVENTHANDLER_H_ */
