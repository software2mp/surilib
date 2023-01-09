/*! \file GeometryNodeFilter.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOMETRYNODEFILTER_H_
#define GEOMETRYNODEFILTER_H_

// Includes Estandar
// Includes Suri
#include "suri/NodeFilterInterface.h"

// Includes Wx
// Defines

namespace suri {

// Forwards
class VectorElement;
class DatasourceManagerInterface;

/*!
 * Clase que representa un filtro para
 * el recorrido de los nodos de un arbol
 * que filtra por tipo de geometria
 */
class GeometryNodeFilter : public NodeFilterInterface {
public:
   /** posibles filtros, pueden sumarse aplicando OR */
   static const int POLYGON_FILTER = 0x01;
   static const int POINT_FILTER = 0x02;
   static const int LINE_FILTER = 0x04;
   /** constructor **/
   GeometryNodeFilter(DatasourceManagerInterface* pDatasourceManager,
                      int FilterType);
   /** destructor **/
   virtual ~GeometryNodeFilter();
   /**
    * Clona instancia de filtro.
    * @return copia de filtro.
    */
   virtual NodeFilterInterface* Clone() const;
   /**
    * Retorna true si el nodo esta filtrado.
    * @param[in] pNode nodo a analizar
    * @return bool que indica si hay que filtrarlo
    */
   virtual bool IsFiltred(TreeNodeInterface* pNode) const;

private:
   /** Verifica el contenido de un grupo para saber si el mismo se filtra **/
   bool IsGroupFiltered(TreeNodeInterface* pNode) const;
   /** Verifica el contenido de un nodo hoja para ver si se debe filtrar el mismo **/
   bool IsNodeFiltered(TreeNodeInterface* pNode) const;
   /** administrador de fuente de datos para la obtencion de las caracteristicas de la
    *  fuente de datos a filtrar */
   DatasourceManagerInterface* pDatasourceManager_;
   /** configuracion activa del filtro **/
   int filterType_;
};

} /** namespace suri */

#endif /* GEOMETRYNODEFILTER_H_ */
