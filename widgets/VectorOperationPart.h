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

#ifndef VECTOROPERATIONPART_H_
#define VECTOROPERATIONPART_H_

// Includes Estandar
// Includes Suri
#include "suri/ProcessAtributeProvider.h"
#include "suri/Part.h"
#include "suri/UseFeatureSelection.h"
#include "VectorOperation.h"
// Includes Wx
// Defines
// forwards

namespace suri {
/** Declara clase BufferPartEvent */
DECLARE_EVENT_OBJECT(VectorOperationPartEvent);

class VectorOperationPart : public UseFeatureSelection, public suri::ProcessAtributeProvider, public suri::Part {
   FRIEND_EVENT_OBJECT(VectorOperationPartEvent);
public:
   /** Constructor */
   VectorOperationPart();
   /** Destructor */
   virtual ~VectorOperationPart();
   /** Crea la ventana de la parte y contecta los eventos */
   bool CreateToolWindow();
   /** Accion de seleccionar una opcion en el Choice de unidades */
   void OnUnitSelection(wxCommandEvent &Event);
   /**
    * Retorna el icono de la herramienta.
    */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;

private:
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update() {}
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Carga los choice del part */
   void LoadChoiceFields();
   /** Accion de seleccionar una opcion en el Choice de primer capa */
   void OnFirstLayerSelection(wxCommandEvent &Event);
   /** Accion de seleccionar una opcion en el Choice de segunda capa */
   void OnSecondLayerSelection(wxCommandEvent &Event);
   /** Devuelve si la parte contiene datos validos. */
   bool HasValidData();
   /** Objeto que conecta los eventos */
   EVENT_OBJECT_PROPERTY(VectorOperationPartEvent);
   VectorOperation::OperationType operation_; /** Tipo de operacion */
   int firstchoice_; /** Id del elemento del primer choice */
   int secondchoice_; /** Id del elemento del segundo choice */
   std::vector<DatasourceInterface*> datasources_; /** Fuentes de datos a procesar */
   std::vector<DatasourceInterface*> inputdatasources_; /** Fuentes de datos de entrada */
   bool hasvaliddata_; /** Devuelve si la parte contiene datos validos */
};

} /** namespace suri */

#endif /* VECTOROPERATIONPART_H_ */
