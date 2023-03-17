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

#ifndef SPATIALSELECTIONPART_H_
#define SPATIALSELECTIONPART_H_

// Includes standard
// Includes Suri
#include "suri/Part.h"
#include "suri/Subset.h"
#include "suri/Element.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ProcessAtributeProvider.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
class World;
class NavigationInputWidget;
/** Declara la clase SpatialSelectionPartEvent */
DECLARE_EVENT_OBJECT(SpatialSelectionPartEvent);

/** Parte para seleccionar subset */
/**
 * Esta clase crea un part que permite seleccionar un subset y lo guarda en
 * el mundo que se pasa por referencia.
 */
class SpatialSelectionPart : public Part, public ProcessAtributeProvider {
public:
   static const int PIXEL_LINE_SUBSET = 0x10;
   static const int SEXAGESIMAL_SUBSET = 0x01;
   static const int DECIMAL_SUBSET = 0x02;
   /** Declara friend la clase SpatialSelectionPartEvent */
   FRIEND_EVENT_OBJECT(SpatialSelectionPartEvent);

   /** Ctor */
   SpatialSelectionPart(DataViewManager* pDataviewManager,
                        World* pWorld, const Subset &DefaultValue, int Flags =
         SEXAGESIMAL_SUBSET);
   /** Dtor */
   virtual ~SpatialSelectionPart();

   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   virtual void SetInitialValues();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Elimina las modificaciones realizadas a la ventana */
   void OnImageButton(wxCommandEvent &Event);
   /** Muestra por pantalla las coordenadas default cargadas en constructor */
   void OnWindowButton(wxCommandEvent &Event);
   /** Retorna el mundo que se va a modificar */
   World* GetWorld();
   /** Informa si el subset seleccionado es valido */
   virtual bool HasValidData();
   /** Verifica que el subset sea valido. */
   virtual bool ValidateSubset(const Subset &SubsetIn);
   /** Evento UIUpdate */
   virtual void OnUiUpdate(wxUpdateUIEvent &Event);
   //! Actualiza el estado de la parte
   virtual void Update() {
   }
   /** Retorna las coordenadas de la ventana en sistema window.
    *  De esta forma no se esta devolviendo el World directamente**/
   void GetInputWindow(Subset& window);
   /** Setea subset interno en las coordenadas del mundo. */
   bool SetInternalSubset(const Subset &SubsetIn);
   /** Metodo auxiliar que crea los NavigationInputWidget correspondientes
    *  al Sistema de ingreso de coordenadas seleccionado */
   bool CreateCoordinatesInputWidgets(int Flags);

protected:

   /** Devuelve subset interno en las coordenadas del mundo. */
   bool GetInternalSubset(Subset &SubsetOut);
   /** Devuelve subset ingresado por el usuario. */
   bool GetWidgetSubset(Subset &SubsetOut);
   /** Setea el subset en herramienta grafica */
   bool SetWidgetSubset(const Subset &SubsetIn);
   /** Compara las dimensiones de dos subsets */
   void CompareSigns(const Subset &Subset1, const Subset &Subset2, bool &EqualX,
                     bool &EqualY);

   NavigationInputWidget* pLrWidget_; /*! puntero a herramienta que obtiene */
   /* coordenadas lr */
   NavigationInputWidget* pUlWidget_; /*! puntero a herramienta que obtiene */
   /* coordenadas ur */
   World* pOriginalWorld_; /*! Mundo que se recive en constructor */
   World* pInternalWorld_; /*! Mundo con el que interactua el usuario. Se */
   /* modifica al aplicar */
   Subset defaultValue_; /*! coordenadas default */
   // Estos subset solo existen para mejorar velocidad
   Subset currentSRSubset_; /*! ultimo subset valido en coord. de mundo. */
   Subset currentLatLonSubset_; /*! subset al que corresponde currentSRSubset_. */
   /* Para saber si hay que recalcular */
   Subset appliedLatLonSubset_; /*! subset aplicado en coord LatLon. */
   suri::CoordinatesTransformation *pCoordTransform_; /*! Utilizado para convertir */
   /* subset mundo->latlon y */
   /* viceversa. */
   /** Declara la propiedad pEventHandler_ del tipo SpatialSelectionPartEvent* */
   EVENT_OBJECT_PROPERTY(SpatialSelectionPartEvent);
   int flags_;
   DataViewManager* pDataviewManager_;
};
}  // namespace suri

#endif /* SPATIALSELECTIONPART_H_ */
