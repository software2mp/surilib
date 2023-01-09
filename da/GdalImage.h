/*! \file GdalImage.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
