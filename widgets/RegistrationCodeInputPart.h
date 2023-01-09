/*! \file RegistrationCodeInputPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef REGISTRATIONCODEINPUTPART_H_
#define REGISTRATIONCODEINPUTPART_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"
// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Genera un part que permite el ingreso de una clave de activacion del software */
class RegistrationCodeInputPart : public Part {
   static const std::string REGISTER_XRC_NAME; /*! Nombre del XRC de registro */
public:
   /** Ctor */
   RegistrationCodeInputPart();

   /** Dtor */
   virtual ~RegistrationCodeInputPart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
};
}  // namespace suri
#endif /* REGISTRATIONCODEINPUTPART_H_ */
