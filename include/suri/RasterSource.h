/*! \file RasterSource.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
