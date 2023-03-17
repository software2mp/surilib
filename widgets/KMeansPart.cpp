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

// Includes standard
#include <vector>

// Includes Suri
#include "KMeansPart.h"
#include "suri/ViewerWidget.h"
#include "MovingWindowController.h"
#include "suri/ClassificationProcess.h"
#include "suri/FactoryHelpers.h"
#include "suri/AuxiliaryFunctions.h"
#include "resources.h"
#include "suri/messages.h"
#include "ClassificationRenderer.h"
#include "KMeansAlgorithm.h"
#include "KMeansCanvas.h"
#include "suri/VectorStyle.h"
#include "SRDStatistics.h"
#include "suri/RasterElement.h"
#include "suri/StatisticsCalculator.h"
#include "SRStatisticsCanvas.h"
#include "SRDStatistics.h"
#include "SRDHistogram.h"
#include "suri/ViewerWidget.h"
#include "suri/World.h"
#include "suri/LayerList.h"
#include "suri/Dimension.h"
#include "suri/KMeansClassificationProcess.h"

// Includes Wx
#include "wx/slider.h"
#include "wx/xrc/xmlres.h"
#include "wx/spinctrl.h"
#include "wx/colour.h"
// Includes App
// Defines
/** Cantidad maxima de clases */
#define INITIAL_CLASS 1
/** Cantidad minima de clases */
#define MAX_CLASS_COUNT 100
/** Cantidad maxima de iteraciones */
#define MIN_CLASS_COUNT 1
/** Cantidad minima de iteraciones */
#define MAX_ITERATIONS_COUNT 100
/** Maximo valor inicial del pixel */
#define MIN_ITERATION_COUNT 1
/** Factor que modifica el threashold */
#define MAX_PIXEL_INITIAL_VALUE 255
void InitKMeansParametersXmlResource();

#define RANDOM_INITIAL_MEANS 0
#define MAXIMUM_DISPERSION_INITIAL_MEANS 1
#define PARTIAL_RANDOM_MEANS 2
#define QUANTIL_MEANS 3
#define MODE_MEANS 4
/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(KMeansPartWidgetEvent, KMeansPart)
      IMPLEMENT_EVENT_CALLBACK(OnScroll, OnScroll(event), wxScrollEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/** Ctor */
KMeansPart::KMeansPart(Element* pElement) :
      NEW_EVENT_OBJECT(KMeansPartWidgetEvent), pElement_(pElement), method_(0) {
   windowTitle_ = wxT(caption_KMEANS_OPTIONS);
   defaultThreshold_ = 0.25;
   defaultClassCount_ = 5;
   defaultMaxIterations_ = 5;

   threshold_ = 0;
   classCount_ = 0;
   maxIterations_ = 0;
}

/** Dtor */
KMeansPart::~KMeansPart() {
   DELETE_EVENT_OBJECT;
}

// --- Metodos de Part ---

/**
 * Crea la ventana de la parte
 * @return true si pudo crear la ventana, false en otro caso
 */
bool KMeansPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_KMEANS_PARAMS_PANEL"));

   GET_CONTROL(*pToolWindow_, wxT("ID_THRESHOLD"), wxSlider)
      ->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(
            KMeansPartWidgetEvent::OnScroll),
                NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_SELECTED_THRESHOLD_TEXT"),
               wxTextCtrl)->SetEditable(false);
   GET_CONTROL(*pToolWindow_, wxT("ID_THRESHOLD"), wxSlider)
      ->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(
            KMeansPartWidgetEvent::OnScroll),
                NULL, pEventHandler_);

   wxSpinCtrl *pclasscountcntl =
         XRCCTRL(*pToolWindow_, wxT("ID_CLASS_COUNT"), wxSpinCtrl);
   pclasscountcntl->SetRange(MIN_CLASS_COUNT, MAX_CLASS_COUNT);

   wxSpinCtrl *pmaxitercntl = XRCCTRL(*pToolWindow_, wxT("ID_CLASS_COUNT"), wxSpinCtrl);
   pmaxitercntl->SetRange(MIN_ITERATION_COUNT, MAX_ITERATIONS_COUNT);

   pToolWindow_->Layout();
   pParentWindow_->Fit();

   return true;
}

/**
 * Devuelve el icono de la parte
 * @param[out] ToolBitmap incono de la ventana
 * @param[in] X ancho del icono de la ventana
 * @param[in] Y alto del icono de la ventana
 */
void KMeansPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_KMEANS_PARAMETERS, ToolBitmap);
}

/**
 * Devuelve el estado del Part.
 * @return true si el part esta habilitado, false en otro caso
 */
bool KMeansPart::IsEnabled() {
   return enabled_;
}

/**
 * Indica si el Part tiene cambios para salvar.
 * @return true si las propiedades tienen valores distintos a los datos cargados
 * en los controles wx, false en otro caso.
 */
bool KMeansPart::HasChanged() {
   bool haschanged = false;

   // Verifico si se modifico el threshold
   if (!FLOAT_COMPARE(threshold_, GetSliderCtrlValue() )) {
      haschanged = true;
   }

   // Verifico si se modifico el numero de clases
   if (classCount_ != GetClassCountCtrlValue()) {
      haschanged = true;
   }

   // Verifico si se modifico el numero de iteraciones
   if (maxIterations_ != GetMaxIterationsCtrlValue()) {
      haschanged = true;
   }
   wxChoice* pinitialselchoice = XRCCTRL(*pToolWindow_, "ID_INITIAL_STRATEGY_CHOICE", wxChoice);
   if (method_ != pinitialselchoice->GetSelection()) {
      haschanged = true;
   }
   return haschanged;
}

/**
 * Salva los cambios realizados en el Part.
 * @return true si pudo salvar los cambios, false en otro caso.
 */
bool KMeansPart::CommitChanges() {
   threshold_ = GetSliderCtrlValue();
   classCount_ = GetClassCountCtrlValue();
   maxIterations_ = GetMaxIterationsCtrlValue();
   if (classCount_ < MIN_CLASS_COUNT || classCount_ > MAX_CLASS_COUNT
         || maxIterations_ < MIN_ITERATION_COUNT
         || maxIterations_ > MAX_ITERATIONS_COUNT) {
      return false;
   }
   
   int methodmeans = 0;
   // Calculo medias iniciales
   wxChoice* pinitialselchoice = XRCCTRL(*pToolWindow_, "ID_INITIAL_STRATEGY_CHOICE", wxChoice);
   method_ = pinitialselchoice->GetSelection();
   switch (method_) {
      case PARTIAL_RANDOM_MEANS:
         methodmeans = KMeansClassificationProcess::PartialRandomMeans;
         break;
      case MAXIMUM_DISPERSION_INITIAL_MEANS:
         methodmeans = KMeansClassificationProcess::MaximumDispersionMeans;
         break;
      case QUANTIL_MEANS:
         methodmeans = KMeansClassificationProcess::QuantilMeans;
         break;
      case MODE_MEANS:
         methodmeans = KMeansClassificationProcess::ModeMeans;
         break;
      case RANDOM_INITIAL_MEANS:      
      default:
         methodmeans = KMeansClassificationProcess::RandomMeans;
         break;

   }
   pPal_->AddAttribute<int>(KMeansClassificationProcess::MeansMethodAttrKey, methodmeans);
   pPal_->AddAttribute<double>(KMeansClassificationProcess::ThresholdAttrKey, threshold_);
   pPal_->AddAttribute<int>(KMeansClassificationProcess::IterationsAttrKey, maxIterations_);
   pPal_->AddAttribute<int>(KMeansClassificationProcess::ClassCountAttrKey, classCount_);
   return true;
}

/**
 * Restaura los valores originales del Part.
 * @return true siempre
 */
bool KMeansPart::RollbackChanges() {
   SetSliderCtrlValue(threshold_);
   SetClassCountCtrlValue(classCount_);
   SetMaxIterationsCtrlValue(maxIterations_);
   wxChoice* pinitialselchoice = XRCCTRL(*pToolWindow_, "ID_INITIAL_STRATEGY_CHOICE", wxChoice);
   method_ = pinitialselchoice->GetSelection();
   return true;
}

/**
 * Inicializa los datos mostrados
 */
void KMeansPart::SetInitialValues() {
   SetSliderCtrlValue(defaultThreshold_);
   threshold_ = defaultThreshold_;
   SetClassCountCtrlValue(defaultClassCount_);
   SetMaxIterationsCtrlValue(defaultMaxIterations_);
   CommitChanges();
}

/**
 * Informa si el subset seleccionado es valido
 * @return true siempre
 */
bool KMeansPart::HasValidData() {
   return true;
}

// --- Metodos de ParametersSelectionPart ---

/**
 * Retorna la cantidad de clases que se usaron para clasificar
 * (Llamar luego de GetClassCount)
 * @return classCount_ cantidad de clases que se usaron para clasificar
 */
int KMeansPart::GetClassCount() {
   return classCount_;
}

/**
 * Retorna las propiedades de la clase en la posicion indicada.
 * Las Posiciones van de 0 a GetClassCount()-1.
 * @param[out] Properties array con propiedades de cada clase
 * @return true siempre
 */
bool KMeansPart::GetClassProperties(std::vector<ClassInformation> &Properties) {
   // Clase valor no valido.
   ClassInformation ndvclassproperties(ClassInformation::NDVIndex,
                                       ClassInformation::NDVName);
   Properties.push_back(ndvclassproperties);

   int classcount = GetClassCount();
   for (int i = 0; i < classcount; i++) {
      // TCK #7175: se cambia la asignacion de colores a random para tener mayor cantidad de colores
      // antes estaba limitado a 16
      VectorStyle::Color color = VectorStyle::Color(rand() % 256, rand() % 256,
                                                    rand() % 256);
      ClassInformation classinformation(i + INITIAL_CLASS);
      classinformation.SetColor(color);
      classinformation.SetName(
            wxString::Format(wxT(NON_PARAMETRIC_CLASS_NAME_VALUE_d),
                              classinformation.GetIndex()).c_str());
      Properties.push_back(classinformation);
   }
   return true;
}

/**
 * Retorna el nodo que se debe agregar a elemento para renderizar
 * Llama a GetInitialMeans para calcular las medias iniciales
 * Itera hasta alcanzar la maxima cantidad de iteraciones permitidas o
 * que el cambio en todas las medias sea menor al threashold. En cada
 * iteracion llama a GetNextMean para mejorar la estimacion de las medias.
 * Genera KMeansAlgorithm con las medias obtenidas de paso anterior, lo
 * guarda en ClasificationRenderer y retorna su nodo.
 * @param[in] pRasterElement elemento del que se obtiene el nodo renderizacion
 * @return nodo renderizacion del elemento
 */
wxXmlNode* KMeansPart::GetClassificationRendererNode(RasterElement* pRasterElement) {
   int classcount = GetClassCount();

   std::vector<std::vector<double> > initialmean;
   std::vector<std::vector<double> > newmean;

   int bandcount = 0;
#ifndef __BETTER_INITIALIZATION_CODE__
   // Busco la cantidad de bandas que se usan para clasificar.
   // Este codigo es temporal. Se necesita mejorar el codigo
   // de inicializacion de los rasters.
   wxXmlNode *pcombinationnode = pRasterElement->GetNode(
         RENDERIZATION_NODE "|" BAND_COMBINATION_NODE);
   if (pcombinationnode) {
      wxXmlNode *pbandsnode = pcombinationnode->GetChildren();
      if (pbandsnode) {
         std::string bands = pbandsnode->GetContent().c_str();
         std::vector<std::string> bandlist = tokenizer(bands, " ");
         std::vector<std::string>::iterator iter = bandlist.begin();
         for (; iter != bandlist.end(); iter++)
            bandcount++;
      }
   }
#endif
   bandcount = pRasterElement->GetBandCount();
   // Calculo medias iniciales
   GetInitialMeans(classcount, bandcount, initialmean);
// #define __KMEANS_WIDGET_ALGORITH__

#ifdef __KMEANS_WIDGET_ALGORITH__
   // Calculo las medias
   bool finish = false;
   bool validmean = true;
   for (int i = 0; i < maxIterations_ && !finish; i++) {
      // Si no pudo calcular la media falla la configuracion del nodo
      validmean = GetNextMean(pRasterElement, initialmean, newmean);
      if (!validmean) {
         SHOW_ERROR(message_MEANS_CALCULATION_ERROR);
         return NULL;
      }

      finish = (CalculateChange(initialmean, newmean) < threshold_ / 100) && false;

      // Imprimo medias generadas
      for (size_t k = 0; k < newmean.size(); k++) {
         for (size_t j = 0; j < newmean[i].size(); j++)
            REPORT_DEBUG("D: ClaseNew [%d], Media([%d]) = %f", k, j, newmean[k][j]);
      }
      // Imprimo medias generadas
      for (size_t k = 0; k < newmean.size(); k++) {
         for (size_t j = 0; j < newmean[i].size(); j++)
            REPORT_DEBUG("D: ClaseOld [%d], Media([%d]) = %f", k, j, initialmean[k][j]);
      }
      if (!finish) {
         initialmean = std::vector<std::vector<double> >(newmean);
      }
   }
#endif
   // Genero nodo de algoritmo
   ClassificationRenderer::Parameters params;
   KMeansAlgorithm* pkmeansalgo = new KMeansAlgorithm();
   REPORT_DEBUG("iteraciones %d umbral %f", maxIterations_, threshold_);
   // Le agrego las clases
   for (int i = 0; i < classcount; i++)
      if (static_cast<int>(initialmean.size()) > i) {
         pkmeansalgo->classes_.push_back(
               KMeansAlgorithm::KMeansAlgorithmClassData(i + INITIAL_CLASS));
         pkmeansalgo->classes_[i].mean_ = std::vector<double>(initialmean[i]);
      }

   // Genero nodo
   params.pAlgorithm_ = pkmeansalgo;
   return ClassificationRenderer::GetXmlNode(params);
}

/**
 * Utiliza KMeansCanvas y RenderizationController para calcular la nueva media.
 * @param[in] pRasterElement elemento raster
 * @param[in] InitialMeans array de medias iniciales
 * @param[out] NextMean nueva media
 * @return true si pudo calcular la nueva media, false en otro caso
 */
bool KMeansPart::GetNextMean(RasterElement* pRasterElement,
                             const std::vector<std::vector<double> > &InitialMeans,
                             std::vector<std::vector<double> > &NextMean) {
   // Busco nodo combinacion de bandas en Raster
   wxXmlNode* pbandcombinationnode = GetNodeByName(
         wxT(RENDERIZATION_NODE "|" BAND_COMBINATION_NODE),
         pRasterElement->GetNode(""));
   if (!pbandcombinationnode) {
      return false;
   }

   // Busco nodo de interpolado en Raster
   wxXmlNode* pinterpolationnode = GetNodeByName(
         wxT(RENDERIZATION_NODE "|" INTERPOLATION_NODE), pRasterElement->GetNode(""));

   Element *prasterelement = Element::Create("RasterElement",
                                             pRasterElement->GetNode(""));
   if (!prasterelement) {
      return false;
   }
   prasterelement->Activate();

   // Limpio el nodo renderizacion del nuevo raster
   wxXmlNode* pnewrenderizationnode = GetNodeByName(wxT(RENDERIZATION_NODE),
                                                    prasterelement->GetNode(""));
   wxXmlNode* pchildren = pnewrenderizationnode->GetChildren();
   while (pchildren) {
      pnewrenderizationnode->RemoveChild(pchildren);
      pchildren = pnewrenderizationnode->GetChildren();
   }

   // Agrego nodo combinacion de bandas
   pnewrenderizationnode->AddChild(new wxXmlNode(*pbandcombinationnode));
   if (!pnewrenderizationnode->GetChildren()) {
      Element::Destroy(prasterelement);
      return false;
   }

   // Agrego nodo de interpolado
   if (pinterpolationnode)
      pnewrenderizationnode->GetChildren()->SetNext(new wxXmlNode(*pinterpolationnode));

   KMeansCanvas *poutcanvas = new KMeansCanvas;
   NextMean.clear();
   poutcanvas->SetInitialMeans(InitialMeans);
   MovingWindowController * pcontroller = new MovingWindowController();

   // Cargo lista a renderizar
   LayerList* pelementlist = new LayerList();

   pelementlist->AddElement(prasterelement);

   // Le paso la imagen completa como mundo
   World* pworld = new World;
   Subset worldextent;
   prasterelement->GetElementExtent(worldextent);
   int width, height;
   pRasterElement->GetRasterSize(width, height);
   pworld->SetSpatialReference(prasterelement->GetSpatialReference().c_str());
   pworld->SetWorld(worldextent);
   pworld->SetWindow(worldextent);
   pworld->SetViewport(width, height);

   // Seteo canvas que calcula estadisticas.
   pcontroller->SetRenderizationList(pelementlist);
   pcontroller->SetWorld(pworld);
   pcontroller->SetOutputCanvas(poutcanvas);
   pcontroller->SetBestBufferSize();
   // Renderizo y retorno las estadisticas.
   bool returnvalue = pcontroller->Render();
   if (returnvalue) {
      poutcanvas->GetNewMeans(NextMean);
   }

   // Elimino objetos usados para calcular estadisticas
   pcontroller->SetRenderizationList(NULL);
   pcontroller->SetWorld(NULL);
   pcontroller->SetOutputCanvas(NULL);

   delete pworld;
   delete pelementlist;
   delete pcontroller;
   delete poutcanvas;
   Element::Destroy(prasterelement);

   return returnvalue;
}

/**
 * Calcula que porcentaje se modificaron las medias. Para cada media calcula la
 * diferencia entre la nueva posicion y la anterior y la divide por el modulo
 * de la nueva media. Se queda con la distancia mas grande.
 * @param[in] InitialMeans las medias que se usaron como entrada a iteracion
 * @param[in] NextMean las medias que se obtubieron de la iteracion
 * @return maximo porcentaje de modificacion entre todas las medias.
 * return = |NextMean-InitialMean|/|NextMean|
 */
double KMeansPart::CalculateChange(
      const std::vector<std::vector<double> > &InitialMeans,
      const std::vector<std::vector<double> > &NextMean) {
   double maxdistance = 0;
   double currentdistance = 0;
   double mean_norm = 0;
   double diference = 0;
   // Calculo la diferencia porcentual mas grande entre todas las medias.
   for (size_t i = 0; i < InitialMeans.size(); i++) {
      diference = 0;
      mean_norm = 0;
      currentdistance = 0;
      for (size_t j = 0; j < InitialMeans[i].size(); j++) {
         diference += (NextMean[i][j] - InitialMeans[i][j]);
         mean_norm += NextMean[i][j] * NextMean[i][j];
         currentdistance += diference * diference;
      }
      currentdistance = currentdistance / mean_norm;
      if (currentdistance > maxdistance) {
         maxdistance = currentdistance;
      }
   }REPORT_DEBUG("D: La distancia mas grande fue: %d", maxdistance);
   return maxdistance;
}

/**
 * Calcula las medias iniciales. Utiliza valores aleatoreos
 * @param[in] ClassCount cantidad de clases
 * @param[in] BandCount cantidad de bandas
 * @param[out] InitialMeans array con las medias iniciales
 */
void KMeansPart::GetInitialMeans(int ClassCount, int BandCount,
                                 std::vector<std::vector<double> > &InitialMeans) {
   InitialMeans.clear();
   wxChoice* pinitialselchoice = XRCCTRL(*pToolWindow_, "ID_INITIAL_STRATEGY_CHOICE", wxChoice);
   /** Propuesta de fix para seleccion de clases iniciales:
    *  En funcion del minimo y maximo de la imagen (por banda)
    *  se fracciona el intervalo (min,max) en la cantidad de clases
    *  y se selecciona la media por cada banda de forma random dentro de esos
    *  randoms
    */

   static suri::raster::data::StatisticsBase* pstatistics = NULL;
   if (!pstatistics) {
      // se hace estatica la variable para evitar que se calcule varias veces
      suri::raster::data::StatisticsCalculator stats(
            dynamic_cast<RasterElement*>(this->pElement_));
      stats.CalculateStatistics(pstatistics, true);
   }
   srand(time(NULL));
   if (!pstatistics) {  
      for (int i = 0; i < ClassCount; i++) {
         InitialMeans.push_back(std::vector<double>());
         for (int j = 0; j < BandCount; j++) {
            double value = rand() % MAX_PIXEL_INITIAL_VALUE;
            InitialMeans[i].push_back(value);
         }
      }
      return;
   }
   switch (pinitialselchoice->GetSelection()) {
      case RANDOM_INITIAL_MEANS:
         GetRandomInitialMeans(ClassCount, BandCount, pstatistics, InitialMeans);
         break;
      case MAXIMUM_DISPERSION_INITIAL_MEANS:
         GetMaximunDispersionInitialMeans(ClassCount, BandCount, pstatistics, InitialMeans);
         break;
      case PARTIAL_RANDOM_MEANS:      
      default:
         GetPartialRandomInitialMeans(ClassCount, BandCount, pstatistics, InitialMeans);
         break;

   }
}

/**
 *  En funcion del minimo y maximo de la imagen (por banda) se generan clases iniciales de forma
 *  aleatoria.
 * @param[in] ClassCount cantidad de clases
 * @param[in] BandCount cantidad de bandas
 * param[in] pStatistics estadisticas de la imagen sobre la cual se calcularan las clases iniciales
 * @param[out] InitialMeans array con las medias iniciales
**/
void KMeansPart::GetRandomInitialMeans(int ClassCount, int BandCount, 
                                       suri::raster::data::StatisticsBase* pStatistics, 
                                       std::vector<std::vector<double> > &InitialMeans) { 
   for (int i = 0; i < ClassCount; i++) {
      InitialMeans.push_back(std::vector<double>());
      for (int j = 0; j < BandCount; j++) {
         double range = (pStatistics->pMax_[j] - pStatistics->pMin_[j]);
         /** se genera de forma aleatoria un vlaor dentro del rango de valores de la banda**/
         double value = pStatistics->pMin_[j] + (static_cast<double>(rand()) / RAND_MAX) * range;
         InitialMeans[i].push_back(value);
      }  
   }
}

/**
 * En funcion del minimo y maximo de la imagen (por banda) se generan clases iniciales 
 * utilizando maxima dispercion por la diagonal
 * @param[in] ClassCount cantidad de clases
 * @param[in] BandCount cantidad de bandas
 * param[in] pStatistics estadisticas de la imagen sobre la cual se calcularan las clases iniciales
 * @param[out] InitialMeans array con las medias iniciales
**/
void KMeansPart::GetMaximunDispersionInitialMeans(int ClassCount, int BandCount, 
                                             suri::raster::data::StatisticsBase* pStatistics, 
                                             std::vector<std::vector<double> > &InitialMeans) {   
   for (int i = 0; i < ClassCount; i++) {
      InitialMeans.push_back(std::vector<double>());
      for (int j = 0; j < BandCount; j++) {
         double range = (pStatistics->pMax_[j] - pStatistics->pMin_[j]);
         double spacing = range / static_cast<double>(ClassCount);
         double value = pStatistics->pMin_[j] + spacing * static_cast<double>(i);
         InitialMeans[i].push_back(value);
      }  
   }
}

/**
 *  En funcion del minimo y maximo de la imagen (por banda)
 *  se fracciona el intervalo (min,max) en la cantidad de clases
 *  y se selecciona la media por cada banda de forma random dentro de esos
 *  randoms
 * @param[in] ClassCount cantidad de clases
 * @param[in] BandCount cantidad de bandas
 * param[in] pStatistics estadisticas de la imagen sobre la cual se calcularan las clases iniciales
 * @param[out] InitialMeans array con las medias iniciales
**/
void KMeansPart::GetPartialRandomInitialMeans(int ClassCount, int BandCount, 
                                             suri::raster::data::StatisticsBase* pStatistics, 
                                             std::vector<std::vector<double> > &InitialMeans) {
   for (int i = 0; i < ClassCount; i++) {
      InitialMeans.push_back(std::vector<double>());
      for (int j = 0; j < BandCount; j++) {
         /** ver en que rango cae **/
         double range = ((pStatistics->pMax_[j] - pStatistics->pMin_[j]) / (double) (ClassCount));
         double bin = rand() % ClassCount;
         double low = bin > 0 ? (bin-1)*range: 0;
         double value = static_cast<double>(rand()) / RAND_MAX;
         value = low + (range * value);
         InitialMeans[i].push_back(value);
      }
   }
}

/**
 * Cambia el valor del slider.
 * @param[in] Threshold nuevo valor del slider, debe estar entre 0 y 100
 */
void KMeansPart::SetSliderCtrlValue(double Threshold) {
   wxSlider *pslider = XRCCTRL(*pToolWindow_, wxT("ID_THRESHOLD"), wxSlider);

   // Modifico el slider para que el desplazamiento sea logaritmico
   // Para que no termine en 99 tengo este caso especial.
   int value = SURI_ROUND(int, Threshold);
   if (Threshold < 100) {
      value = SURI_ROUND(int, log10(Threshold+1)*50);
   }

   if (value >= 0 && value <= 100) {
      pslider->SetValue(value);
      GET_CONTROL(*pToolWindow_, wxT("ID_SELECTED_THRESHOLD_TEXT"), wxTextCtrl)
         ->ChangeValue(wxString::Format(wxT(message_PERCENTAGE_d), value).c_str());
   }
}

/**
 * Retorna el valor del slider con el umbral para la clasificacion
 * @return valor del slider que representa el umbral de la clasificacion
 */
double KMeansPart::GetSliderCtrlValue() {
   wxSlider *pslider = XRCCTRL(*pToolWindow_, wxT("ID_THRESHOLD"), wxSlider);
   // Para que no termine en 99 tengo este caso especial.
   if (pslider->GetValue() == 100) {
      return 100;
   }
   // Modifico el slider para que el desplazamiento sea logaritmico
   double value = ((double) pslider->GetValue()) / 50;
   return pow(10, value) - 1; 
}

/**
 * Cambia la cantidad de clases en control
 * @param[in] ClassCount cantidad de clases
 */
void KMeansPart::SetClassCountCtrlValue(int ClassCount) {
   wxSpinCtrl *pspincntl = XRCCTRL(*pToolWindow_, wxT("ID_CLASS_COUNT"), wxSpinCtrl);

   if (!pspincntl) {
      return;
   }

   if (ClassCount >= MIN_CLASS_COUNT && ClassCount <= MAX_CLASS_COUNT) {
      pspincntl->SetValue(ClassCount);
   }
}

/**
 * Retorna la cantidad de clases seleccionada por usuario
 * @return cantidad de clases en el control
 */
int KMeansPart::GetClassCountCtrlValue() {
   wxSpinCtrl *pspincntl = XRCCTRL(*pToolWindow_, wxT("ID_CLASS_COUNT"), wxSpinCtrl);

   if (!pspincntl) {
      return 0;
   }

   return pspincntl->GetValue();
}

/**
 * Cambia el numero maximo de iteraciones en control
 * @param[in] MaxIterations numero maximo de iteraciones
 */
void KMeansPart::SetMaxIterationsCtrlValue(int MaxIterations) {
   wxSpinCtrl *pspincntl = XRCCTRL(*pToolWindow_, wxT("ID_MAX_ITERATIONS"), wxSpinCtrl);

   if (!pspincntl) {
      return;
   }

   if (MaxIterations >= MIN_ITERATION_COUNT && MaxIterations <= MAX_ITERATIONS_COUNT) {
      pspincntl->SetValue(MaxIterations);
   }
}

/** Retorna el numero maximo de iteraciones seleccionada por usuario */
int KMeansPart::GetMaxIterationsCtrlValue() {
   wxSpinCtrl *pspincntl = XRCCTRL(*pToolWindow_, wxT("ID_MAX_ITERATIONS"), wxSpinCtrl);

   if (!pspincntl) {
      return 0;
   }

   return pspincntl->GetValue();
}

/**
 * modifica el control que informa la posicion
 * @param[in] Event evento scroll
 */
void KMeansPart::OnScroll(wxScrollEvent& Event) {
   // Para asignarle el label a la barra se usa escala lineal y no logaritmica
   std::string percentasstring = wxString::Format(wxT(message_PERCENTAGE_d),
                                                  Event.GetPosition()).c_str();

   GET_CONTROL(*pToolWindow_, wxT("ID_SELECTED_THRESHOLD_TEXT"), wxTextCtrl)
      ->ChangeValue(percentasstring.c_str());
}
}  // namespace suri

