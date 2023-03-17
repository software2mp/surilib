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

#ifndef BANDCOMBINATIONSELECTIONPART_H_
#define BANDCOMBINATIONSELECTIONPART_H_

// Includes Suri
#include "BandSelectionPart.h"

// Include wx
#include "wx/checkbox.h"

/** namespace suri */
namespace suri {

// Forwards
class RasterLayer;

/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(BandCombinationSelectionPartEvent);

/** Clase que permite cambiar la combinacion de bandas a renderizar de un elemento */
/**
 * Agrega dos listas al control de seleccion de bandas, una para seleccionar
 * combinacion de bandas RGB y una para escala de grises.
 * Modifica el nodo combinacion del elemento y actualiza el viewer.
 */
class BandCombinationSelectionPart : public BandSelectionPart {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(BandCombinationSelectionPartEvent);
   /** Constructor legacy*/
   explicit BandCombinationSelectionPart(RasterElement* pElement);
   /** Constructor */
   explicit BandCombinationSelectionPart(RasterLayer* pRasterLayer);
   /** Destructor */
   virtual ~BandCombinationSelectionPart();
   /** Crea la vantana de la parte */
   virtual bool CreateToolWindow();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Configura las columnas de la lista de salida */
   void ConfigureOutputBandList();
   /** Carga los valores iniciales en las propiedades */
   virtual void SetInitialValues();
   /** Pasa los valores de las propiedades a los controles wx */
   virtual bool RollbackChanges();
   /** Devuelve si los controles tienen datos validos seleccionados */
   virtual bool HasValidData();
   /** Devuelve el estado de la propiedad modified_ */
   virtual bool HasChanged();
   /** Aplica la nueva seleccion de bandas al elemento. */
   virtual bool CommitChanges();
   /** Metodo que se ejecuta cuando se des/selecciona escala de grises */
   void OnGreyScaleChecked(wxCommandEvent &Event);
   /** Devuelve la posicion en la lista de salida en la que se inserta la proxima banda */
   virtual int GetNextInsertPosition();
   /** Inserta la banda indicada en la posicion indicada */
   virtual bool AddOutputBand(int BandIndex, int Position);
   /** Selecciona el item siguiente en el control wx */
   void SelectNextItem();
   /** Agrega el nodo combinacion de bandas al elemento */
   void SetBandCombinationNode();
   /** Devuelve una lista con las bandas seleccionadas */
   virtual std::vector<int> GetOutputBands();
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
   /** Lanzado cuando cambia el tamanio de la ventana */
   void OnResize(wxSizeEvent &Event);
   /** Informa si esta en escala de grises */
   bool IsGreyScale();

protected:
   /** Selecciona como lista de salida la lista RGB */
   void SelectRgbBandList();
   /** Selecciona como lista de salida la lista en escala de grises */
   void SelectGreyScaleBandList();
   /** Cambia la lista activa. */
   std::vector<int> GetElementBandCombination();

private:
   EVENT_OBJECT_PROPERTY(BandCombinationSelectionPartEvent); /*! Objeto que */
   /* conecta los */
   /* eventos */
   RasterElement * pElement_; /*! Puntero al elemento (sobre el que se esta */
   RasterLayer* pRasterLayer_; /*! Puntero a la capa vectorial **/
   /* aplicando la herramienta) */
   wxListView * pOutputList_; /*! Puntero a la lista de bandas de salida */

   /** Elimina el tag de realce si existe. */
   void RemoveEnhancement(Element* pElement);
};
}

#endif /* BANDCOMBINATIONSELECTIONPART_H_ */
