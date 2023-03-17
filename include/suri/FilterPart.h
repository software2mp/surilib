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

#ifndef FILTERPART_H_
#define FILTERPART_H_

// Includes Estandar
// Includes Suri
#include "FilterProcess.h"
#include "LibraryItemSelectionInterface.h"
#include "Part.h"
#include "ProcessAtributeProvider.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class LibraryManager;
class BasicLibraryItemSelector;

class FilterPart: public Part, public ProcessAtributeProvider,
   public LibraryItemSelectionInterface{
public:
   explicit FilterPart(LibraryManager* pManager, int Filters = NO_FILTER);
   virtual ~FilterPart();
   // ---------- Permite acceder a los datos configurados ---------
   /** Obtiene el tamanio del kernel del primer elemento de la lista de kernels */
   void GetKernelSize(int &Rows, int &Columns);
   /** Obtiene el tipo de filtro que se va a aplicar */
   virtual FilterProcess::FilterType GetFilterType() = 0;
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Crea la ventana de la parte. */
   virtual bool CreateToolWindow() = 0;
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Configura el editor con el item que se pasa por parametro. Se pasa por parametro
    * un boolean que indica si se trata de la primera seleccion (para que el part no piense
    * que posee cambios cuando en realidad es la seleccion inicial del CGU)*/
   void SetActiveItem(const LibraryItem* pItem);
   /** Actualiza el estado de la parte */
   virtual void Update();
protected:
   /** Devuelve la lista con los filtros */
   virtual std::list<std::vector<std::vector<double> > > GetFilterMatrix(
         FilterProcess::FilterType Type , int Size) = 0;
   /** Devuelve el nodo de renderizacion */
      wxXmlNode* CreateFilterRenderizationNode(std::vector<int> &BandList);
   // ---------- Metodos que interactuan con la gui ----------
   /** Obtiene el tipo de filtro seleccionado en el control wx */
   virtual FilterProcess::FilterType GetSelectedFilterType() = 0;
   /** Configura el tipo de filtro seleccionado en el control wx */
   virtual void SetSelectedFilterType(FilterProcess::FilterType Type) = 0;
   /** Obtiene el tamanio del kernel seleccionado del control wx */
   virtual void GetSelectedKernelSize(FilterProcess::FilterType Type, int &Rows, int &Columns) = 0;
   /** Configura la dimension del kernel en interfaz grafica*/
   virtual void SetSelectedKernelSize(FilterProcess::FilterType Type, int Rows, int Columns) = 0;
   // ---------- Metodos que configuran el nodo -----------
   /** Devuelve el nodo con el algebra de banda segun el filtro seleccionado */
   virtual wxXmlNode* GetBandsEquationNode(int BandCount) = 0;
   /** Devuelve la lista de filtros seleccionados */
   std::list<std::vector<std::vector<double> > > GetSelectedFilterList(int BandCount);
   /** Atributos **/
   LibraryManager* pManager_;
   BasicLibraryItemSelector* pItemSelector_;
   /* filtros */
   std::list<std::vector<std::vector<double> > > filterList_; /*! Lista con los */
   FilterProcess::FilterType selectedFilter_; /*! Tipo de filtro seleccionado */
   const LibraryItem* pSelectedFilter_;
};

} /** namespace suri */

#endif /* FILTERPART_H_ */
