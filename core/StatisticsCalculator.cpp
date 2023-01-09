/*! \file StatisticsCalculator.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "suri/StatisticsCalculator.h"
#include "suri/xmlnames.h"
#include "MovingWindowController.h"
#include "KMeansAlgorithm.h"
#include "KMeansCanvas.h"
#include "suri/RasterElement.h"
#include "SRStatisticsCanvas.h"
#include "SRDStatistics.h"
#include "SRDHistogram.h"
#include "suri/ViewerWidget.h"
#include "suri/World.h"
#include "suri/LayerList.h"
#include "suri/Dimension.h"

// Includes Wx
// Defines
// forwards

namespace suri {

namespace raster {

namespace data {

StatisticsCalculator::StatisticsCalculator(RasterElement* pRaster) :
      pElement_(pRaster), useViewer_(true){
}

StatisticsCalculator::~StatisticsCalculator() {
}

/** Realiza el calculo de estadisticas sobre el raster configurado **/
bool StatisticsCalculator::CalculateStatistics(
      suri::raster::data::StatisticsBase* &pStatistics, bool ComputeAllBands, bool InterBandStats,
      bool UseViewer) {
   useViewer_ = UseViewer;
   return DoCalculateStatistics(&pStatistics, NULL, false, ComputeAllBands, InterBandStats);
}

/** Realiza el calculo de estadisticas e histograma sobre el raster configurado **/
bool StatisticsCalculator::CalculateStatistics(
      suri::raster::data::StatisticsBase* &pStatistics,
      suri::raster::data::HistogramBase* &pHistogram, bool ComputeAllBands,
      bool InterBandStats, const std::vector<double>& Min,
      const std::vector<double>& Max) {
   return DoCalculateStatistics(&pStatistics, &pHistogram, true, ComputeAllBands,
                                InterBandStats, Min, Max);
}

/** Realiza el calculo de histograma en funcion de las estadisticas que se pasan por parametro **/
bool StatisticsCalculator::CalculateHistogram(
      suri::raster::data::StatisticsBase* pStatistics,
      suri::raster::data::HistogramBase** pHistogram, bool ComputeAllBands,
      const std::vector<double>& Min, const std::vector<double>& Max) {
   if (!pStatistics) return false;
   if (!pElement_) return false;

   bool HasNoDataValue = false;
   double NoDataValue = 0.0;
   RetrieveNoDataValue(HasNoDataValue, NoDataValue);
   MovingWindowController* pcontroller = new MovingWindowController();

   World *pworld = new World();
   ConfigureWorld(pworld);
   LayerList *plist = new LayerList();
   ConfigureList(plist, ComputeAllBands);

   pcontroller->SetRenderizationList(plist);
   pcontroller->SetWorld(pworld);
   pcontroller->SetBestBufferSize();

   suri::render::HistogramCanvas histcanvas;
   histcanvas.SetNoDataValueAvailable(HasNoDataValue);
   histcanvas.SetNoDataValue(NoDataValue);
   histcanvas.SetStatistics(pStatistics);
   if (static_cast<int>(Min.size()) == pStatistics->GetBandCount()) {
      for (size_t b = 0; b < Min.size(); ++b)
         histcanvas.SetCustomMin(Min[b], b);
   }
   if (static_cast<int>(Max.size()) == pStatistics->GetBandCount()) {
      for (size_t b = 0; b < Max.size(); ++b)
         histcanvas.SetCustomMax(Max[b], b);
   }
   pcontroller->SetOutputCanvas(&histcanvas);
   if (pcontroller->Render())
      *pHistogram = histcanvas.GetHistogram();
   return *pHistogram != NULL;
}

/** Metodo auxiliar que realiza el calculo de estadistica e histograma **/
bool StatisticsCalculator::DoCalculateStatistics(
      suri::raster::data::StatisticsBase** pStatistics,
      suri::raster::data::HistogramBase** pHistogram, bool CalculateHistogram,
      bool ComputeAllBands, bool InterBandStats,
      const std::vector<double>& Min, const std::vector<double>& Max) {
   if (!pElement_)
      return false;

   bool HasNoDataValue = false;
   double NoDataValue = 0.0;
   RetrieveNoDataValue(HasNoDataValue, NoDataValue);

   suri::render::StatisticsCanvas statscanvas(InterBandStats);
   statscanvas.SetNoDataValueAvailable(HasNoDataValue);
   statscanvas.SetNoDataValue(NoDataValue);

   MovingWindowController* pcontroller = new MovingWindowController();

   World *pworld = new World();
   ConfigureWorld(pworld);
   LayerList *plist = new LayerList();
   ConfigureList(plist, ComputeAllBands);

   pcontroller->SetRenderizationList(plist);
   pcontroller->SetWorld(pworld);
   pcontroller->SetOutputCanvas(&statscanvas);
   pcontroller->SetBestBufferSize();

   if (pcontroller->Render()) {
      *pStatistics = statscanvas.GetStatistics();
      if (CalculateHistogram) {
         suri::render::HistogramCanvas histcanvas;
         histcanvas.SetNoDataValueAvailable(HasNoDataValue);
         histcanvas.SetNoDataValue(NoDataValue);
         histcanvas.SetStatistics(*pStatistics);
         if (static_cast<int>(Min.size()) == (*pStatistics)->GetBandCount()) {
            for (size_t b = 0; b < Min.size(); ++b)
               histcanvas.SetCustomMin(Min[b], b);
         }
         if (static_cast<int>(Max.size()) == (*pStatistics)->GetBandCount()) {
            for (size_t b = 0; b < Max.size(); ++b)
               histcanvas.SetCustomMax(Max[b], b);
         }

         pcontroller->SetOutputCanvas(&histcanvas);
         if (pcontroller->Render()) {
            *pHistogram = histcanvas.GetHistogram();
         }

      }
   }

   pcontroller->SetRenderizationList(NULL);
   pcontroller->SetWorld(NULL);
   pcontroller->SetOutputCanvas(NULL);

   delete pworld;
   delete plist;
   delete pcontroller;

   if (!pStatistics || (!pHistogram && CalculateHistogram))
      return false;
   return true;
}

/**
 * Configura el mundo para el renderizador. Si hay un raster activo asigna al
 * window y al extent del mundo recibido el subset interseccion entre el extent
 * y el window del muno activo. Si no hay raster activo asigna al mundo recibido
 * el extent del elemento.
 * @param[in] pWorld mundo que se quiere configurar
 */
void StatisticsCalculator::ConfigureWorld(World *pWorld) {
   World * pactiveworld = NULL;
    // Si el elemento esta activo, obtengo el mundo del viewer activo
    if (pElement_->IsActive()) {
       // Obtengo el viewer activo
       ViewerWidget *pviewer = ViewerWidget::GetActiveViewer();
       if (pviewer) {
          pactiveworld = pviewer->GetWorld();
       }
    }
    // Le asigna al mundo recibido la referencia espacial del elemento
    pWorld->SetSpatialReference(pElement_->GetSpatialReference().c_str());

    // Creo un subset para asignar el window y el world al mundo recibido, y uno
    // para obtener el tamanio del viewport
    Subset subset(0, 0, 0, 0), viewport(0, 0, 0, 0);
    // Si pude obtener el mundo del viewer activo asigna al subset la interseccion
    // entre el window y el world del mundo activo
    if (pactiveworld && useViewer_) {
       Subset window(0, 0, 0, 0), world(0, 0, 0, 0);
       pactiveworld->GetWindow(window);
       pactiveworld->GetWorld(world);
       subset = Intersect(window, world);
       // Transforma las coordenadas del window del mundo recibido a cordenadas del
       // viewport usando el mundo obtenido del viewer
       pactiveworld->InverseTransform(subset.lr_, viewport.lr_);
       pactiveworld->InverseTransform(subset.ul_, viewport.ul_);
    } else {
       // Si no pudo obtener el mundo activo (porque el elemento no esta desplegado)
       // asigno al subset y al viewport el extent del elemento
       int width = 0, height = 0;
       RasterElement* praster = dynamic_cast<RasterElement*>(pElement_);
       praster->GetRasterSize(width, height);
       praster->GetElementExtent(subset);
       // El tamanio del viewport es igual al de la imagen
       viewport.lr_.x_ = width;
       viewport.lr_.y_ = height;
    }
    // Uso el subset generado para asignar el window y el world al mundo recibido
    pWorld->SetWorld(subset);
    pWorld->SetWindow(subset);

    // Auxiliar para obtener alto y ancho del viewport
    Dimension auxmatriz(viewport);

    // Asigna el tamanio del raster en P-L al tamanio del viewport
    pWorld->SetViewport(SURI_TRUNC(int, auxmatriz.GetWidth() ),
                        SURI_TRUNC(int, auxmatriz.GetHeight() ));
}


/**
 * Agrega un elemento a la lista.
 * @param[in] pList Lista con el elemento a renderizar
 */
void StatisticsCalculator::ConfigureList(LayerList *pList, bool ComputeAllBands) {
   pList->AddElement(CreateElement(ComputeAllBands));
}

/**
 * Crea un raster a partir del elemento raster sobre el que se aplica la herramienta,
 * y le agrega la combinacion de bandas del elemento original.
 * @return pelement Elemento raster creado a partir del original
 * \attention el elemento devuelto y su eliminacion son responsabilidad del
 * solicitante.
 */
RasterElement * StatisticsCalculator::CreateElement(bool ComputeAllBands) {

   std::string url = pElement_->GetUrl().c_str();
   RasterElement *pelement = dynamic_cast<RasterElement*>(Element::Create(
         "RasterElement", url.c_str(), pElement_->GetMetadataMap()));
   if (!pelement) {
      return pelement;
   }

   if (ComputeAllBands) {
      std::vector<int> bands(pelement->GetBandCount());
      for (int b = 0; b < pelement->GetBandCount(); ++b) {
         bands[b] = b;
      }
      std::string bandcomb = join(bands.begin(), bands.end());
      // Creo un nodo a partir del nodo combinacion del elemento
      wxXmlNode *prnode = pelement->AddNode(pelement->GetNode(wxT("")),
      RENDERIZATION_NODE);

      pelement->AddNode(prnode, BAND_COMBINATION_NODE, bandcomb);

   } else {
      // Ruta al nodo combinacion de bandas
      wxString combinationnodepath = wxString(wxT(RENDERIZATION_NODE))
            + wxT(NODE_SEPARATION_TOKEN) + wxString(BAND_COMBINATION_NODE);

      // Creo un nodo a partir del nodo combinacion del elemento
      wxXmlNode *pbandcombination = new wxXmlNode(
            *pElement_->GetNode(combinationnodepath));

      wxXmlNode *prnode = pelement->AddNode(pelement->GetNode(wxT("")),
      RENDERIZATION_NODE);
      pelement->AddNode(prnode, pbandcombination);
   }
   Option::OptionsMapType options = pElement_->GetMetadataMap().GetOption();
   if (!options.empty()) {
      // si lo pudo inicializar configura el xml con la metada
      pelement->AddNode(pelement->GetNode(FILE_NODE), RAW_METADATA);
      Option::OptionsMapType::const_iterator it = options.begin();
      wxXmlNode* prawmeta = pelement->GetNode(
      FILE_NODE NODE_SEPARATION_TOKEN RAW_METADATA);
      for (; it != options.end(); ++it) {
         pelement->AddNode(prawmeta, it->first, it->second);
      }
   }

   pelement->Activate(true);

   return pelement;
}

/**
 * Devuelve el valor de dato invalido si esta definido.
 */
void StatisticsCalculator::RetrieveNoDataValue(bool& HasNoDataValue,
                                               double& NoDataValue) {
   HasNoDataValue = false;
   NoDataValue = 0.0;
   wxXmlNode* pdatanotvalidvalue = pElement_->GetNode(
   PATH_NOT_VALID_DATA_VALUE_NODE_PIPES);
   if (pdatanotvalidvalue) {
      pdatanotvalidvalue = pdatanotvalidvalue->GetChildren();
      if (pdatanotvalidvalue->GetType() == wxXML_TEXT_NODE) {
         wxString ctt = pdatanotvalidvalue->GetContent();
         HasNoDataValue = true;
         NoDataValue = StringToNumber<double>(ctt.mb_str());
      }
   }
}
} /** namespace raster **/
} /** namespace data **/

} /** namespace suri */
