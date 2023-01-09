/*! \file ImageBuffer.h */
/*
 * Biblioteca de procesamiento GeoEspacial SuriLib.
 * Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 * Este producto contiene software desarrollado por
 * SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <vector>
#include <string>
// Includes Suri
#include "suri/Image.h"
#include "ImageBuffer.h"
// Includes Wx
// Defines
// forwards

namespace suri {
    
const unsigned int ImageBuffer::kBufferHeight = 300;

/**
 * Constructor
 */
ImageBuffer::ImageBuffer (const Image* pImage,
                          const std::vector<int>& selectedBands) : updateBufferOffset_(0) {
   this->selectedBand = selectedBands[0];
   this->pImage_ = pImage;
   this->imgHeight_ = pImage_->GetSizeY();
   this->imgWidth_ = pImage_->GetSizeX();
   this->bufferHeight_ = std::min<unsigned int>(kBufferHeight, this->imgHeight_);
   this->bufferOffsetY_ = 0;
   this->dataSize_ = this->pImage_->GetDataSize();
   this->pBuffer_.resize(imgWidth_ * 
         (bufferHeight_ + this->updateBufferOffset_) * this->dataSize_);
   //Leo el  primer cacho de imagen
   this->pImage_->GetBand(this->selectedBand)->Read(&this->pBuffer_[0], 0, 0,
                                   this->imgWidth_, this->bufferHeight_);
}

/**
 * Destructor
 */
ImageBuffer::~ImageBuffer() {}


/**
 * Avanza el buffer, devuelve true si el buffer tiene contenido válido
 * o false si terminó de recorrer la imagen
 */
bool ImageBuffer::UpdateBuffer() {
   unsigned int remaining = this->imgHeight_ - this->bufferOffsetY_;
   if (!remaining){
      return false;
   }
   if (remaining < this->updateBufferOffset_){
      return false;
   }
   MoveOffsetBuffer();
   // El buffer es mas grande de lo que falta leer
   if (this->bufferHeight_  > remaining){
      this->bufferHeight_ = remaining;
   }
   // Distancia desde donde tiene que escribir el buffer
   unsigned int bufferWriteOffset = this->updateBufferOffset_ * this->imgWidth_;
   REPORT_DEBUG("Leyendo %d-%d en %p:", this->imgWidth_, this->bufferOffsetY_ + this->bufferHeight_, GetBuffer(bufferWriteOffset));
   this->pImage_->GetBand(this->selectedBand)->Read(&this->pBuffer_[bufferWriteOffset * this->dataSize_] ,
                                   0, this->bufferOffsetY_, this->imgWidth_,
                                   this->bufferOffsetY_ + this->bufferHeight_);
   return true;
}

void* ImageBuffer::GetBuffer(unsigned int offset) {
   return &this->pBuffer_[offset * this->dataSize_];
}

/**
 * @pre bufferHeight_ > updateBufferOffset_
 */
void ImageBuffer::MoveOffsetBuffer() {
   unsigned int lowerBuffStartY = this->bufferHeight_ - this->updateBufferOffset_;
   unsigned int lowerBuffStartPos = lowerBuffStartY * this->imgWidth_ ;
   unsigned int offsetSize = this->updateBufferOffset_ * this->imgWidth_ ;
   memcpy(&this->pBuffer_[0], GetBuffer(lowerBuffStartPos),  offsetSize * this->dataSize_ );
}

unsigned int ImageBuffer::GetImageHeight() const {
   return this->imgHeight_;
}

unsigned int ImageBuffer::GetImageWidth() const {
   return this->imgWidth_;
}

bool ImageBuffer::Read(void* dest, int ulx, int uly, int lrx, int lry) {
   unsigned char* ucharDest = static_cast<unsigned char*>(dest);
   if (!IsBuffered(ulx, uly)){
      OnMiss(ulx, uly);
   }
   int bufferUly = uly - this->bufferOffsetY_;
   int bufferLry = lry - this->bufferOffsetY_;
   int lineWidth = lrx - ulx;
   // Aprovecho que el buffer es vertical
   int bufferTop = std::min(bufferLry, static_cast<int>(this->bufferHeight_));
   for (int y = bufferUly; y < bufferTop; ++y){
      int lineStart = y * this->imgWidth_ + ulx;
      memcpy(ucharDest, GetBuffer(lineStart), lineWidth * this->dataSize_);
      ucharDest += lineWidth * this->dataSize_;
   }
   // Si el bloque era más grande que el buffer
   if (bufferLry > static_cast<int>(bufferHeight_)){
      return Read(ucharDest, ulx, uly + bufferHeight_, lrx, lry);
   }
   return true;
}

void ImageBuffer::SetBufferHeight(unsigned int bufferHeight) {
   this->bufferHeight_ = bufferHeight;
}

void ImageBuffer::SetUpdateBufferOffset(unsigned int offset) {
   this->updateBufferOffset_ = offset;
   this->pBuffer_.resize(imgWidth_ * 
         (bufferHeight_ + this->updateBufferOffset_) * this->dataSize_);
}

int ImageBuffer::GetDataSize() const {
   return this->pImage_->GetDataSize();
}

std::string ImageBuffer::GetDataType() const {
   return this->pImage_->GetDataType();
}


bool ImageBuffer::IsBuffered (unsigned int x, unsigned int y ) {
   return (y >= this->bufferOffsetY_ &&
      y < (this->bufferOffsetY_ + this->bufferHeight_));
}


/**
 * @pre missedY es valido
 */
void ImageBuffer::OnMiss(unsigned int missedX, unsigned int missedY) {
   this->bufferOffsetY_ = missedY;
   UpdateBuffer();
}

}
