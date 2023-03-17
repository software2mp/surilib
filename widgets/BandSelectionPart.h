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

#ifndef BANDSELECTIONPART_H_
#define BANDSELECTIONPART_H_

// Includes standar
#include <vector>
#include <list>

// Icludes suri
#include "suri/Part.h"
#include "suri/RasterElement.h"

// Includes wx
#include "wx/wx.h"
#include "wx/listctrl.h"

/** namespace suri */
namespace suri {

// Forwards
class RasterLayer;

/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(BandSelectionPartEvent);

/** Clase base para la seleccion de bandas */
/**
 * Esta clase sirve de base para las partes que implementan la seleccion de
 * bandas.
 * Crea una lista con las bandas del elemento recibido. Implementa los metodos
 * relacionados con la lista de entrada, y tiene metodos virtuales relativos
 * a las listas de salida que se implementan en cada clase derivada.
 * Implementa el metodo que crea el nodo renderizacon con la lista de bandas
 * seleccionadas.
 */
class BandSelectionPart : public Part {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(BandSelectionPartEvent);
   /** Constructor legacy */
   explicit BandSelectionPart(RasterElement * pElement);
   /** Constructor */
   explicit BandSelectionPart(RasterLayer * pRasterLayer);
   /** Destructor */
   virtual ~BandSelectionPart();
   /** Crea la ventana de la herramienta */
   virtual bool CreateToolWindow();
   /** Configura la lista donde se cargan las bandas de entrada */
   void ConfigureInputBandList();
   /** Carga la lista de bandas de entrada */
   void LoadInputBands();
   /** Devuelve la lista de bandas seleccionadas */
   virtual std::vector<int> GetOutputBands()=0;
   /** Crea el nodo combinacion de bandas */
   wxXmlNode * CreateBandCombinationNode();
   /** Metodo que se ejecuta al hacer doble click en la lista de entrada */
   void OnInputSelection(wxListEvent &Event);
   /** Agrega las bandas seleccionadas */
   bool AddOutputBands();
   /** Metodo que devuelve la posicion para insertar el proximo elemento */
   virtual int GetNextInsertPosition()=0;
   /** Agrega una banda en una posicion indicada */
   virtual bool AddOutputBand(int BandIndex, int Position)=0;
   /** Devuelve la lista de bandas seleccionadas en la lista de entrada */
   std::vector<int> GetInputSelectedBands();
   /** Evento temporal */
   void OnUiUpdate(wxUpdateUIEvent &Event);
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
   /** Lanzado cuando cambia el tamanio de la ventana */
   void OnResize(wxSizeEvent &Event);

protected:
   /** Establece el ancho de una columna. */
   void SetColumnWidth(wxListCtrl * pList, const int ColumnIdx, const int Width);
   /** Devuelve el ancho optimo de una columna */
   int GetColumnOptimalWidth(const wxListCtrl * pList, const int ColumnIdx);
   /** Devuelve la suma de los anchos optimos de las columnas. */
   int GetTotalOptimalWidth(const wxListCtrl * pList, std::list<int> ColumnIdxs);
   /** Devuelve el tamnio de las columnas sin ajustar. */
   int GetTotalWidth(const wxListCtrl * pList, std::list<int> ColumnIdxs);
   /** Ajusta a su ancho optimo las columnas. */
   void AdjustColumns(wxListCtrl * pList, std::list<int> ColumnIdxs);
   /** Ajusta al ancho optimo la ultima columna. */
   void AdjustLastColumn(wxListCtrl * pList);
   /** Ajusta al ancho optimo todas las columnas. */
   void AdjustTable(wxListCtrl * pList);

   int bandCount_; /*! Cantidad de bandas del raster */
   wxArrayString bandNames_; /*! Nombres de las bandas */
   std::vector<int> bandList_; /*! Lista con las bandas de salida */
private:
   EVENT_OBJECT_PROPERTY(BandSelectionPartEvent); /*! Objeto para conectar eventos */
};
}

#endif /* BANDSELECTIONPART_H_ */
