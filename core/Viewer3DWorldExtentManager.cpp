/*! \file Viewer3DWorldExtentManager.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <cstddef>

// Includes Suri
#include "Viewer3DWorldExtentManager.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
Viewer3DWorldExtentManager::Viewer3DWorldExtentManager() {
   // No hace nada por defecto
}

/**
 * Destructor
 */
Viewer3DWorldExtentManager::~Viewer3DWorldExtentManager() {
   // No hace nada por defecto
}

/**
 * Metodo que actualiza el mundo
 */
void Viewer3DWorldExtentManager::SetExtent() {
   // No hace nada por defecto
}

/**
 * Devuelve un puntero a LayerList
 * @return
 */
LayerList* Viewer3DWorldExtentManager::GetLayerList() {
   // Devuelve NULL por defecto
   return NULL;
}

/**
 * Devuelve un puntero a World
 * @return
 */
World* Viewer3DWorldExtentManager::GetWorld() {
   // Devuelve NULL por defecto
   return NULL;
}

} /** namespace suri */
