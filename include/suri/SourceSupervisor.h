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
