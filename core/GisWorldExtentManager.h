/*! \file GisWorldExtentManager.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GISWORLDEXTENTMANAGER_H_
#define GISWORLDEXTENTMANAGER_H_

// Includes Estandar

// Includes Suri
#include "suri/WorldExtentManager.h"

// Includes Wx

// Defines

// forwards

namespace suri {

// Forwards
class LayerList;
class World;

/**
 * WorldExtentManager para Gis
 */
class GisWorldExtentManager : public WorldExtentManager {
public:
   /** Constructor */
   GisWorldExtentManager(LayerList* pLayerList, World* pWorld);
   /** Destructor */
   virtual ~GisWorldExtentManager();
   /** Metodo que actualiza el mundo */
   virtual void SetExtent();
   /** Devuelve un puntero a LayerList */
   virtual LayerList* GetLayerList();
   /** Devuelve un puntero a World */
   virtual World* GetWorld();

private:
   LayerList* pLayerList_; /*! Lista con elementos que generan el world extent */
   World* pWorld_; /*! Mundo donde se carga el nuevo extent */
};

} /** namespace suri */

#endif /* GISWORLDEXTENTMANAGER_H_ */
