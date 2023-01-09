/*! \file KMeansCanvas.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef KMEANSCANVAS_H_
#define KMEANSCANVAS_H_

// Includes standard
// Includes Suri
#include "MemoryCanvas.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Clase que calcula las nuevas medias usando el algoritmo de KMeans. */
/**
 * Se cargan las medias originales que se quieren mejorar.
 * Cuando se renderiza una imagen usando este canvas se calculan las nuevas
 * medias a partir de los pixeles.
 */
class KMeansCanvas : public MemoryCanvas {
public:
   /** ctor */
   KMeansCanvas();
   /** dtor */
   virtual ~KMeansCanvas();

   /** Canvas vectorial */
   virtual wxDC *GetDC();
   /** Computa la nueva media usando los pixeles en canvas */
   virtual void Flush(const Mask *pMask);
   /** Retorna la media calculada durante la renderizacion */
   virtual void GetNewMeans(std::vector<std::vector<double> > &NewMeans);
   /** Retorna la media calculada durante la renderizacion */
   virtual void SetInitialMeans(const std::vector<std::vector<double> > &InitialMeans);
   /** Limpia los dato internos */
   virtual void Reset();
protected:
   std::vector<std::vector<double> > initialMeans_; /*! Medias para determinar */
   /* a que clase pertenece */
   /* cada pixel */
   std::vector<std::vector<double> > newMeans_; /*! Suma de todos los pixeles */
   /* en cada clase(luego de */
   /* renderizar) */
   std::vector<double> pixelCount_; /*! Cantidad de pixeles en cada clase */
   /* (luego de renderizar) */
};
}

#endif /* KMEANSCANVAS_H_ */
