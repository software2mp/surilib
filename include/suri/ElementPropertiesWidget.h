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

#ifndef ELEMENTPROPERTIESWIDGET_H_
#define ELEMENTPROPERTIESWIDGET_H_

// Includes suri
#include "suri/Widget.h"
#include "suri/NotebookWidget.h"

// Includes standard

// Includes Suri

// Includes Wx

// Includes App

// Defines

// Forwards
class wxCommandEvent;

/** namespace suri */
namespace suri {

// Forwards
class Element;
class DataViewManager;
class DatasourceInterface;
class LayerInterface;

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(ElementPropertiesWidgetEvent);

/** Clase/Herramienta que muestra las propiedades de un Element */
/**
 * Esta clase tiene la ventana con las propiedades de un elemento del contexto
 * que permite verlas y modificarlas.
 * \note hereda de NotebookWidget ya que su GUI es un notebook donde se agrupan
 * las propiedades (por funcionalidad) en cada pestania.
 */
class ElementPropertiesWidget : public NotebookWidget {
   /** Ctor. de Copia. */
   ElementPropertiesWidget(const ElementPropertiesWidget &ElementPropertiesWidget);

public:
   /** ctor \deprecated **/
   ElementPropertiesWidget(
		   DataViewManager* pDataViewManager, Element *pElement,
                           long Style = wxNB_LEFT, NotebookWidget::BookType Type =
                                 Listbook);
   /** Constructor */
   ElementPropertiesWidget(
         DataViewManager *pDataViewManager,
                           DatasourceInterface *pDatasource, LayerInterface* pLayer,
                           bool VisualizationProperties = false,
                           long Style = wxNB_LEFT, NotebookWidget::BookType Type =
                                 Listbook);
   /** Destructor */
   virtual ~ElementPropertiesWidget();
   /** Agrega los Parts */
   bool InsertParts();

private:
   /*! Objeto que conecta los eventos */
   EVENT_OBJECT_PROPERTY(ElementPropertiesWidgetEvent);
   /** Referencia al elemento que muestra las propiedades \deprecated*/
   Element* pElement_;
   /** Administrador de informacion de la aplicacion **/
   DataViewManager *pDataViewManager_;
   /** Puntero a fuente de datos */
   DatasourceInterface *pDatasource_;
   /** Puntero a capa */
   LayerInterface* pLayer_;
   /** bool que indica que se trata de las propiedades de visualzacion de
    *  la capa
    */
   bool visualizationProperties_;
};
}  // namespace suri

#endif /*ELEMENTPROPERTIESWIDGET_H_*/
