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

#ifndef FILTREDELEMENTLISTWIDGET_H_
#define FILTREDELEMENTLISTWIDGET_H_

// Includes standard
#include <list>
#include <string>

// Includes Suri
#include "suri/ElementListWidget.h"

// Includes Wx

// Includes App

// Defines

// Forwards

/** namespace suri */
namespace suri {

// Forwards
class DatasourceManagerInterface;

/** Muestra en una lista los elementos de un LayerList que son del tipo seleccionado */
/**
 * Muestra una lista de elementos de un tipo definido, y sus iconos asociados.
 *
 */
class FiltredElementListWidget : public ElementListWidget {
private:
   /** Ctor. de Copia. */
   FiltredElementListWidget(const FiltredElementListWidget &FiltredElementListWidget,
                            DatasourceManagerInterface *DatasourceManager);

public:
   /** Constructor */
   FiltredElementListWidget(wxWindow *pParent, suri::LayerList *pLayerList);
   /** Destructor */
   virtual ~FiltredElementListWidget();
   /** Determina si hay que mostrar el elemento */
   virtual bool CanShowElement(Element* pElement);
   /** Retorna el dibujo que hay que mostrar junto al elemento */
   virtual std::string GetElementIcon(Element* pElement);
   /** Agrega un tipo de elemento a la lista de tipos que se muestran en la lista */
   virtual void AddElementType(std::string TypeAsString);
   /** Limpia la lista de tipos de elementos desplegados */
   virtual void ClearElementTypes();
   /** Retorna los elementos seleccionados */
   virtual std::list<Element*> GetActiveElements();
   /** Limpia los elementos seleccionados */
   virtual void ClearActiveElements();

private:
   std::set<std::string> elementsTypesShown_; /*! Tipos de elementos que se muestran */
   std::map<Element*, std::string> elementIcons_; /*! Icono asociado a cada elemento */
   int fileIndex_; /*! indice para los elementos de la lista */
};

}

#endif /* FILTREDELEMENTLISTWIDGET_H_ */
