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
