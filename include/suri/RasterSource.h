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

#ifndef RASTERSOURCE_H_
#define RASTERSOURCE_H_

#include <string>
#include <set>

namespace suri {

class SourceSupervisor;

/** clase base para todas las que proveen informacion raster */
/**
 *  Esta es la clase base que presenta una interfaz de acceso a datos raster
 * genericos.
 *  Maneja los distintos tipos de datos.
 *  Tiene metodo de clonacion para cuando es necesario agregar una misma
 * instancia a varias fuentes (como al agregar en imagen)
 */
class RasterSource {
public:
   friend class SourceSupervisor;

   /** Ctor. */
   RasterSource(SourceSupervisor *pSupervisor = NULL);

   /** Dtor. */
   virtual ~RasterSource();

   /** Tamanio del raster X e Y */
   virtual void GetSize(int &SizeX, int &SizeY) const;

   /** Tamanio X del raster */
   int GetSizeX() const;

   /** Tamanio Y del raster */
   int GetSizeY() const;

   /** Retorna un buffer interno con los datos */
   virtual void* GetBlock(int BlockX, int BlockY)=0;

   /** Carga el buffer con el subset */
   virtual bool Read(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry)=0;

   /** Tamanio del bloque X e Y */
   virtual void GetBlockSize(int &SizeX, int &SizeY) const;

   /** Tamanio X del bloque */
   int GetBlockSizeX() const;

   /** Tamanio Y del bloque */
   int GetBlockSizeY() const;

   /** Nombre del tipo de dato */
   virtual std::string GetDataType() const;

   /** Tamanio del dato en bytes */
   virtual int GetDataSize() const;

   /** Agrega una fuente */
   virtual void PushSource(RasterSource *pSource);

   /** Elimina la ultima fuente */
   virtual RasterSource *PopSource();

   /** Actualiza las fuentes de atras para adelante */
   void UpdateSource();

   /** Actualiza la fuente */
   virtual void Update() { };

   /**
    * Obtiene el size recomendado teniendo en cuenta la existencia de overviews.
    */
   virtual void CalcRecommendedSize(int XSize, int YSize, int& XRecomm, int& YRecomm);

   /**
    * Estable el tamanio recomendado.
    */
   virtual void SetRecommendedSize(int XRecomm, int YRecomm);

   /**
    * Indica si fue establecida un tamanio recomendado.
    */
   bool HasRecommendedSize();

   /**
    * Resetea el estado de los datos referidos a las dimensiones recomendadas.
    */
   virtual void ResetRecommendedStatus();

protected:
   RasterSource* pSource_; /*! Fuente de datos (tipo stack/FILO) */
   int xRecomm_;
   int yRecomm_;

private:
   SourceSupervisor *pSupervisor_; /*! Supervisor de la clase */

   /** Clase abstracta no se puede copiar */
   RasterSource(const RasterSource&);

   /** Operador que compara dos RasterSource */
   RasterSource& operator=(const RasterSource&);
};

}  // namespace suri

#endif /*RASTERSOURCE_H_*/
