/*! \file Viewer3DWorldExtentManager.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWER3DWORLDEXTENTMANAGER_H_
#define VIEWER3DWORLDEXTENTMANAGER_H_

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
 * WorldExtentManager para Viewer3D
 */
class Viewer3DWorldExtentManager : public WorldExtentManager {
public:
   /** Constructor */
   Viewer3DWorldExtentManager();
   /** Destructor */
   virtual ~Viewer3DWorldExtentManager();
   /** Metodo que actualiza el mundo */
   virtual void SetExtent();
   /** Devuelve un puntero a LayerList */
   virtual LayerList* GetLayerList();
   /** Devuelve un puntero a World */
   virtual World* GetWorld();
};

} /** namespace suri */

#endif /* VIEWER3DWORLDEXTENTMANAGER_H_ */
