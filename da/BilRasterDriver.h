/*! \file BilRasterDriver.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef BILRASTERDRIVER_H_
#define BILRASTERDRIVER_H_

// Includes Estandar
// Includes Suri
#include "suri/RawRasterDriver.h"

// Includes Wx
// Defines
// forwards

namespace suri {
namespace core {
namespace raster {
namespace dataaccess {
namespace driver {

/**
 *  Clase encargada de la lectura/escritura sobre archivos raster con interlineado
 *  bil
 */
class BilRasterDriver : public suri::core::raster::dataaccess::driver::RawRasterDriver {
   /** ctor copia **/
   BilRasterDriver(const BilRasterDriver&);

public:
   BilRasterDriver(const std::string& Mux, const std::string& FileName,
                   int Pixels, int Lines);
   virtual ~BilRasterDriver();
   /** string que identifica al interlineado del driver **/
   static std::string MuxIdentifier;

private:
   /** array que tiene el contenido de la banda en memoria **/
   char* pData_;
   /** Tamanio del bloque X e Y */
   virtual bool DoGetBlockSize(int &SizeX, int &SizeY) const;
   /** Carga el buffer con el subset **/
   virtual bool DoRead(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry);
   /** Escribe mas de una banda **/
   virtual bool DoWrite(const std::vector<int> &BandIndex, std::vector<void*> &Data,
                        int Ulx, int Uly, int Lrx, int Lry);
   /** Retorna un buffer interno con los datos **/
   virtual void* DoGetBlock(int BlockX, int BlockY);
};
}  /** namespace driver **/
}  /** namespace dataaccess **/
}  /** namespace raster **/
}  /** namespace core **/
} /** namespace suri */

#endif /* BILRASTERDRIVER_H_ */
