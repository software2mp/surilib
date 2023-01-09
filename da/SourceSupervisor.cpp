/*! \file SourceSupervisor.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/SourceSupervisor.h"

/** namespace suri */
namespace suri {
/**
 *  En el destructor se toman las fuentes supervisadas que quedan y se les
 * elimina el supervisor.
 *
 *  Esto deberia considerarse un error, por no hacer Image::PopSource() antes
 * de destruir la imagen. Generar una excepcion? Mensaje de advertencia?
 *
 *  Se debe definir
 */
SourceSupervisor::~SourceSupervisor() {
// __STRICT_SOURCE_SUPERVISOR__ determina si al destruir el SourceSupervisor
// se "desregistra" de las fuentes manejadas para asi evitar que se lo llame
// desde una fuente.
#if __STRIC_SOURCE_SUPERVISOR__ != 1
   // Si quedan fuentes supervisadas les saca el supervisor
   SourceSetType::iterator it = childSources_.begin();
   while (it != childSources_.end()) {
      (*it)->pSupervisor_ = NULL;
      it++;
   }
#endif   // __STRIC_SOURCE_SUPERVISOR__
}

/**
 * crea una nueva instancia de la fuente manejada
 * @return puntero a RasterSource creado
 */
RasterSource *SourceSupervisor::CreateSource() {
   RasterSource *psource = Create();
   AddSource(psource);
   return psource;
}

/**
 * Elimina una fuente
 * @param[in] pSource RasterSource que se quiere eliminar
 */
void SourceSupervisor::DelSource(RasterSource *pSource) {
   childSources_.erase(pSource);
}

/**
 * Agrega una fuente
 * @param[in] pNewSource nuevo RasterSource
 */
void SourceSupervisor::AddSource(RasterSource *pNewSource) {
   childSources_.insert(pNewSource);
}
}  // namespace suri

