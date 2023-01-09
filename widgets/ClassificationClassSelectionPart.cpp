/*! \file ClassificationClassSelectionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "ClassificationClassSelectionPart.h"

// Includes standard

// Includes Suri
#include "HtmlTreePart.h"

// Includes Wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"
// Includes App

// Defines

namespace suri {
/** Ctor */
ClassificationClassSelectionPart::ClassificationClassSelectionPart(
      LayerList* pLayerList, bool NonClassifiedClassParametersSelector) :
      PartCollection(wxT("ID_CLASS_SELECTION_PANEL"), caption_CLASS_SELECTION_PART) {
   Part *phtmltree = new HtmlTreePart(pLayerList);
   AddPart(phtmltree, wxT("ID_LAYER_LIST_PANEL"));

   // Seteo flag para ocultar part con propiedades de clase 'Sin clase'
   nonClassifiedClassParametersSelector_ = NonClassifiedClassParametersSelector;
}

/** Dtor */
ClassificationClassSelectionPart::~ClassificationClassSelectionPart() {
}

/* Oculto propiedades de clase 'sin clase'
 * Es necesario para el caso de estadisticas. */
bool ClassificationClassSelectionPart::ConfigureWidget() {
   PartCollection::ConfigureWidget();
   if (!nonClassifiedClassParametersSelector_) {
      GetWidget()->Hide("ID_NONCLASSIFIEDCLASS_PROPERTIES_STATICBOX");
   }

   return true;
}
}
