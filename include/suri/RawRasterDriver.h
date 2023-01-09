/*! \file RawRasterDriver.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RAWRASTERDRIVER_H_
#define RAWRASTERDRIVER_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "RasterDriver.h"
#include "RasterWriter.h"
#include "suri/DataTypes.h"

// Includes Wx
// Defines
// forwards

namespace suri {
namespace core {
namespace raster {
namespace dataaccess {
namespace driver {

/**
 * Clase base que representa un driver para un archivo "crudo". Es el encargado
 * de encapsular el acceso I/O
 */
class RawRasterDriver : public suri::RasterDriver, public suri::RasterWriter {
   /** ctor copia**/
   RawRasterDriver(const RawRasterDriver&);

public:
   /** Estructura para el configurado de offset del driver **/
   typedef struct {
      struct RawOffset{
         // inicializacion
         RawOffset() : headerOffset_(0), tailOffset_(0) {
         }
         size_t headerOffset_;
         size_t tailOffset_;
      };
      RawOffset fileOffset_;
      RawOffset bandOffset_;
      RawOffset lineOffset_;;
   } RawDriverOffset;

   /** ctor en donde se indica el interlineado (mux), el nombre del driver
    *  y el archivo sobre el cual opera
    */
   RawRasterDriver(const std::string& Mux,
                   const std::string &WriterName,
                   const std::string &Filename,
                   int Pixels, int Lines) :
         RasterWriter(WriterName, Filename),
         mux_(Mux), pSucesor_(NULL),
         pWriterFunc_(NULL), bandReaderIndex_(-1),
         npixels_(Pixels), nlines_(Lines) {
      RasterDriver::sizeX_ = Pixels;
      RasterDriver::sizeY_ = Lines;
      RasterWriter::sizeX_ = Pixels;
      RasterWriter::sizeY_ = Lines;
      driverName_ = "RawRasterDriver";
   }
   /** dtor **/
   virtual ~RawRasterDriver() {
      if (pSucesor_)
         delete pSucesor_;
   }
   /**
    *  Retorna un buffer interno con los datos
    *  Template method que intenta capturar la solicitud de GetBlock y luego,
    *  en caso de falla, delega la solicitud a su sucesor.  */
   virtual void* GetBlock(int BlockX, int BlockY) {
      void* pblock = DoGetBlock(BlockX, BlockY);
      if (pblock == NULL && pSucesor_) {
         pblock = pSucesor_->GetBlock(BlockX, BlockY);
      }
      return pblock;
   }

   /** Carga el buffer con el subset
    *  Template method que intenta capturar la solicitud de lectura y luego,
    *  en caso de falla, delega la solicitud a su sucesor.  */
   virtual bool Read(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry) {
      bool success = DoRead(pBuffer, Ulx, Uly, Lrx, Lry);
      if (!success && pSucesor_)
         success =  pSucesor_->Read(pBuffer, Ulx, Uly, Lrx, Lry);
       return success;
   }
   /** Escribe mas de una banda
    *  Template method que intenta capturar la solicitud de escritura y luego,
    *  en caso de falla, delega la solicitud a su sucesor.  */
   virtual void Write(const std::vector<int> &BandIndex, std::vector<void*> &Data, int Ulx,
                      int Uly, int Lrx, int Lry) {
      if (!DoWrite(BandIndex, Data, Ulx, Uly, Lrx, Lry) && pSucesor_)
         pSucesor_->Write(BandIndex, Data, Ulx, Uly, Lrx, Lry);
   }
   /** Tamanio del bloque X e Y
    *  Template method que intenta capturar la solicitud de GetBlockSize y luego,
    *  en caso de falla, delega la solicitud a su sucesor.  */
   virtual void GetBlockSize(int &SizeX, int &SizeY) const {
      if (!DoGetBlockSize(SizeX,SizeY) && pSucesor_)
         pSucesor_->GetBlockSize(SizeX,SizeY);
   }

   /** Agrega un sucesor en la cadena de responsabilidades**/
   void Adducesor(RawRasterDriver* pSucesor) {
      if (!pSucesor_) {
         pSucesor_ = pSucesor;
         return;
      }
      // en caso de que exista sucesor se coloca a lo ultimo de la cadena
      RawRasterDriver* pnext = pSucesor_;
      while (pnext->pSucesor_) {
         pnext = pnext->pSucesor_;
      }
      pnext->pSucesor_ = pSucesor;
   }
   /** Indica el indice de la banda sobre la cual se realiza la lectura de datos
    *  Ademas configura recursivamente a los sucesores */
   void SetBandReaderIndex(int BandReaderIndex) {
      bandReaderIndex_ = BandReaderIndex;
      if (pSucesor_)
         pSucesor_->SetBandReaderIndex(BandReaderIndex);
   }

   /** Indica el offset a aplicar sobre la lectura/escritura de datos.
    *  Ademas configura recursivamente a los sucesores
    */
   void SetOffset(RawDriverOffset& offset) {
      offset_ = offset;
      if (pSucesor_)
         pSucesor_->SetOffset(offset);
   }

   /** Indica la cantidad de bandas del driver.
    * Configura recursivamente la cantidad de bandas
    */
   void SetBandCount(int BandCount) {
      // Las dos clases que hereda tienen el mismo atributo.
      RasterDriver::bandCount_ = BandCount;
      RasterWriter::bandCount_ = BandCount;
      if (pSucesor_) {
         pSucesor_->SetBandCount(BandCount);
      }
   }

   /** Nombre del tipo de dato. Configura de forma recursiva
    *  a sus sucesores el tipo de dato */
   virtual void SetDataType(const std::string& DataType) {
      RasterWriter::SetDataType(DataType);
      dataName_ = DataType;
      RasterWriter::dataSize_ = SizeOf(DataType);
      RasterDriver::dataSize_ = SizeOf(DataType);
      if (pSucesor_) {
         pSucesor_->SetDataType(DataType);
      }
   }

protected:
   // Declaracion de puntero a funcion para escritura
   typedef void (*WriterFunc)(std::ostream &OutStream, std::vector<void*> Data, int,
                                 int, int);
   /** Tamanio del bloque X e Y */
   virtual bool DoGetBlockSize(int &SizeX, int &SizeY) const=0;
   /** Carga el buffer con el subset **/
   virtual bool DoRead(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry)=0;
   /** Escribe mas de una banda **/
   virtual bool DoWrite(const std::vector<int> &BandIndex, std::vector<void*> &Data,
                        int Ulx, int Uly, int Lrx, int Lry)=0;
   /** Retorna un buffer interno con los datos **/
   virtual void* DoGetBlock(int BlockX, int BlockY)=0;
   std::string mux_;
   RawRasterDriver* pSucesor_;
   WriterFunc* pWriterFunc_;
   /** indice de banda que responde a la lectura **/
   int bandReaderIndex_;
   RawDriverOffset offset_;
   int npixels_;
   int nlines_;
};

}  /** namespace driver **/
}  /** namespace dataaccess **/
}  /** namespace raster **/
}  /** namespace core **/
} /** namespace suri */

#endif /* RAWRASTERDRIVER_H_ */
