/*! \file DaubechiesTransformStrategy.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#define SUB_VECTOR_SIZE 8

// Includes Estandar
#include <string>
// Includes Suri
#include "DaubechiesTransformStrategy.h"
#include "suri/DataTypes.h"
// Includes Wx
// Defines
// forwards

namespace suri {

template<class T>
void ReadSubVectorH(int offset, void* data, int width,
                    int height, double* subVector);
template<class T>
void ReadSubVectorV(int offset, void* data, int width,
                    int height, double* subVector);

// Inicializa mapa de tipos de datos.
INITIALIZE_DATATYPE_MAP(DaubechiesTransformStrategy::ReadFunctionType,
                        ReadSubVectorH);
INITIALIZE_DATATYPE_MAP(DaubechiesTransformStrategy::ReadFunctionType,
                        ReadSubVectorV);

double DaubechiesTransformStrategy::D4ScalingCoefficients[8] =
      {-0.010597401784997278334, 0.03288301166698294476, 0.03084138183598696473,
       -0.1870348117188811443, -0.02798376941698384884, 0.6308807679295903581,
       0.714846570552541527, 0.2303778133088552338};
double DaubechiesTransformStrategy::D4WaveletCoefficients[8] =
      {-0.2303778133088552338, 0.714846570552541527, -0.6308807679295903581,
       -0.02798376941698384884, 0.1870348117188811443, 0.03084138183598696473,
       -0.03288301166698294476, -0.010597401784997278334};
/**
 * Constructor
 */
DaubechiesTransformStrategy::DaubechiesTransformStrategy(
      void* data, int width, int height, const std::string& dataType):
         data_(data), width_(width), height_(height), dataType_(dataType) {
   this->readSubVectorFunction_ = ReadSubVectorV<unsigned char>;
}

/**
 * Destructor
 */
DaubechiesTransformStrategy::~DaubechiesTransformStrategy() {}

float ScalarProduct(double* vectorA, double* vectorB, int size) {
   float result = 0;
   for (int i = 0; i < size; ++i) {
      result += vectorA[i] * vectorB[i];
   }
   return result;
}

DaubechiesTransformHorizontal::DaubechiesTransformHorizontal(
      void* data, int width, int height, const std::string& dataType):
            DaubechiesTransformStrategy(data, width, height, dataType) {}

/**
 * Lee una porcion de un vector de tipo T y lo devuelve en double
 */
template<class T>
void ReadSubVectorH(int offset, void* data,
                    int width, int height, double* subVector) {
   int hOffset = offset % width;
   // Cuantos bits me faltan para completar el vector
   int overflow = 0;
   if (hOffset + SUB_VECTOR_SIZE > width) {
      overflow = (offset + SUB_VECTOR_SIZE) % width;
   }
   for (int i = 0; i < SUB_VECTOR_SIZE - overflow; ++i) {
      subVector[i] = static_cast<T*>(data)[offset + i];
   }
   // Espejo
   for (int i = 0; i < overflow; ++i) {
      // Ultimo valor valido del vector, desde ahi espejo.
      int lastValue = SUB_VECTOR_SIZE - overflow;
      subVector[lastValue + i] = static_cast<T*>(data)[offset + lastValue - i - 1];
   }
}

/**
 * Tomo un vector horizontal y transformo pixel a pixel
 */
float* DaubechiesTransformHorizontal::Transform() {
   const int imageSize = this->width_ * this->height_;
   float *output = new float[imageSize];
   double subVector[SUB_VECTOR_SIZE];
   int offset = 0;
   this->readSubVectorFunction_ = ReadSubVectorHTypeMap[this->dataType_];
   for (int line = 0; line < this->height_; ++line) {
      // Le resto 1 por si es de ancho impar
      for (int j = 0; j < this->width_ - 1; j += 2) {
         readSubVectorFunction_(offset, this->data_, this->width_, this->height_, subVector);
         output[offset] = ScalarProduct(D4ScalingCoefficients, subVector, SUB_VECTOR_SIZE);
         output[offset + 1] = ScalarProduct(D4WaveletCoefficients, subVector, SUB_VECTOR_SIZE);
         offset += 2;
      }
      // Si el ancho es impar, retrocedo 1 pixel y recalculo;
      if (this->width_ % 2) {
         --offset;
         readSubVectorFunction_(offset, this->data_, this->width_, this->height_, subVector);
         output[offset] = ScalarProduct(D4ScalingCoefficients, subVector, SUB_VECTOR_SIZE);
         output[offset + 1] = ScalarProduct(D4WaveletCoefficients, subVector, SUB_VECTOR_SIZE);
         offset += 2;
      }
   }
   return output;
}

/**
 * Constructor
 */
DaubechiesTransformVertical::DaubechiesTransformVertical(
      void* data, int width, int height, const std::string& dataType):
            DaubechiesTransformStrategy(data, width, height, dataType) {}

template<class T>
void ReadSubVectorV(int offset, void* data, int width,
                    int height, double* subVector) {
   int vOffset = offset / width;
   // Cuantos bits me faltan para completar el vector
   int overflow = 0;
   if (vOffset + SUB_VECTOR_SIZE > height) {
      overflow = (vOffset + SUB_VECTOR_SIZE) % height;
   }
   for (int i = 0; i < SUB_VECTOR_SIZE - overflow; ++i) {
      subVector[i] = static_cast<T*>(data)[offset + i * width];
   }
   // Espejo
   for (int i = 0; i < overflow; ++i) {
      // Ultimo valor valido del vector, desde ahi espejo.
      int lastValue = SUB_VECTOR_SIZE - overflow;
      int mirrorOffset = offset + (lastValue - i - 1) * width;
      subVector[lastValue + i] = static_cast<T*>(data)[mirrorOffset];
   }
}

/**
 * Tomo vectores de forma vertical y transformo haciendo un barrido horizontal
 * de a 2 lineas por vez.
 */
float* DaubechiesTransformVertical::Transform() {
   const int imageSize = this->width_ * this->height_;
   float *output = new float[imageSize];
   double subVector[SUB_VECTOR_SIZE];
   int offset = 0;
   this->readSubVectorFunction_ = ReadSubVectorVTypeMap[this->dataType_];
   for (int line = 0; line < this->height_- 1; line += 2) {
      for (int j = 0; j < this->width_; ++j) {
         readSubVectorFunction_(offset, this->data_, this->width_, this->height_, subVector);
         output[offset] = ScalarProduct(D4ScalingCoefficients,
                                        subVector, SUB_VECTOR_SIZE);
         output[offset + this->width_] = ScalarProduct(D4WaveletCoefficients,
                                                       subVector, SUB_VECTOR_SIZE);
         ++offset;
      }
      offset += this->width_;
   }
   return output;
}

/**
 * Constructor
 */
DaubechiesTransform2D::DaubechiesTransform2D(
      void* data, int width, int height, const std::string& dataType):
            DaubechiesTransformStrategy(data, width, height, dataType) {}

/**
 * Transformo de forma vertical y luego horizontal
 */
float* DaubechiesTransform2D::Transform() {
   // Primer etapa: transformo vertical
   DaubechiesTransformVertical vertTransform(this->data_, this->width_,
                                             this->height_, this->dataType_);
   float* stage1 = vertTransform.Transform();
   // Segunda etapa: transformo horizontal
   DaubechiesTransformHorizontal horizTransform(stage1, this->width_,
                                             this->height_, "float");
   float* stage2 = horizTransform.Transform();
   // Libero memoria
   delete[] stage1;
   return stage2;
}

} /** namespace suri */
