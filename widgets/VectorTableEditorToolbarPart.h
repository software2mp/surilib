/*! \file VectorTableEditorToolbarPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORTABLEEDITORTOOLBARPART_H_
#define VECTORTABLEEDITORTOOLBARPART_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"

// Includes Wx

// Includes App

// Defines

/** suri::VectorTableEditorToolbarPart: comment */
/*
 *
 */

namespace suri {
class VectorTableEditorToolbarPart : public suri::Part {
public:

   static const std::string VECTOR_TABLE_TOOLBAR_XRC_NAME; /*! Nombre de xrc de Part */
   /** Ctor: */
   VectorTableEditorToolbarPart(bool ShowEditColumnButtons = true);
   /** Dtor */
   virtual ~VectorTableEditorToolbarPart();

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
   /** Permite a clases que hereden modificar el widget luego de crear la ventana */
   virtual bool ConfigureWidget();
private:
   bool showEditColumnButtons_;
};
}  // suri
#endif /* VECTORTABLEEDITORTOOLBARPART_H_ */
