/*! \file ElementPropertiesWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
