/*! \file ActiveRasterWorldExtentManager.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ACTIVERASTERWORLDEXTENTMANAGER_H_
#define ACTIVERASTERWORLDEXTENTMANAGER_H_

// Includes standard

// Includes suri
#include "WorldExtentManager.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {

// Forwards
class LayerList;
class World;

/** Clase que permite generar un subset con los extents de todos raster activos */
/**
 * Clase que recibe una lista de elementos y un mundo, y asigna al extent del
 * mundo un subset que contenga los extent de todos los elementos activos de la
 * lista.
 */
class ActiveRasterWorldExtentManager : public WorldExtentManager {
public:
   /** Constructor */
   ActiveRasterWorldExtentManager(LayerList* pLayerList, World* pWorld);
   /** Destructor */
   virtual ~ActiveRasterWorldExtentManager();
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
}

#endif /*ACTIVERASTERWORLDEXTENTMANAGER_H_*/
