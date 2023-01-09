/*! \file RawImage.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RAWIMAGE_H_
#define RAWIMAGE_H_

// Includes Estandar
#include <string>
#include <map>

// Includes Suri
#include "suri/Image.h"
#include "suri/RawRasterDriver.h"

// Includes Wx

// Defines

// forwards

namespace suri {
namespace core {
namespace raster {
namespace dataaccess {

/**
 *  Clase que representa una imagen en formato "crudo"
 *  Para estos tipos de imagenes es posible definir los offset de archivo, linea
 *  y banda (encabezado y cola) asi tambien como el interlineado que contiene
 *  (BIP, BSQ o BIL)
 */
class RawImage : public suri::Image {
public:
   /** constructor **/
   RawImage();
   /** dtor */
   virtual ~RawImage();
   /** Inspecciona el archivo, para saber si es del formato que maneja */
   virtual bool Inspect(const std::string &Filename, ImageAccessType ImageAccess,
                        int BandCount, int SizeX, int SizeY,
                        const std::string &DataType);
   /** Metodo que realiza la lectura completa del archivo en el modo de lectura
    * deseado. Carga las bandas e inicializa los drivers.*/
   virtual Image* Load(const std::string &Filename, ImageAccessType ImageAccess,
                       int BandCount, int SizeX, int SizeY,
                       const std::string &DataType);
   /** Crea una imagen envi */
   static Image* Create() {
      return new RawImage;
   }
   /** Retorna el nombre de la clase */
   static std::string GetClassId() {
      return "RawImage";
   }
protected:
   /** Inicializacion de la clase */
   virtual void InitializeClass();
   /** Configura el driver con el interlineado que se pasa por parametro **/
   driver::RawRasterDriver* GetDriver(const std::string &Mux,
                                      const std::string &Filename, int SizeX,
                                      int SizeY);
   /** Metodo auxiliar que obtiene el offset configurado en las opciones de la imagen **/
   driver::RawRasterDriver::RawDriverOffset GetRawOffset();
   static std::map<std::string, int> DataTypes_;
};

} /** namespace dataacess */
} /** namespace raster */
} /** namespace core */
} /** namespace suri */

#endif /* RAWIMAGE_H_ */
