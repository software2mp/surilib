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

#ifndef BUFFERPART_H_
#define BUFFERPART_H_

// Includes Estandar
// Includes Suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "VectorOperation.h"
// Includes Wx
// Defines

namespace suri {

// fordwards
class World;
class DataViewManager;

/** Declara clase BufferPartEvent */
DECLARE_EVENT_OBJECT(BufferPartEvent);

/** Part que permite configurar el calculo de buffer para capas vectoriales */
class BufferPart : public Part, public ProcessAtributeProvider {
   /** Declara friend la clase de evento BufferPartEvent */
   FRIEND_EVENT_OBJECT(BufferPartEvent);
public:
   /** Constructor */
   BufferPart(DataViewManager *pDataViewManager, World* pInputWorld);
   /** Destructor */
   virtual ~BufferPart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update() {}
   /** Retorna el icono de la herramienta. */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Devuelve si la parte contiene datos validos. */
   virtual bool HasValidData();

protected:
   /** Crea la ventana de la parte y conecta los eventos */
   bool CreateToolWindow();
   /** Habilita/Deshabilita los controles correspondientes al radio button */
   void SetStatus(bool isManual);
   /** Accion de seleccionar el radio button manual */
   void OnRadioButtonManual(wxCommandEvent &Event);
   /** Accion de seleccionar el radio button del campo */
   void OnRadioButtonField(wxCommandEvent &Event);
   /** Accion de seleccionar una opcion en el Choice de distancias */
   void OnChoiceSelection(wxCommandEvent &Event);
   /** Accion de seleccionar una opcion en el Choice de unidades */
   void OnUnitSelection(wxCommandEvent &Event);
   /** Accion de ingresar un valor en el TextCtrl de distancia */
   void OnDistanceTexCtrlChanged(wxCommandEvent &Event);
   /** Obtiene la distancia */
   double GetInternalDistance();
   /** Obtiene la distancia */
   double GetDistanceFromTextCtrl();
   /** Obtiene la cantidad de anillos concentricos */
   int GetInternalQuantity();
   /** Carga los choice del part */
   void LoadChoiceFields();
   /** Obtiene el nombre del campo seleccionado en el choice */
   std::string GetFieldFromChoice();
   /** Obtiene el nombre del campo seleccionado en el choice */
   std::string GetField();
   /** Obtiene el factor de conversion de la unidad a metros */
   double GetConversionFactor();
   /** Setea el factor de conversion de la unidad a metros */
   void SetConversionFactor();
   /** Retorna el valor del factor de conversion de la unidad a metros */
   double GetConversionFactorValue(std::string SelectedUnit);
   /** Puntero al mundo */
   World *pInputWorld_;
   /** Administrador con informacion de proyecto */
   DataViewManager *pDataViewManager_;
   /** Objeto que conecta los eventos */
   EVENT_OBJECT_PROPERTY(BufferPartEvent);
   double distance_; /** Distancia del buffer */
   int quantity_; /** Cantidad de anillos concentricos */
   std::string field_; /** Nombre de la columna */
   bool isfromfield_; /** Indica si la distancia proviene de una columna */
   bool reloadfield_ ; /** Indica si es necesario cargar el nombre de la columna nuevamente */
   double conversionfactor_;
   VectorOperation::OperationType operation_; /** Tipo de operacion */
};

} /** namespace suri */

#endif /* BUFFERPART_H_ */
