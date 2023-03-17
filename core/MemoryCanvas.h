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

#ifndef MEMORYCANVAS_H_
#define MEMORYCANVAS_H_

// Includes standard
#include <vector>

// Includes suri
#include "suri/Canvas.h"

// forwards
class wxBitmap;
class wxMemoryDC;

/** namespace suri */
namespace suri {
class RasterBand;

/** Canvas que guarda la informacion en memoria */
/**
 *  Esta clase implementa la interfaz de Canvas y guarda los datos en memoria.
 *
 *  El almacenamiento se realiza en RasterBand y se provee interfaz de acceso
 * a las mismas.
 *
 *  Se utiliza para lectura/escritura de datos renderizados y buffers
 * temporales.
 * \note hereda de Canvas para permitir que el pipeline de renderizacion pueda
 *       escribir resultados en memoria
 */
class MemoryCanvas : public Canvas {
   /** Ctor. de Copia. */
   MemoryCanvas(const MemoryCanvas &MemoryCanvas);

public:
   /** ctor */
   MemoryCanvas();
   /** dtor */
   virtual ~MemoryCanvas();
   /** Metodo de clonacion */
   virtual MemoryCanvas *Clone() const;
// ----------------------------- RASTER -----------------------------
   /** Tamanio */
   virtual void SetSize(const int SizeX, const int SizeY);
   /** Lectura del canvas */
   virtual void Read(std::vector<int> &BandIndex, std::vector<void*> &Data) const;
   /** Escritura al canvas */
   virtual void Write(std::vector<int> &BandIndex, std::vector<void*> &Data,
                      const Mask *pMask = NULL);
// ----------------------------- VECTORIAL -----------------------------
   /** Canvas vectorial */
   /**
    *  Retorna un canvas wxDC que permite realizar operaciones vectoriales, ej.
    * DrawLine, etc.
    *  Al tratarse de un canvas visual, solo escribira en un maximo de 3 bandas
    * asumiendo 012 == RGB.
    *  El uso que se le da en esta clase es para escribir en un wxMemoryDC y
    * de ahi pasarlo al buffer interno correspondiente (usando
    * Canvas::FlushDC()). En clases derivadas puede implementarse de otras
    * maneras (ej. ScreenCanvas, escribe directamente a la pantalla).
    * \pre Solo soporta escritura sobre las tres primeras bandas.
    * \post Los datos deben ser pasados al buffer principal usand FlushDC().
    * @return Canvas sobre el cual escribir datos vectoriales.
    * \todo Implementar GetDC()
    */
   virtual wxDC *GetDC();
   /** Escribe los datos */
   virtual void Flush(const Mask *pMask);
   /** Pasa al buffer interno la renderizacion sobre wxDC */
   /**
    *  Permite pasar del DC al buffer interno.
    */
   virtual void FlushDc();
   /** Limpia los datos del canvas */
   virtual void Clear();
   /** Limpia los datos del DC */
   virtual void ClearDc();
// -------------------------- MULTI-RASTER --------------------------
   /** Retorna la cantidad de bandas */
   virtual int GetBandCount() const;
   /** Permite setear la cantidad de bandas */
   virtual void SetBandCount(int BandCount, bool SaveBands = false);
// --------------------------- DATA-TYPE ----------------------------
   /** Nombre del tipo de dato */
   virtual std::string GetDataType() const;
   /** Tamanio del dato en bytes */
   virtual int GetDataSize() const;
   /** Nombre del tipo de dato */
   virtual void SetDataType(const std::string& DataType);

// ----------------------------- BANDAS -----------------------------
   /** obtiene la banda */
   virtual RasterBand *GetBand(int BandIndex) const;
   /** Setea una banda deleteando la que habia */
   virtual void SetBand(RasterBand* pBand, int BandIndex);
   /** Retorna buffers internos con los datos */
   virtual void GetInternalData(std::vector<int> &BandIndex,
                                std::vector<void*> &OutputData);
protected:
private:
   /** Elimina las bandas asociadas */
   void DeleteBands();
   std::vector<RasterBand*> bandVector_; /*! Vector de bandas asociadas */
   std::string dataType_; /*! Tipo de dato que maneja el Canvas */
   wxBitmap *pDcBitmap_; /*! Bitmap asociado al DC */
   wxMemoryDC *pDc_; /*! DC utilizado para escritura de vectores */
   int bandCount_; /*! Cantidad de bandas (utilizado para poder inicializar */
   /* en cualquier orden) */
};
}

#endif /*MEMORYCANVAS_H_*/
