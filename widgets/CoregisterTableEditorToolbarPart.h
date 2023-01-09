/*! \file CoregisterTableEditorToolbarPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COREGISTERTABLEEDITORTOOLBARPART_H_
#define COREGISTERTABLEEDITORTOOLBARPART_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"
// Includes Wx

// Includes App

// Defines

/** suri::CoregisterTableEditorToolbarPart: comment */
/*
 *
 */

namespace suri {
class CoregisterTableEditorToolbarPart : public suri::Part {
public:
   CoregisterTableEditorToolbarPart();
   virtual ~CoregisterTableEditorToolbarPart();

   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado del Part */
   virtual void Update();

private:
   static const std::string VECTOR_COREGISTER_TABLE_TOOLBAR_XRC_NAME;
protected:
};
}  // namespace suri

#endif /* COREGISTERTABLEEDITORTOOLBARPART_H_ */
