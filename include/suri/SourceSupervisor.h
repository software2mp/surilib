/*! \file SourceSupervisor.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SOURCESUPERVISOR_H_
#define SOURCESUPERVISOR_H_

// Includes suri
#include "RasterSource.h"

/** namespace suri */
namespace suri {

/** Clase que agrega fuentes en una imagen y permite control desde afuera */
/**
 *  Es una clase para supervisar fuentes.
 *
 *  Esta clase debe retornar fuentes del tipo que supervisa y agregarlas a la
 * lista, las clases RasterSource deben desregistrarse (DelSource()) cuando
 * son destruidas.
 *
 *  Las clases derivadas presentan la interfaz de control y transmiten a las
 * instancias de RasterSource que supervisan los cambios. Esto permite
 * controlar clases metidas en las fuentes de una banda desde afuera de la
 * banda e incluso la imagen. La lista se usa para no cometer el error de
 * referenciar un puntero perdido.
 *
 *  Las clases manejadas son eliminadas desde la imagen.
 */
class SourceSupervisor {
public:
   /** ctor */
   SourceSupervisor() {
   }
   /** dtor */
   virtual ~SourceSupervisor();
   /** crea una nueva instancia de la fuente manejada */
   RasterSource *CreateSource();
   /** Elimina una fuente */
   void DelSource(RasterSource *pSource);

protected:
   /** Agrega una fuente a la lista */
   void AddSource(RasterSource *pNewSource);
   /** crea una nueva instancia de la fuente manejada */
   /**
    *  Metodo que crea la clase manejada. Se implementa en las clases derivadas
    *  ya que cambia segun el tipo. (esto puede ser template entonces?)
    *  \TODO(Javier - Sin TCK): Analizar la posibilidad de que sea template
    */
   virtual RasterSource *Create()=0;
   /** tipo conjunto de fuentes */
   typedef std::set<RasterSource*> SourceSetType;
   SourceSetType childSources_; /*! conjunto de fuentes manejadas */

private:
   /** Ctor. de Copia. */
   SourceSupervisor(const SourceSupervisor &SourceSupervisor);
};

}

#endif /*SOURCESUPERVISOR_H_*/
