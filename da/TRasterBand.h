/*! \file TRasterBand.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TRASTERBAND_H_
#define TRASTERBAND_H_

// Includes standard
#include <cassert>
#include <string>
#include <math.h>
#include <iostream>

// Includes suri
#include "suri/RasterBand.h"
#include "suri/RasterDriver.h"
#include "suri/DataTypes.h"
#include "logmacros.h"

/** namespace suri */
namespace suri {
/** Template que hereda de RasterBand y especializa el tipo de dato */
/**
 *  Clase template derivada de RasterBand que especializa en un tipo de dato
 */
template<class T>
class TRasterBand : public RasterBand {
   /** Ctor. de Copia. */
   TRasterBand(const TRasterBand &TRasterBand);

public:
   /** Ctor */
   TRasterBand();
   /** Dtor */
   virtual ~TRasterBand();
   /** Modifica la dimension de la banda */
   virtual void SetSize(const int SizeX, const int SizeY);
   /** Acceso lento a los datos */
   virtual void operator()(int X, int Y, const double &Value);
   /** Acceso lento a los datos (version const) */
   virtual double operator()(int X, int Y) const;
   /** Entrega un buffer con los datos representados */
   virtual void *GetBlock(int BlockX, int BlockY);
   /** Carga el buffer con los datos */
   virtual bool Read(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry);
   /** Escribe un bloque de banda */
   virtual void Write(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry);
   /** Escribe un bloque de banda */
   virtual void Write(void *pData, int BlockX, int BlockY);
   /** Funcion estatica para la factoria */
   static RasterBand* Create();
   /** Funcion que retorna el ClassId para la factoria */
   static std::string GetClassId() {
      return DataInfo<T>::Name;
   }
   /** Retorna el tipo de dato */
   virtual std::string GetDataType() const {
      return DataInfo<T>::Name;
   }
   /** Retorna el tamanio del tipo de dato */
   virtual int GetDataSize() const {
      return dataSize_;
   }
// ----------------------------- SOURCES ----------------------------
   /** Necesita un pop especial */
   virtual RasterSource *PopSource();
   /** Necesita un push especial */
   virtual void PushSource(RasterSource *pSource);
protected:
   /** Reserva espacio para un subset y retorna el puntero al mismo. */
   T* AllocateSpace(int SizeX, int SizeY);
private:
   /** tipo estructura para los datos de los bloques */
   /**
    * todo: documentar
    */
   class BlockData {
   public:
      /** Ctor. */
      BlockData() :
            pData_(NULL), isDirty_(false) {
      }
      /** Libera la memoria reservada para los datos */
      void Free() {
         delete[] pData_;
         pData_ = NULL;
      }
      T* pData_; /*! puntero a los datos */
      bool isDirty_; /*! Indica si se le escribio */
   } BlockDataType;
   /** Reserva, carga y devuelve un bloque con los datos */
   BlockData &GetDataBlock(int BlockX, int BlockY);
   /** Elimina del cache de bloques */
   void FreeCache(size_t MaxBlocks = 4, bool DirtyToo = false);
   /** tipo matriz de bloques reservados */
   typedef std::map<std::pair<int, int>, BlockData> BlockMatrixType;
   BlockMatrixType blockMatrix_; /*! Bloques de datos */
   T *pData_; /*! puntero con los datos */
   int ulx_; /*! ultimo subset reservado */
   int uly_; /*! ultimo subset reservado */
   int lrx_; /*! ultimo subset reservado */
   int lry_; /*! ultimo subset reservado */
   static const int dataSize_ = DataInfo<T>::Size; /*! tamanio del tipo de dato */
};

/** ctor */
template<class T>
TRasterBand<T>::TRasterBand() :
      pData_(NULL), ulx_(-1), uly_(-1), lrx_(-1), lry_(-1) {
}

/** dtor */
template<class T>
TRasterBand<T>::~TRasterBand() {
   FreeCache(0, true);
   delete[] pData_;
}

/**
 * Modifica el tamanio de la banda
 * @param[in] SizeX ancho de la banda
 * @param[in] SizeY alto de la banda
 */
template<class T>
void TRasterBand<T>::SetSize(const int SizeX, const int SizeY) {
   // Llamo al metodo de la clase base
   RasterBand::SetSize(SizeX, SizeY);
   // Si no pudo cambiar las dimensiones, salgo (probablemente, tiene pSource)
   int x, y;
   GetSize(x, y);
   if (x != SizeX || y != SizeY) {
      return;
   }
   // Elimino todos los datos
   FreeCache(0, true);
   delete[] pData_;
   // pongo en el mismo estado que el ctor
   pData_ = NULL;
   ulx_ = -1;
   uly_ = -1;
   lrx_ = -1;
   lry_ = -1;
}

/** Acceso lento a los datos */
/**
 * @param[in] X coord x del pixel
 * @param[in] Y coord y del pixel
 * @param[out] Value valor del pixel
 * \todo completar
 */
template<class T>
void TRasterBand<T>::operator()(int X, int Y, const double &Value) {
   assert(false);
}

/** Acceso lento a los datos (solo lectura) */
/**
 * @param[in] X coord x del pixel
 * @param[in] Y coord y del pixel
 * @return valor del pixel
 * \todo completar
 */
template<class T>
double TRasterBand<T>::operator()(int X, int Y) const {
   assert(false);
   return 0;
}

/** Regresa el buffer interno con los datos cargados del subset pedido */
/**
 *  Implementar algun tipo de persistencia del subset de manera que no se tenga
 * que cargar 2 veces el mismo.
 *  Se puede implementar un cache para los subsets pedidos, etc.
 * @param[in] BlockX numero de columna del bloque
 * @param[in] BlockY numero de fila del bloque
 * @return puntero al bloque
 */
template<class T>
void* TRasterBand<T>::GetBlock(int BlockX, int BlockY) {
   return GetDataBlock(BlockX, BlockY).pData_;
}

/** Llena el buffer que le pasan con el subset */
/**
 *  Utiliza la fuente de datos para llenar el buffer, el mismo debe ser del
 * tipo de dato que maneja la clase y tener memoria reservada.
 * @param[out] pBuffer donde se cargan los datos
 * @param[in] Ulx esquina upper left x del subset que quiero leer
 * @param[in] Uly esquina upper left y del subset que quiero leer
 * @param[in] Lrx esquina lower right x del subset que quiero leer
 * @param[in] Lry esquina lower right y del subset que quiero leer
 * \pre El buffer debe ser del tipo que maneja la clase
 * \pre El buffer debe tener memoria reservada
 * @return bool que indica si pudo leer la imagen
 */
template<class T>
bool TRasterBand<T>::Read(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry) {
   if (!pBuffer) {
      return false;
   }
   // tamanio del bloque
   int block_x_size, block_y_size;
   GetBlockSize(block_x_size, block_y_size);
   if (block_x_size == 0 || block_y_size == 0) {
      REPORT_AND_FAIL_VALUE("D:Error en el tamano de bloque de datos (%d,%d)", false,
                            block_x_size, block_y_size);
   }

   // si coincide el tamanio pedido y la fase del UL es cero
   // retorno el bloque
   if ((Lrx - Ulx) == block_x_size && (Lry - Uly) == block_y_size
         && Ulx % block_x_size == 0 && Uly % block_y_size == 0) {
      BlockData db = GetDataBlock((int) (Ulx / block_x_size),
                                  (int) (Uly / block_y_size));
      if (!db.pData_) return false;

      memcpy(pBuffer, db.pData_, block_x_size * block_y_size * dataSize_);
      return true;
   }

   // block inicial en x
   int iblockx = Ulx / block_x_size;
   // block final en x
   int fblockx = (int) ceil((double) Lrx / block_x_size);
   int iblocky = Uly / block_y_size;
   int fblocky = (int) ceil((double) Lry / block_y_size);

   int buffer_step = (Lrx - Ulx);

   for (int j = iblocky; j < fblocky; j++) {
      int rowcount = std::min((j + 1) * block_y_size, Lry)
            - std::max(j * block_y_size, Uly);
      for (int i = iblockx; i < fblockx; i++) {
         int colcount = std::min((i + 1) * block_x_size, Lrx)
               - std::max(i * block_x_size, Ulx);

         // obtengo el puntero de lectura
         T *pblock = GetDataBlock(i, j).pData_;
         if (!pblock) {
            return false;
         }
         // Calculo y aplico el offset de lectura
         int block_x_offset = std::max(i * block_x_size, Ulx) - i * block_x_size;
         int block_y_offset = std::max(j * block_y_size, Uly) - j * block_y_size;
         int block_offset = block_x_offset + block_y_offset * block_x_size;
         pblock += block_offset;
         T *pbuff = static_cast<T*>(pBuffer);
         // Calculo y aplico el offset de escritura
         int buff_x_offset = std::max(i * block_x_size, Ulx) - Ulx;
         int buff_y_offset = std::max(j * block_y_size, Uly) - Uly;
         int buff_offset = buff_x_offset + buff_y_offset * buffer_step;
         pbuff += buff_offset;
         for (int c = 0; c < rowcount; c++)
            memcpy(pbuff + c * buffer_step, pblock + c * block_x_size,
                   colcount * dataSize_);
      }
   }
   return true;
}

/** Escribe el buffer */
/**
 * @param[in] pBuffer donde se cargan los datos
 * @param[in] Ulx esquina upper left x del subset que quiero guardar
 * @param[in] Uly esquina upper left y del subset que quiero guardar
 * @param[in] Lrx esquina lower right x del subset que quiero guardar
 * @param[in] Lry esquina lower right y del subset que quiero guardar
 */
template<class T>
void TRasterBand<T>::Write(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry) {
   // tamanio del bloque
   int block_x_size, block_y_size;
   GetBlockSize(block_x_size, block_y_size);
   if (block_x_size == 0 || block_y_size == 0) {
      REPORT_AND_FAIL("D:Tamano de bloque igual a cero.");
   }
   // block inicial en x
   int iblockx = Ulx / block_x_size;
   // block final en x
   int fblockx = (int) ceil((double) Lrx / block_x_size);
   int iblocky = Uly / block_y_size;
   int fblocky = (int) ceil((double) Lry / block_y_size);

   int buffer_step = (Lrx - Ulx);

   for (int j = iblocky; j < fblocky; j++) {
      int rowcount = std::min((j + 1) * block_y_size, Lry)
            - std::max(j * block_y_size, Uly);
      for (int i = iblockx; i < fblockx; i++) {
         int colcount = std::min((i + 1) * block_x_size, Lrx)
               - std::max(i * block_x_size, Ulx);

         // obtengo el puntero de lectura
         BlockData db = GetDataBlock(i, j);
         T *pblock = db.pData_;
         if (!pblock) {
            return;
         }
         // lo ensucio porque lo voy a escribir
         db.isDirty_ = true;
         // Calculo y aplico el offset de lectura
         int block_x_offset = std::max(i * block_x_size, Ulx) - i * block_x_size;
         int block_y_offset = std::max(j * block_y_size, Uly) - j * block_y_size;
         int block_offset = block_x_offset + block_y_offset * block_x_size;
         pblock += block_offset;
         T *pbuff = static_cast<T*>(pBuffer);
         // Calculo y aplico el offset de escritura
         int buff_x_offset = std::max(i * block_x_size, Ulx) - Ulx;
         int buff_y_offset = std::max(j * block_y_size, Uly) - Uly;
         int buff_offset = buff_x_offset + buff_y_offset * buffer_step;
         pbuff += buff_offset;
         for (int c = 0; c < rowcount; c++)
            memcpy(pblock + c * block_x_size, pbuff + c * buffer_step,
                   colcount * dataSize_);
      }
   }
}

/** Escribe un bloque de banda */
/**
 * @param[in] pData datos que quiero escribir
 * @param[in] BlockX numero de columna del bloque
 * @param[in] BlockY numero de fila del bloque
 */
template<class T>
void TRasterBand<T>::Write(void *pData, int BlockX, int BlockY) {
   int x, y;
   GetBlockSize(x, y);
   Write(pData, BlockX * x, BlockY * y, (BlockX + 1) * x, (BlockY + 1) * y);
}

/** Funcion de creacion (para la factoria) */
/**
 * return RasterBand del tipo correcto
 */
template<class T>
RasterBand* TRasterBand<T>::Create() {
   return new TRasterBand<T> ;
}

// ----------------------------- SOURCES ----------------------------
/** Necesita un pop especial */
/**
 *  Elimino los bloques cacheados porque no se si siguen siendo validos
 *  @return RasterSource extraida
 */
template<class T>
RasterSource *TRasterBand<T>::PopSource() {
   // Elimino los bloques cacheados
   FreeCache(0, true);
   return RasterBand::PopSource();
}

/** Necesita un push especial */
/**
 *  Elimino los bloques cacheados porque no se si siguen siendo validos
 *  @param[in] pSource RasterSource que quiero agregar
 */
template<class T>
void TRasterBand<T>::PushSource(RasterSource *pSource) {
   // Elimino los bloques cacheados
   FreeCache(0, true);
   RasterSource::PushSource(pSource);
}

/** Reserva espacio */
/**
 *  Reserva espacio para un subset y retorna el puntero al mismo.
 *  @param[in] SizeX ancho del espacio alocado
 *  @param[in] SizeY altura del espacio alocado
 * \post La memoria reservada se encuentra inicializada en 0 (salvo que se
 *  defina el simbolo NO_INITIALIZE_RASTER_MEMORY
 */
template<class T>
T* TRasterBand<T>::AllocateSpace(int SizeX, int SizeY) {
   // valida el subset
   if (SizeX < 1 || SizeY < 1) {
      return NULL;
   }
   T* pdata = new T[SizeX * SizeY];
#ifndef NO_ITIALIZE_RASTER_MEMORY
   memset(pdata, 0, SizeX * SizeY * sizeof(T));
#endif
   return pdata;
}

/**
 *  Busca el bloque en el mapa, si esta lo retorna, si no lo encuentra,
 * lo crea, inicializa la memoria, carga los datos correspondientes al bloque,
 * lo agrega al mapa y lo retorna.
 * @param[in] BlockX numero de columna del bloque
 * @param[in] BlockY numero de fila del bloque
 */
template<class T>
typename TRasterBand<T>::BlockData&
TRasterBand<T>::GetDataBlock(int BlockX, int BlockY) {
   // busco el bloque
   typename BlockMatrixType::iterator it = blockMatrix_.find(
         std::make_pair(BlockX, BlockY));
   std::pair<typename BlockMatrixType::iterator, bool> result;
   // si existe lo retorno (cache HIT)
   if (it != blockMatrix_.end()) {
#ifdef __DEBUG__
      BlockData tempbd = it->second;
      tempbd.isDirty_ = tempbd.isDirty_;
#endif
      return it->second;
   } else
   // sino, lo creo, leo los datos, lo agrego y lo retorno
   {
      BlockData bd;
      int bsx, bsy;   // Tamanio de bloque
      GetBlockSize(bsx, bsy);
      // AVISO DE CODIGO DIABOLICO: no intente establecer pData_ en NULL
      // salvo dentro del entorno del IF (el mismo ha sido protegido con
      // bendeciones divinas). De hacerlo una lluvia de SIGSEGVs, ardiente
      // como el fuego, os hara arder por siempre.
      bd.pData_ = AllocateSpace(bsx, bsy);
      if (pSource_
            && !pSource_->Read(bd.pData_, bsx * BlockX, bsy * BlockY,
                               bsx * (1 + BlockX), bsy * (1 + BlockY))) {
         delete[] bd.pData_;
         bd.pData_ = NULL;
      }
      // Trato de limpiar el cache
      FreeCache();

      result = blockMatrix_.insert(std::make_pair(std::make_pair(BlockX, BlockY), bd));

      if (!result.second) {
         REPORT_AND_FAIL_VALUE("D:Error al insertar bloque de datos.",
                               result.first->second);
      }
   }
   return result.first->second;
}

/** Elimina del cache de bloques */
/**
 *  Elimina los bloques cacheados no sucios (modificados) hasta dejar un maximo
 * de MaxBlocks.
 * \post Si la cantidad de bloques modificados supera a MaxBlocks y
 *      DirtyToo==false quedaran solo los bloques modificados.
 * \post Si la cantidad de bloques modificados supera a MaxBlocks y
 *      DirtyToo==true quedara un maximo de MaxBlocks bloques.
 * \post Si la cantidad de bloques modificados es inferios a MaxBlocks, quedara
 *      un maximo de MaxBlocks bloques.
 *
 * @param[in] MaxBlocks Cantidad maxima de bloques que quedran en el cache
 * @param[in] DirtyToo Eliminar incluso bloques que hayan sido modificados
 */
template<class T>
void TRasterBand<T>::FreeCache(size_t MaxBlocks, bool DirtyToo) {
   typename BlockMatrixType::iterator it = blockMatrix_.begin();
   while (blockMatrix_.size() > MaxBlocks && it != blockMatrix_.end()) {
      BlockData tempdb = it->second;
      if (!tempdb.isDirty_ || DirtyToo) {
         tempdb.Free();
         blockMatrix_.erase(it);
         it = blockMatrix_.begin();
      } else {
         it++;
      }
   }
}

typedef TRasterBand<unsigned char> UCharBandType; /*! 8 bits no-signado */
typedef TRasterBand<short> ShortBandType; /*! 16 bits signado */
typedef TRasterBand<unsigned short> UShortBandType; /*! 16 bits no-signado */
typedef TRasterBand<int> IntBandType; /*! 32 bits signado */
typedef TRasterBand<unsigned int> UIntBandType; /*! 32 bits no-signado */
typedef TRasterBand<float> FloatBandType; /*! 32 bits float */
typedef TRasterBand<double> DoubleBandType; /*! 64 bits float */
}  // namespace suri

#endif /*TRASTERBAND_H_*/
