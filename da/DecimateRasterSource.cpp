/*! \file DecimateRasterSource.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "DecimateRasterSource.h"

// Includes suri
#include "suri/DataTypes.h"
#include "suri/AuxiliaryFunctions.h"

/** namespace suri */
namespace suri {
/** Template decimador */
/**
 *  Este template se utiliza para decimar los datos en la dimension X.
 *  Guarda el dato cada Step posiciones de pSrc en pDest hasta el tamanio Size.
 * \pre pDest debe tener Size tamanio reservado
 * \pre pSrc debe tener Size*Step tamanio reservado
 * \post pDest tiene los datos de pSrc cada Step pasos
 * @param[out] pDest puntero a los datos decimados
 * @param[in] pSrc puntero a los datos a decimar
 * @param[in] Size Tamanio en T de pDest
 * @param[in] Step Cantidad de posiciones de pSrc por cada posicion de pDest
 * \todo Here be interpolation
 */
template<typename T>
void decimate(void* pDest, void* pSrc, size_t Size, double Step) {
   T* pdest = static_cast<T*>(pDest);
   T* psrc = static_cast<T*>(pSrc);
   // \todo Here be interpolation
   for (size_t i = 0; i < Size; i++)
      pdest[i] = psrc[SURI_ROUND(int, i*Step)];
}

/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(DecimateRasterSource::DecimateFunc, decimate);

/** ctor */
/**
 * @param[in] pSupervisor es el supervisor
 * @param[in] NewX nueva cantidad de columnas
 * @param[in] NewY nueva cantidad de filas, en cero indica que mantenga la
 *  proporcion
 */
DecimateRasterSource::DecimateRasterSource(SourceSupervisor *pSupervisor, int NewX,
                                           int NewY) :
      RasterSource(pSupervisor), sizeX_(NewX), sizeY_(NewY), stepX_(1), stepY_(1),
      pFunction_(NULL), pBuffer_(NULL) {
}

/** Destructor */
DecimateRasterSource::~DecimateRasterSource() {
   delete[] pBuffer_;
}

/**
 * Establece la dimension deseada
 *  Modifica los factores de decimado de manera que la nueva imagen tenga las
 * dimensiones pasadas.
 * @param[in] SizeX nueva cantidad de columnas.
 * @param[in] SizeY nueva cantidad de filas.
 */
void DecimateRasterSource::SetSize(int SizeX, int SizeY) {
   sizeX_ = SizeX;
   sizeY_ = SizeY;
   Update();
}

/**
 *  Modifica la cantidad de columnas manteniendo la relacion de aspecto
 * de la imagen original.
 * @param[in] SizeX Nueva cantidad de columnas
 */
void DecimateRasterSource::SetSizeX(int SizeX) {
   SetSize(SizeX, 0);
}

/**
 *  Modifica la cantidad de filas manteniendo la relacion de aspecto de la
 * imagen original
 * @param[in] SizeY Nueva cantidad de filas.
 */
void DecimateRasterSource::SetSizeY(int SizeY) {
   SetSize(0, SizeY);
}

/**
 * Obtiene el tamanio original de la fuente de datos.
 * @param[out] SizeX tamano en X. Cantidad de columnas.
 * @param[out] SizeY tamano en Y. Cantidad de filas.
 */
void DecimateRasterSource::GetRealSize(int &SizeX, int &SizeY) {
   pSource_->GetSize(SizeX, SizeY);
}

// ----------------------------- BLOQUE -----------------------------
/**
 * Tamanio del bloque X e Y
 * @param[out] SizeX cantidad de columnas
 * @param[out] SizeY cantidad de filas
 */
void DecimateRasterSource::GetBlockSize(int &SizeX, int &SizeY) const {
   int realbsx, realbsy;
   RasterSource::GetBlockSize(realbsx, realbsy);
   SizeX = SURI_ROUND(int, realbsx/stepX_);
   if (SizeX > sizeX_) {
      SizeX = sizeX_;
   }
   if (SizeX < 1) {
      SizeX = 1;
   }
   SizeY = SURI_ROUND(int, realbsy/stepY_);
   if (SizeY > sizeY_) {
      SizeY = sizeY_;
   }
   if (SizeY < 1) {
      SizeY = 1;
   }
}

/**
 * Retorna un buffer interno con los datos
 * @param[in] BlockX cantidad de columnas del bloque de datos
 * @param[in] BlockY cantidad de filas del bloque de datos
 * @return Bloque con datos. NULL si ocurrio un error de lectura
 */
void* DecimateRasterSource::GetBlock(int BlockX, int BlockY) {
   delete[] pBuffer_;
   int x, y;
   GetBlockSize(x, y);
   pBuffer_ = new unsigned char[x * y * GetDataSize()];
   if (!Read(pBuffer_, x * BlockX, y * BlockY, (x + 1) * BlockX, (y + 1) * BlockY)) {
      delete[] pBuffer_;
      pBuffer_ = NULL;
   }
   return pBuffer_;
}

/** carga los datos del subset pedido */
/**
 *  Pide a su fuente los datos del subset sin decimar y carga los datos
 * (decimando) en el buffer.
 *
 * \pre pBuffer debe tener reservada la memoria
 * \post los datos de la fuente
 * @param[out] pBuffer es el buffer donde se copian los datos
 * @param[in] Ulx Coordenada X superior
 * @param[in] Uly Coordenada Y superior
 * @param[in] Lrx Coordenada X inferior
 * @param[in] Lry Coordenada X inferior
 */
bool DecimateRasterSource::Read(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry) {
   if (!pFunction_) {
      return false;
   }
   int realulx = SURI_ROUND(int, Ulx*stepX_), reallrx = SURI_ROUND(int, Lrx*stepX_);
   int realwidth = reallrx - realulx;
   char *ptempbuffer = new char[realwidth * pSource_->GetDataSize()];
   char* pwrite = static_cast<char*>(pBuffer);

   bool validdata = true;
   for (int j = Uly; j < Lry && validdata; j++) {
      int realy = SURI_ROUND(int, j*stepY_);
      validdata = pSource_->Read(ptempbuffer, realulx, realy, reallrx, realy + 1);
      if (validdata) {
         pFunction_(pwrite, ptempbuffer, Lrx - Ulx, stepX_);
         pwrite += (Lrx - Ulx) * pSource_->GetDataSize();
      }
   }
   delete[] ptempbuffer;

   return validdata;
}

/** Metodo de las fuentes que actualiza los datos */
/**
 *  Actualiza los pasos en X e Y basado en la dimension deseada.
 *  Carga en pFunction_ el puntero a la funcion template que decima el tipo de
 * dato de la fuente.
 * \post La funcion pFuntcion_ queda cargada con el tipo de dato de la fuente.
 * \post stepX_ y stepY_ tienen los pasos necesarios para cumplir con las
 * dimensiones deseadas.
 */
void DecimateRasterSource::Update() {
   pFunction_ = decimateTypeMap[pSource_->GetDataType()];

   int realx = 0, realy = 0;
   pSource_->CalcRecommendedSize(sizeX_, sizeY_, realx, realy);
   pSource_->SetRecommendedSize(realx, realy);

   // no pueden ser cero los dos al mismo tiempo
   if (sizeX_ == 0 && sizeY_ == 0) {
      sizeX_ = realx;
      sizeY_ = realy;
   }

   // Si alguno es cero, hay que mantener la proporcion
   if (sizeX_ != 0) {
      stepX_ = realx < sizeX_ ? 1 : (double) realx / sizeX_;
      sizeY_ = sizeY_ ? sizeY_ : realy * sizeX_ / realx;
      stepY_ = realy < sizeY_ ? 1 : (double) realy / sizeY_;
   } else {
      stepY_ = realy < sizeY_ ? 1 : (double) realy / sizeY_;
      sizeX_ = sizeX_ ? sizeX_ : realx * sizeY_ / realy;
      stepX_ = realx < sizeY_ ? 1 : (double) realx / sizeX_;
   }
}

}
