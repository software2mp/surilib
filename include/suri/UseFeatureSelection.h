/*! \file UseFeatureSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes Estandar
#include <vector>
// Includes Suri
#include "LayerInterface.h"
#include "VectorElement.h"
#include "VectorEditor.h"
#include "VectorDatasource.h"
#include "LayerHtmlListItem.h"
// Includes Wx
// Defines
// forwards


#ifndef __USE_FEATURE_SELECTION__
#define __USE_FEATURE_SELECTION__
namespace suri {

class UseFeatureSelection{
protected:
  /**
   * Devuelve un  HotLinkVectorLayer* con un elemento generado en memoria a partir de
   * la seleccion de pElement.
   */
   LayerInterface* CreateSelectionList(Element* pElement);
   /**
    * Crea un vector en memoria del tipo indicado.
    */
   VectorEditor* CreateMemoryVector(
                          const std::string& VectorName, std::map<std::string, std::string>& Geoms,
                           const std::string& Srs);
   /**
    *Crea un LayerHtmlListItem dado un elmento con una seleccion , devuelve NULL  si el elemento
    *es NULL, o si el elemento no tiene una seleccion.
    */
   ui::LayerHtmlListItem* GetItemSelection(Element* pElement);

   /** Devuelve el Datasource indicado por el id, NULL en caso de no hallarlo */
   VectorDatasource* GetVectorDatasourceFromLayer(SuriObject::UuidType LayerId);

   /** Devuelve el vector datasource dado su id */
   VectorDatasource* GetVectorDatasource(SuriObject::UuidType DatasourceId);

   /** fuente de datos de la seleccion en memoria **/
   std::vector<VectorDatasource*> pSelection_;

   /** Dado un elemento devuelve el datasource de su seleccion */
   VectorDatasource* GetSelectedVectorDatasource(Element* pElement);

   /** mapea los ids de las layers con los datasources */
   std::map<SuriObject::UuidType, VectorDatasource*> selectionMapFromLayer_;
};

}
#endif
