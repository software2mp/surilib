/*! \file ElementPropertiesWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
