/*! \file ScreenCanvas.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "ScreenCanvas.h"

// Includes standard
#include <cmath>
#include <algorithm>

// Includes suri
#include "suri/messages.h"
#include "suri/DataTypes.h"
#include "Mask.h"
#include "logmacros.h"
#include "wxmacros.h"

// Includes wx
#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include "wx/gdicmn.h"

// defines
/** Cantidad de bandas utilizadas por ScreenCanvas */
#ifdef __SCREENCANVAS_HANDLES_GRAYSCALE__
#  define SCREENCANVAS_BAND_COUNT 3
#else
#  define SCREENCANVAS_BAND_COUNT 0
#endif

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(PaintHandler, ScreenCanvas)IMPLEMENT_EVENT_CALLBACK(OnScroll, OnScroll(event), wxScrollWinEvent)
   IMPLEMENT_EVENT_CALLBACK(OnScrollBar, OnScrollBar(event), wxScrollWinEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/* Ctor. de Copia.
 * @param[in] pWindow ventana sobre la que se dibuja.
 * @return instancia de la clase ScreenCanvas
 */
ScreenCanvas::ScreenCanvas(wxWindow *pWindow) :
      pScreen_(pWindow), pFlushedBitmap_(NULL), pPaintedBitmap_(NULL),
      NEW_EVENT_OBJECT(PaintHandler),
      startX_(0),
      startY_(0),
      offsetX_(0),
      offsetY_(0),
      isScrollingX_(false),
      isScrollingY_(false),
      pDeviceContext_(NULL),
      isPainting_(false),
      desiredBands_(SCREENCANVAS_BAND_COUNT) {
   pWindow->Connect(wxEVT_SCROLLWIN_THUMBTRACK,
                    wxScrollWinEventHandler(PaintHandler::OnScroll), NULL,
                    pEventHandler_);
   pWindow->Connect(wxEVT_SCROLLWIN_LINEUP,
                    wxScrollWinEventHandler(PaintHandler::OnScrollBar), NULL,
                    pEventHandler_);
   pWindow->Connect(wxEVT_SCROLLWIN_LINEDOWN,
                    wxScrollWinEventHandler(PaintHandler::OnScrollBar), NULL,
                    pEventHandler_);
   // Inicializa el tamano
   int x, y;
   GetScreenSize(x, y);
   SetSize(x, y);
}

/** Destructor. */
ScreenCanvas::~ScreenCanvas() {
   DELETE_EVENT_OBJECT;
   if (pFlushedBitmap_ != pPaintedBitmap_) {
      delete pPaintedBitmap_;
   }
   delete pFlushedBitmap_;
   pPaintedBitmap_ = NULL;
   pFlushedBitmap_ = NULL;
   for (size_t i = 0; i < internalData_.size(); i++)
      delete[] internalData_[i];
}


/** Metodo de clonacion */
ScreenCanvas *ScreenCanvas::Clone() const {
   return NULL;
}

/** Limpia el contenido de la pantalla */
void ScreenCanvas::Clear() {
   if (pFlushedBitmap_ != pPaintedBitmap_) {
      delete pPaintedBitmap_;
   }
   pPaintedBitmap_ = new wxBitmap;
   if (pFlushedBitmap_ && pFlushedBitmap_->IsOk()) {
      *pPaintedBitmap_ = *pFlushedBitmap_;
   }
   ClearBitmap();
   int x = 0, y = 0;
   GetSize(x, y);
   *pFlushedBitmap_ = wxBitmap(wxImage(x, y, true));
   REPORT_DEBUG("D:Clear");
}

// ----------------------------- RASTER -----------------------------
/* Devuelve el Tamanio de la pantalla
 * @param[out] SizeX ancho de la pantalla
 * @param[out] SizeY alto de la pantalla
 */
void ScreenCanvas::GetScreenSize(int &SizeX, int &SizeY) const {
   SizeX = -1;
   SizeY = -1;
   if (pScreen_) {
      pScreen_->GetClientSize(&SizeX, &SizeY);
   } else {
      REPORT_AND_RETURN("D:Control de pantalla inexistente");
   }
}

/** Escritura al canvas */
/**
 * Escribe los datos de Data, en las bandas indicadas por BandIndex en el
 * propio canvas.()
 * Guardo los datos de Data en buffer con estructura r1g1b1r2g2b2r3...... y lo
 * guardo en wxImage. Con esto genero un wxBitmap y lo guardo en wxMemoryDC.
 * Finalmente guarda los datos en pFlushedBitmap_.
 * @param[in] BandIndex indice de bandas donde se escriben los datos
 * @param[in] Data vector de datos que deben escribirse en las bandas locales
 *                 indicadas por BandIndex.
 * @param[in] pMask macara(no se usa)
 */
void ScreenCanvas::Write(std::vector<int> &BandIndex, std::vector<void*> &Data,
                         const Mask *pMask) {
   if (Data.size() == 0) {
      REPORT_AND_RETURN("D:Cantidad de bandas nula.");
   }
   if (*std::max_element(BandIndex.begin(), BandIndex.end()) > GetBandCount()) {
      REPORT_AND_RETURN("D:Los indices solicitados exceden a los disponibles.");
   }
   if (Data.size() != static_cast<size_t>(desiredBands_)) {
      REPORT_AND_RETURN("D:Se utilizan 1 o 3 bandas.");
   }
   int x, y;
   GetSize(x, y);
   if (x == 0 || y == 0) {
      return;
   }
   ClearBitmap();
   // no se elimina prgbimage ya que de eso se encarga wxImage
   wxImage img =
         pFlushedBitmap_->IsOk() ? pFlushedBitmap_->ConvertToImage() :
                                   wxImage(x, y, true);
   unsigned char *prgbimage = (unsigned char*) malloc(x * y * 3);
   std::vector<unsigned char> maskdata(x * y);
   memset(&maskdata[0], Mask::Opaque, x * y);
   if (pMask) {
      std::vector<int> bands(1, 0);
      std::vector<void*> maskWrap(1, &maskdata[0]);
      pMask->Read(bands, maskWrap);
   }

   // inicializo con los datos de la imagen
   if (img.IsOk()) {
      memcpy(prgbimage, img.GetData(), x * y * 3);
   } else {
      memset(prgbimage, 0, x * y * 3);
   }
   for (size_t b = 0; b < BandIndex.size(); b++) {
      unsigned char *pdata = static_cast<unsigned char*>(Data.at(b));
      for (int i = 0; i < x * y; i++) {
         if (!pMask || maskdata[i] == Mask::Opaque) {
            prgbimage[i * 3 + BandIndex[b]] = pdata[i];
#ifdef __SCREENCANVAS_HANDLES_GRAYSCALE__
            // Si tengo una sola banda, la copio a las otras.
            if (desiredBands_ == 1) {
               prgbimage[i * 3 + 1] = pdata[i];
               prgbimage[i * 3 + 2] = pdata[i];
            }
#endif
         }
      }
   }
   // creo la imagen a partir de los datos RGB
   wxImage image(x, y, prgbimage);
   prgbimage = NULL;
   // creo el bitmap a partir de la imagen
   wxBitmap tembitmap(image);
   wxMemoryDC *psource = new wxMemoryDC;
   psource->SelectObjectAsSource(tembitmap);
   wxMemoryDC *ptarget = new wxMemoryDC;

   ptarget->SelectObject(*pFlushedBitmap_);
   ptarget->Blit(0, 0, x, y, psource, 0, 0);
   psource->SelectObjectAsSource(wxNullBitmap);
   ptarget->SelectObject(wxNullBitmap);
   // se esta mezclando malloc con delete
   delete ptarget;
   delete psource;
}

// ----------------------------- VECTORIAL -----------------------------
/** Canvas vectorial */
/**
 *  Retorna un canvas wxDC que permite realizar operaciones vectoriales, ej.
 * DrawLine, etc.
 * @return puntero a wxDC en caso de exito.
 * @return NULL en caso de error.
 */
wxDC *ScreenCanvas::GetDC() {
   if (pDeviceContext_) {
      return pDeviceContext_;
   }
   // si no existe o es invalido lo creo
   ClearBitmap();
   // dc de entrada
   pDeviceContext_ = new wxMemoryDC;
   pDeviceContext_->SelectObject(*pFlushedBitmap_);
   // verifico que este todo ok
   if (!pDeviceContext_->IsOk()) {
      REPORT_AND_RETURN_VALUE("D:No se pudo crear el wxMemoryDC.", NULL);
   }
   pDeviceContext_->SetBrush(*wxTRANSPARENT_BRUSH);
   return pDeviceContext_;
}

/** Refresca */
/**
 *  Manda un Refresh de la ventana.
 */
void ScreenCanvas::Flush(const Mask *pMask) {
   REPORT_DEBUG("D:Flush");
   if (pDeviceContext_) {
      pDeviceContext_->SelectObject(wxNullBitmap);
      delete pDeviceContext_;
      pDeviceContext_ = NULL;
   }
   delete pPaintedBitmap_;
   pPaintedBitmap_ = pFlushedBitmap_;

   // flag que indica que hubo refresh del bitmap (que se termino de scrollear)
   isScrollingX_ = false;
   isScrollingY_ = false;
   // offsets para pintar en caso de scroll
   startX_ = pScreen_->GetScrollPos(wxHORIZONTAL);
   startY_ = pScreen_->GetScrollPos(wxVERTICAL);
   offsetX_ = pScreen_->GetScrollPos(wxHORIZONTAL);
   offsetY_ = pScreen_->GetScrollPos(wxVERTICAL);

   pScreen_->Refresh();
   REPORT_DEBUG("D:post-Refresh");
}

// -------------------------- MULTI-RASTER --------------------------
/* Retorna la cantidad de bandas
 * @return cantidad de bandas
 */
int ScreenCanvas::GetBandCount() const {
   return desiredBands_;
}

/* Permite modificar la cantidad de bandas (en algunos casos)
 * @param[in] BandCount Cantidad de bandas. Puede tener valores 1 o 3.
 */
void ScreenCanvas::SetBandCount(int BandCount, bool SaveBands) {
#ifdef __SCREENCANVAS_HANDLES_GRAYSCALE__
   if ((BandCount == 3 || BandCount == 1) && BandCount != desiredBands_) {
      desiredBands_ = BandCount;
      Clear();
   }
#else
   // siempre 3 = RGB
   REPORT_AND_FAIL("D:La pantalla utiliza 3 bandas (RGB), no %d", BandCount);
#endif
}

// --------------------------- DATA-TYPE ----------------------------
/**
 * Nombre del tipo de dato
 * @return el nombre del tipo de dato.
 */
std::string ScreenCanvas::GetDataType() const {
   return DataInfo<unsigned char>::Name;
}

/**
 * Tamanio del dato en bytes
 * @return el tamano del dato en bytes
 */
int ScreenCanvas::GetDataSize() const {
   return DataInfo<unsigned char>::Size;
}

/** Nombre del tipo de dato */
void ScreenCanvas::SetDataType(const std::string& DataType) {
   if (GetDataType() != DataType) {
      REPORT_AND_FAIL("D:La pantalla utiliza tipo de dato uchar, no %s",
                      DataType.c_str());
   }
}

/** Limpia y reinicializa en bitmap */
void ScreenCanvas::ClearBitmap() {
   int x, y;
   GetSize(x, y);
   if (!pFlushedBitmap_ || !pFlushedBitmap_->IsOk() || pFlushedBitmap_->GetWidth() != x
         || pFlushedBitmap_->GetHeight() != y) {
      REPORT_DEBUG("D:Eliminando bitmap");
      delete pFlushedBitmap_;
      if (x > 0 && y > 0) {
         pFlushedBitmap_ = new wxBitmap(wxImage(x, y, true));
      } else {
         pFlushedBitmap_ = new wxBitmap;
      }
   }
}

/** Metodo conectado a la pintura del bitmap a la ventana */
/**
 *  Evento OnPain asociado a la ventana (wxWindow) que representa este canvas.
 *
 *  Delega en OnBackgroundErase.
 *
 * @param[in] Event evento
 * @param[in] Dc es el Device Context para pintar.
 */
void ScreenCanvas::OnPaint(wxPaintEvent &Event, wxDC &Dc) {
   wxEraseEvent eraseevt(Event.GetId(), &Dc);
   OnBackgroundErase(eraseevt);
}
/*!
 * Retorna el flag para que se pinte el fondo.
 * @return PaintEventInterface::Backgorund
 */
PaintEventInterface::PaintEventFlags ScreenCanvas::GetPaintFlags() const {
   return PaintEventInterface::Background;
}

/**
 *  Evento OnBackgroundErase asociado a la ventana (wxWindow) que representa este canvas.
 *
 *  Este metodo toma el bitmap generado en Write (destino de la renderizacion)
 * y lo pinta en la ventana de destino. Realiza una verificacion de con un cast
 * para determinar si la ventana destino es un wxScrolledWindow y de ser asi
 * determina si debe refrescar el bitmap con un offset o simplemente llenar
 * la totalidad de la ventana. Esto se debe a que al mover las barras de
 * desplazamiento, se generan eventos OnPaint/OnBackgroundErase
 * @param[in] Event evento
 */
void ScreenCanvas::OnBackgroundErase(wxEraseEvent &Event) {
   Event.Skip(false);
   if (isPainting_ || !pPaintedBitmap_ || !pPaintedBitmap_->IsOk()) {
      return;
   }
   isPainting_ = true;
   REPORT_DEBUG("D: Pintando");
   // Obtiene las dimensiones del PaintedBitmap_
   int x = pPaintedBitmap_->GetWidth();
   int y = pPaintedBitmap_->GetHeight();

   // dc de entrada
   wxMemoryDC memdc;

   int sx, sy;
   GetScreenSize(sx, sy);

   memdc.SelectObjectAsSource(*pPaintedBitmap_);

   // Se reemplaza por wxPaintDC porque en MSW pinta fondo de negro
   // wxAutoBufferedPaintDC dc(pScreen_);

   // de de salida (pantalla, wxPaintDC porque es un evento OnPaint)
   wxDC *pdc = Event.GetDC();

   // verifico que este todo ok
   if (!memdc.IsOk()) {
      REPORT_AND_RETURN("D:No se pudo crear el wxMemoryDC.");
   }
   if (!pdc->IsOk()) {
      REPORT_AND_RETURN("D:Error en wxPaintDC proveniente de Event.GetDC().");
   }

   // copia el bitmap a la pantalla

   // posicion de origen, lo que hice de scroll
   int xdest = startX_ - offsetX_, ydest = startY_ - offsetY_;

   // ancho del bitmap - lo que hice de scroll
   int deltax = (int) (x - std::abs((double) (xdest))), deltay = (int) (y
         - std::abs((double) (ydest)));

   // Setea el background brush a negro
   pdc->SetBackground(*wxTheBrushList->FindOrCreateBrush(*wxBLACK));

   // Obtiene la region que no tiene imagen
   wxRegion region(0, 0, sx, sy);
   region.Subtract(
         wxRect(xdest > 0 ? 0 : -xdest, ydest > 0 ? 0 : -ydest, deltax, deltay));

   // La setea como clipping region (region valida para dibujar)
   pdc->SetClippingRegion(region);

   // Limpia la pantalla
   pdc->Clear();

   // Libera la clipping region para que sea toda la pantalla
   pdc->DestroyClippingRegion();

   // Dibuja el MemDC en el PaintDC
   pdc->Blit(xdest > 0 ? 0 : -xdest, ydest > 0 ? 0 : -ydest, deltax, deltay, &memdc,
             xdest > 0 ? x - deltax : 0, ydest > 0 ? y - deltay : 0, wxCOPY, true);

   // limpia el dc de entrada
   memdc.SelectObjectAsSource(wxNullBitmap);
   isPainting_ = false;
   REPORT_DEBUG("D:Fin pintando");
}

/* Metodo conectado al scroll de la ventana
 * @param[in] Event evento
 */
void ScreenCanvas::OnScroll(wxScrollWinEvent &Event) {
   if (!isPainting_) {
      int pos = Event.GetPosition();

      switch (Event.GetOrientation()) {
         case wxHORIZONTAL: {
            if (!isScrollingX_) {
               isScrollingX_ = true;
               offsetX_ = Event.GetPosition();   //pScreen_->GetScrollPos(wxHORIZONTAL);
            }

            startX_ = pos;
            break;
         }
         case wxVERTICAL: {
            if (!isScrollingY_) {
               isScrollingY_ = true;
               offsetY_ = Event.GetPosition();     //pScreen_->GetScrollPos(wxVERTICAL);
            }

            startY_ = pos;
            break;
         }
         default:
            break;
      }REPORT_DEBUG("D:Comienzo %d;%d", startX_, startY_);
      REPORT_DEBUG("D:Offset   %d;%d", offsetX_, offsetY_);
   }
   pScreen_->Refresh();
}

/* Metodo conectado al scroll de la ventana
 * @param[in] Event evento
 */
void ScreenCanvas::OnScrollBar(wxScrollWinEvent &Event) {
   wxScrollWinEvent event(Event.GetEventType(),
                          pScreen_->GetScrollPos(Event.GetOrientation()),
                          Event.GetOrientation());
   OnScroll(event);
   Event.Skip();
}

/** Retorna buffers internos con los datos */
/**
 * Llena OutputData con los punteros a los buffers internos correspondientes
 * a los indices indicados en BandIndex del Canvas. En caso de no tener los
 * datos representados en forma de punteros, debera realizar una copia de
 * datos.
 * \attention : Los datos apuntados por los punteros pueden cambiar, por lo
 *              que deben ser copiados o utilizados lo antes posible.
 * @param[in] BandIndex Indices de los datos solicitados
 * @param[out] OutputData Array con punteros a los datos internos
 */
void ScreenCanvas::GetInternalData(std::vector<int> &BandIndex,
                                   std::vector<void*> &OutputData) {
   int bands = 3;
#ifdef __SCREENCANVAS_HANDLES_GRAYSCALE__
   bands = desiredBands_;
#endif
   int x, y;
   GetSize(x, y);
   // este bucle no deberia extenderse hasta mas alla del primer if
   for (size_t b = 0; b < BandIndex.size(); b++) {
      if (BandIndex[b] >= GetBandCount()) {
         REPORT_AND_RETURN("D:MemoryCanvas, se excede el numero de bandas.");
      }
   }
   // limpio los datos internos
   for (size_t i = 0; i < internalData_.size(); i++)
      delete[] internalData_[i];
   internalData_.clear();
   // limpio el vector de salida
   OutputData.clear();
   // reservo espacio
   for (int i = 0; i < bands; i++) {
      internalData_.push_back(new unsigned char[x * y]);
      OutputData.push_back(internalData_.back());
   }

   wxImage img = pFlushedBitmap_->ConvertToImage();

   unsigned char *pimgdata = img.GetData();
   for (size_t b = 0; b < BandIndex.size(); b++)
      for (int i = 0; i < x * y; i++)
         internalData_[b][i] = pimgdata[3 * i + BandIndex[b]];
}
}
