/*! \file DecimalCoordinateInputPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DECIMALCOORDINATEINPUTPART_H_
#define DECIMALCOORDINATEINPUTPART_H_

// Includes standard
// Includes Suri
#include "suri/Part.h"

// Includes Wx
// Defines

namespace suri {
/** Permite agregar coordenadas de tipo decimal */
/**
 * Permite agregar coordenadas de tipo decimal
 * Esta compuesto por 2 areas de texto apra escribir
 * las coord.
 */
class DecimalCoordinateInputPart : public suri::Part {
   /** Nombre del XRC */
   static const std::string DECIMAL_COORD_INPUT_XRC_NAME;

public:
   /** Ctor */
   DecimalCoordinateInputPart();
   /** Dtor */
   virtual ~DecimalCoordinateInputPart();
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
};
}

#endif /* DECIMALCOORDINATEINPUTPART_H_ */
