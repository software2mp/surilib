/*! \file BipRasterWriter.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef BIPRASTERWRITER_H_
#define BIPRASTERWRITER_H_

// Includes standard
#include <fstream>

// Includes suri
#include "suri/RasterWriter.h"

/** namespace suri */
namespace suri {
/** Writer multiplexado en BIP */
/**
 *  Clase que hereda de RasterWriter y escribe datos en multiplexado BIP.
 *
 * \todo Hacer que pueda escribir random en X, Y y Bandas
 */
class BipRasterWriter : public RasterWriter {
   /** Ctor. de Copia. */
   BipRasterWriter(const BipRasterWriter &BipRasterWriter);

public:
   /** tipo de funcion usada para decimar */
   typedef void (*WriterFunc)(std::ostream &OutStream, std::vector<void*> Data, int,
                              int, int);
   /** ctor */
   BipRasterWriter(const std::string &Filename);
   /** dtor */
   virtual ~BipRasterWriter();

   /** Cambia el tipo de dato */
   virtual void SetDataType(const std::string &DataType);
   /** Bloque de bandas/subset que debe escribir */
   virtual bool GetNextBlock(std::vector<int> &BandIndex, int &Ulx, int &Uly, int &Lrx,
                             int &Lry);
   /** Escritura de un bloque */
   virtual void Write(const std::vector<int> &BandIndex, std::vector<void*> &Data, int Ulx,
                      int Uly, int Lrx, int Lry);
protected:
private:
   std::ofstream ostream_; /*! Stream de escritura */
   bool fullWrite_; /*! flag que indica si se llego al fin del archivo */
   WriterFunc pWriteFunc_; /*! Funcion de escritura */
};
}

#endif /*BIPRASTERWRITER_H_*/
