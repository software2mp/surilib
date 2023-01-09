/*! \file DatasourceInterface.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <wx/wx.h>
#include <string>
// Includes Suri
#include "suri/DatasourceInterface.h"
#include "suri/AbstractFactory.h"
#include "suri/Element.h"
#include "suri/Image.h"
#include "suri/Vector.h"

// Includes Wx

// Defines

// forwards

namespace suri {
/** Macro de la factoria que crea metodos de implementacion */
HAS_FACTORY_IMPLEMENTATION_COMMON(DatasourceInterface, 0)

/** Macro de la factoria que crea metodos de implementacion */
HAS_FACTORY_IMPLEMENTATION(
      DatasourceInterface, 2,
      DeclareParameters_2(const std::string&, const Option&))

/** Macro de la factoria que crea metodos de implementacion */
HAS_FACTORY_IMPLEMENTATION(DatasourceInterface, 1,
                           DeclareParameters_1(const wxXmlNode*))
/** Macro de la factoria que crea metodos de implementacion */
HAS_FACTORY_IMPLEMENTATION(DatasourceInterface, 1, DeclareParameters_1(Element*))

/** Macro de la factoria que crea metodos de implementacion */
HAS_FACTORY_IMPLEMENTATION(DatasourceInterface, 1, DeclareParameters_1(DatasourceInterface*))
/** Constructor
 *  @param[in] DataType
 **/
DatasourceInterface::DatasourceInterface(const wxString& DataType) : blocked_(false),
                                                                     readonly_(false) {
}

/** Destructor */
DatasourceInterface::~DatasourceInterface() {
}

/**
 * Inicializa una los atributos correspondientes. Se encuentra en desuso por ahora
 * @param[in] FileName
 */
DatasourceInterface *DatasourceInterface::Initialize(const std::string& FileName,
                                                     const Option& Metadata) {
   REPORT_DEBUG("Se esta llamando al inicializar de DatasourceInterface!!");
   return NULL;
}

/**
 * Inicializa una los atributos correspondientes. Se encuentra en desuso por ahora
 * @param[in] pRootNode
 */
DatasourceInterface *DatasourceInterface::Initialize(const wxXmlNode* pRootNode) {
   REPORT_DEBUG("Se esta llamando al inicializar de DatasourceInterface!!");
   return NULL;
}

/**
 * Inicializa una los atributos correspondientes. Se encuentra en desuso por ahora
 * @param[in] pElement
 */
DatasourceInterface *DatasourceInterface::Initialize(Element *pElement) {
   REPORT_DEBUG("Se esta llamando al inicializar de DatasourceInterface!!");
   return NULL;
}

/**
 * Inicializa una los atributos correspondientes. Se encuentra en desuso por ahora
 * @param[in] pElement
 */
DatasourceInterface *DatasourceInterface::Initialize(DatasourceInterface *pDatasource) {
   REPORT_DEBUG("Se esta llamando al inicializar de DatasourceInterface!!");
   return NULL;
}

/** Indica si la fuente de datos se encuentra bloqueada **/
bool DatasourceInterface::IsBlocked() const {
   return blocked_;
}

/** Bloquea la fuente de datos **/
bool DatasourceInterface::Block() {
   if (blocked_)
      return false;
   blocked_ = true;
   return true;
}

/** Bloquea la fuente de datos **/
bool DatasourceInterface::Unblock() {
   if (!blocked_)
      return false;
   blocked_ = false;
   return true;
}

/** Retorna si la fuente de datos es de solo lectura **/
bool DatasourceInterface::IsReadOnly() const {
   return readonly_;
}

/** Bloquea la edicion de la fuente de datos **/
bool DatasourceInterface::SetReadOnly() {
   if (readonly_)
      return false;
   readonly_ = true;
   return true;
}

/** Bloquea la edicion de la fuente de datos **/
bool DatasourceInterface::UnsetReadOnly() {
   if (!readonly_)
      return false;
   readonly_ = false;
   return true;
}
} /** namespace suri */
