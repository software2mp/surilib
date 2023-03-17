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

#ifndef WXSLAYERIMPORTER_H_
#define WXSLAYERIMPORTER_H_

// Includes Estandar
#include <map>
#include <vector>

// Includes Suri
#include "suri/DialogWidget.h"
#include "suri/WxsGetCapabilities.h"
#include "suri/LibraryItemSelectionInterface.h"

// Includes Wx
#include "wx/treebase.h"

// Defines
// forwards
class wxTreeCtrl;

namespace suri {

// forwards
class DataViewManager;
class BasicLibraryItemSelector;
class Option;

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(WxsLayerImporterEvent);

/** clase que representa **/
class WxsLayerImporter : public DialogWidget, public LibraryItemSelectionInterface {
public:
   /**  Constructor */
   WxsLayerImporter(DataViewManager* pDataViewManager, std::string& FactoryString,
                    Option& Metadata);
   /** Destructor **/
   virtual ~WxsLayerImporter();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();
   /**
    * Configura el editor con el item que se pasa por parametro. Se pasa por parametro
    * un boolean que indica si se trata de la primera seleccion (para que el part no piense
    * que posee cambios cuando en realidad es la seleccion inicial del CGU)
    */
   virtual void SetActiveItem(const LibraryItem* pItem);

   FRIEND_EVENT_OBJECT(WxsLayerImporterEvent);
protected:
   EVENT_OBJECT_PROPERTY(WxsLayerImporterEvent);
   /** Metodo que se llama cuando se presiona el boton Aceptar */
   void OnButtonOk(wxCommandEvent &Event);
   /** Metodo que se llama cuando se presiona el boton Cancelar */
   void OnButtonCancel(wxCommandEvent &Event);
   /** Metodo que se llama cuando se presiona el boton de refresh */
   void OnRefreshDataRequest(wxCommandEvent &Event);
   /** Metodo que actualiza las capas disponibles asociadas al servicio
    *  wms ingresado  **/
   void UpdateAvailableLayers();
   /** Metodo que se llama cuando se actualiza la capa seleccionada **/
   void OnLayerSelectionChange(wxTreeEvent& Event);
   /** Metodo que se llama cuando se actualiza el sistema de referencia **/
   void OnSrsSelectionChange(wxCommandEvent &Event);
   /**
    * Metodo encargado de cargar la informacion correspondiente en la interfaz
    * grafica de la capa que se pasa por parametro
    */
   void LoadLayerInfo(const std::string& LayerId);
   /** Funcion recursiva que carga el arbol **/
   void LoadLayerTree(WxsLayersInformation::WxsLayerNode& Current,
                      wxTreeItemId& ParentId, wxTreeCtrl* pTreeCtrl,
                      bool IsRoot = false);
   /** Retorna el valor ulx */
   std::string GetMinX();
   /** Retorna el valor lrx */
   std::string GetMaxX();
   /** Retorna el valor lry */
   std::string GetMinY();
   /** Retorna el valor uly */
   std::string GetMaxY();
   /** Retorna del sistema de referencia */
   std::string GetSpatialReference();
   /** Retorna el estilo */
   std::string GetStyle();
   /** Retorna el formato */
   std::string GetFormat();
   /** Retorna la url */
   std::string GetUrl();
   /** Retorna el parametro TileMatrix de un TileMatrixSet */
   void GetTileMatrix(std::string Srs);
   void RefreshData();
   /** Metodo que carga la informacion de bounding box de la capa en los  campos correspondientes */
   void LoadBoundingBoxInfo(WxsLayersInformation::BoundingBox& BoundingBox);
   WxsCapabilitesInformation getCapabilitesResponse_;
   std::map<std::string, WxsLayersInformation::WxsLayerNode*> layermap_;
   DataViewManager* pDataViewManager_;
   BasicLibraryItemSelector* pItemSelector_;
   std::string lastLayer_;
   /** String resultado de la seleccion de la capa wms */
   std::string& factoryString_;
   /** tipo del servicio que proceso con exito **/
   std::string serviceType_;
   Option& metadata_;
};
} /** namespace  suri **/

#endif /* WXSLAYERIMPORTER_H_ */
