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
