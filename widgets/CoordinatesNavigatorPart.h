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

#ifndef __COORDINATES_NAVIGATOR_PART__H__
#define __COORDINATES_NAVIGATOR_PART__H__

// Includes Estandar

// Includes suri
#include "suri/Part.h"
#include "suri/CoordinatesNavigationInputInterface.h"
#include "suri/LibraryClient.h"
#include "LibraryItemEditorPart.h"

// Includes wx
#include "wx/choicebk.h"

namespace suri {
	
// Forwards
class CoordinatesEventHandler;
class LibraryManager;

/** Clase que representa el componente de navegacion por coordenadas
 *  de la aplicacion
 */
class CoordinatesNavigatorPart : public Part,
                                 public CoordinatesNavigationInputInterface {
	/** Constructor copia **/
	CoordinatesNavigatorPart(const CoordinatesNavigatorPart&);

	public:
		/** ctor **/
		CoordinatesNavigatorPart(LibraryManager* pManager, bool UseLibrary = true);
		/** dtor **/
		virtual ~CoordinatesNavigatorPart();
		/** Indica si el Part tiene cambios para salvar. */
		virtual bool HasChanged();
		/** Salva los cambios realizados en el Part. */
		virtual bool CommitChanges();
		/** Restaura los valores originales del Part. */
		virtual bool RollbackChanges();
		/** Inicializa el part */
		virtual void SetInitialValues();
		/** Se crea la ventana de herramienta en las clases derivadas */
		virtual bool CreateToolWindow();
	   /** retorna la coordenada que ingresaron, informa si es valida */
	   virtual bool SetCoordinate(Coordinates Coordinate);
	   /** retorna la coordenada que ingresaron, informa si es valida */
	   virtual bool GetCoordinate(Coordinates &Coordinate);
	   /** Actualiza el estado de la parte */
	   virtual void Update() {}
	   virtual Widget* GetInputWidget() {
	      return GetWidget();
	   }

	   virtual void SetDataViewManager(DataViewManager* pDataViewManager);
	   /** Habilita/deshabilita la navegacion pixel/linea **/
	   virtual void EnablePixelLineNavigation(bool Enable = true);

	protected:
		/** Evento que se dispara cuando se cambio de pagina del book */
		void OnPageChanged(wxBookCtrlBaseEvent &Event);
		/** Metodo que se llama cuando se selecciona el radio
		 *  de decimal **/
		void OnDecimalRadioSelected(wxCommandEvent &Event);
		/** Metodo que se llama cuando se selecciona el radio 
		 *  de sexagesimal */
		void OnSexagesimalRadioSelected(wxCommandEvent &Event);
      /** Metodo que se llama cuando se selecciona el radio
       *  de decimal **/
      virtual void OnDecimalSelected() {}
      /** Metodo que se llama cuando se selecciona el radio
       *  de sexagesimal */
      virtual void OnSexagesimalSelected() {}
	   /** Metodo que se llama cuando se configura el sistema de referencia. Permite
	    *  realizar alguna accion luego de configurar el sistema de referencia
	    *  de trabajo
	    */
	   virtual void OnSpatialReferenceSet();
	   /** Metodo auxiliar que actualiza el panel de ingreso de coordenadas
	    *  en funcion del sistema de referencia
	    */
	   virtual void UpdateCoordsInput();
	   /** Configura el ingreso de coordenadas en modo decimal */
	   void ActiveDecimalInput(bool IsProj = false);
	   /** Configura el ingreso de coordenadas en modo sexagesimal */
	   void ActiveSexagesimalInput();
		/** Clase para manejar eventos  del Part **/
		friend class CoordinatesEventHandler;
		EVENT_OBJECT_PROPERTY(CoordinatesEventHandler);

		friend class BasicLibraryItemSelector;
		BasicLibraryItemSelector* pItemSelector_;
      bool useLibrary_;
		LibraryClient* pClient_;
		std::vector<CoordinatesNavigationInputInterface*> coordsInput_;
		/** Posicion actual del widget de ingreso de cordenadas **/
		size_t currentCoordInput_;
		/** posicion actual del widget de ingreso de coordenadas utilizando
		 *  sistema de referencia
		 */
		size_t currentSpatialCoordInput_;
		/** variable que contiene la pagina sacada del choicebook cuando
		 *  se selecciona una imagen sin sr
		 */
		wxWindow* pCoordPage_;
      /** variable que contiene la pagina sacada del choicebook cuando
       *  se deshabilita la navegacion raster
       */
      wxWindow* pRasterCoordPage_;
};

}  /** namespace suri **/

#endif  /** __COORDINATES_NAVIGATOR_PART__H__ */
