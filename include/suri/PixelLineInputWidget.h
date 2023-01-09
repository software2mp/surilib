/*! \file PixelLineInputWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
