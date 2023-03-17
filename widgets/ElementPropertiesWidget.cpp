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

// Includes standard

// Includes Suri
#include "suri/ElementPropertiesWidget.h"
#include "suri/Element.h"
#include "suri/messages.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DatasourceInterface.h"
#include "suri/LayerInterface.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/sstream.h"

// Includes App

// Defines

// Forwards

/** namespace suri */
namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ElementPropertiesWidgetEvent, ElementPropertiesWidget)
      // Implementar eventos aqui si los hubiese
END_IMPLEMENT_EVENT_OBJECT /** \endcond */

/**
 * Constructor
 * @param[in]  pElement: puntero al elemento.
 * @param[in]	Style: estilo de notebook(donde pone las pestanias)
 * @param[in]	Type: tipo de book(ver clase NotebookWidget)
 */
ElementPropertiesWidget::ElementPropertiesWidget(DataViewManager* pDataViewManager,
                                                 Element* pElement, long Style,
                                                 NotebookWidget::BookType Type) :
      NotebookWidget(_(caption_ELEMENT_PROPERTIES), Style, Type),
      NEW_EVENT_OBJECT(ElementPropertiesWidgetEvent),
      pElement_(pElement), pDataViewManager_(pDataViewManager), pDatasource_(NULL),pLayer_(NULL) {
   visibleButtons_ = SUR_BTN_OK | SUR_BTN_CANCEL | SUR_BTN_APPLY;
   CreateTool();
   InsertParts();
}

/**
 * Constructor
 */
ElementPropertiesWidget::ElementPropertiesWidget(DataViewManager* pDataViewManager,
                                                 DatasourceInterface* pDatasource,
                                                 LayerInterface* pLayer,
                                                 bool VisualizationProperties,
                                                 long Style,
                                                 NotebookWidget::BookType Type) :
      NotebookWidget(_(caption_ELEMENT_PROPERTIES), Style, Type),
      NEW_EVENT_OBJECT(ElementPropertiesWidgetEvent),
      pElement_(NULL), pDataViewManager_(pDataViewManager), pDatasource_(pDatasource),
      pLayer_(pLayer), visualizationProperties_(VisualizationProperties) {
   visibleButtons_ = SUR_BTN_OK | SUR_BTN_CANCEL | SUR_BTN_APPLY;
   CreateTool();
   InsertParts();
}

/**
 * Destructor
 */
ElementPropertiesWidget::~ElementPropertiesWidget() {
   DELETE_EVENT_OBJECT;
}

/**
 * Objetivo: Agrega al container los Part recibidos desde los Elementos.\n
 * Tambien carga el xml en el Part de edicion del xml.
 * @return Devuelve verdadero si pudo agregar, sino devuelve false
 */
bool ElementPropertiesWidget::InsertParts() {
   Element* pelement = (pDatasource_) ? pDatasource_->GetElement() : pElement_;
   std::vector<Part *> vectorparts;
   if (!visualizationProperties_) {
      vectorparts = pelement->GetParts(pDataViewManager_, pLayer_, pDatasource_);
   } else {
      vectorparts = pelement->GetVisualizationParts(pDataViewManager_, pLayer_, pDatasource_);
   }
   std::vector<Part *>::iterator it = vectorparts.begin();
   for (; it != vectorparts.end(); it++) {
      // si el elemento esta en edicion, las partes estan en modo solo lectura
      if (pelement->IsEdited()) {
         (*it)->SetEnabled(false);
      }
      if (!InsertPart((*it))) {
         REPORT_DEBUG("D:Error insertando Part.");
      }
   }

   return true;
}

} /** namespace suri */
