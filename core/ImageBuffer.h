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
