/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
