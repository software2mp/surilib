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

#ifndef GPSTOOL_H_
#define GPSTOOL_H_

// Includes standard
// Includes Suri
#include "suri/Widget.h"
#include "DrawTool.h"

// Includes Wx

// Includes App

// Forwards
class VectorEditor;

// Defines

/** namespace suri */
namespace suri {
// Forwards
class Viewer2D;
class Button;
class VectorEditor;
class DataViewManager;
class DatasourceManagerInterface;

/** Permite editar vectores usando coordenadas */
/**
 * Herramienta que permite cargar puntos usando coordenadas
 * Muestra un boton para puntos, uno para lineas y otro para
 * poligonos.
 * A estos botones les carga estilos gps para diferenciarlos
 */
class GpsTool : public DrawTool {
   /** Ctor. de Copia. */
   GpsTool(const GpsTool &MeassureTool);

public:
   /** Ctor. default. */
   GpsTool(int VectorGroup, VectorEditor* pVectorEditor,
         DataViewManager* pDataViewManager =
         NULL, const suri::SuriObject::UuidType& ViewcontextId = SuriObject::NullUuid,
         wxWindow *pParent = NULL);
   /** Dtor. */
   virtual ~GpsTool();
   /** Creacion de la ventana */
   virtual bool CreateToolWindow();
   /** Retorna la ventana asociada a la herramienta */
   virtual wxWindow *GetToolWindow() const {
      return (const_cast<GpsTool*>(this))->GetWindow();
   }
protected:

private:
   int vectorGroup_; /*! grupo de la lista para los vectores */
   VectorEditor* pVectorEditor_; /*! VectorEditor que se usa para dibujar */
   DataViewManager* pDataViewManager_; /*! Visualizador del contexto */
   DatasourceManagerInterface* pDatasourceManager_; /*! Fuente de datos del contexto */
   suri::SuriObject::UuidType viewcontextId_; ///< Id que identifica cada contexto.
};
}  // namespace suri

#endif /*GPSTOOL_H_*/
