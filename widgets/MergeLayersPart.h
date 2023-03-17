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
