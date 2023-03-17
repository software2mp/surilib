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

#ifndef SELECTIONPART_H_
#define SELECTIONPART_H_

#include <cstddef>
#include <vector>
#include <list>
#include <utility>
#include <sstream>
#include <algorithm>
#include <string>

// Icludes suri
#include "suri/Part.h"
#include "suri/RasterElement.h"
#include "BandInfo.h"

// Includes wx
#include "wx/wx.h"
#include "wx/listctrl.h"

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

/** Mascara que indica que campos son validos en los items de las listas */
#define MASK wxLIST_MASK_TEXT|wxLIST_MASK_IMAGE|wxLIST_MASK_WIDTH

/** Offset en caracteres para el calculo del ancho maximo de los items de las listas */
#define OPTIMAL_ITEM_WIDTH_CHARACTER_OFFSET 0

namespace suri {

/**
 * Clase base para objetos que van a recibir notificaciones de algun evento ocurrido.
 */
class EventListener {
public:
   /**
    * Metodo a implementar por las clases derivadas para procesar
    * la notificacion recibida.
    */
   virtual void EventEmitted(void* pPartP) = 0;
};

// Forward declarations
class InputBandListener;


/**
 * Clase que va a recibir notificaciones sobre controles de destino vacios.
 */
class OutputEmptyListener : public EventListener {
public:
   /**
    * Procesa la notificacion.
    */
   virtual void EventEmitted(void* pPtr) {
      OnOutputEmpty();
   }

   /**
    * Metodo a implementar por la clase que recibe las notificaciones
    * para recibir un aviso de que la lista (o control) de salida es
    * vacia.
    */
   virtual void OnOutputEmpty() = 0;
};

/**
 * Clase que va a recibir notificaciones sobre controles de destino
 * que requieren elementos de entrada.
 */
class InputRequestedListener : public EventListener {
public:
   /**
    * Procesa la notificacion.
    */
   virtual void EventEmitted(void* pPtr) {
      OnInputRequested();
   }

   /**
    * Metodo a implementar por la clase que recibe las notificaciones
    * para recibir un aviso de que el control de destino solicita
    * elementos de entrada.
    */
   virtual void OnInputRequested() = 0;
};

/**
 * Clase base para los parts que van a actuar como seleccionadores de bandas.
 */
class SelectionPart : public Part, public OutputEmptyListener, public InputRequestedListener {
public:
   /**
    * Ctor
    */
   SelectionPart();

   /**
    * Dtor
    */
   virtual ~SelectionPart();

   /**
    * Crea la ventana de la herramienta
    */
   virtual bool CreateToolWindow();

   /**
    * Actualiza el estado de la parte
    */
   virtual void Update();

   /**
    * Indica si el Part tiene cambios para salvar
    */
   virtual bool HasChanged();

   /**
    * Salva los cambios realizados en el Part.
    */
   virtual bool CommitChanges();

   /**
    * Restaura los valores originales del Part.
    */
   virtual bool RollbackChanges();

   /**
    * Inicializa el part
    */
   virtual void SetInitialValues();

   /**
    * Devuelve la lista de bandas seleccionadas en la lista de entrada
    */
   virtual std::vector<BandInfo> GetInputSelectedBands() = 0;

   /**
    * Devuelve los nombres de las bandas disponibles.
    */
   virtual std::vector<BandInfo> GetAvailableBands() const = 0;

   /**
    * Configura el objeto que va a recibir las notificacion de que
    * se selecciono una banda.
    */
   void SetInputBandListener(InputBandListener* pInputBandListenerP);

   /**
    * Devuelve el objeto que va a recibir las notificaciones de bandas
    * seleccionadas.
    */
   InputBandListener* GetInputBandListener() const;

   /**
    * Metodo a implementar en las clases derivadas para poder aplicar,
    * de ser necesario, alguan correccion a los controles (UI)
    */
   virtual void UpdateSourceCtrl() = 0;

   /** Establece el ancho de una columna. */
   static void SetColumnWidth(wxListCtrl * pList, const int ColumnIdx, const int Width);

   /** Devuelve el ancho optimo de una columna */
   static int GetColumnOptimalWidth(const wxListCtrl * pList, const int ColumnIdx);

   /** Devuelve la suma de los anchos optimos de las columnas. */
   static int GetTotalOptimalWidth(const wxListCtrl * pList, std::list<int> ColumnIdxs);

   /** Devuelve el tamnio de las columnas sin ajustar. */
   static int GetTotalWidth(const wxListCtrl * pList, std::list<int> ColumnIdxs);

   /** Ajusta a su ancho optimo las columnas. */
   static void AdjustColumns(wxListCtrl * pList, std::list<int> ColumnIdxs);

   /** Ajusta al ancho optimo la ultima columna. */
   static void AdjustLastColumn(wxListCtrl * pList);

   /** Ajusta al ancho optimo todas las columnas. */
   static void AdjustTable(wxListCtrl * pList);

   /** */
   virtual void OnOutputEmpty();

   /** */
   virtual void OnInputRequested();

private:
   InputBandListener* pInputBandListener_;  /** Notificaciones de bandas seleccionadas */
};

/**
 * Clase que va a recibir notificaciones sobre bandas seleccionadas.
 */
class InputBandListener : public EventListener {
public:
   /**
    * Procesa la seleccion de bandas.
    */
   virtual void EventEmitted(void* pPartP) {
      SelectionPart* ppart = static_cast<SelectionPart*>(pPartP);
      if (ppart != NULL) {
         std::vector<BandInfo> bands = ppart->GetInputSelectedBands();
         OnBandSelected(bands);
      }
   }

   /**
    * Metodo a implementar por la clase que recibe las notificaciones
    * para recibir la lista de bandas seleccionadas.
    */
   virtual void OnBandSelected(std::vector<BandInfo> BandIxsP) = 0;
};

}

#endif  // SELECTIONPART_H_
