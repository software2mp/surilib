/*! \file HtmlTreePart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef HTMLTREEPART_H_
#define HTMLTREEPART_H_

// Includes standard
// Includes Suri
#include "suri/Part.h"
#include "suri/LayerList.h"
#include "FiltredElementListWidget.h"

// Includes Wx
// Includes App
// Defines

namespace suri {
/** Part que encapsula al arbol html */
/**
 * Part que encapsula al arbol html.
 *
 * todo extraer la logica de classselectionpart
 */
class HtmlTreePart : public suri::Part {
// Ctor de copia
   HtmlTreePart(const HtmlTreePart &) {
   }
   ;

   /** ID del panel del part dentro del xrc */
   static const std::string HTML_TREE_XRC_NAME;

public:
   HtmlTreePart(LayerList* pLayerList = NULL);
   virtual ~HtmlTreePart();

   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valor's originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Permite a clases que hereden modificar el widget luego de crear la ventana */
   virtual bool ConfigureWidget();

protected:
private:
   LayerList *pLayerList_;
};
}

#endif /* HTMLTREEPART_H_ */
