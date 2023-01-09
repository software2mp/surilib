/*! \file EnviImage.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ENVIIMAGE_H_
#define ENVIIMAGE_H_

// Includes standard
#include <map>

// Includes suri
#include "suri/Image.h"

/** namespace suri */
namespace suri {
/** Clase que maneja imagenes estandar Envi HDR */
/**
 * Maneja imagenes estandar Envi HDR: verifica el archivo de la imagen para
 * saber si maneja el formato, carga la imagen y permite crear imagenes en este
 * formato.
 */
class EnviImage : public Image {
   /** Ctor. de Copia. */
   EnviImage(const EnviImage &EnviImage);
public:
   /** ctor */
   EnviImage();
   /** dtor */
   virtual ~EnviImage();
   /** Inspecciona el archivo, para saber si es del formato que maneja */
   virtual bool Inspect(const std::string &Filename, ImageAccessType ImageAccess,
                        int BandCount, int SizeX, int SizeY,
                        const std::string &DataType);
   /** Carga la imagen */
   virtual Image* Load(const std::string &Filename, ImageAccessType ImageAccess,
                       int BandCount, int SizeX, int SizeY,
                       const std::string &DataType);
   /** Crea una imagen envi */
   static Image* Create() {
      return new EnviImage;
   }
   /** Retorna el nombre de la clase */
   static std::string GetClassId() {
      return "EnviImage";
   }
protected:
   /** Inicializacion de la clase */
   virtual void InitializeClass();
private:
   static std::map<std::string, int> enviDataType_; /*! Tipo de dato */
};
}

#endif /*ENVIIMAGE_H_*/
