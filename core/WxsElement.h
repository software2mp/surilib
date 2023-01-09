/*! \file WxsElement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXSELEMENT_H_
#define WXSELEMENT_H_

// Includes standard
#include <map>
// Includes Suri
#include "suri/RasterElement.h"
#include "suri/World.h"
// Includes Wx
// Defines

namespace suri {

/** Clase que representa imagenes remostas obtenidas mediante wms */
class WxsElement : public RasterElement {
private:
   /** Ctor de copia */
   WxsElement(const WxsElement &WxsElement);

public:
   /** Ctor */
   WxsElement();
   /** Dtor */
   ~WxsElement();

   /** Guarda el extent del raster en el nodo. */
   virtual void SetElementExtent(const Subset &WorldExtent);

   /** Devuelve los Part que se van a mostrar en las Propiedades. */
   virtual std::vector<Part *> DoGetParts(
         DataViewManager *pDataViewManager = NULL,
         LayerInterface *pLayer = NULL,
         DatasourceInterface *pDatasource = NULL);

protected:
   /** Parsea url y retorna datos de interes(url, layer, tipo de capa y estilo)*/
   bool ParseWxsFilename(const std::string &FileName,
                         std::map<std::string, std::string> &Keys);

private:
   /** Inicializa WxsElement con archivo en FileName */
   virtual RasterElement* Initialize(const std::string &FileName,
                                     const Option& Metadata = Option());

   /** Genera xml que requiere GDAL para abrir una imagen. */
   virtual std::string CreateWxsXml(const std::string &FileName, const Option& Metadata);

   /** Devuelve vector con Parts especificos de la visualizacion elemento. */
   virtual std::vector<suri::Part*> DoGetVisualizationParts(
         DataViewManager* pDataViewManager = NULL, LayerInterface *pLayer = NULL,
         DatasourceInterface *pDatasource = NULL);

};

}  //namespace

#endif /* WXSELEMENT_H_ */
