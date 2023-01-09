/*! \file ScreenCanvas.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SCREENCANVAS_H_
#define SCREENCANVAS_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes suri
#include "suri/Canvas.h"
#include "suri/PaintEventInterface.h"

// Defines
/** Permite visualizar una imagen en escala de grises */
#define __SCREENCANVAS_HANDLES_GRAYSCALE__

class wxWindow;
class wxBitmap;
class wxScrollWinEvent;
class wxPaintEvent;
class wxMemoryDC;
class wxEraseEvent;

/** namespace suri */
namespace suri {
/** Clase que le permite al canvas responder a eventos de la ventana */
class PaintHandler;
class PaintEventInterface;

/** canvas cuyo destino es un control en pantalla */
/**
 * Este canvas pinta sobre un control wxWindow los datos que se le escriben.
 * Reporta las dimensiones del control, el tipo de dato siempre como unsigned
 * char y 3 bandas BIP.
 *  En el metodo Write, crea un bitmap con el buffer que se le pasa.
 *  Se conecta al evento OnPaint de la ventana y en el mismo copia el bitmap
 * al DC de la ventana.
 * \note hereda de Canvas para permitir que el pipeline de renderizacion pueda
 *       escribir resultados a por pantalla
 */
class ScreenCanvas : public Canvas, public ForwardPaintEventImpl {
   /** Ctor. de Copia. */
   ScreenCanvas(const ScreenCanvas &ScreenCanvas);

public:
   /** para poder ejecutar metodos protegidos */
   friend class PaintHandler;

   /** ctor */
   /**
    * @param[in] pWindow Ventana sobre la que se pintara la imagen
    */
   ScreenCanvas(wxWindow *pWindow);
   virtual ~ScreenCanvas();
   /** Metodo de clonacion */
   virtual ScreenCanvas *Clone() const;

   /** Limpia el contenido de la pantalla */
   virtual void Clear();
// ----------------------------- RASTER -----------------------------
   /** Tamanio de la pantalla */
   void GetScreenSize(int &SizeX, int &SizeY) const;
   /** Escritura al canvas */
   virtual void Write(std::vector<int> &BandIndex, std::vector<void*> &Data,
                      const Mask *pMask = NULL);
// ----------------------------- VECTORIAL -----------------------------
   /** Canvas vectorial */
   /**
    *  Retorna un canvas asociado a la pantalla. Para permitir la escritura
    * de vectores.
    * @return Canvas asociado a la ventana de visualizacion.
    */
   virtual wxDC *GetDC();
   /** Refresca */
   /**
    *  Manda un Refresh de la ventana.
    */
   virtual void Flush(const Mask *pMask);
// -------------------------- MULTI-RASTER --------------------------
   /** Retorna la cantidad de bandas */
   virtual int GetBandCount() const;
   /** Permite modificar la cantidad de bandas (en algunos casos) */
   virtual void SetBandCount(int BandCount, bool SaveBands = false);
// --------------------------- DATA-TYPE ----------------------------
   /** Nombre del tipo de dato */
   virtual std::string GetDataType() const;
   /** Tamanio del dato en bytes */
   virtual int GetDataSize() const;
   /** Nombre del tipo de dato */
   virtual void SetDataType(const std::string& DataType);
   /** Metodo sobrecardado de PaintEventInterface para pintar controles */
   virtual void OnPaint(wxPaintEvent&, wxDC&);
   /** Metodo sobrecardado de PaintEventInterface para pintar fondo de controles */
   virtual void OnBackgroundErase(wxEraseEvent&);
   /** Metodo para indicar los eventos a los que se desea responder */
   virtual PaintEventInterface::PaintEventFlags GetPaintFlags() const;
protected:
   /** Limpia y reinicializa en bitmap */
   void ClearBitmap();
   /** Metodo conectado a la pintura del bitmap a la ventana */
   void OnPaint(wxPaintEvent &Event);
   /** Metodo conectado al scroll de la ventana */
   void OnScroll(wxScrollWinEvent &Event);
   /** Metodo conectado al scroll de la ventana */
   void OnScrollBar(wxScrollWinEvent &Event);
   /** Retorna buffers internos con los datos */
   virtual void GetInternalData(std::vector<int> &BandIndex,
                                std::vector<void*> &OutputData);
private:
   wxWindow *pScreen_; /*! wxWindow en el que se dibuja */
   wxBitmap *pFlushedBitmap_; /*! encapsula el concepto de un bitmap en */
   /* forma independiente de la plataforma */
   wxBitmap *pPaintedBitmap_; /*! Bitmap que se pinta en el evento OnPaint */
   PaintHandler *pEventHandler_; /*! maneja eventos OnPaint, OnBackgroundErase, */
   /* OnScroll, OnScrollBar de pScreen_ */
   int startX_; /*! Guarda posicion del scroll */
   int startY_; /*! Guarda posicion del scroll */
   int offsetX_; /*! Guarda posicion en que se empezo a hacer scroll */
   int offsetY_; /*! Guarda posicion en que se empezo a hacer scroll */
   bool isScrollingX_; /*! recuerda si se esta haciendo scroll en x */
   bool isScrollingY_; /*! recuerda si se esta haciendo scroll en y */
   wxMemoryDC *pDeviceContext_; /*! Herramienta para dibujar sobre bitmap */
   bool isPainting_; /*! Evita que se inicie OnPaint cuando hay otro en curso */
   int desiredBands_; /*! Permite manejar 1 o 3 bandas */
   std::vector<unsigned char*> internalData_; /*! datos internos del canvas */
};
}

#endif /*SCREENCANVAS_H_*/
