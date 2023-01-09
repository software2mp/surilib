/*! \file Canvas.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes estandar
#include <string>
#include <algorithm>
#include <vector>

// Includes suri
#include "suri/Canvas.h"
#include "Mask.h"
#include "logmacros.h"
#include "suri/messages.h"
#include "suri/Image.h"

/** namespace suri */
namespace suri {
/** ctor */
Canvas::Canvas() :
      sizeX_(0), sizeY_(0) {
}

/** dtor */
Canvas::~Canvas() {
}

/**
 *  Inicializa el canvas para que sea compatible con el pasado sin modificar
 * los datos si ya son compatibles.
 *
 * @param pCanvas Canvas a tomar los parametros.
 * @return Valida que se haya podido hacer la inicializacion (canvas compatibles)
 */
bool Canvas::InitializeAs(const Canvas *pCanvas) {
   if (pCanvas && (*this != *pCanvas)) {
      if (GetDataType() != pCanvas->GetDataType())
                  SetDataType(pCanvas->GetDataType());
      int x, y;
      pCanvas->GetSize(x, y);
      if (x != GetSizeX() || y != GetSizeY())
            SetSize(x, y);
      if  (this->GetBandCount() != pCanvas->GetBandCount())
                     SetBandCount(pCanvas->GetBandCount());
      SetNoDataValue(pCanvas->GetNoDataValue());
      SetNoDataValueAvailable(pCanvas->IsNoDataValueAvailable());
      SetAllBandsNdv(pCanvas->GetAllBandsNdv());
      Clear();
   }
   return (pCanvas && (*this != *pCanvas));
}

/** Tamanio del raster X e Y */
/**
 *  Obtiene el tamanio del canvas
 * @param[out] SizeX Dimension en X del canvas
 * @param[out] SizeY Dimension en Y del canvas
 */
void Canvas::GetSize(int &SizeX, int &SizeY) const {
   if (sizeX_ < 0 || sizeY_ < 0) {
      REPORT_AND_RETURN("D:Canvas con dimensiones no seteadas.");
   }
   SizeX = sizeX_;
   SizeY = sizeY_;
}

/**
 * Tamanio X del raster
 * @return ancho del raster
 */
int Canvas::GetSizeX() const {
   int temp = 0, dummy = 0;
   GetSize(temp, dummy);
   return temp;
}

/**
 * Tamanio Y del raster
 * @return alto del raster
 */
int Canvas::GetSizeY() const {
   int temp = 0, dummy = 0;
   GetSize(dummy, temp);
   return temp;
}

/** Tamanio */
/**
 *  Setea las dimensiones del canvas.
 *  Este metodo es mas optimo que llamar a SetSizeX SetSizeY ya que estos dos
 * relizan cada uno una llamada a este.
 * @param[in] SizeX Dimension en X del canvas
 * @param[in] SizeY Dimension en Y del canvas
 */
void Canvas::SetSize(const int SizeX, const int SizeY) {
   if (SizeX < 1 || SizeY < 1) {
      REPORT_AND_RETURN("D:Canvas con dimensiones invalidas. (%d;%d).", SizeX, SizeY);
   }

   sizeX_ = SizeX;
   sizeY_ = SizeY;
}

/**
 * Asigna el tamanio X del raster
 * @param[in] Size ancho del raster
 */
void Canvas::SetSizeX(const int Size) {
   SetSize(Size, sizeY_);
}

/**
 * Asigna el tamanio Y del raster
 * @param[in] Size alto del raster
 */
void Canvas::SetSizeY(const int Size) {
   SetSize(sizeX_, Size);
}

/** Lectura del canvas */
/**
 * Lee los datos propios y los almacena en BandIndex y Data.
 * \pre Los indices indicados por BandIndex deben ser menores a la
 *      cantidad de datos disponible.
 * \post En caso de que BandIndex venga vacio, se lee lo mejor que se
 *      puede, es decir, el minimo entre la cantidad de bandas disponibles
 *      y la cantidad de punteros donde almacenar.
 * @param[in] BandIndex vector de indices de los datos leidos.
 * @param[out] BandIndex vector de indices de los datos leidos.
 * @param[out] OutputData vector de datos.
 */
void Canvas::Read(std::vector<int> &BandIndex, std::vector<void*> &OutputData) const {
   if (OutputData.empty()) {
      REPORT_AND_RETURN("D: Output data esta vacio");
   }
   // Verifica que lo pedido no se exceda de lo disponible
   for (size_t i = 0; i < BandIndex.size(); i++)
      if (BandIndex.at(i) >= GetBandCount()) {
         REPORT_AND_RETURN("D: Indices de bandas incorrectos");
      }
   // Si Bandindex esta vacio, lee las bandas que puede
   if (BandIndex.empty()) {
      // la menor cantidad disponible es lo mejor que puede leer
      size_t maxavailablebands = std::min(OutputData.size(),
                                          static_cast<size_t>(GetBandCount()));
      for (size_t i = 0; i < maxavailablebands; i++)
         BandIndex.push_back(i);
   }
   std::vector<void*> banddata(BandIndex.size());
   const_cast<Canvas*>(this)->GetInternalData(BandIndex, banddata);
   int x = 0, y = 0;
   GetSize(x, y);
   int size = GetDataSize() * x * y;
   for (size_t i = 0; i < BandIndex.size(); i++){
      void* src = banddata[i];
      memcpy(OutputData[i], src, size);
   }
}

/** Lectura hacia otro canvas (in-place) */
/**
 *  Carga en pTarget los datos locales sin buffers intermedios.
 * @param[out] pTarget canvas donde se escriben los datos.
 * @param[out] pMask Mascara de los datos leidos
 */
void Canvas::Read(Canvas *pTarget, const Mask *pMask) const {
   if (!pTarget || pTarget == this) {
      return;
   }
   if (*pTarget != *this) {
      REPORT_AND_RETURN("D: Los canvas no son compatibles.");
   }

   std::vector<int> bandindex(GetBandCount());
   std::vector<void*> data(GetBandCount());
   for (size_t i = 0; i < bandindex.size(); i++)
      bandindex[i] = i;
   const_cast<Canvas*>(this)->GetInternalData(bandindex, data);
   // Me aseguro que la mascara es compatible con el canvas
   const Mask *pmask = NULL;
   if (pMask) {
      int x, y;
      pMask->GetSize(x, y);
      if (pTarget->GetSizeX() == x && pTarget->GetSizeY() == y) {
         pmask = pMask;
      }
   }
      pTarget->Write(bandindex, data, pmask);
}

/** Escritura desde otro canvas (in-place) */
/**
 *  Escribe los datos del canvas pSource en el propio sin utilizar buffers
 * temporales.
 * @param[in] pSource Canvas de donde provienen los datos que se escriben en
 *                    el propio.
 * @param[in] pMask Mascara de los datos
 */
void Canvas::Write(const Canvas *pSource, const Mask *pMask) {
   if (pSource) {
      pSource->Read(this, pMask);
   } else {
      REPORT_AND_FAIL("D:Canvas destino nulo");
   }
}

// ----------------------------- VECTORIAL -----------------------------
/** Canvas vectorial */
/**
 *  Retorna un canvas wxDC que permite realizar operaciones vectoriales, ej.
 * DrawLine, etc.
 *  Al tratarse de un canvas visual, solo escribira en un maximo de 3 bandas
 * asumiendo 012 == RGB.
 *  El uso que se le da en esta clase es para escribir en un wxMemoryDC y
 * de ahi pasarlo al buffer interno correspondiente (usando
 * Canvas::FlushDC()). En clases derivadas puede implementarse de otras
 * maneras (ej. ScreenCanvas, escribe directamente a la pantalla).
 * \pre Solo soporta escritura sobre las tres primeras bandas.
 * \post Los datos deben ser pasados al buffer principal usand FlushDC().
 * @return Canvas sobre el cual escribir datos vectoriales.
 * \todo Implementar GetDC()
 */
wxDC *Canvas::GetDC() {
   return NULL;
}

/** Pasa al buffer interno la renderizacion sobre wxDC */
/**
 *  Permite pasar del DC al buffer interno.
 */
void Canvas::Flush(const Mask *pMask) {
}


// --------------------------- DEBUG ----------------------------
/**
 * Guarda el contenido del Canvas en un archivo GeoTiff en disco.
 * @param Filename Archivo al que se salva el Canvas.
 */
void Canvas::SaveCanvas(const std::string Filename) const {
   int x, y;
   GetSize(x, y);
   Image *pimage = Image::Open(Filename, Image::WriteOnly, "GTiff",
                           GetBandCount(), x, y, GetDataType());
   if (pimage) {
// (\todo Javier #2232): Al implementar Clone() se puede reemplazar este
// codigo por uno mas compacto y correcto.
      std::vector<int> bands;
      for (int b = 0; b < GetBandCount(); ++b)
         bands.push_back(b);
      std::vector<void*> data;
      const_cast<Canvas*>(this)->GetInternalData(bands, data);

      Canvas *ptemp = Clone();
      if (ptemp) {
         ptemp->Flush(NULL);

         ptemp->GetInternalData(bands, data);
         pimage->Write(bands, data, 0, 0, x, y);
         delete ptemp;
      }

      Image::Close(pimage);
   }
}


/** operador de igualdad para canvas */
/**
 * Compara tamanos, numero de bandas y tipo de datos.
 * @param[in] Lhs Canvas a la izquierda del operador ==
 * @param[in] Rhs Canvas a la derecha del operador ==
 * @return true si los canvas son iguales, false en otro caso.
 */
bool operator==(const Canvas &Lhs, const Canvas &Rhs) {
   return !(Lhs != Rhs);
}

/** operador de inequidad para canvas */
/**
 * Compara tamanos, numero de bandas y tipo de datos.
 * @param[in] Lhs Canvas a la izquierda del operador !=
 * @param[in] Rhs Canvas a la derecha del operador !=
 * @return true si los canvas son distintos, false en otro caso.
 */
bool operator!=(const Canvas &Lhs, const Canvas &Rhs) {
   int lhs_x, lhs_y;
   Lhs.GetSize(lhs_x, lhs_y);
   int rhs_x, rhs_y;
   Rhs.GetSize(rhs_x, rhs_y);
   int lhs_count = Lhs.GetBandCount();
   int rhs_count = Rhs.GetBandCount();
   return lhs_x != rhs_x || lhs_y != rhs_y || lhs_count != rhs_count
         || Lhs.GetDataType() != Rhs.GetDataType();
}







}
