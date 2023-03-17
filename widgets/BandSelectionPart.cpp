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

#include "BandSelectionPart.h"

// Includes standar
#include <sstream>
#include <algorithm>

// Includes suri
#include "suri/xmlnames.h"
#include "RasterLayer.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"
#include "wx/listctrl.h"
#include "wx/bitmap.h"
#include "wx/imaglist.h"
#include "wx/icon.h"
#include "wx/dcclient.h"

// Includes App
#include "resources.h"

// Defines
/** Tamanio de las imagenes de las listas */
#define LIST_IMAGE_SIZE 16
/** Posicion de la columna en la lista */
#define IMAGE_COLUMN 0
/** Posicion de la columna en la lista */
#define INPUT_BAND_NAME_COLUMN 1
/** Ancho de la columna de imagen en pixeles */
#define IMAGE_COLUMN_EXTRA_WIDTH 10

#ifdef __WXMSW__
/** Offset en caracteres para el calculo del ancho maximo del header de las listas */
#  define OPTIMAL_HEADER_WIDTH_CHARACTER_OFFSET -1
/** offset en pixels sobre el ancho de la lista para ajustar por scrollbar */
#  define LIST_CLIENTSIZE_OFFSET 0
#else
/** Offset en caracteres para el calculo del ancho maximo del header de las listas */
#  define OPTIMAL_HEADER_WIDTH_CHARACTER_OFFSET 0
/** offset en pixels sobre el ancho de la lista para ajustar por scrollbar */
#  define LIST_CLIENTSIZE_OFFSET -7
#endif
/** Offset en caracteres para el calculo del ancho maximo de los items de las listas */
#define OPTIMAL_ITEM_WIDTH_CHARACTER_OFFSET 0
/** Mascara que indica que campos son validos en los items de las listas */
#define MASK wxLIST_MASK_TEXT|wxLIST_MASK_IMAGE|wxLIST_MASK_WIDTH

/** inicializa los recursos de la ventana */
void InitBandSelectionXmlResource();

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(BandSelectionPartEvent, BandSelectionPart)IMPLEMENT_EVENT_CALLBACK(OnInputSelection, OnInputSelection(event), wxListEvent)
   IMPLEMENT_EVENT_CALLBACK(OnUiUpdate, OnUiUpdate(event), wxUpdateUIEvent)
   IMPLEMENT_EVENT_CALLBACK(OnResize, OnResize(event), wxSizeEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor.
 * Obtiene la cantidad y nombre de las bandas del elemento y los guarda en las
 * propiedades.
 * @param[in] pElement puntero al elemento
 */
BandSelectionPart::BandSelectionPart(RasterElement * pElement) :
      NEW_EVENT_OBJECT(BandSelectionPartEvent)
{
   bandCount_ = pElement->GetBandCount();
   std::string bandname;
   for (int bandindex = 0; bandindex < bandCount_; bandindex++) {
      pElement->GetBandName(bandname, bandindex);
      bandNames_.Add(bandname);
   }
}

/**
 * Constructor.
 * Obtiene la cantidad y nombre de las bandas del elemento y los guarda en las
 * propiedades.
 * @param[in] pRasterLayer puntero al elemento
 */
BandSelectionPart::BandSelectionPart(RasterLayer * pRasterLayer) :
      NEW_EVENT_OBJECT(BandSelectionPartEvent) {
   bandCount_ = pRasterLayer->GetBandCount();
   std::string bandname;
   for (int bandindex = 0; bandindex < bandCount_; bandindex++) {
      pRasterLayer->GetBandName(bandname, bandindex);
      bandNames_.Add(bandname);
   }
}

/** Destructor */
BandSelectionPart::~BandSelectionPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Carga el XRC de la lista de bandas de entrada.
 * Conecta el evento UIUpdate a la lista de bandas de entrada.
 * Invoca al metodo que configura la lista de entrada.
 * @return true si pudo cargar el xrc
 * @return false si fallo la carga del xrc
 */
bool BandSelectionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_BAND_SELECTION_PART"));
   if (!pToolWindow_) {
      return false;
   }

   // Conecta a la lista de entrada el evento doble click sobre un elemento
   wxListCtrl *plist = XRCCTRL(*pToolWindow_, wxT("ID_INPUT_BAND_LIST"), wxListView);
   if (!plist) {
      return false;
   }

   plist->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                  wxListEventHandler(BandSelectionPartEvent::OnInputSelection), NULL,
                  pEventHandler_);

   pToolWindow_->Connect(wxEVT_UPDATE_UI,
                         wxUpdateUIEventHandler(BandSelectionPartEvent::OnUiUpdate),
                         NULL, pEventHandler_);

   // Agrega el titulo del sizer de seleccion de bandas
   GET_CONTROL(*pToolWindow_, wxT("ID_BAND_LISTS_SIZER"),
         wxStaticBox)->SetLabel(windowTitle_);
   ConfigureInputBandList();
   plist->Connect(wxEVT_SIZE, wxSizeEventHandler(BandSelectionPartEvent::OnResize),
                  NULL, pEventHandler_);
   return true;
}

/**
 * Crea dos columnas en la lista de entrada.
 * Crea la lista de iconos para las bandas.
 */
void BandSelectionPart::ConfigureInputBandList() {
   // Obtengo el icono para las bandas
   wxBitmap imageband;
   GET_BITMAP_RESOURCE(wxT(icon_IMAGE_BAND), imageband);
   // Creo la lista de imagenes para la lista de entrada
   wxImageList *pimageinlist = new wxImageList(LIST_IMAGE_SIZE, LIST_IMAGE_SIZE);
   pimageinlist->Add(imageband);

   // Agrego la lista de imagenes para iconos de las bandas
   wxListCtrl *plist = XRCCTRL(*pToolWindow_, wxT("ID_INPUT_BAND_LIST"), wxListView);
   // Si no puede acceder al control elimina la lista de imagenes
   if (!plist) {
      delete pimageinlist;
      return;
   }
   plist->AssignImageList(pimageinlist, wxIMAGE_LIST_SMALL);

   // creo las columnas de la lista de entrada
   int imgwidth = LIST_IMAGE_SIZE + IMAGE_COLUMN_EXTRA_WIDTH;

   // Columna imagen
   plist->InsertColumn(IMAGE_COLUMN, wxEmptyString, wxLIST_FORMAT_CENTER, imgwidth);
   // Columna nombres
   plist->InsertColumn(INPUT_BAND_NAME_COLUMN, _(label_INPUT_BANDLIST),
                       wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
   LoadInputBands();
}

/**
 * Carga las bandas del elemento en la lista de bandas de entrada
 */
void BandSelectionPart::LoadInputBands() {
   wxListCtrl * plist = XRCCTRL(*pToolWindow_, wxT("ID_INPUT_BAND_LIST"),
         wxListView);
   if (!plist) {
      return;
   }

   // Cargo las bandas
   size_t bandcount;

   bandcount = bandNames_.GetCount();
   int imageindex = 0;
   for (size_t i = 0; i < bandcount; i++) {
      plist->InsertItem(i, wxEmptyString, imageindex);
      plist->SetItem(i, INPUT_BAND_NAME_COLUMN, bandNames_.Item(i));
   }
}

/**
 * Crea un nodo combinacion de bandas con la seleccion de bandas.
 * Carga las bandas en un string, y crea un nodo combinacion con el string
 * con la lista de bandas.
 * @return pCombinationNode nodo xml con la combinacion de bandas seleccionada
 */
wxXmlNode * BandSelectionPart::CreateBandCombinationNode() {
   wxXmlNode * pCombinationNode = NULL;
   std::string bandlist;
   std::vector<int>::iterator iter = bandList_.begin();

   for (; iter != bandList_.end(); iter++) {
      std::ostringstream auxname;
      auxname << *iter;
      bandlist += auxname.str();
      bandlist += " ";
   }
   // Elimina el utimo caracter " "
   if (!bandlist.empty()) {
      bandlist = bandlist.substr(0, bandlist.length() - 1);
   }

   pCombinationNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                    wxT(BAND_COMBINATION_NODE));
   pCombinationNode->AddChild(
         new wxXmlNode(pCombinationNode, wxXML_TEXT_NODE, wxEmptyString,
                       wxT(bandlist)));

   return pCombinationNode;
}

/**
 * Metodo que se ejecuta cuando se hace doble click sobre una banda de la lista
 * de entrada, invoca al metodo que agrega las bandas seleccionadas.
 * @param[in] Event evento doble click sobre un item de la lista de entrada
 */
void BandSelectionPart::OnInputSelection(wxListEvent &Event) {
   AddOutputBands();
}

/**
 * Obtiene las bandas seleccionadas de la lista de entrada y obtiene la posicion
 * para agregar en la lista de salida. Agrega cada banda en la posicion corres
 * pondiente.
 * @return result true si pudo agregar todas las bandas
 * @return result false si fallo al agregar alguna banda
 */
bool BandSelectionPart::AddOutputBands() {
   bool result = true;

   std::vector<int> bands = GetInputSelectedBands();
   std::vector<int>::iterator iter = bands.begin();

   for (; iter != bands.end(); iter++)
      if (!AddOutputBand(*iter, GetNextInsertPosition())) {
         result = false;
      }

   return result;
}

/**
 * Obtiene las bandas seleccionadas de la lista de entrada y las inserta
 * en un vector.
 * @return bands vector con las bandas selecionadas de la lista de entrada.
 */
std::vector<int> BandSelectionPart::GetInputSelectedBands() {
   std::vector<int> bands;

   int bandinputindex;
   int firstband;

   // Obtiene el indice item seleccionada
   bandinputindex = USE_CONTROL(*pToolWindow_, wxT("ID_INPUT_BAND_LIST"),
         wxListView, GetFirstSelected(), wxNOT_FOUND);
   firstband = bandinputindex;
   while (bandinputindex != wxNOT_FOUND) {
      bands.push_back(bandinputindex);

      bandinputindex = USE_CONTROL(*pToolWindow_, wxT("ID_INPUT_BAND_LIST"),
            wxListView, GetNextSelected(bandinputindex), wxNOT_FOUND);
      if (bandinputindex == firstband) {
         break;
      }
   }

   return bands;
}

/**
 * Evento temporal, compara las bandas de la lista wx contra las guardadas en
 * la propiedad.
 * Si no hay ninguna banda seleccionada, modified_ permanece en true,
 * de esta manera obliga a seleccionar alguna banda.
 * @param[in] Event evento UIUpdate
 */
void BandSelectionPart::OnUiUpdate(wxUpdateUIEvent &Event) {
   modified_ = false;

   if (bandList_ != GetOutputBands()) {
      modified_ = true;
   } else {
      errorMessage_ = message_INVALID_BAND_SELECTED;
   }
   Event.Skip();
}

/**
 * Este metodo se ejecuta cuando la lista recibe los eventos de resize para
 * poder ajustar los tamanios de las columnas.
 * @param[in]	Event: evento
 */
void BandSelectionPart::OnResize(wxSizeEvent &Event) {
   AdjustLastColumn(dynamic_cast<wxListCtrl*>(Event.GetEventObject()));
}

/**
 * Permite establecer el ancho de la columna indicada.
 * @param[in]	pList puntero a la lista que posee la columna.
 * @param[in]	ColumnIdx indice de la columna que se quiere modificar.
 * @param[in]	Width ancho (en pixeles) que se le quiere asignar a la columna.
 */
void BandSelectionPart::SetColumnWidth(wxListCtrl * pList, const int ColumnIdx,
                                       const int Width) {
   if (!pList) {
      return;
   }

   pList->SetColumnWidth(ColumnIdx, Width);
}

/**
 * Devuelve el ancho optimo de la columna indicada. Este ancho se calcula como
 * el maximo valor entre el ancho minimo necesario para mostrar el titulo de la
 * columna y el ancho minimo necesario para mostrar el contenido de los items de
 * la columna. El ancho minimo necesario para mostrar el contenido de los items
 * se calcula sumando el ancho del texto del item mas el ancho de la imagen (si
 * es que el item tiene una imagen asignada).
 * @param[in]	pList puntero a la lista que posee la columna.
 * @param[in]	ColumnIdx indice de la columna de la cual se desea el ancho optimo.
 * @return entero que significa el ancho optimo de la columna.
 * @return 0 (cero) en caso de error.
 */
int BandSelectionPart::GetColumnOptimalWidth(const wxListCtrl * pList,
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
   for (int i = 0; i < countitem; i++) {
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
 * @param[in]	pList puntero a la lista que posee la columna.
 * @param[in]	ColumnIdxs indices de las columna cuyo ancho optimo es requerido.
 * @return entero que significa el ancho optimo de la columna.
 * @return 0 (cero) en caso de error.
 */
int BandSelectionPart::GetTotalOptimalWidth(const wxListCtrl * pList,
                                            std::list<int> ColumnIdxs) {
   int retval = 0;
   if (!pList) {
      return retval;
   }

   std::list<int>::iterator it = ColumnIdxs.begin();
   for (; it != ColumnIdxs.end(); it++)
      retval += GetColumnOptimalWidth(pList, *it);

   return retval;
}

/**
 * Devuelve la suma de los anchos de las columnas indicadas. Invoca al
 * metodo wxListCtrl::GetColumnWidth; sumariza y devuelve el resultado del metodo
 * mencionado.
 * @param[in]	pList puntero a la lista que posee la columna.
 * @param[in]	ColumnIdxs indices de las columna cuyo ancho es requerido.
 * @return entero que significa el ancho optimo de la columna.
 * @return 0 (cero) en caso de error.
 */
int BandSelectionPart::GetTotalWidth(const wxListCtrl * pList,
                                     std::list<int> ColumnIdxs) {
   int retval = 0;
   if (!pList) {
      return retval;
   }

   std::list<int>::iterator it = ColumnIdxs.begin();
   for (; it != ColumnIdxs.end(); it++)
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
 * @param[in]	pList puntero a la lista que posee las columnas.
 * @param[in]	ColumnIdxs indices de las columna a ajustar.
 */
void BandSelectionPart::AdjustColumns(wxListCtrl * pList, std::list<int> ColumnIdxs) {
   if (!pList || ColumnIdxs.size() == 0) {
      return;
   }

   // obtengo los indices de las columnas que no se ajustan (las fijas)
   std::list<int> fixedcolumns;
   for (int i = 0; i < pList->GetColumnCount(); i++) {
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
   for (; it != ColumnIdxs.end(); it++) {
      for (; extrawidthplus > 0; extrawidthplus--)
         SetColumnWidth(pList, *it, GetColumnOptimalWidth(pList, *it) + extrawidth + 1);

      SetColumnWidth(pList, *it, GetColumnOptimalWidth(pList, *it) + extrawidth);
   }
}

/**
 * Ajusta la ultima columna para que ocupe el ancho total disponible del Control.
 * Invoca al metodo ::AdjustColumns pasandole el indice de la ultima columna.
 * @param[in]	pList puntero a la lista que posee las columnas.
 */
void BandSelectionPart::AdjustLastColumn(wxListCtrl * pList) {
   if (!pList) {
      return;
   }
   std::list<int> lastcolumn(1, (pList->GetColumnCount() - 1));
   AdjustColumns(pList, lastcolumn);
}

/**
 * Ajusta todas las columnas para que ocupen el ancho total disponible del
 * Control.
 * Invoca al metodo ::AdjustColumns pasandole el indice de las columnas de la
 * lista.
 * @param[in]	pList puntero a la lista que posee las columnas.
 */
void BandSelectionPart::AdjustTable(wxListCtrl * pList) {
   if (!pList) {
      return;
   }
   std::list<int> columns;
   for (int i = 0; i < pList->GetColumnCount(); i++)
      columns.push_back(i);
   AdjustColumns(pList, columns);
}
}
