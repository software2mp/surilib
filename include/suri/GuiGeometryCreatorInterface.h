/** \file GuiGeometryCreatorInterface.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GUIGEOMETRYCREATORINTERFACE_H_
#define GUIGEOMETRYCREATORINTERFACE_H_

// Includes suri
#include "suri/Coordinates.h"

namespace suri {

/**
 * Interfaz que brinda VectorEditorTask para crear geometrias
 */
class GuiGeometryCreatorInterface {
public:
   /** Dtor */
   virtual ~GuiGeometryCreatorInterface() {}

   /** Agrega un punto al final de la geometria. */
   virtual bool AppendPoint(const Coordinates &PointCoordinate) = 0;
   /** Elimina el ultimo punto que agregue a la geometria */
   virtual bool RemoveLastPoint() = 0;
   /** Mueve el punto fantasma. */
   virtual bool MovePhantomPoint(const Coordinates &NewCoordinate) = 0;
   /** Posiciona el punto fantasma sobre el ultimo punto agregado */
   virtual bool HidePhantomPoint() = 0;
};

} /* namespace suri */
#endif /* GUIGEOMETRYCREATORINTERFACE_H_ */
