/*! \file LatitudeLongitudeInputPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LATITUDELONGITUDEINPUTPART_H_
#define LATITUDELONGITUDEINPUTPART_H_

// Includes Suri
#include "suri/Part.h"
#include "suri/LatitudeLongitudeInputWidget.h"

namespace suri {
/** Part de ingreso de coordenadas */
/**
 * Part de ingreso de coordenadas.
 * Encierra un widget de manejo de coordenadas
 * todo: pasar la logica de widget al part.
 */
class LatitudeLongitudeInputPart : public suri::Part {
public:
   /** Ctor. */
   LatitudeLongitudeInputPart();
   /** Dtor. */
   virtual ~LatitudeLongitudeInputPart();
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool SetCoordinate(Coordinates LatLongCoord);
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valor's originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();

protected:
private:
// Widget interno
// todo: ver como manejar el acceso a los widgets internos
   LatitudeLongitudeInputWidget *pCoordInput;
};
}

#endif /* LATITUDELONGITUDEINPUTPART_H_ */
