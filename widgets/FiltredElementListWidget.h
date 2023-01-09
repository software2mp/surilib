/*! \file FiltredElementListWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
