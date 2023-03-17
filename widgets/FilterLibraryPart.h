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

#ifndef FILTERLIBRARYPART_H_
#define FILTERLIBRARYPART_H_

#include "suri/FilterPart.h"

namespace suri {

class FilterLibraryPartEvent;

class FilterLibraryPart: public FilterPart {
public:
   /**
    * Ctor.
    */
   explicit FilterLibraryPart(LibraryManager* pManager, int Filters = NO_FILTER);

   /**
    * Dtor.
    */
   virtual ~FilterLibraryPart();

   /** Crea la ventana de la parte. */
   virtual bool CreateToolWindow();

   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();

   /** Por defecto la lista de bandas esta vacia */
   virtual void SetInitialValues();

   /** Obtiene el icono de la parte. */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;

   /** Devuelve el nodo con el algebra de banda*/
   wxXmlNode* GetBandsEquationNode(int BandCount);

   /** Evento temporal */
   void OnUiUpdate(wxUpdateUIEvent &Event);

protected:
   /** Obtiene el tipo de filtro que se va a aplicar */
   FilterProcess::FilterType GetFilterType();

   /** Devuelve LibraryFilter */
   virtual FilterProcess::FilterType GetSelectedFilterType();

   /** Implementa interfaz*/
   virtual void SetSelectedFilterType(FilterProcess::FilterType Type);

   /** Devuelve la lista con los filtros */
   std::list<std::vector<std::vector<double> > > GetFilterMatrix(
         FilterProcess::FilterType Type, int Size);

   /**
    * NO SE IMPLEMENTA.
    */
   virtual void GetSelectedKernelSize(FilterProcess::FilterType Type, int &Rows,
                                      int &Columns);

   /**
    * NO SE IMPLEMENTA.
    */
   virtual void SetSelectedKernelSize(FilterProcess::FilterType Type, int Rows,
                                      int Columns);
private:
   EVENT_OBJECT_PROPERTY(FilterLibraryPartEvent);
   bool selected_;
};

}  // namespace suri

#endif /* FILTERLIBRARYPART_H_ */
