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

#ifndef HISTOGRAMMATCHINGPART_H_
#define HISTOGRAMMATCHINGPART_H_

// Includes standard
#include <string>
#include <vector>

// Includes Suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/Element.h"

// Includes wx
#include "wx/listctrl.h"

/** namespace suri */
namespace suri {

/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(HistogramMatchingPartEvent);

/**
 * Clase que permite configurar la herramienta ajuste de histograma
 */
class HistogramMatchingPart : public Part, public ProcessAtributeProvider {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(HistogramMatchingPartEvent);
   /** Ctor. */
   HistogramMatchingPart(std::vector<Element*> Selected);
   /** Dtror. */
   virtual ~HistogramMatchingPart();
   /** Crea la ventana de la parte */
   bool CreateToolWindow();
   /** Devuelve el estado del Part. */
   bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   bool CommitChanges();
   /** Restaura los valores originales del Part. */
   bool RollbackChanges();
   /** Inicializa el part */
   void SetInitialValues();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Devuelve si la parte tiene datos validos */
   bool HasValidData();
   /** Actualiza el estado de la parte */
   virtual void Update() {}
   /** Si se selecciona una nueva imagen se reinician las listas**/
   void OnChoiceSelected(wxCommandEvent &Event);
   /** Maneja el evento de click sobre el boton que modifica las bandas de entrada */
   void OnAddBandButton(wxCommandEvent &Event);
   /** Maneja el evento de click sobre el boton que modifica las bandas de salida */
   void OnDeleteBandButton(wxCommandEvent &Event);
   /** Maneja el evento de click sobre el boton que modifica el ordenamiento de las bandas */
   void OnUpButton(wxCommandEvent &Event);
   /** Maneja el evento de click sobre el boton que modifica el ordenamiento de las bandas */
   void OnDownButton(wxCommandEvent &Event);
   /** Maneja el evento de doble click sobre un elemento de la lista de bandas de entrada */
   void OnInputSelection(wxCommandEvent &Event);
   /** Maneja el evento de doble click sobre un elemento de la lista de bandas de salida */
   void OnOutputSelection(wxCommandEvent &Event);

private:
   /** Inicializa la lista pasada por parametro */
   void InitializeList(wxListCtrl* pList);
   /** Devuelve el string q se encuentra en la fila Row , columna Column de la lista pList
    * si Row -1 devuelve el elemento seleccionado, string vacio si fallo */
   wxString GetItemSelection(wxListCtrl* pList, int Column, int Row = -1) const;
   /** Agrega la banda a la lista pOutputBands_, si ya se encuentra no hace nada */
   void AddBand(const wxString& BandNumber, const wxString& BandName);
   /** Elimina una banda de la lista pOutputBands_ */
   void DeleteBand(const wxString& BandName);
   /* devuelve un contenedor de pares (numero de banda, nombre) de pOutputBands_*/
   std::vector<std::pair<wxString, wxString> > GetBands();
   std::vector<int> GetOutputBands();
   wxListCtrl* pInputBands_;
   wxListCtrl* pOutputBands_;
   wxChoice * pChoice_;
   bool changed_;
   int lastSelection_;
   std::vector<Element*> selected_;
   EVENT_OBJECT_PROPERTY(HistogramMatchingPartEvent); /*! Objeto para conectar */
};
}  // namespace suri
#endif /* HISTOGRAMMATCHINGPART_H_ */

