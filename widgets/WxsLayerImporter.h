/*! \file WxsLayerImporter.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
