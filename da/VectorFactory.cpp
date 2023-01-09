/*! \file VectorFactory.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "VectorFactory.h"

// Includes suri
#include "suri/Vector.h"
#include "MemoryVector.h"

/** namespace suri */
namespace suri {
/** Registra las distintas clases de vectores */
/**
 *  Registra las clases derivadas de Vector que manejan los distintos formatos.
 * Utiliza los metodos estaticos de las clases derivadas ::GetClassId() y
 * ::Create() para agregar a la factoria.
 *
 *  El orden en que se registran las clases es relevante ya que se prueba FIFO
 * cada clase para identificar los archivos, es decir las de arriba tienen
 * precedencia sobre las de abajo.
 */
void RegisterAllVectors() {
#ifdef __GDAL__
   Vector::RegisterVector(MemoryVector::GetClassId(), MemoryVector::Create);
   Vector::RegisterVector(Vector::GetClassId(), Vector::Create);
#endif
}
}  // namespace suri
