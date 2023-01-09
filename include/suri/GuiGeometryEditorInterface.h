/** \file GuiGeometryEditorInterface.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GUIGEOMETRYEDITORINTERFACE_H_
#define GUIGEOMETRYEDITORINTERFACE_H_

// Includes standard
#include <set>

// Includes suri
#include "suri/Subset.h"
#include "suri/SuriObject.h"

namespace suri {

/**
 * Interfaz que brinda VectorEditorTask para editar geometrias
 */
class GuiGeometryEditorInterface {
public:
   /** Ctor */
   virtual ~GuiGeometryEditorInterface() {}

   /** Selecciona un punto dentro del subset */
   virtual std::set<SuriObject::UuidType> FindPoints(const Subset &SearchSubset) = 0;
   /** Selecciona un punto con el id indicado */
   virtual bool SelectPoint(const SuriObject::UuidType &PointId) = 0;
   /** Cambia las coordenadas del punto seleccionado. */
   virtual bool MoveSelectedPoint(const Coordinates &NewCoordinate) = 0;
   /** Elimina el punto seleccionado de la geometria */
   virtual bool DeleteSelectedPoint() = 0;
   /** Retorna el id del punto seleccionado */
   virtual SuriObject::UuidType GetSelectedPoint() = 0;
};

} /* namespace suri */
#endif /* GUIGEOMETRYEDITORINTERFACE_H_ */
