/*! \file ElementManager.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <list>

// Includes Suri
#include "suri/ElementManager.h"

// Includes Wx

// Includes OGR

// Defines

/** namespace suri */
namespace suri {

/**
 * Constructor
 */
ElementManagerClass::ElementManagerClass() :
      pAddFunc_(NULL), pDeleteFunc_(NULL), pDeletesFunc_(NULL) {
}

/**
 * Destructor
 */
ElementManagerClass::~ElementManagerClass() {}

/**
 * Llama a la funcion callback para agregar un elemento
 * @param[in] pElementNode Nodo del elemento
 * @return True en caso de exito
 * @return False si no esta registrada la funcion o error al agregar.
 */
bool ElementManagerClass::AddElement(wxXmlNode *pElementNode) {
   if (pAddFunc_) {
      return pAddFunc_(pDatasourceManager_, pElementNode);
   }
   REPORT_AND_FAIL_VALUE("D: No se resgistro ninguna funcion para agregar elementos.", false);
}

/**
 * Llama a la funcion callback para eliminar un elemento
 * @param[in] ElementUid Identificador del elemento
 * @return True en caso de exito
 * @return False si no esta registrada la funcion o error al agregar.
 */
bool ElementManagerClass::DeleteElement(const Element::UidType& ElementUid) {
   if (pDeleteFunc_) {
      return pDeleteFunc_(pDatasourceManager_, ElementUid);
   }
   REPORT_AND_FAIL_VALUE("D: No se resgistro ninguna funcion para eliminar elementos.", false);
}

/**
 * Llama a la funcion callback para eliminar un elemento
 * @param[in] ElementUids Lista con Identificadores de los elementos
 * @return True en caso de exito
 * @return False si no esta registrada la funcion o error al agregar.
 */
bool ElementManagerClass::DeleteElements(
      const std::list<suri::Element::UidType>& ElementUids) {
   if (pDeletesFunc_) {
      return pDeletesFunc_(pDatasourceManager_, ElementUids);
   }
   REPORT_AND_FAIL_VALUE("D: No se resgistro ninguna funcion para eliminar elementos.", false);
}

/**
 * Registra la funcion
 * @param[in] pFunction Funcion callback
 */
void ElementManagerClass::RegisterAddElement(
      DatasourceManagerInterface* DatasourceManager, AddElementFunction pFunction) {
   pDatasourceManager_ = DatasourceManager;
   pAddFunc_ = pFunction;
}

/**
 * Registra la funcion
 * @param[in] pFunction Funcion callback
 */
void ElementManagerClass::RegisterDeleteElement(
      DatasourceManagerInterface* DatasourceManager, DeleteElementFunction pFunction) {
   pDatasourceManager_ = DatasourceManager;
   pDeleteFunc_ = pFunction;
}

/**
 * Registra la funcion
 * @param[in] pFunction Funcion callback
 */
void ElementManagerClass::RegisterDeleteElements(
      DatasourceManagerInterface* DatasourceManager, DeleteElementsFunction pFunction) {
   pDatasourceManager_ = DatasourceManager;
   pDeletesFunc_ = pFunction;
}

/**
 * Obtiene la instancia. Singleton
 */
ElementManagerClass& ElementManager::Instance() {
   static ElementManagerClass object;
   return object;
}

}  // namespace suri

