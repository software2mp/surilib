/*! \file LayerTreeWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LAYERTREEWIDGET_H_
#define LAYERTREEWIDGET_H_

// Includes standard
#include <map>
#include <string>

// Includes Suri
#include "suri/Widget.h"
#include "View.h"
#include "ItemId.h"
#include "wxSimpleHtmlTreeCtrl.h"
// Includes Wx

// Includes App

// Defines

namespace suri {

// Forwards Suri
class wxSimpleHtmlTreeCtrl;
class LayerList;
class Element;
class ElementHtmlConfigurationData;
class LayerTreeEventHandler;
class DataViewManager;
class ViewcontextInterface;

/**
 * Clase que usa wxSimpleHtmlTreeCtrl para mostrar el contenido de LayerList
 * por pantalla
 */
class LayerTreeWidget : public Widget, View {

public:
   /** Ctor */
   LayerTreeWidget(wxWindow *pParent, DataViewManager *pDataViewManager,
                   ViewcontextInterface *pViewcontext);
   /** Dtor */
   ~LayerTreeWidget();

   /** Crea la ventana de herramienta */
   virtual bool CreateToolWindow();

   // --- FUNCIONES TEMPORALES MIENTRAS EL MODELO NO SOPORTE GRUPOS ---

   /** Crea un grupo con los elementos seleccionados */
   bool CreateGroupWithSelectedElements();
   /** Si hay uno o mas grupos seleccionados se eliminan. */
   bool DeleteSelectedGroups();

   // ---------- FUNCIONES INTERNAS DE LA BIBLIOTECA. ----------

   /** Obtiene el elemento a partir del id del nodo */
   Element* GetElement(const ItemId &Id);
   /** Retorna el control wx que despliega el arbol */
   wxSimpleHtmlTreeCtrl* GetWxTreeCtrl();
   /** Busca la siguiente hoja en el arbol */
   ItemId GetNextTreeLeaf(const ItemId &CurrentLeaf);

   /** Obtiene la layerlist \deprecated*/
   LayerList *GetLayerList();

protected:
   /** Recibe el update */
   virtual void Update(Model *pModel);
   /** Recibe el aviso de delete */
   virtual void OnModelDelete(Model *pModel);

private:
   /** Sincroniza la interfaz grafica con la LayerList */
   void Update();
   /** Agrega elemento al arbol despues de la hoja que recibe por parametro */
   ItemId AddNodeAfterLeaf(const ItemId &LastLeaf, Element* pElement);

   /** Obtiene del usuario el nombre del nuevo grupo */
   bool GetNewGroupName(std::string &NewGroupName);
   /*! Puntero a control grafico que  grafica el arbol.*/
   wxSimpleHtmlTreeCtrl* pHtmlTreeCtrl_;
   /*! Viewcontext que posee la lista con la que se configura el arbol */
   ViewcontextInterface *pViewcontext_;
   /** Administrador que posee las fuentes de datos*/
   DataViewManager *pDataViewManager_;
   /*! Mapa que permite configurar html de un elemento dado.
    *  Se creo para actualizar elementos en Update de lista, no usar
       fuera de este metodo */
   std::map<Element*, ElementHtmlConfigurationData*> elementsConfigData_;
   /*! Handler que responde a eventos del arbol */
   LayerTreeEventHandler* pEventHandler_;
};

}  //namespace

#endif /* LAYERTREEWIDGET_H_ */
