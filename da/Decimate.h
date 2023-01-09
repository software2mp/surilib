/*! \file Decimate.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DECIMATE_H_
#define DECIMATE_H_

// Includes standard
#include <map>
#include <string>

// Includes suri
#include "suri/SourceSupervisor.h"

/** namespace suri */
namespace suri {
/** Clase para realizar decimacion a nivel fuente */
/**
 *  Esta clase supervisa a clases derivadas de RasterSource que realizan
 * decimacion. La misma se agrega a una imagen a traves del metodo
 * Image::PushSource(). Este metodo utiliza la interfaz SourceSupervisor para
 * requerirle las fuentes para cada una de las bandas. Desde el codigo cliente
 * se utiliza esta clase derivada para cambiar la dimension de las fuentes
 * creadas al agregarlas a las bandas.
 */
class Decimate : public SourceSupervisor {
   /** Ctor. de Copia. */
   Decimate(const Decimate &Decimate);

public:
   /** ctor */
   Decimate(int SizeX, int SizeY = 0);
   /** dtor */
   virtual ~Decimate();

   /** Debe setear la nueva dimension */
   virtual void SetSize(int SizeX, int SizeY = 0);
   /** Debe setear la dimension X manteniendo la proporcion */
   virtual void SetSizeX(int SizeX);
   /** Debe setear la dimension Y manteniendo la proporcion */
   virtual void SetSizeY(int SizeY);
   /** Permite obtener las dimensiones decimadas a partir de las reales */
   virtual void Real2Resized(double RealX, double RealY, double &ResizedX,
                             double &ResizedY) const;
   /** Permite obtener las dimensiones reales a partir de las decimadas */
   virtual void Resized2Real(double ResizedX, double ResizedY, double &RealX,
                             double &RealY) const;

protected:
   /** Crea las fuentes que supervisa */
   virtual RasterSource *Create();
private:
   int sizeX_; /*! Tamanio de columnas decimado */
   int sizeY_; /*! Tamanio de filas decimado */
};
}

#endif /*DECIMATE_H_*/
