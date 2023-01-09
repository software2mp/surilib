/*! \file BipRasterWriter.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "BipRasterWriter.h"

// Includes standard
#include <iostream>

// Includes suri
#include "suri/DataTypes.h"
#include "suri/messages.h"
#include "logmacros.h"

/** namespace suri */
namespace suri {
/**
 * Funcion template que escribe los datos en BIP
 * @param[in] Data vector de punteros. Son las bandas.
 * @param[in] BufferSize Tamano de los punteros de Data
 * @param[in] StreamStep Salto en bytes para posicionar el puntero de escritura
 * de los datos.
 * @param[in] BlockSize Tamano del bloque de datos a escribir.
 * @param[in] OutStream stream de salida con los datos organizados en BIP.
 */
template<typename T>
void writebil(std::ostream &OutStream, std::vector<void*> Data, int BufferSize,
              int StreamStep, int BlockSize) {
   size_t size = Data.size();
   // vector con los punteros de entrada
   T** pdata = new T*[size];
   for (size_t i = 0; i < size; i++)
      pdata[i] = static_cast<T*>(Data[i]);
   // vector con los datos multiplexados
   T* pmuxeddata = new T[size * BlockSize];
   for (int i = 0; i < BufferSize; i += BlockSize) {
      // multiplexo
      for (int j = 0; j < BlockSize; j++)
         for (size_t k = 0; k < size; k++)
            pmuxeddata[j * size + k] = pdata[k][i + j];
      // escribo al stream
      OutStream.write((char*) (pmuxeddata), BlockSize * size * sizeof(T));
      // se mueve dentro del stream para saltear el borde del subset
      OutStream.seekp(StreamStep, std::ios_base::cur);
   }
   delete[] pmuxeddata;
   delete[] pdata;
}
/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(BipRasterWriter::WriterFunc, writebil);

/**
 * Constructor
 * @param[in] Filename nombre del archivo.
 * @return instancia de la clase BipRasterWriter
 */
BipRasterWriter::BipRasterWriter(const std::string &Filename) :
      RasterWriter("BipWriter", Filename), fullWrite_(false), pWriteFunc_(NULL) {
   ostream_.open(Filename.c_str(), std::ofstream::binary);
}

/** Destructor */
BipRasterWriter::~BipRasterWriter() {
   // Si no pase por el fin del archivo, escribo un 0 para que al cerrar
   // quede con el tamanio deseado
   if (!fullWrite_) {
      ostream_.seekp(GetSizeX() * GetSizeY() * GetBandCount() * GetDataSize(),
                     std::ios::beg);
      ostream_ << 0;
   }
   ostream_.close();
}

/**
 * Cambia el tipo de dato
 * @param[in] DataType tipo de dato.
 */
void BipRasterWriter::SetDataType(const std::string &DataType) {
   RasterWriter::SetDataType(DataType);
   pWriteFunc_ = writebilTypeMap[dataType_];
}

/** Bloque de bandas/subset que debe escribir */
/**
 *  Esta funcion devuelve el subset que el driver puede escribir.
 *
 *  Se utiliza cuando la escritura no puede ser aleatoria y el writer tiene
 * que indicar el bloque que requiere para realizar una escritura atomica.
 *
 * @param[out] BandIndex un vector con los indices de las bandas
 * @param[out] Ulx coordenada X de la esquina UL del subset
 * @param[out] Uly coordenada Y de la esquina UL del subset
 * @param[out] Lrx coordenada X de la esquina LR del subset
 * @param[out] Lry coordenada Y de la esquina LR del subset
 * @return true si se encontro otro bloque
 * @return false si se llego al final del archivo
 */
bool BipRasterWriter::GetNextBlock(std::vector<int> &BandIndex, int &Ulx, int &Uly,
                                   int &Lrx, int &Lry) {
   if (!RasterWriter::GetNextBlock(BandIndex, Ulx, Uly, Lrx, Lry)) {
      // muevo al inicio del archivo
      ostream_.seekp(0, std::ios::beg);
      return false;
   }
   return true;
}

/** Escritura de un bloque */
/**
 *  Esta funcion devuelve el subset que el driver puede escribir.
 *
 *  Se utiliza cuando la escritura no puede ser aleatoria y el writer tiene
 * que indicar el bloque que requiere para realizar una escritura atomica.
 *
 * @param[in] BandIndex un vector con los indices de las bandas
 * @param[in] Data un vector con los punteros a los datos de las bandas
 * @param[in] Ulx coordenada X de la esquina UL del subset
 * @param[in] Uly coordenada Y de la esquina UL del subset
 * @param[in] Lrx coordenada X de la esquina LR del subset
 * @param[in] Lry coordenada Y de la esquina LR del subset
 *
 * \pre BandIndex debe ser [0;GetBandCount())
 * \pre Data debe tener una cantidad de punteros igual a GetBandCount()
 */
void BipRasterWriter::Write(const std::vector<int> &BandIndex, std::vector<void*> &Data,
                            int Ulx, int Uly, int Lrx, int Lry) {
   // Verifico que tenga todo el mismo tamanio
   if (BandIndex.size() != Data.size() || (int) BandIndex.size() != GetBandCount()) {
      REPORT_AND_FAIL("D:BipWriter:Las dimensiones de los vectores no concuerdan.");
   }
   // Verifico que esten todas las bandas y en orden creciente
   for (int i = 0; i < GetBandCount(); i++)
      if (BandIndex[i] != i) {
         REPORT_AND_FAIL("D:BipWriter: Las bandas seleccionadas no son validas.");
      }

   // Si el puntero de escritura no esta en la posicion correcta, lo lleva
   std::ios::pos_type wantedpos = (GetSizeX() * Uly + Ulx) * GetDataSize()
         * GetBandCount();
   if (wantedpos != ostream_.tellp()) {
      ostream_.seekp(wantedpos, std::ios::beg);
   }
   // Si paso por el fin del archivo, seteo el flag
   if (Lrx >= GetSizeX() && Lry >= GetSizeY()) {
      fullWrite_ = true;
   }

   pWriteFunc_(ostream_, Data, (Lrx - Ulx) * (Lry - Uly),
               (GetSizeX() - (Lrx - Ulx)) * GetDataSize() * GetBandCount(), Lrx - Ulx);
}
}
