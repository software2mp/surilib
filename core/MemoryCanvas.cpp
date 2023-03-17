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

// Includes estandar
#include <string>
#include <vector>

// Includes suri
#include "MemoryCanvas.h"
#include "suri/messages.h"
#include "suri/DataTypes.h"

// Includes wx
#include "wx/dcmemory.h"
#include "wx/image.h"

/**
 * Esta diferencia tiene que ver con la implementacion de wxBitmap
 * en Linux. Si la profundidad es de 32 se crea un buffer en memoria
 * utilizando gdk_pixbuf_new (GIMP Drawing Kit). Si la profundidad es
 * distinta trata de crear un buffer tambien en memoria pero haciendo
 * uso de componenetes que necesitan de un window (Gtk) por lo que hay
 * que correr la libreria en un entorno de X server.
 * 
 * \ver wxWidgets-2.8.12/src/gtk/bitmap.cpp (wxBitmap::Create)
 * \ver wxWidgets-2.8.12/src/gtk/app.cpp (wxApp::GetGdkVisual)
 */
#ifdef __LINUX__
#define BITMAP_DEPTH    32
#else
#define BITMAP_DEPTH    24
#endif  // __LINUX__

/** namespace suri */
namespace suri {
/**
 * Constructor
 */
MemoryCanvas::MemoryCanvas() :
      pDcBitmap_(new wxBitmap), pDc_(new wxMemoryDC), bandCount_(0) {
   dataType_ = DataInfo<void>::Name;
   pDc_->SelectObject(*pDcBitmap_);
}

/**
 * Destructor
 */
MemoryCanvas::~MemoryCanvas() {
   DeleteBands();
   pDc_->SelectObject(wxNullBitmap);
   delete pDcBitmap_;
   delete pDc_;
}

/** Metodo de clonacion */
MemoryCanvas *MemoryCanvas::Clone() const {
   MemoryCanvas *ptemp = new MemoryCanvas;
   // inicializo con los mismos parametros
   ptemp->InitializeAs(this);
   // limpio
   ptemp->Clear();
   // copio los datos raster
   dynamic_cast<Canvas*>(ptemp)->Write(this, NULL);
   // copio los datos vectoriales

   int x, y;
   GetSize(x, y);
   ptemp->pDc_->Blit(wxPoint(0, 0), wxSize(x, y), pDc_, wxPoint(0, 0));
   return ptemp;
}

// ----------------------------- RASTER -----------------------------
/**
 * Setea las dimensiones del canvas.
 * Este metodo es mas optimo que llamar a SetSizeX SetSizeY ya que estos dos
 * relizan cada uno una llamada a este.
 * @param[in] SizeX Dimension en X del canvas
 * @param[in] SizeY Dimension en Y del canvas
 */
void MemoryCanvas::SetSize(const int SizeX, const int SizeY) {
   Canvas::SetSize(SizeX, SizeY);
   SetBandCount(GetBandCount());
   for (size_t i = 0; i < bandVector_.size(); i++)
      bandVector_[i]->SetSize(SizeX, SizeY);
}

/** Lectura del canvas */
/**
 * @param[in] BandIndex vector con nro de bandas a leer
 * @param[out] Data tendra la banda indicada por BandIndex en cada posicion
 */
void MemoryCanvas::Read(std::vector<int> &BandIndex, std::vector<void*> &Data) const {
   if (bandVector_.empty()) {
      REPORT_AND_RETURN("D:MemoryCanvas, no se encuentra inicializado.");
   }
   int x, y;
   GetSize(x, y);
   for (size_t i = 0; i < BandIndex.size(); i++) {
      if (BandIndex[i] >= GetBandCount()) {
         REPORT_AND_RETURN("D:MemoryCanvas[%d], se excede el numero de bandas disponibles [%d].",
                           BandIndex[i], GetBandCount());
      }
      // Escribe en la banda deseada el buffer asociado
      bandVector_.at(BandIndex[i])->Read(Data.at(i), 0, 0, x, y);
   }
}

// Pasa a la clase Canvas + el metodo GetInternalData
#ifdef __UNUSED_CODE__
/** Lectura hacia otro canvas (in-place) */
void MemoryCanvas::Read(Canvas *pTarget, Mask *pMask) const
{
   int count = GetBandCount();
   std::vector<int> idx(count);
   std::vector<void*> data(count);
   // cargo los datos de las bandas.
   for ( int i=0; i<count; i++ )
   {
      // solo hay bloque 0,0
      data[i]=bandVector_.at(i)->GetBlock(0, 0);
      idx[i]=i;
   }
   // de aca puede saltar error por no permitir la modificacion del canvas
   // de destino
   bool equal = true;
   if ( *this!=*pTarget )
   {
      TRY {
         pTarget->SetDataType(GetDataType() );
         pTarget->SetBandCount(count);
         int x, y;
         GetSize(x, y);
         pTarget->SetSize(x, y);
      }CATCH {equal=false;}
   }
   if ( equal&&*this==*pTarget ) {
      pTarget->Write(idx, data, pMask);
   } else {
      REPORT_DEBUG("D:No pudo modificarse el canvas destino.");
   }
}
#endif   // __UNUSED_CODE__
/**
 * Escribe los datos
 * @param[in] BandIndex array con los indices de bandas
 * @param[in] Data destino donde se escriben los datos
 * @param[in] pMask Mascara de los datos
 */
void MemoryCanvas::Write(std::vector<int> &BandIndex, std::vector<void*> &Data,
                         const Mask *pMask) {
   if (bandVector_.empty()) {
      REPORT_AND_RETURN("D:MemoryCanvas, no se encuentra inicializado.");
   }
   int x, y;
   GetSize(x, y);
   for (size_t i = 0; i < BandIndex.size(); i++) {
      if (BandIndex[i] >= GetBandCount()) {
         REPORT_AND_RETURN("D:MemoryCanvas, se excede el numero de bandas.");
      }
      // Escribe en la banda deseada el buffer asociado
      bandVector_.at(BandIndex[i])->Write(Data.at(i), 0, 0, x, y);
   }
}

// ----------------------------- VECTORIAL -----------------------------
/** Canvas vectorial */
/**
 * Retorna un DC asociado a un bitmap en memoria.
 * El bitmap sera bajado al canvas al hacer un Flush()
 *
 * @return DC asociado a un bitmap en caso de tratarse de un canvas uchar
 * @return NULL en caso de no tener tipo de dato uchar
 */
wxDC *MemoryCanvas::GetDC() {
   // Solo retorna DC si el canvas tiene tipo de dato uchar
   if (pDc_->IsOk() && GetDataType() == DataInfo<unsigned char>::Name) {
      return pDc_;
   }
   return NULL;
}

/** Pasa al buffer interno la renderizacion sobre wxDC */
/**
 * Baja los datos del bitmap asociado al DC al canvas.
 * \pre Tipo de dato del canvas == uchar
 * \note Utiliza como transparente el color negro pintado sobre el canvas
 * \todo Hacer que utilice la mascara o alfa channel.
 * \note Si se trata de un canvas con 3 bandas, utiliza los colores pintados en
 *       el DC
 * \note Si se trata de un canvas con cantidad de bandas distinta a 3, convierte
 *       los colores a escala de grises y aplica dichos grises a todas las bandas
 */
void MemoryCanvas::Flush(const Mask *pMask) {
   FlushDc();
   REPORT_DEBUG("D:MemoryCanvas::Flush()");
}

/** Pasa al buffer interno la renderizacion sobre wxDC */
/**
 * Baja los datos del bitmap asociado al DC al canvas.
 * \pre Tipo de dato del canvas == uchar
 * \note Utiliza como transparente el color negro pintado sobre el canvas
 * \todo Hacer que utilice la mascara o alfa channel.
 * \note Si se trata de un canvas con 3 bandas, utiliza los colores pintados en
 *       el DC
 * \note Si se trata de un canvas con cantidad de bandas distinta a 3, convierte
 *       los colores a escala de grises y aplica dichos grises a todas las bandas
 */
void MemoryCanvas::FlushDc() {

   // si no es uchar, no hace nada
   if (GetDataType() != DataInfo<unsigned char>::Name
// Sirve para evitar el BadAlloc por Canvas muy grande.
#ifdef __IGNORE_DC_ON_HUGE_CANVAS__
                                       || pDcBitmap_->IsNull()
#endif
                                                               ) {
      return;
   }
   std::vector<int> bandcount(GetBandCount());
   std::vector<void*> banddata(GetBandCount());
   // offset sobre el indice para usar sobre el array de datos de la imagen
   std::vector<int> imagedataoffset(GetBandCount(), 0);
   int x, y;
   GetSize(x, y);
   int w = pDcBitmap_->GetWidth(), h = pDcBitmap_->GetHeight();
   if (x != w || y != h) {
      REPORT_AND_RETURN("D: Inconsistencia entre el canvas(%d;%d) y el DcBitmap(%d;%d)",
                        x, y, w, h);
   }
#ifdef __UNUSED_CODE__
   // No se usa porque en el ciclo for que viene despues se sobrescribe banddata[0]
   unsigned char *ptemp = new unsigned char [x*y];
   banddata[0] = ptemp;
#endif
   for (int i = 0; i < GetBandCount(); i++) {
      bandcount[i] = i;
      banddata[i] = new unsigned char[x * y];
   }
   // obtiene una imagen del bmp
   wxImage dcimage = pDcBitmap_->ConvertToImage();
   // convierte a escala de grises
   if (GetBandCount() != 3) {
      dcimage = dcimage.ConvertToGreyscale();
   } else {
      for (int i = 0; i < GetBandCount(); i++)
         imagedataoffset[i] = i;
   }
   unsigned char *pdcimagedata = dcimage.GetData();
   // Lee los datos del canvas
   Read(bandcount, banddata);
   // copia los colores distintos del negro
   // \todo: Tener en cuenta la mascara, deshardcodear los ceros
   for (int i = 0; i < w * h; i++) {
      if (!(pdcimagedata[i * 3] == 0 && pdcimagedata[i * 3 + 1] == 0
            && pdcimagedata[i * 3 + 2] == 0)) {
         for (int j = 0; j < GetBandCount(); j++)
            static_cast<unsigned char*>(banddata[j])[i] = pdcimagedata[i * 3
                  + imagedataoffset[j]];
      }
   }
   dcimage.Destroy();
   // escribe los datos al canvas
   // \todo: Tener en cuenta la mascara
   Write(bandcount, banddata, NULL);
   for (int i = 0; i < GetBandCount(); i++)
      delete[] static_cast<unsigned char*>(banddata[i]);
#ifdef __UNUSED_CODE__
   /**
    * Para poder implementar el requerimiento de exportar visualizacion (TCK #3967) es necesario
    * conservar la informacion del DC. Se analizo el historial de revisiones y no se realizaba
    * ningun tipo de Clear en el Flush hasta que se agrego el metodo ClearDc en la revision 12037.
    * Por el momento se comenta el metodo ya que a partir de las pruebas realizadas no impacta en
    * el resto de los procesos.
    */
   ClearDc();
#endif
   REPORT_DEBUG("D:MemoryCanvas::FlushDc()");
}

/** Limpia los datos del canvas */
void MemoryCanvas::Clear() {
   // limpia y reinicializa las bandas
   SetBandCount(GetBandCount());
   ClearDc();
   FlushDc();
}

/** Limpia los datos del DC */
void MemoryCanvas::ClearDc() {
   if (pDcBitmap_->IsOk()) {
      pDc_->SetBackground(*wxBLACK_BRUSH);
      pDc_->Clear();
   }
}
// -------------------------- MULTI-RASTER --------------------------
/**
 * Retorna la cantidad de bandas
 * @return cantidad de bandas
 */
int MemoryCanvas::GetBandCount() const {
   if (bandVector_.empty()) {
      return bandCount_;
   }
   return bandVector_.size();
}

/**
 * Permite setear la cantidad de bandas
 * @param[in] BandCount cantidad de bandas
 */

void MemoryCanvas::SetBand(RasterBand* pBand, int BandIndex) {
   if (bandVector_[BandIndex])
      delete bandVector_[BandIndex];
   bandVector_[BandIndex] = pBand;
}


void MemoryCanvas::SetBandCount(int BandCount, bool SaveBands) {
   std::vector<RasterBand*> aux;
   std::vector<RasterBand*> backupBands = bandVector_;
   bandVector_.clear();
   std::vector<RasterBand*>::const_iterator it = backupBands.begin();
   int bandindex = 0;
   if (backupBands.size() > 0) {
      for (; it != backupBands.end(); ++it) {
         if  ( (bandindex < BandCount) && SaveBands) {
            aux.push_back(*it);
         } else {
            bandVector_.push_back(*it);
         }
         bandindex++;
      }
   }
   DeleteBands();
   int x = 0, y = 0;
   GetSize(x, y);
   for (int i = 0; i < BandCount; i++) {
      RasterBand *pband = NULL;
      if ( (i > static_cast<int>(aux.size() -1)) || !SaveBands ) {
         pband = RasterBand::Create(GetDataType());
         if ( pband ) pband->SetSize(x, y);
      } else {
         pband = aux[i];
      }
      if (pband){
         bandVector_.push_back(pband);
      }
   }
   // si inicializo bandas, inicializa el DC
   if (!bandVector_.empty()) {
      pDc_->SelectObject(wxNullBitmap);
// Sirve para evitar el BadAlloc por Canvas muy grande.
// \todo (javier-Sin TCK) No esta probado.
#ifdef __IGNORE_DC_ON_HUGE_CANVAS__
      if (x*y < 10000*10000) {
         pDcBitmap_->Create(x, y, BITMAP_DEPTH);
         pDc_->SelectObject(*pDcBitmap_);
      } else {
         *pDcBitmap_ = wxNullBitmap;
      }
#else
      pDcBitmap_->Create(x, y, BITMAP_DEPTH);
      pDc_->SelectObject(*pDcBitmap_);
#endif  // __IGNORE_DC_ON_HUGE_CANVAS__
      bandCount_ = bandVector_.size();
   } else {
      bandCount_ = BandCount;
   }
}

// --------------------------- DATA-TYPE ----------------------------
/**
 * Nombre del tipo de dato
 * @return string con el tipo de dato de las bandas, vacio si el vector de bandas esta vacio
 */
std::string MemoryCanvas::GetDataType() const {
   if (bandVector_.empty()) {
      return dataType_;
   }

   return bandVector_.at(0)->GetDataType();
}

/**
 * Tamanio del dato en bytes
 * @return tamanio del dato en bytes
 */
int MemoryCanvas::GetDataSize() const {
   if (bandVector_.empty()) {
      return SizeOf(GetDataType());
   }

   return bandVector_.at(0)->GetDataSize();
}

/**
 * Asigna el nombre del tipo de dato
 * @param[in] DataType tipo de dato
 */
void MemoryCanvas::SetDataType(const std::string& DataType) {
   dataType_ = DataType;
   SetBandCount(GetBandCount());
}

// ----------------------------- BANDAS -----------------------------
/**
 * obtiene la banda para un indice determinado
 * @param[in] BandIndex vector con nro de bandas a leer
 * \todo delegar a la factoria
 * @return puntero a la banda
 */
RasterBand *MemoryCanvas::GetBand(int BandIndex) const {
   return bandVector_.at(BandIndex);
}

/**
 * Limpia el vector con las bandas, y libera la memoria de cada banda.
 */
void MemoryCanvas::DeleteBands() {
   std::vector<RasterBand*>::iterator it = bandVector_.begin();
   while (it != bandVector_.end()) {
      // \todo delegar a la factoria
      delete (*it);
      it++;
   }
   bandVector_.clear();
   bandCount_ = 0;
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
void MemoryCanvas::GetInternalData(std::vector<int> &BandIndex,
                                   std::vector<void*> &OutputData) {
   OutputData.clear();
   for (size_t i = 0; i < BandIndex.size(); i++){
      void* bandData = GetBand(BandIndex[i])->GetBlock(0, 0);
      /**
       * TODO: En el proceso de georref automatica, si la imagen es de tamanio
       * n*500 + 1, bandData a veces es null :S
       */
      if (bandData){
         OutputData.push_back(bandData);
      }
   }
}
}
