/*! \file ImageBuffer.h */
/*
 * Biblioteca de procesamiento GeoEspacial SuriLib.
 * Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 * Este producto contiene software desarrollado por
 * SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef IMAGEBUFFER_H
#define IMAGEBUFFER_H

// Includes Estandar
#include <vector>
#include <string>
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri{
   
class Image;

class ImageBuffer{
public:
   /**
    * Constructor
    */
   explicit ImageBuffer(const Image *pImage,
                        const std::vector<int>& selectedBands);
   /**
    * Destructor
    */
   ~ImageBuffer();
   /**
    * Lee los datos en un bloque
    */
   virtual bool Read(void* dest, int ulx, int uly, int lrx, int lry);
   /** Dimensiones del buffer **/
   void SetBufferHeight(unsigned int offset);
   /** Cantidad de lineas que se conservan por update **/
   void SetUpdateBufferOffset(unsigned int offset);
   /**
    * Devuelve el valor del dato en la coordenada X,Y
    */
   template <class T>
   T GetPixel(unsigned int X, unsigned int Y);
   /**
    * Escribe en el buffer (sin writeback)
    */
   template <class T>
   void SetPixel (int X, int Y, T value);
   /** Dimensiones de la imagen **/
   unsigned int GetImageHeight() const; 
   unsigned int GetImageWidth() const;
   /** Otros metadatos **/
   std::string GetDataType() const;
   int GetDataSize() const;
protected:
   /**
    * Se la llama cuando quiero leer una parte que no esta en el buffer.
    */
   virtual void OnMiss(unsigned int missedX, unsigned int missedY);
private:
   /**
    * Devuelve el buffer
    */
   void* GetBuffer(unsigned int offset = 0);
   /**
    * Copia la sección inferior del buffer a la parte superior.
    */
   void MoveOffsetBuffer();
   std::vector<unsigned char> pBuffer_;
   /** Imagen a leer **/
   const Image* pImage_;
   /** Banda a leer**/
   int selectedBand;
   /** Dimensiones de la imagen **/
   unsigned int imgHeight_;
   unsigned int imgWidth_;
   unsigned int dataSize_;
   /** Dimensiones del buffer **/
   unsigned int updateBufferOffset_;
   unsigned int bufferHeight_;
   unsigned int bufferOffsetY_;
   /**
    * Avanza el buffer, devuelve true si el buffer tiene contenido válido
    * o false si terminó de recorrer la imagen
    */
   bool UpdateBuffer();
   /**
    * Me fijo si la coordenada esta en el buffer.
    */
    bool IsBuffered( unsigned int x, unsigned int y );
   /**
    * Altura default del buffer de lectura
    */
   static const unsigned int kBufferHeight;
};

template <class T>
T ImageBuffer::GetPixel(unsigned int X, unsigned int Y){
   if (!IsBuffered(X, Y)){
      OnMiss(X, Y);
   }
   unsigned int bufY = Y - this->bufferOffsetY_;
   unsigned int pos = X + bufY * this->imgWidth_;
   return reinterpret_cast<T*>(&this->pBuffer_.front())[pos];
}

template <class T>
void ImageBuffer::SetPixel(int X, int Y, T value){
   if (!IsBuffered(X, Y)){
      OnMiss(X, Y);
   }
   unsigned int bufY = Y - this->bufferOffsetY_;
   unsigned int pos = X + bufY * this->imgWidth_;
   reinterpret_cast<T*>(&this->pBuffer_.front())[pos] = value;
}


} // namespace suri

#endif // IMAGEBUFFER_H
