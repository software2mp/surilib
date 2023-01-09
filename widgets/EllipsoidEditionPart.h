/*! \file EllipsoidEditionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ELLIPSOIDEDITIONPART_H_
#define ELLIPSOIDEDITIONPART_H_

// Includes standard
#include <string>

// Includes Suri
#include "LibraryItemEditorPart.h"
#include "suri/SuriObject.h"

namespace suri {

// forwards
class LibraryManager;

/**
 * Permite setear los parametros de un sistema de elipsoide,
 * para definir elipsoides propios.
 *
 * Los parametros que permite configurar son:
 * - Nombre.
 * - Autoridad:Codigo.
 * - Semieje Mayor.
 * - Semieje menor o Inversa Factor Achatamiento.
 *
 */
class EllipsoidEditionPart : public LibraryItemEditorPart {
   /** ctor copia**/
   EllipsoidEditionPart(const EllipsoidEditionPart &);

   /** Nombre del XRC que representa la ventana. */
   static const std::string ELLIPSOID_EDITION_XRC_NAME;
   /** Nombre del XRC que representa el panel donde se ubica el CGU */
   static const std::string ELLIPSOID_CGU_XRC_NAME;

public:
   /** Ctor*/
   EllipsoidEditionPart(
         const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
         UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature);
   /** Dtor. */
   virtual ~EllipsoidEditionPart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
    *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
    *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
    *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
    *   @return vector con atributos
    *   @return NULL en caso de que la informnacion del part no sea valida
    *   **/
   virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields();

private:
   /**
    *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
    *  Este metodo es el que realmente tienen que implementar las clases hijas para
    * lograr la especializacion a la hora de iniciar el part (metodo que se llama
    * en CreateToolWindow, a modo de Template Method).
    **/
   virtual bool ConfigureFeatures();

   /**
    * Metodo auxiliar privado que es necesario implementar en las clases hijas.
    * Este metodo es el encargado de actualizar realmente la informacion en el part
    * Es llamado en el metodo SetActiveItem a modo de Template Method
    **/
   virtual void UpdateFields(const LibraryItem* pItem, bool Modifiable = true);
};
}

#endif /* ELLIPSOIDEDITIONPART_H_ */
