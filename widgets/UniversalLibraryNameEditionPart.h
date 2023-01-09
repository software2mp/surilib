/*! \file UniversalLibraryNameEditionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef UNIVERSALLIBRARYNAMEEDITIONPART_H_
#define UNIVERSALLIBRARYNAMEEDITIONPART_H_

// Includes standard
// Includes Suri
#include "suri/Part.h"

// Includes Wx
// Includes App
// Defines

namespace suri {
/** Editor del nombre de la biblioteca. */
/**
 * Editor del nombre de la biblioteca.
 * Permite seleccionar el nombre de un item de biblioteca.
 * El editor de nombre tiene :
 * - Texto de nombre
 * - Area para escribir el nombre
 * - Boton de guardar item
 * - Boton de eliminar item
 * - Checkbox de mostrar favoritos
 * Las opciones de representacion del editor son las siguientes:
 * - Mostrar/ocultar texto del nombre
 * - Mostrar en modo solo lectura
 * - Mostrar/ocultar texto del nombre Checkbox de mostrar favoritos
 */
class UniversalLibraryNameEditionPart : public suri::Part {
   /** Nombre del XRC que representa la ventana */
   static const std::string LIBRARY_NAME_EDITION_XRC_NAME;

public:

   /** Muestra el nombre */
   static const int SHOW_NAME = 0x01;

   /** Muestra el check de favoritos */
   static const int SHOW_FAVOURITES = 0x02;

   /** Pone el editor en modo read only */
   static const int READ_ONLY = 0x04;

   static const int SHOW_DELETE_BUTTON = 0x08;

public:
   UniversalLibraryNameEditionPart(int flags = SHOW_NAME | SHOW_FAVOURITES);
   virtual ~UniversalLibraryNameEditionPart();

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
   /** Permite a clases que hereden modificar el widget luego de crear la ventana */
   virtual bool ConfigureWidget();

protected:
private:
   int representationflags_;
};
}

#endif /* UNIVERSALLIBRARYNAMEEDITIONPART_H_ */
