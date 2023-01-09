/*! \file ImageFactory.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "ImageFactory.h"

// Includes suri
#include "GdalImage.h"
#include "EnviImage.h"
#include "suri/RawImage.h"

namespace suri {

// alias de namespace
namespace dataaccess = core::raster::dataaccess;
/** Registra las distintas clases de imagen */
/**
 *  Registra las clases derivadas de Image que manejan los distintos formatos.
 * Utiliza los metodos estaticos de las clases derivadas ::GetClassId() y
 * ::Create() para agregar a la factoria.
 *
 *  El orden en que se registran las clases es relevante ya que se prueba FIFO
 * cada clase para identificar los archivos, es decir las de arriba tienen
 * precedencia sobre las de abajo.
 */
void RegisterAllImages() {
   Image::RegisterImage(dataaccess::RawImage::GetClassId(),
                        dataaccess::RawImage::Create);
#ifdef __GDAL__
   Image::RegisterImage(GdalImage::GetClassId(), GdalImage::Create);
#endif

   Image::RegisterImage(EnviImage::GetClassId(), EnviImage::Create);
}
}  // namespace suri
