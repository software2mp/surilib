/*! \file PixelLineInputPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PIXELLINEINPUTPART_H_
#define PIXELLINEINPUTPART_H_

#include "suri/Part.h"

namespace suri {
class PixelLineInputPart : public suri::Part {
public:
   /** Ctor */
   PixelLineInputPart();
   /** Dtor */
   virtual ~PixelLineInputPart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa los datos mostrados */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
};
}

#endif /* PIXELLINEINPUTPART_H_ */
