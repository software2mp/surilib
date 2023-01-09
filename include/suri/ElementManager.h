/*! \file ElementManager.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ELEMENTMANAGER_H_
#define ELEMENTMANAGER_H_

// Includes standard
#include <list>

// Includes Suri
#include "suri/Element.h"
#include "suri/TSingleton.h"
#include "suri/DatasourceManagerInterface.h"

// Includes Wx

// Includes OGR

// Defines

/** namespace suri */
namespace suri {
// Forwards
class ElementManager;

/** Abstrae la logica de agregado y eliminado de elementos a la aplicacion */
/**
 *  Como la aplicacion es la que determina la forma en que se agregan y eliminan
 * elementos, y la biblioteca agrega y elimina en forma automatica, es necesario
 * tener una forma de que la biblioteca delegue en la aplicacion estas
 * operaciones. Para ello se utiliza esta clase que permite agrega y eliminar
 * elementos.
 */
class ElementManagerClass {
   /** ctor privado (para usar como singleton) */
   ElementManagerClass();
   /** ctor de copia privado (para usar como singleton) */
   ElementManagerClass(const ElementManagerClass&);
   /** dtor privado (para usar como singleton) */
   virtual ~ElementManagerClass();

public:
// Codigo previo usando el template singleton que no se puede exportar en dll
#ifdef __UNUSED_CODE__
   /** clase singleton "friend" para que la pueda crear. */
   friend class TSingleton<ElementManagerClass>;
#else
   friend class ElementManager;
#endif
   /** tipo de la funcion callback que agrega elementos */
   typedef bool (*AddElementFunction)(DatasourceManagerInterface*, wxXmlNode*);
   /** tipo de la funcion callback que elimina 1 elemento */
   typedef bool (*DeleteElementFunction)(DatasourceManagerInterface*, const Element::UidType&);
   /** tipo de la funcion callback que elimina N elementos */
   typedef bool (*DeleteElementsFunction)(DatasourceManagerInterface*, const std::list<suri::Element::UidType>&);

   /** Metodo que agrega elementos */
   bool AddElement(wxXmlNode *pElementNode);
   /** Metodo que elimina 1 elemento */
   bool DeleteElement(const Element::UidType& ElementUid);
   /** Metodo que elimina N elementos */
   bool DeleteElements(const std::list<suri::Element::UidType>& ElementUids);
   /** Registra la funcion llamada en AddElement */
   void RegisterAddElement(DatasourceManagerInterface*, AddElementFunction pFunction);
   /** Registra la funcion llamada en DeleteElement */
   void RegisterDeleteElement(DatasourceManagerInterface*, DeleteElementFunction pFunction);
   /** Registra la funcion llamada en DeleteElements */
   void RegisterDeleteElements(DatasourceManagerInterface*, DeleteElementsFunction pFunction);

private:
   AddElementFunction pAddFunc_; /*! funcion para agregar */
   DeleteElementFunction pDeleteFunc_; /*! funcion para eliminar */
   DeleteElementsFunction pDeletesFunc_; /*! funcion para eleiminar N */

   /** Para reutilizar los callbacks con el DatasourceManager. Es el mismo para todos **/
   DatasourceManagerInterface *pDatasourceManager_;
};

// Codigo previo usando el template singleton que no se puede exportar en dll
#ifdef __UNUSED_CODE__
/** Tipo singleton de la clase para administrar elementos */
typedef TSingleton<ElementManagerClass> ElementManager;
#else
class ElementManager {
   ElementManager() {
   }
   ElementManager(const ElementManager&);
   ElementManager& operator=(const ElementManager&);

public:
   virtual ~ElementManager() {
   }
   static ElementManagerClass& Instance();
};
#endif
}

#endif /* ELEMENTMANAGER_H_ */
