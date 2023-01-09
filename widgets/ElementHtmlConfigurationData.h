/*! \file ElementHtmlConfigurationData.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ELEMENTHTMLCONFIGURATIONDATA_H_
#define ELEMENTHTMLCONFIGURATIONDATA_H_

// Includes standard
#include <map>

// Includes Suri
#include "suri/Element.h"
#include "View.h"
#include "HtmlConfigurationData.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

class wxSimpleHtmlTreeCtrl;

/**
 * Clase que genera la configuracion de un nodo de wxSimpleHtmlTreeCtrl
 * a partir del elemento de la lista.
 */
class ElementHtmlConfigurationData : public HtmlConfigurationData, View {
public:
   /** Ctor */
   ElementHtmlConfigurationData(Element* pElement, wxSimpleHtmlTreeCtrl* pWxTreeCtrl);
   /** Dtor */
   virtual ~ElementHtmlConfigurationData();
   /** Retorna elemento asociado al nodo */
   Element* GetElement() const;
   /** Actualiza el control con el contenido del elemento */
   void Update();

protected:
   /** Recibe el update */
   virtual void Update(Model *pModel);
   /** Recibe el aviso de delete */
   virtual void OnModelDelete(Model *pModel);

private:
   Element* pElement_; /*! Elemento asociado al nodo */
   wxSimpleHtmlTreeCtrl* pWxTreeCtrl_; /*! Permite actualizar interfaz despues */
   /* de Update */
};

}  //namespace

#endif /* ELEMENTHTMLCONFIGURATIONDATA_H_ */
