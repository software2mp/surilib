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

#ifndef UNIVERSALSUBSETINPUTPART_H_
#define UNIVERSALSUBSETINPUTPART_H_

// Includes Estandar

// Includes Suri
#include "UniversalCoordinateInputPart.h"
#include "suri/World.h"
#include "SpatialSelectionPart.h"
#include "suri/ProcessAtributeProvider.h"

// Includes Wx

// Defines

// forwards

namespace suri {

// forwards
class UniversalSubsetInputPartEvent;

/*!
 * Componente universal para Seleccion de Subset.
 */
class UniversalSubsetInputPart : public suri::UniversalCoordinateInputPart,
                                 public ProcessAtributeProvider {
   /** ctor copia **/
   UniversalSubsetInputPart(const UniversalSubsetInputPart&);

public:
   /** ctor **/
   UniversalSubsetInputPart(DataViewManager* pDataviewManager, LibraryManager *pManager,
                            World* pWorld, const Subset& InputSubset,
                            ProcessAdaptLayer* pProcessAdaptLayer = NULL);
   /** dtor **/
   virtual ~UniversalSubsetInputPart();
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool GetCoordinate(Coordinates &Coordinate);
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool SetCoordinate(Coordinates Coordinate);
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool GetSubset(Subset& OutputSubset);
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool SetSubset(Subset& NewSubset);
   /** Devuelve el world que se encuentra activo en el part **/
   World* GetWorld();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Informa si el subset seleccionado es valido */
   virtual bool HasValidData();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Inicializa el part */
   void SetInitialValues();

private:
   /**
    * Metodo auxiliar privado que agrega comportamiento adicional al commit de la
    * clase base. Este comportamiento adicional es necesario cuando
    * el commit involucra objetos adicionales a los items de biblioteca
    * del editor (ej. reproyeccion, en donde se necesita actualizar los params
    * de reproyeccion ademas de leer el item de biblioteca)
    * @return true en caso de realizar correctamente el commit "interno"
    * @return false en caso de que ocurra algun error durante el commit
    */
   virtual bool OnCommitChanges();
   /**
    * Metodo auxiliar privado que agrega comportamiento adicional al rollback de la
    * clase base. Este comportamiento adicional es necesario cuando
    * el rollback involucra objetos adicionales a los items de biblioteca
    * del editor (ej. parts compuesto en donde se necesita llamar a los rollbacks de
    * los mismos)
    * @return true en caso de realizar correctamente el rollback "interno"
    * @return false en caso de que ocurra algun error durante el rollback
    */
   virtual bool OnRollback();
   /** Metodo auxiliar que actualiza el widget correspondiente a el ingreso de coordenadas
    *  que corresponda*/
   virtual bool UpdateCoordinatesInputWidget(bool ReplaceWidget = false);
   /**
    *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
    *  Este metodo es el que realmente tienen que implementar las clases hijas para
    * lograr la especializacion a la hora de iniciar el part (metodo que se llama
    * en CreateToolWindow, a modo de Template Method).
    **/
   virtual bool ConfigureFeatures();
   friend class UniversalSubsetInputPartEvent;
   /** Evento que se ejecuta al des/activar el checkbox **/
   void OnCheckboxEventHandler(wxCommandEvent &Event);
   SpatialSelectionPart* pSubsetSelectionPart_;
   World* pWorld_;
   Subset inputSubset_;
   ProcessAdaptLayer* pProcessAdaptLayer_;
   UniversalSubsetInputPartEvent* pCheckEventHandler_;
   SpatialSelectionPart* pRasterSubsetSelectionPart_;
   bool raster_;
};

} /** namespace suri */

#endif /* UNIVERSALSUBSETINPUTPART_H_ */
