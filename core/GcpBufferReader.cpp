/*! \file GcpBufferReader.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Suri
#include "GcpBufferReader.h"
#include "suri/DataTypes.h"
#include "suri/Image.h"

namespace suri {


const unsigned int GcpBufferReader::kProximityRadius = 13;

/**
 * Constructor
 */
GcpBufferReader::GcpBufferReader(Image* pImage,
                          const std::vector<int>& gcpBand) :
    buffer_(pImage, gcpBand){
   unsigned int imgHeight = pImage->GetSizeY();
   this->proximityWindowRadius_ = kProximityRadius;
   this->buffer_.SetUpdateBufferOffset(kProximityRadius);
   this->offsetX_ = kProximityRadius;
   this->offsetY_ = kProximityRadius;
   if (imgHeight < (proximityWindowRadius_ * 2)) {
      // TODO matiaslafroce: tirar una excepcion bien hecha
      throw "Invalid image size";
   }
}

/**
 * Recorre el buffer en busca de un candidato a GCP. Si encuentra un punto,
 * indica sus coordenadas por medio de los parametros. Si el archivo se termina
 * y no encuentra mas puntos, devuelve false.
 * @param candidateX Coordenada X del candidato encontrado
 * @param candidateY Coordenada Y del candidato encontrado
 */
bool GcpBufferReader::GetNextCandidate (int& candidateX, int& candidateY) {
   unsigned int height = this->buffer_.GetImageHeight();
   unsigned int width = this->buffer_.GetImageWidth();
   while (this->offsetY_ < (height - this->proximityWindowRadius_)){
      while (this->offsetX_ < (width - this->proximityWindowRadius_)){
         // @pre: los candidatos me vienen en formato float
         if (this->buffer_.GetPixel<float>(this->offsetX_, offsetY_) != 0){
            candidateX = this->offsetX_;
            candidateY = this->offsetY_;
            // Avanzo para que la próxima no repita el candidato
            ++offsetX_;
            return true;
         }
         ++offsetX_;
      }
      this->offsetX_ = this->proximityWindowRadius_;
      ++offsetY_;
   }
   return false;
}


/**
 * Elimina los candidatos ubicados en la coordenada indicada.
 * Además, elimina los vecinos ubicados en una ventana de proximidad
 * del GCP generado.
 */
void GcpBufferReader::RemoveCandidates(unsigned int gcpX, unsigned int gcpY) {
   // Paso a las coordenadas del archivo de candidatos
   int ulx = gcpX - this->proximityWindowRadius_;
   int uly = gcpY - this->proximityWindowRadius_;
   int lrx = gcpX + this->proximityWindowRadius_ + 1;
   int lry = gcpY + this->proximityWindowRadius_ + 1;
   ulx = std::max<int>(0, ulx);
   uly = std::max<int>(0, uly);
   lrx = std::min<int>(this->buffer_.GetImageWidth(), lrx);
   lry = std::min<int>(this->buffer_.GetImageHeight(), lry);
   for (int yPos = uly; yPos < lry; ++yPos){
      for (int xPos = ulx; xPos < lrx; ++xPos){
         this->buffer_.SetPixel<float>(xPos, yPos, 0);
      }
   }
}

unsigned int GcpBufferReader::GetProximityWindowRadius() const {
   return this->proximityWindowRadius_;
}

void GcpBufferReader::SetProximityWindowRadius(unsigned int ProxRadius) {
   this->proximityWindowRadius_ = ProxRadius;
}


}
