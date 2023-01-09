/*! \file AboutPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ABOUTPART_H_
#define ABOUTPART_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"
// Includes Wx

// Includes App

// Defines

namespace suri {

/*!
 *
 */
class AboutPart : public suri::Part {
public:
   /** Ctor */
   AboutPart();
   /** Dtor */
   virtual ~AboutPart();

   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado del Part */
   virtual void Update();
   /** Permite modificar el widget luego de crear la ventana */
   virtual bool ConfigureWidget();

private:
   static const std::string ABOUT_XRC_NAME; /*! Nombre de xrc de Part */
};
}
#endif /* ABOUTPART_H_ */
