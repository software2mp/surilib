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

#ifndef PIXELLINEINPUTWIDGET_H_
#define PIXELLINEINPUTWIDGET_H_

// Includes suri
#include "suri/NavigationInputWidget.h"
// Includes wx
#include "wx/event.h"

/** namespace suri */
namespace suri {
class World;
class RasterElement;
class DataViewManager;
/** Permite ingresar coordenadas en PixelLinea */
/**
 * Retorna la coordenada ingresada en el sistema de referecenia de la imagen
 * activa en ese momento.
 */
class PixelLineInputWidget : public NavigationInputWidget {
   /** Ctor. Copia */
   PixelLineInputWidget(const PixelLineInputWidget &PixelLineTool) {
   }
   ;
public:
   PixelLineInputWidget();
   /** dtor */
   virtual ~PixelLineInputWidget();

   /** Creacion de la ventana */
   virtual bool CreateToolWindow();
   /** Titulo de la ventana (en caso de pParent!=NULL */
   virtual const wxString GetWindowTitle() const;
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool GetCoordinate(Coordinates &PixelLineCoord);
#ifdef __UNUSED_CODE__
   /** ctor */
   PixelLineInputWidget(World* pWorld);
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual void SetCurrentRasterElement(RasterElement* pRasterElement);
#endif
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool SetCoordinate(Coordinates PixelLineCoord);

   /** */
   DataViewManager* GetDataViewManager() {
      return pDataViewManager_;
   }

   /** */
   void SetDataViewManager(DataViewManager* pDataViewManager) {
      pDataViewManager_ = pDataViewManager;
   }

protected:
   /** Obtiene el icono del navegador */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

private:
   DataViewManager* pDataViewManager_;  // Data view manager.

   /**
    * Transforma las coordenadas recibidas usando el modelo raster del elemento.
    */
   void TransformCoordinates(Coordinates& Coords, bool Inverse = false);
};
}

#endif /* PIXELLINEINPUTWIDGET_H_ */
