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

#include <string>

#include "SelectionPart.h"

#include "CacheRenderer.h"
#include "MemoryCanvas.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"
#include "wx/listctrl.h"
#include "wx/bitmap.h"
#include "wx/imaglist.h"
#include "wx/icon.h"
#include "wx/dcclient.h"

namespace suri {

/**
 * Ctor
 */
SelectionPart::SelectionPart() {
   pInputBandListener_ = NULL;
}

/**
 * Dtor
 */
SelectionPart::~SelectionPart() {
}

/**
 * Crea la ventana de la herramienta
 *
 * @return true por defecto.
 */
bool SelectionPart::CreateToolWindow() {
   return true;
}

/**
 * Actualiza el estado de la parte
 */
void SelectionPart::Update() {
}

/**
 * Indica si el Part tiene cambios para salvar
 *
 * @return false por defecto.
 */
bool SelectionPart::HasChanged() {
   return false;
}

/**
 * Salva los cambios realizados en el Part.
 *
 * @return true por defecto.
 */
bool SelectionPart::CommitChanges() {
   return true;
}

/**
 * Restaura los valores originales del Part.
 *
 * @return false por defecto.
 */
bool SelectionPart::RollbackChanges() {
   return false;
}

/**
 * Inicializa el part
 */
void SelectionPart::SetInitialValues() {
}

/**
 * Configura el objeto que va a recibir las notificacion de que
 * se selecciono una banda.
 *
 * @param[in] pInputBandListenerP Objeto que va a recibir las notificaciones
 * de banda seleccionada.
 */
void SelectionPart::SetInputBandListener(InputBandListener* pInputBandListenerP) {
   pInputBandListener_ = pInputBandListenerP;
}

/**
 * Devuelve el objeto que va a recibir las notificaciones de bandas
 * seleccionadas.
 *
 * @return Objeto configurado para recibir las notificaciones de banda seleccionada.
 */
InputBandListener* SelectionPart::GetInputBandListener() const {
   return pInputBandListener_;
}

/**
 * Permite establecer el ancho de la columna indicada.
 * @param[in]  pList puntero a la lista que posee la columna.
 * @param[in]  ColumnIdx indice de la columna que se quiere modificar.
 * @param[in]  Width ancho (en pixeles) que se le quiere asignar a la columna.
 */
void SelectionPart::SetColumnWidth(wxListCtrl * pList, const int ColumnIdx,
                                   const int Width) {
   if (pList != NULL)
      pList->SetColumnWidth(ColumnIdx, Width);
}

/**
 * Devuelve el ancho optimo de la columna indicada. Este ancho se calcula como
 * el maximo valor entre el ancho minimo necesario para mostrar el titulo de la
 * columna y el ancho minimo necesario para mostrar el contenido de los items de
 * la columna. El ancho minimo necesario para mostrar el contenido de los items
 * se calcula sumando el ancho del texto del item mas el ancho de la imagen (si
 * es que el item tiene una imagen asignada).
 * @param[in]  pList puntero a la lista que posee la columna.
 * @param[in]  ColumnIdx indice de la columna de la cual se desea el ancho optimo.
 * @return entero que significa el ancho optimo de la columna.
 * @return 0 (cero) en caso de error.
 */
int SelectionPart::GetColumnOptimalWidth(const wxListCtrl * pList,
                                         const int ColumnIdx) {
   if (!pList || ColumnIdx < 0 || ColumnIdx >= pList->GetColumnCount()) {
      return 0;
   }

   // obtengo el titulo de la columna
   wxListItem column;
   column.SetMask(MASK);
   pList->GetColumn(ColumnIdx, column);
   wxWindowDC wdc(const_cast<wxListCtrl*>(pList));
   int columnoptimalwidth = (column.GetText().Len()
         + OPTIMAL_HEADER_WIDTH_CHARACTER_OFFSET) * wdc.GetCharWidth();

   // obtengo el tamano de las imagenes
   int imgwidth = 0, imgheight = 0;
   wxImageList *pimglist = pList->GetImageList(wxIMAGE_LIST_SMALL);
   if (pimglist) {
      pimglist->GetSize(0, imgwidth, imgheight);
   }

   // obtengo el ancho maximo de los items.
   int itemoptimalwidth = 0;
   int countitem = pList->GetItemCount();
   for (int i = 0; i < countitem; ++i) {
      // obtengo el item, y multiplico la long. de su texto por el ancho de la fuente.
      wxListItem item;
      item.SetId(i);
      item.SetColumn(ColumnIdx);
      item.SetMask(MASK);
      pList->GetItem(item);

      int itemtextwidth = (item.GetText().Len() + OPTIMAL_ITEM_WIDTH_CHARACTER_OFFSET)
            * wdc.GetCharWidth();

      // si el item tiene imagen, le sumo el ancho de la misma.
      int itemimgwidth = 0;
      if (item.GetImage() >= 0 && item.GetImage() < pimglist->GetImageCount()) {
         itemimgwidth = imgwidth;
      }

      itemoptimalwidth = std::max(itemoptimalwidth, itemtextwidth + itemimgwidth);
   }

   return std::max(itemoptimalwidth, columnoptimalwidth);
}

/**
 * Devuelve la suma de los anchos optimos de las columnas indicadas. Invoca al
 * metodo ::GetColumnOptimalWidth; sumariza y devuelve el resultado del metodo
 * mencionado.
 * @param[in]  pList puntero a la lista que posee la columna.
 * @param[in]  ColumnIdxs indices de las columna cuyo ancho optimo es requerido.
 * @return entero que significa el ancho optimo de la columna.
 * @return 0 (cero) en caso de error.
 */
int SelectionPart::GetTotalOptimalWidth(const wxListCtrl * pList,
                                        std::list<int> ColumnIdxs) {
   int retval = 0;
   if (!pList) {
      return retval;
   }

   std::list<int>::iterator it = ColumnIdxs.begin();
   for ( ; it != ColumnIdxs.end(); ++it)
      retval += GetColumnOptimalWidth(pList, *it);

   return retval;
}

/**
 * Devuelve la suma de los anchos de las columnas indicadas. Invoca al
 * metodo wxListCtrl::GetColumnWidth; sumariza y devuelve el resultado del metodo
 * mencionado.
 * @param[in]  pList puntero a la lista que posee la columna.
 * @param[in]  ColumnIdxs indices de las columna cuyo ancho es requerido.
 * @return entero que significa el ancho optimo de la columna.
 * @return 0 (cero) en caso de error.
 */
int SelectionPart::GetTotalWidth(const wxListCtrl * pList, std::list<int> ColumnIdxs) {
   int retval = 0;
   if (!pList) {
      return retval;
   }

   std::list<int>::iterator it = ColumnIdxs.begin();
   for ( ; it != ColumnIdxs.end(); ++it)
      retval += pList->GetColumnWidth(*it);

   return retval;
}

/**
 * Modifica las columnas indicadas, para que se ajusten al ancho total
 * disponible del control.
 * Obtiene el espacio disponible (ancho del control - ancho actual de las
 * columnas que no se ajustan); calcula el ancho extra que hay que sumarle a las
 * columnas que se van a ajustar; invoca al metodo ::SetColumnWidth obteniendo
 * el ancho optimo de la columna mas el espacio extra que se calculo previamente.
 * @param[in]  pList puntero a la lista que posee las columnas.
 * @param[in]  ColumnIdxs indices de las columna a ajustar.
 */
void SelectionPart::AdjustColumns(wxListCtrl* pList, std::list<int> ColumnIdxs) {
   if (!pList || ColumnIdxs.size() == 0) {
      return;
   }

   // obtengo los indices de las columnas que no se ajustan (las fijas)
   std::list<int> fixedcolumns;
   for (int i = 0, leni = pList->GetColumnCount(); i < leni; ++i) {
      std::list<int>::iterator it = std::find(ColumnIdxs.begin(), ColumnIdxs.end(), i);
      if (it == ColumnIdxs.end()) {
         fixedcolumns.push_back(i);
      }
   }

   // calculo espacio disponible
   int availablewidth = pList->GetClientSize().GetWidth() + LIST_CLIENTSIZE_OFFSET
         - GetTotalWidth(pList, fixedcolumns);

   // calculo ancho extra que hay que sumar a cada columna
   int extrawidth = 0, extrawidthplus = 0;
   int totaloptimalwidth = GetTotalOptimalWidth(pList, ColumnIdxs);
   if (availablewidth > totaloptimalwidth) {
      extrawidth = (availablewidth - totaloptimalwidth) / ColumnIdxs.size();
      extrawidthplus = (availablewidth - totaloptimalwidth) % ColumnIdxs.size();
   }

   std::list<int>::iterator it = ColumnIdxs.begin();
   for (; it != ColumnIdxs.end(); ++it) {
      for (; extrawidthplus > 0; --extrawidthplus)
         SetColumnWidth(pList, *it, GetColumnOptimalWidth(pList, *it) + extrawidth + 1);

      SetColumnWidth(pList, *it, GetColumnOptimalWidth(pList, *it) + extrawidth);
   }
}

/**
 * Ajusta la ultima columna para que ocupe el ancho total disponible del Control.
 * Invoca al metodo ::AdjustColumns pasandole el indice de la ultima columna.
 * @param[in]  pList puntero a la lista que posee las columnas.
 */
void SelectionPart::AdjustLastColumn(wxListCtrl * pList) {
   if (pList != NULL) {
      std::list<int> lastcolumn(1, (pList->GetColumnCount() - 1));
      AdjustColumns(pList, lastcolumn);
   }
}

/**
 * Ajusta todas las columnas para que ocupen el ancho total disponible del
 * Control.
 * Invoca al metodo ::AdjustColumns pasandole el indice de las columnas de la
 * lista.
 * @param[in]  pList puntero a la lista que posee las columnas.
 */
void SelectionPart::AdjustTable(wxListCtrl* pList) {
   if (pList != NULL) {
      std::list<int> columns;
      for (int i = 0, leni = pList->GetColumnCount(); i < leni; ++i)
         columns.push_back(i);
      AdjustColumns(pList, columns);
   }
}

/**
 *
 */
void SelectionPart::OnOutputEmpty() {
}

/**
 *
 */
void SelectionPart::OnInputRequested() {
   if (GetInputBandListener())
      GetInputBandListener()->OnBandSelected(GetInputSelectedBands());
}

}  // namespace suri
