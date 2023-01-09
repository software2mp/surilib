/*! \file WorldExtentManager.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WORLDEXTENTMANAGER_H_
#define WORLDEXTENTMANAGER_H_

// Includes standard

// Includes suri

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {

// Forwards
class LayerList;
class World;

/** Interfaz que permite definir la forma en que se modifica la ventana. */
/**
 * Renderization manager utiliza SetExtent cada vez que se
 * renderiza para determinar el extent del mundo.
 * El objetivo de esta clase es permitir mayor control sobre el
 * algoritmo que se usa a las clases externas.
 * Se puede utilizar WorldExtentManager directamente para que
 * no se modifique el mundo al renderizar
 */
class WorldExtentManager {
public:
   /** Constructor */
   WorldExtentManager() {};
   virtual ~WorldExtentManager() {};
   /** Metodo que actualiza el mundo */
   virtual void SetExtent()=0;
   /** Devuelve un puntero a LayerList */
   virtual LayerList* GetLayerList()=0;
   /** Devuelve un puntero a World */
   virtual World* GetWorld()=0;
};

}

#endif /*WORLDEXTENTMANAGER_H_*/
