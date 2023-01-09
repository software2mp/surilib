/*! \file MergeLayersPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MERGELAYERSPART_H_
#define MERGELAYERSPART_H_

// Includes Estandar
#include <vector>

// Includes Suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "VectorOperation.h"
#include "suri/UseFeatureSelection.h"

// Includes Wx
#include "wx/imaglist.h"
#include "wx/treectrl.h"
#include "resources.h"
#include "wx/listctrl.h"

// Defines
// forwards

namespace suri {

// forwards
class DatasourceInterface;

/** Declara clase MergeLayersPartEvent */
DECLARE_EVENT_OBJECT(MergeLayersPartEvent);

/**
 *  Control grafico para la seleccion  de campos de capas vectoriales
 */
class MergeLayersPart : public UseFeatureSelection, public Part, public ProcessAtributeProvider {
   FRIEND_EVENT_OBJECT(MergeLayersPartEvent);
public:
   MergeLayersPart(std::vector<DatasourceInterface*>& DatasourceVector);
   virtual ~MergeLayersPart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update(){};
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();
   /** Mueve hacia arriba un campo seleccionado en la capa destino */
   void OnFieldSortUpClick(wxCommandEvent &Event);
   /** Mueve hacia abajo un campo seleccionado en la capa destino */
   void OnFieldSortDownClick(wxCommandEvent &Event);
   /** Agrega un campo de la capa origen a la capa destino */
   void OnAddFieldClick(wxCommandEvent &Event);
   /** Quita un campo de la capa destino */
   void OnRevokeFieldClick(wxCommandEvent &Event);
   /** Agrega una fila a la lista */
   bool AppendRowToList(wxString dsid, wxString Column, wxString Origin);
   /** Retorna el icono de la herramienta. */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Devuelve si la parte contiene datos validos. */
   virtual bool HasValidData();
   /** Arma un string con las fuentes de datos y los campos del mapa */
   std::string ConvertMapToString();
   /** Borra el item seleccionado del mapa */
   void EraseFieldFromMap(std::vector<std::string> CompleteItem);
   /** Verifica que no haya registros duplicados */
   bool CanAppendRow(wxString Dsid, wxString Column);

private:
   /** Columna # */
   static const int NUMBER_COL = 0;
   /** Columna Columna*/
   static const int COLUMN_COL = 1;
   /** Columna Origen */
   static const int ORIGIN_COL = 2;
   /** Cantidad de columnas */
   static const int ITEM_COUNT = 3;
   /** Fuentes de datos seleccionadas */
   std::vector<DatasourceInterface*> datasourceVector_;
   /** Tipo de operacion */
   VectorOperation::OperationType operation_;
   /** Objeto que conecta los eventos */
   EVENT_OBJECT_PROPERTY(MergeLayersPartEvent);
   /** Ruta y nombre del archivo */
   std::string fileName_;
   /** Devuelve si la parte contiene datos validos */
   bool hasvaliddata_;
   /**
    * Contiene los nombres de los campos seleccionados para la capa destino y
    * el id de la fuente de datos a la cual corresponden
    */
   std::multimap<std::string, std::string> fieldsmultipmap_;
};

} /** namespace suri */

#endif /* MERGELAYERSPART_H_ */
