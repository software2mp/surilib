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

#ifndef WXHOTLINKEDITOR_H_
#define WXHOTLINKEDITOR_H_

// Includes Estandar
// Includes Suri
#include "wxGenericTableBase.h"
// Includes Wx
#include "wx/wx.h"
#include "wx/grid.h"
// Defines
// forwards

namespace suri {

class wxHotLinkEditor : public wxGridCellTextEditor {
public:
   /** ctor */
   explicit wxHotLinkEditor(wxGenericTableBase* pTable);
   /** dtor */
   virtual ~wxHotLinkEditor();
   /** Lo que se efectua cuando comienza la edicion de la celda */
   virtual void BeginEdit(int Row, int Column, wxGrid* pGrid);
   /** Creacion del objeto actual (es requerido por wxwidgets para inicializar el objeto) */
//   virtual void Create(wxWindow* pParent, wxWindowID Id, wxEvtHandler* pEventHandler);
   /** Lo que se efectua cuando finaliza la edicion de la celda */
   virtual bool EndEdit(int Row, int Column, wxGrid* pGrid);
   /** Reseteo del valor original de la celda */
//   virtual void Reset();
   /** Clonacion del objeto actual */
//   virtual wxGridCellEditor* Clone() const;
   /** Obtencion de valor de la celda */
//   virtual wxString GetValue() const;



private:
   wxGenericTableBase* pTable_;  /*! Puntero a la tabla */
   wxString oldValue_;           /*! Valor viejo */
   wxString newValue_;           /*! Valor nuevo */
};

} /** namespace suri */

#endif /* WXHOTLINKEDITOR_H_ */
