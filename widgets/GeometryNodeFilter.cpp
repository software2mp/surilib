/*! \file GeometryNodeFilter.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/messages.h"
#include "GeometryNodeFilter.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/VectorElement.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 * @param[in] pDatasourceManager administrador de fuente de datos
 * @param[in] FilterType tipo de filtro que se desea aplicar sobre las geometrias
 */
GeometryNodeFilter::GeometryNodeFilter(DatasourceManagerInterface* pDatasourceManager,
                                       int FilterType) :
                                       pDatasourceManager_(pDatasourceManager),
                                       filterType_(FilterType) {
}

/** Destructor **/
GeometryNodeFilter::~GeometryNodeFilter() {
}

/**
 * Clona instancia de filtro.
 * @return copia de filtro.
 */
NodeFilterInterface* GeometryNodeFilter::Clone() const {
   return new GeometryNodeFilter(pDatasourceManager_, filterType_);
}

/**
 * Retorna true si el nodo esta filtrado.
 * @param[in] pNode nodo a analizar
 * @return bool que indica si hay que filtrarlo
 */
bool GeometryNodeFilter::IsFiltred(TreeNodeInterface* pNode) const {
   if (pNode->AllowsChildren()) {
      return IsGroupFiltered(pNode);
   }
   return IsNodeFiltered(pNode);
}

/** Verifica el contenido de un grupo para saber si el mismo se filtra **/
bool GeometryNodeFilter::IsGroupFiltered(TreeNodeInterface* pNode) const {
   return IsNodeFiltered(pNode);
}

/** Verifica el contenido de un nodo hoja para ver si se debe filtrar el mismo **/
bool GeometryNodeFilter::IsNodeFiltered(TreeNodeInterface* pNode) const {
   DatasourceInterface* pdatasource = pDatasourceManager_->GetDatasource(pNode->GetContent());
   if (!pdatasource)
      return true;
   VectorElement* pelement = dynamic_cast<VectorElement*>(pdatasource->GetElement());
   if (!pelement)
      return true;

   bool filtered = true;
   std::string type = pelement->GetTypeAsString().c_str();
   if (type.compare(TYPE_DECORATOR(message_POLYGON).c_str()) == 0) {
      filtered = !(filterType_ & POLYGON_FILTER);
   } else if (type.compare(TYPE_DECORATOR(message_POINT).c_str()) == 0) {
      filtered = !(filterType_ & POINT_FILTER);
   } else if (type.compare(TYPE_DECORATOR(message_LINE).c_str()) == 0) {
      filtered = !(filterType_ & LINE_FILTER);
   }
   return filtered;
}
} /** namespace suri */
