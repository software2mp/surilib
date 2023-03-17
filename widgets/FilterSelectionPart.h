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

#ifndef FILTERSELECTIONPART_H_
#define FILTERSELECTIONPART_H_

// Includes standard
#include <list>
#include <vector>
#include <map>

// Includes suri
#include "suri/FilterPart.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xml/xml.h"

/** namespace suri */
namespace suri {

// Forwards
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(FilterSelectionPartEvent);

/** Flag para que se muestre filtro pasa bajos */
#define NO_FILTER                 0x00000000
/** Flag para que se muestre filtro pasa bajos */
#define LOWPASS_FILTER                 0x00000001
/** Flag para que se muestre filtro pasa altos */
#define HIGHPASS_FILTER              0x00000002
/** Flag para que se muestre filtro deteccion de borde */
#define EDGEDETECTION_FILTER            0x00000004
/** Flag para que se muestre filtro para encontrar el menos frecuente */
#define MINORITY_FILTER     0x00000008
/** Flag para que se muestre opcion para encontrar el mas frecuente */
#define MAYORITY_FILTER     0x00000010

/** Clase que crea una parte que sirve para seleccionar tipo y tamanio del filtro */
/**
 * Parte para la configuracion del filtro a aplicar sobre la imagen.
 * En base a los parametros seleccionados crea el filtro correspondiente.
 * Crea una lista de filtros, repitiendo el filtro generado tantas veces como
 * bandas de la imagen se seleccionaron. Con esa lista se configura una variable
 * de tipo parameters, a la que tambien se le configura las dimensiones del
 * kernel; con esta variable se obtiene un nodo xml del filtro de convolucion,
 * que es el nodo que devuelve la herramienta.
 */
class FilterSelectionPart : public FilterPart{
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(FilterSelectionPartEvent);
   /** Ctor. */
   explicit FilterSelectionPart(LibraryManager* pManager, int Filters = NO_FILTER);
   /** Dtor. */
   virtual ~FilterSelectionPart();

   //  ---------- Metodos de part ----------
   /** Crea la ventana de la parte y contecta el evento UIUpdate. */
   virtual bool CreateToolWindow();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Por defecto la lista de bandas esta vacia */
   virtual void SetInitialValues();
   /** Devuelve el icono de la parte */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Evento temporal */
   void OnUiUpdate(wxUpdateUIEvent &Event);

   // ---------- Permite acceder a los datos configurados ---------
   /** Obtiene el tipo de filtro que se va a aplicar */
   FilterProcess::FilterType GetFilterType();

   // ---------- Metodos que configuran el nodo -----------
   /** Devuelve el nodo con el algebra de banda segun el filtro seleccionado */
   wxXmlNode* GetBandsEquationNode(int BandCount);
   /** Devuelve el nodo de renderizacion */
   wxXmlNode* CreateFilterRenderizationNode(std::vector<int> &BandList);
   /** Crea el nodo con el algebra de bandas correspondiente al filtro deteccion de bordes */
   wxXmlNode* CreateEdgeDetectionFilterEquationNode(int BandCount);

protected:
   // ---------- Metodos que interactuan con la gui ----------
   /** Obtiene el tipo de filtro seleccionado en el control wx */
   FilterProcess::FilterType GetSelectedFilterType();
   /** Configura el tipo de filtro seleccionado en el control wx */
   void SetSelectedFilterType(FilterProcess::FilterType Type);
   /** Obtiene el tamanio del kernel seleccionado del control wx */
   void GetSelectedKernelSize(FilterProcess::FilterType Type, int &Rows, int &Columns);
   /** Configura la dimension del kernel en interfaz grafica*/
   virtual void SetSelectedKernelSize(FilterProcess::FilterType Type, int Rows, int Columns);

   // ---------- Metodos que calcular matriz de convolucion ----------
   /** Devuelve la lista con los filtros */
   std::list<std::vector<std::vector<double> > > GetFilterMatrix(
         FilterProcess::FilterType Type, int Size);
   /** Crea una lista de filtros pasabajos */
   std::list<std::vector<std::vector<double> > > CreateLowPassFilter(int Size);
   /** Crea una lista de filtros pasaaltos */
   std::list<std::vector<std::vector<double> > > CreateHighPassFilter(int Size);
   /** Crea una lista de filtros de deteccion de bordes */
   std::list<std::vector<std::vector<double> > > CreateEdgeDetectionFilter();


private:
   /* eventos */
   EVENT_OBJECT_PROPERTY(FilterSelectionPartEvent); /*! Clase para conectar */
   int filters_; /*! Determina los filtros que se mostraran al usuario. Si -1 se muestran todos */
   std::map<FilterProcess::FilterType, int> suportedFilters_; /*! Guarda los filtros */
                                      /* soportados y su posicion en choice */

};
}

#endif /* FILTERSELECTIONPART_H_ */
