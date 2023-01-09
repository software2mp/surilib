/*! \file LayerFactory.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LAYERFACTORY_H_
#define LAYERFACTORY_H_


// Includes Estandar

// Includes Suri
#include "suri/DatasourceInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

// Forwards Suri
class TerrainElement;

class LayerInterface;

/** clase que colabora con LayerInterface para
 *  crear las instancias correspondientes.
 *  Sustito de AbstractFactory para este caso
 *  No se encarga de destruir las instancias que crea
 */
class LayerFactory {
public:
   static LayerInterface* CreateLayer(DatasourceInterface* pDatasource);

private:
   /** Metodo auxiliar para la creacion de la capa a partir de un element*/
   static LayerInterface* CreateLayerFromElement(
         DatasourceInterface* pDatasource, Element* pElement);
   /** Metodo auxiliar para la copia de la capa activa de los ds vectoriales **/
   static void SetActiveLayerFromElement(Element* pElement, Element* pElementCopy);
};
} /* namespace suri*/
#endif /* LAYERFACTORY_H_ */
