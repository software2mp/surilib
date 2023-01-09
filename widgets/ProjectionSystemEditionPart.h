/*! \file ProjectionSystemEditionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PROJECTIONSYSTEMEDITIONPART_H_
#define PROJECTIONSYSTEMEDITIONPART_H_

// Includes standard
// Includes Suri

#include "suri/PartCollection.h"
#include "UniversalLibraryNameEditionPart.h"

// Includes Wx
// Includes App
// Defines

namespace suri {
/** Permite setear los parametros de un sistema de referencia geografico. */
/**
 * Permite setear los parametros de un sistema de referencia geografico,
 * para definir sistemas de referencia propios.
 *
 * Los parametros que permite configurar son:
 * - Nombre
 * - Autoridad:Codigo
 * - Sistema geografico asociado
 * - Unidades
 * - Ejes gemelos (si se usan y su orientacion)
 * - Tipo de proyeccion
 * - Parametros de proyeccion
 *
 */
class ProjectionSystemEditionPart : public PartCollection {
   /** Nombre del XRC que representa la ventana. */
   static const std::string PROJECTION_SYSTEM_EDITION_XRC_NAME;

public:
   /** Ctor. Carga el XRC de la pantalla, e instancia los subpart */
   ProjectionSystemEditionPart();
   /** Dtor. */
   virtual ~ProjectionSystemEditionPart();
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

#endif /* PROJECTIONSYSTEMEDITIONPART_H_ */