/*! \file FileCanvas.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "FileCanvas.h"

// Includes estandar
#include <string>
#include <vector>

// Includes suri
#include "suri/messages.h"
#include "MemoryCanvas.h"
#include "logmacros.h"
#include "suri/DataTypes.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Dimension.h"

/** namespace suri */
namespace suri {

template<typename T1>
void SetValue(const std::vector<void*>& Bands, int Position, double Value) {
   std::vector<void*>::const_iterator it = Bands.begin();
   bool noData = true;
   T1 value = static_cast <T1> (Value);
   for (it = Bands.begin(); it != Bands.end(); ++it) {
      T1* band = static_cast<T1*>(*it);
      if (band[Position] != 0) {
         noData = false;
      }
   }
   if (noData) {
      for (it = Bands.begin(); it != Bands.end(); ++it) {
         T1* band = static_cast<T1*>(*it);
         band[Position] = value;
      }
   }
}

typedef void (*Sfunction)(const std::vector<void*>&, int, double);


/** Genera un mapa doble (matriz) indexado por tipos a la funcion */
INITIALIZE_DATATYPE_MAP(Sfunction, SetValue);

/**
 * Constructor
 * @param[in] pImage puntero a la imagen
 */
FileCanvas::FileCanvas(Image *pImage) :
      pImage_(pImage), filename_(""), imageWidth_(0), imageHeight_(0), extraDataSize_(0) {
   SetDataType(pImage_ ? pImage_->GetDataType() : DataInfo<void>::Name);
   SetBandCount(pImage_ ? pImage_->GetBandCount() : 0);
   noDataValue_ = 0;
}

/**
 * Constructor
 * @param[in] Filename nombre de archivo de salida
 * @param[in] SizeX ancho de la imagen
 * @param[in] SizeY alto de la imagen
 * @param[in] ImageOptions opciones para la imagen
 */
FileCanvas::FileCanvas(const std::string &Filename, const int SizeX, const int SizeY,
                       const Option &ImageOptions) :
      pImage_(NULL), filename_(Filename), imageWidth_(SizeX), imageHeight_(SizeY),
      extraDataSize_(0), extraData_(std::vector<void*>()), imageOptions_(ImageOptions) {
   std::string datainfo = imageOptions_.GetOption("DataInfo");
   std::string bandcount = imageOptions_.GetOption("BandCount");
   noDataValue_ = 0;
   if (!datainfo.empty() && !bandcount.empty()) {
      SetDataType(datainfo);
      SetBandCount(StringToNumber<int>(bandcount));
   }
}


void FileCanvas::AddFeathering(){
   if (!intersection_.IsValid()) return;
   std::vector<int> bandindex(GetBandCount());
   for (int x = 0; x < GetBandCount(); x++) bandindex[x] = x;
   int ulx = SURI_ROUND(int, intersection_.ul_.x_);
   int uly = SURI_ROUND(int, intersection_.ul_.y_);
   int lrx = SURI_ROUND(int, intersection_.lr_.x_);
   int lry = SURI_ROUND(int, intersection_.lr_.y_);
   pImage_->Write(bandindex, featheringData_, ulx, uly, lrx, lry);
}

void FileCanvas::SetBackgroundColour() {
   std::vector<int> bandindex(GetBandCount());
   std::vector<void*> imageBands(GetBandCount());
   Image* pimage = Image::Open(filename_);
   int datasize = pimage->GetDataSize();
   for (int i = 0; i < GetBandCount(); i++) {
         bandindex[i] = i;
         imageBands[i] = new unsigned char[imageWidth_ * imageHeight_
               * datasize];
   }
   pimage->Read(bandindex, imageBands, 0, 0, imageWidth_, imageHeight_);
   Image::Close(pimage);
   std::vector<void*>::const_iterator it = imageBands.begin();
   for (int x = 0; x < imageWidth_ * imageHeight_; x++) {
      SetValueTypeMap[GetDataType()](imageBands, x, noDataValue_);
   }
   pImage_ = Image::Open(filename_, Image::WriteOnly, "", GetBandCount(),
                               imageWidth_, imageHeight_, GetDataType(), imageOptions_);
   pImage_->Write(bandindex, imageBands, 0, 0, imageWidth_, imageHeight_);
   Image::Close(pImage_);
}


void FileCanvas::SetFeathering(const Subset& Intersection, const std::vector<void*>& Data,
                                                           unsigned char NoDataValue){
   noDataValue_ = NoDataValue;
   featheringData_ = Data;
   intersection_ = Intersection;
}


/**
 * Destructor
 */
FileCanvas::~FileCanvas() {
   AddFeathering();
   Image::Close(pImage_);
   if (noDataValue_ != 0)
       SetBackgroundColour();
   // elimino los viejos
   for (size_t i = 0; i < extraData_.size(); i++)
      delete[] static_cast<unsigned char*>(extraData_[i]);
}

/**
 * Pasa al buffer interno la renderizacion sobre wxDC
 * Permite pasar del DC al buffer interno.
 */
void FileCanvas::Flush(const Mask *pMask) {
   if (!pImage_) {
      pImage_ = Image::Open(filename_, Image::WriteOnly, "", GetBandCount(),
                            imageWidth_, imageHeight_, GetDataType(), imageOptions_);
      if (!pImage_) {
         REPORT_AND_FAIL("D:No se pudo crear la imagen de salida");
         return;
      }
   }
   MemoryCanvas::Flush(pMask);
   std::vector<int> bandindex(GetBandCount());
   std::vector<void*> data(GetBandCount());
   std::vector<void*> blockdata(GetBandCount());
   int x = 0, y = 0;
   GetSize(x, y);
   for (int i = 0; i < GetBandCount(); i++) {
      bandindex[i] = i;
      data[i] = new unsigned char[x * y * GetDataSize()];
      blockdata[i] = data[i];
   }
   // leo los datos del buffer
   Read(bandindex, data);
   int blockx = 0, blocky = 0;
   pImage_->GetBlockSize(blockx, blocky);
   // incremento del buffer
   int bufferstep = blockx * blocky * GetDataSize();
   // cantidad de datos escritos en palabras del tipo de dato
   int datawritten = 0;
   int ulx = 0, uly = 0, lrx = 0, lry = 0;
   // escribo los datos del buffer en bloques del driver
   // asume que el buffer contiene toda la imagen
   if (extraDataSize_ > 0 && GetNextBlock(bandindex, ulx, uly, lrx, lry)) {
      for (int b = 0; b < GetBandCount(); b++) {
         // copio los datos restantes para completar un bloque de datos
         memcpy(
               static_cast<unsigned char*>(extraData_[b])
                     + extraDataSize_ * GetDataSize(),
               blockdata[b], (blockx * blocky - extraDataSize_) * GetDataSize());
      }
      pImage_->Write(bandindex, extraData_, ulx, uly, lrx, lry);
      // los datos que se escribieron del canvas
      datawritten += (blockx * blocky - extraDataSize_);
      // muevo el puntero de datos del canvas hacia adelante en los datos escritos
      for (int b = 0; b < GetBandCount(); b++)
         blockdata[b] = static_cast<unsigned char*>(blockdata[b])
               + datawritten * GetDataSize();
   }
   bool canvasflushed = false;
   while (!canvasflushed && GetNextBlock(bandindex, ulx, uly, lrx, lry)) {
      // escribo
      pImage_->Write(bandindex, blockdata, ulx, uly, lrx, lry);
      // muevo el puntero de lectura
      for (int b = 0; b < GetBandCount(); b++)
         blockdata[b] = static_cast<unsigned char*>(blockdata[b]) + bufferstep;
      // incremento la cantidad de datos escritos
      datawritten += blockx * blocky;
      // si no queda espacio en el canvas para escribir el proximo bloque
      // salgo
      if (x * y - datawritten < blockx * blocky) {
         if (x * y > datawritten) {
            extraDataSize_ = x * y - datawritten;
            // elimino los viejos
            for (size_t i = 0; i < extraData_.size(); i++)
               delete[] static_cast<unsigned char*>(extraData_[i]);
            extraData_.clear();
            // reservo espacio
            for (int b = 0; b < GetBandCount(); b++) {
               extraData_.push_back(new unsigned char[blockx * blocky * GetDataSize()]);
               memcpy(extraData_[b], blockdata[b], extraDataSize_ * GetDataSize());
            }
         }
      }
      // se agoto el canvas
      canvasflushed = (x * y - datawritten < blockx * blocky);
   }
   for (int i = 0; i < GetBandCount(); i++)
      delete[] static_cast<unsigned char*>(data[i]);
}

// ----------------------------- BLOQUE -----------------------------
/**
 *  Permite determinar cual es el bloque de escritura del archivo que se
 * debe renderizar. Esto es por las posibles limitaciones de los formatos
 * para escribir en forma aleatoria.
 *
 *  Este metodo es forwardeado a la imagen interna.
 *
 * @param[out] BandIndex Indice de bandas que deben renderizarse.
 * @param[out] Ulx Columna superior isquierda del bloque a renderizar.
 * @param[out] Uly Fila superior isquierda del bloque a renderizar.
 * @param[out] Lrx Columna inferior derecha del bloque a renderizar.
 * @param[out] Lry Fila inferior derecha del bloque a renderizar.
 */
bool FileCanvas::GetNextBlock(std::vector<int> &BandIndex, int &Ulx, int &Uly, int &Lrx,
                              int &Lry) {
   bool result = pImage_ ? pImage_->GetNextBlock(BandIndex, Ulx, Uly, Lrx, Lry) : false;
   return result;
}

/** Devuelve el la ruta con el que fue creado el archivo **/
std::string FileCanvas::GetFileName() const {
   return this->filename_;
}
}
