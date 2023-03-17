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

#ifndef WORLD_H_
#define WORLD_H_

// Includes standard
#include <string>

// Includes suri
#include "suri/Subset.h"
#include "Model.h"

/** namespace suri */
namespace suri {
/** Forwards */
class RasterSpatialModel;

/** Model Ventana de Mundo, maneja las coordenadas de la vista (la grilla) */
/**
 *  Representa el sistema coordenado que puede ser renderizado, el subset
 * que se desea renderizar y la dimension de la matriz de salida.
 *  Estos parametros son representados por :
 * - World: El mundo, son las coordenadas posibles.
 * - Window: La ventana, es el subset o area que se desea renderizar.
 * - Viewport: El puerto, es la dimension de la matriz raster de salida,
 *   resultado de la renderizacion.
 * World y Window son coordenadas de tipo real expresadas en el sistema
 *  coordenado descripto por la referencia espacial.
 * Viewport representa una matriz con eje en la esquina superior izquierda
 *  con eje positivo hacia abajo y la derecha.
 *  Windowing es la operacion por la cual se mapea el contenido de la ventana
 * al Viewport.
 *  Clipping es la operacion mediante la cual se eliminan los elementos que
 * estan fuera de la ventana y no son mapeados al Viewport.
 * \todo Es necesario tener las coordenadas maximas? es necesario World?
 * \note herada de Model para informar a los views cuando es modificado
 *       (ventana, mundo, viewport o spatial reference)
 */
class World : public Model {
public:
   /** ctor */
   World();
   /** Ctor. de Copia. */
   World(const World &WorldWindow);
   /** dtor */
   virtual ~World();
// --------------------- SET -----------------------
   /** Vuelve el mundo a el estado original */
   virtual void ResetWorld();
   /** Cambia las coordenadas del mundo */
   virtual void SetWorld(double WorldUlx, double WorldUly, double WorldLrx,
                         double WorldLry);
   /** Cambia las coordenadas del mundo */
   virtual void SetWorld(const Subset &NewWorld);
   /** Agrega a las coordenadas del mundo */
   virtual void AppendWorld(double WorldUlx, double WorldUly, double WorldLrx,
                            double WorldLry);
   virtual void AppendWorld(const Subset &MoreWorld);
   /** Cambia las coordenadas de la ventana */
   virtual void SetWindow(double WindowUlx, double WindowUly, double WindowLrx,
                          double WindowLry);
   /** Cambia las coordenadas de la ventana */
   virtual void SetWindow(const Subset &Window);
   /** Cambia las coordenadas del Viewport */
   virtual void SetViewport(int ViewportWidth, int ViewportHeight);
   /** Carga el sistema de referencia del mundo */
   virtual void SetSpatialReference(const std::string &Wkt);
   /** Obtiene el sistema de referencia del mundo */
   virtual std::string GetSpatialReference() const;
// --------------------- GET -----------------------
   /** Retorna las coordenadas de mundo (limites para window) */
   virtual void GetWorld(double &WorldUlx, double &WorldUly, double &WorldLrx,
                         double &WorldLry) const;
   /** Retorna las coordenadas de mundo (limites para window) */
   virtual void GetWorld(Subset &World) const;
   /** Retorna las coordenadas de la ventana en sistema window */
   virtual void GetWindow(double &WindowUlx, double &WindowUly, double &WindowLrx,
                          double &WindowLry) const;
   /** Retorna las coordenadas de la ventana en sistema window */
   virtual void GetWindow(Subset &Window) const;
   /** Retorna el tamanio del Viewport */
   virtual void GetViewport(int &ViewportWidth, int &ViewportHeight) const;
// ------------------- TRANSFORM -------------------
   /** Calcula el RasterSpatialModel correspondiente al mundo */
   virtual void UpdateSpatialModel();
   /** Transforma las coordenadas de Mundo en coordenadas de Viewport */
   virtual void W2VTransform(double WorldX, double WorldY, int &ViewportX,
                             int &ViewportY) const;
   /** Transforma las coordenadas de Viewport en coordenadas de Mundo */
   virtual void V2WTransform(int ViewportX, int ViewportY, double &WorldX,
                             double &WorldY) const;
   /** Transforma las coordenadas de Mundo en coordenadas de Viewport */
   virtual void InverseTransform(const Coordinates &World, Coordinates &Viewport) const;
   /** Transforma las coordenadas de Viewport en coordenadas de Mundo */
   virtual void Transform(const Coordinates &Viewport, Coordinates &World) const;
// ------------------- CONSULTA -------------------
   /** retorna true si el mundo esta inicializado, viewport!=0 y tiene wkt_ no nulo */
   virtual bool IsInitialized() const;
   /**retorna el tamanio de pixel */
   void GetPixelSize(double &x, double &y) const;

private:
   bool initialized_; /*! Flag que indica si las coordenadas fueron */
   /* inicializadas */
   Subset world_; /*! Coordenadas de mundo */
   Subset window_; /*! Coordenadas de ventana */
   int viewportW_; /*! Dimension X del viewport */
   int viewportH_; /*! Dimension Y del viewport */
   std::string wkt_; /*! String con la referencia espacial del mundo */
   RasterSpatialModel *pRasterModel_; /*! Modelo de Transf. de Viewport->World */
};

/** operador de igualdad */
bool operator==(const World &Lhs, const World &Rhs);

/** operadore de desigualdad */
bool operator!=(const World &Lhs, const World &Rhs);
}

#endif /*WORLDWINDOW_H_*/
