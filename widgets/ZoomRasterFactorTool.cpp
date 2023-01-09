/*! \file ZoomRasterFactorTool.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes standard
#include <string>

// Includes Suri
#include "suri/messages.h"
#include "suri/ViewerWidget.h"
#include "suri/World.h"
#include "suri/WorldView.h"
#include "suri/LayerList.h"
#include "suri/ListView.h"
#include "suri/RasterElement.h"
#include "suri/RasterSpatialModel.h"
#include "suri/Dimension.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

#include "suri/ZoomRasterFactorTool.h"

// Includes Wx
#include "wx/wx.h"

// Includes App

// Defines
/** Numero de campo en el statusbar donde se muestra el factor de zoom */
#define ZOOM_FACTOR_FIELD_NUMBER 3
/** Ancho en pixeles del campo en el statusbar donde se muestra el factor de zoom */
#define ZOOM_FACTOR_FIELD_WITDTH 100
/** Porcentaje de error aceptado (entre 0 y 1) */
#define ERROR_ZOOM_LIMIT 0

/** namespace suri */
namespace suri {
/** Constructor. */
ZoomRasterFactorTool::ZoomRasterFactorTool() :
      pWorldViewer_(NULL), pListViewer_(NULL), viewportWidth_(0), viewportHeight_(0),
      rasterWidth_(0), rasterHeight_(0) {
}

/** Destructor */
ZoomRasterFactorTool::~ZoomRasterFactorTool() {
   delete pWorldViewer_;
   delete pListViewer_;
}

/**
 * Metodo virtual para indicar que la herramienta se ha des/activado
 * @param[in] Active nuevo estado de activacion de la herramienta
 */
bool ZoomRasterFactorTool::DoSetActive(bool Active) {
   wxStatusBar *psbar =
         (pViewer_ && dynamic_cast<wxFrame*>(
               pViewer_->GetFrame())) ?
                     dynamic_cast<wxFrame*>(pViewer_->GetFrame())->GetStatusBar() : NULL;

   if (psbar && Active) {
      // Este array es para definir el ancho de cada campo del statusbar
      int pwidtharray[ZOOM_FACTOR_FIELD_NUMBER];
      pwidtharray[0] = -1;
      pwidtharray[1] = -1;
      pwidtharray[2] = ZOOM_FACTOR_FIELD_WITDTH;

      // Se asegura que el statusbar tenga la cantidad de campos necesarios
      if (psbar->GetFieldsCount() < ZOOM_FACTOR_FIELD_NUMBER) {
         psbar->SetFieldsCount(ZOOM_FACTOR_FIELD_NUMBER, pwidtharray);
      }
   }

   // Retorna falso para que este activa siempre
   // Debido al comportamiento de Tool::SetActive
   active_ = true;
   return false;
}

/**
 * Registra los cambios en el mundo y en la lista de elemento.
 * @param[in] pViewer Visualizador del contexto asociado.
 * @return true
 */
bool ZoomRasterFactorTool::DoSetViewer(ViewerWidget *pViewer) {
   delete pWorldViewer_;
   pWorldViewer_ = new WorldView<ZoomRasterFactorTool>(this,
                                                       &ZoomRasterFactorTool::OnChange);

   World *pworld = pViewer ? pViewer->GetWorld() : NULL;
   if (!pworld) {
      return false;
   }

   pworld->RegisterViewer(pWorldViewer_);

   return true;
}

/** Registra cambios en la lista */
/**
 * Elimina lista anterior y agrega viewer a la lista actual.
 * @param[in] pList nueva lista de la herramienta
 */
bool ZoomRasterFactorTool::DoSetList(LayerList *pList) {
   if (!pList) {
      return false;
   }

   delete pListViewer_;
   pListViewer_ = new ListView<ZoomRasterFactorTool>(this,
                                                     &ZoomRasterFactorTool::OnChange);

   pList->RegisterViewer(pListViewer_);
   return true;
}

/**
 * Transforma las coordenadas del window, del sistema de referencia espacial
 * del mundo al del raster. Transforma las coordenadas a sistema pixel y linea.
 * Obtiene las dimensiones del Viewport y calcula el cociente entre el
 * alto / ancho del vieport contra el alto / ancho de la imagen.
 */
void ZoomRasterFactorTool::ObtainMatrixSizes() {
   // si no tiene los datos, sale
   if (!pList_ || !pViewer_ || !pViewer_->GetWorld()) {
      return;
   }

   // Reseteo todos los valores en cero para que no quede congelado en el
   // Statusbar el factor anterior
   rasterWidth_ = 0;
   rasterHeight_ = 0;
   viewportWidth_ = 0;
   viewportHeight_ = 0;

   RasterElement *pelement = NULL;
   // Recorro la lista buscando un Raster activo
   if (pList_->GetActiveCount() == 0) {
      return;
   }

   LayerList::ElementListType::iterator it = pList_->GetElementIterator();
   while (it != pList_->GetElementIterator(false)) {
      if ((*it)->IsActive() && dynamic_cast<RasterElement*>(*it)) {
         pelement = dynamic_cast<RasterElement*>(*it);
         break;
      }
      it++;
   }
   if (!pelement) {
      REPORT_DEBUG("D:No se pudo encontrar el raster activo");
      return;
   }

   Subset windowsubset;
   pViewer_->GetWorld()->GetWindow(windowsubset);

   // Creo un RasterSpatialModel utilizando el del elemento raster activo
   RasterSpatialModel *prastermodel = RasterSpatialModel::Create(
         pelement->GetRasterModel());
   if (!prastermodel) {
      REPORT_AND_FAIL("D:No se pudo crear RasterSpatialModel");
      return;
   }

   // UL y LR de la ventana en coordenadas de P,L de la imagen
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                pViewer_->GetWorld()->GetSpatialReference());
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                static_cast<std::string>(pelement->GetSpatialReference()));
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                     params);
   TransformationFactoryBuilder::Release(pfactory);
   if (pct) {
      // A partir de aca en Raster SR
      pct->Transform(windowsubset.ul_);
      pct->Transform(windowsubset.lr_);
      REPORT_DEBUG("D:Referencia espacial del mundo coincide con la del raster");
      // A partir de aca windowsubset tiene coordenadas de matriz
      prastermodel->InverseTransform(windowsubset.ul_);
      prastermodel->InverseTransform(windowsubset.lr_);
   }
   delete pct;
   // Auxiliar para obtener alto y ancho de la matriz
   Dimension auxmatriz(windowsubset);

   // Obtengo las dimensiones del Viewport
   int x = 0, y = 0;
   if (pViewer_->GetWorld()) {
      pViewer_->GetWorld()->GetViewport(x, y);
   }

   // Ancho y alto del raster pueden ser 0
   rasterWidth_ = auxmatriz.GetWidth();
   rasterHeight_ = auxmatriz.GetHeight();
   viewportWidth_ = x;
   viewportHeight_ = y;
}

/**
 * Calcula los valores de factor de zoom que se muestran por pantalla
 * Actualiza el statusbar con el factor de zoom.
 */
void ZoomRasterFactorTool::UpdateZoomRasterFactor() {
   wxFrame * pviewerframe =
         pViewer_ ? dynamic_cast<wxFrame*>(pViewer_->GetFrame()) : NULL;
   wxStatusBar *psbar = pviewerframe ? pviewerframe->GetStatusBar() : NULL;

   if (psbar) {
      float zoomrasterfactor = 0;
      float zoomviewportfactor = 0;

      wxString message;
      // Forma vieja de mostrar el factor de escala i.e. [ 1 : 1 ]
#ifdef __UNUSED_CODE__
      if ( rasterWidth_ > viewportWidth_ && rasterWidth_ != 0 && viewportWidth_ != 0 ) {
         if ( ( (rasterWidth_ % viewportWidth_) ) < (ERROR_ZOOM_LIMIT*rasterWidth_) ) {
            rasterWidth_ = rasterWidth_ - (rasterWidth_ % viewportWidth_);
         } else if ( rasterWidth_ < viewportWidth_ && viewportWidth_ != 0 &&
               rasterWidth_ != 0 ) {
            if ( ( (viewportWidth_ % rasterWidth_) ) < (ERROR_ZOOM_LIMIT*viewportWidth_) ) {
               viewportWidth_ = viewportWidth_ - (viewportWidth_ % rasterWidth_);
            }
         }
      }

      zoomrasterfactor = viewportWidth_ / ObtainGreatestCommonDivisor(rasterWidth_, viewportWidth_);
      zoomviewportfactor = rasterWidth_ / ObtainGreatestCommonDivisor(rasterWidth_, viewportWidth_);

      // Comprueba que el factor de zoom este dentro de rango
      if ( zoomrasterfactor > 0 && zoomviewportfactor > 0 ) {
         str = _(wxString::Format(wxT("[%0.2f : %0.2f]"), zoomrasterfactor, zoomviewportfactor) );
         // Si la razon entre el ancho del viewport y el del window no coincide con
         // la razon entre el alto del viewport y el del window, se marca con un * en.
         if ( (rasterWidth_ / viewportWidth_) != (rasterHeight_ / viewportHeight_) ) {
            str += _(wxT("*") );
         }
      }
#endif
      // Forma nueva de mostrar el factor como x[Factor] o x1 / [Factor]
      if (rasterWidth_ != 0 && viewportWidth_ != 0) {
         if (viewportWidth_ >= rasterWidth_) {
            zoomrasterfactor = viewportWidth_ / rasterWidth_;
            zoomviewportfactor = 1;
            message = wxString::Format(_(format_ZOOM_IN), zoomrasterfactor);
         } else {
            zoomviewportfactor = rasterWidth_ / viewportWidth_;
            zoomrasterfactor = 1;
            message = wxString::Format(_(format_ZOOM_OUT), zoomviewportfactor);
         }
      }

      if (zoomrasterfactor == 0 || zoomviewportfactor == 0) {
         // Fuera de rango, no se muestra
         message = _(message_NOT_AVAILABLE);
      }

      psbar->SetStatusText(message, ZOOM_FACTOR_FIELD_NUMBER - 1);
   }
}

/**
 * Metodo llamado cuando ocurre un cambio en el mundo, y que llama al metodo
 * Update para actualizar el estado del statusbar.
 */
void ZoomRasterFactorTool::OnChange(Model* model) {
   Update();
}

/**
 * Metodo que invoca a los metodos que actualizan los valores mostrados en el
 * statusbar
 */
void ZoomRasterFactorTool::Update() {
   if (active_) {
      ObtainMatrixSizes();
      UpdateZoomRasterFactor();
   }
}

#ifdef __UNUSED_CODE__
/**
 * Recibe dos enteros y calcula el maximo comun divisor entre ambos
 * @param[in] FirstNumber
 * @param[in] SecondNumber
 *	@return gcd: maximo comun divisor ente los dos numeros
 */
int ZoomRasterFactorTool::ObtainGreatestCommonDivisor(int FirstNumber, int SecondNumber) {
   int divisor = 2;
   int gcd = 1;

   if (FirstNumber > SecondNumber) {
      int temp = FirstNumber;
      FirstNumber = SecondNumber;
      SecondNumber = temp;
   }
   do {
      if ( FirstNumber%divisor == 0 && SecondNumber%divisor == 0 ) {
         gcd = gcd * divisor;
         FirstNumber = FirstNumber / divisor;
         SecondNumber = SecondNumber / divisor;
         divisor = 2;
      } else {
         divisor++;
      }
   }while (divisor <= FirstNumber);
   return gcd;
}
#endif
}  // namespace suri
