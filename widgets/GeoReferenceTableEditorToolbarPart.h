/*! \file GeoReferenceTableEditorToolbarPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOREFERENCETABLEEDITORTOOLBARPART_H_
#define GEOREFERENCETABLEEDITORTOOLBARPART_H_

// Includes standard

// Includes Suri
#include "suri/PartCollection.h"
// Includes Wx

// Includes App

// Defines

namespace suri {
/** suri::GeoReferenceTableEditorToolbarPart: comment */
/*
 *
 */
class GeoReferenceTableEditorToolbarPart : public suri::PartCollection {
public:
   /** Ctor */
   GeoReferenceTableEditorToolbarPart();
   /** Dtor */
   virtual ~GeoReferenceTableEditorToolbarPart();

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
   static const std::string VECTOR_GEOREFERENCE_TABLE_TOOLBAR_XRC_NAME;
protected:
};
}  // namespace suri

#endif /* GEOREFERENCETABLEEDITORTOOLBARPART_H_ */
