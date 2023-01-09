/*! \file HtmlTreePart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "HtmlTreePart.h"

namespace suri {
const std::string HtmlTreePart::HTML_TREE_XRC_NAME = wxT("ID_HTML_TREE_MAIN_PANEL");

HtmlTreePart::HtmlTreePart(LayerList* pLayerList) :
      Part(HTML_TREE_XRC_NAME) {
   pLayerList_ = pLayerList;
}

HtmlTreePart::~HtmlTreePart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool HtmlTreePart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool HtmlTreePart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool HtmlTreePart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void HtmlTreePart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void HtmlTreePart::Update() {
}

/** Permite a clases que hereden modificar el widget luego de crear la ventana */
bool HtmlTreePart::ConfigureWidget() {
   Widget *ptree = new FiltredElementListWidget(GetWidget()->GetWindow(), pLayerList_);

   GetWidget()->AddControl(ptree, "ID_HTML_TREE_PANEL");

   return true;
}
}
