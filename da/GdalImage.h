/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#ifndef GDALIMAGE_H_
#define GDALIMAGE_H_

#ifdef __GDAL__

// Includes standard
#include <string>
#include <map>

// Includes suri
#include "suri/Image.h"

class GDALDataset;

/** namespace suri */
namespace suri {
/** Clase derivada de Image para lectura usando GDAL */
/**
 *  Esta clase utiliza la biblioteca GDAL para abrir imagenes.
 */
class GdalImage : public Image {
   /** Ctor. de Copia. */
   GdalImage(const GdalImage &GdalImage);

public:
   /** ctor */
   GdalImage();
   /** dtor */
   virtual ~GdalImage();
   /** Inspecciona el archivo, para saber si es del formato que maneja */
   virtual bool Inspect(const std::string &Filename, ImageAccessType ImageAccess,
                        int BandCount, int SizeX, int SizeY,
                        const std::string &DataType);
   /** Carga la imagen */
   virtual Image* Load(const std::string &Filename, ImageAccessType ImageAccess,
                       int BandCount, int SizeX, int SizeY,
                       const std::string &DataType);
   /** Crea una imagen gdal */
   static Image* Create() {
      return new GdalImage;
   }
   /** Retorna el nombre de la clase */
   static std::string GetClassId() {
      return "GdalImage";
   }
   /**  */
   static const std::string SUBDATASET_OPTION;
   /** */
   static const std::string SUBDATASET_SEPARATOR;
   /** Devuelve una lista de strings con los url de los subdatasets separados por
    * SUBDATASET_SEPARATOR */
   std::string GetDatasets();

   /** Obtiene los metadatos de la imagen */
   virtual std::map<std::string, std::map<std::string, std::string> > GetMetadata(
         const std::vector<std::string>& Domains) const;

   /** Establece el sistema de referencia */
   virtual void SetSrs(const std::string& Wkt);

   /** Establece la transformacion para georreferenciacion */
   virtual void SetGeoTransform(double* pGeoTransform);

protected:
   /** Inicializa para lectura */
   void InitializeRead(const std::string &Filename);
   /** Inicializa para escritura */
   void InitializeWrite(const std::string &Filename);
   /** Inicializacion de la clase */
   virtual void InitializeClass();

private:
   /*! Objeto GDAL que representa la imagen */
   GDALDataset *pDataset_;
};

}

#endif   // __GDAL__
#endif /*GDALIMAGE_H_*/
