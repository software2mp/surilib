/*! \file FilterSelectionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <vector>
#include <list>
#include <map>
#include <string>
#include <utility>

#include "FilterSelectionPart.h"

// Includes suri
#include "suri/AuxiliaryFunctions.h"
#include "ConvolutionFilterRenderer.h"
#include "BandMathRenderer.h"
#include "BandInfo.h"
#include "suri/xmlnames.h"
#include "suri/BasicLibraryItemSelector.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/LibraryItemAttributeFactory.h"
#include "suri/ConvolutionFilterItemAttribute.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/choicebk.h"

// Includes App
#include "resources.h"

// Defines
#define INITIAL_SIZE 3
/** Valores de la mascara x del filtro de Sobel para el filtro de deteccion de bordes */
#define EDGE_DETECTION_KERNEL_MASKX "-1 0 1 -2 0 2 -1 0 1"
/** Valores de la mascara x del filtro de Sobel para el filtro de deteccion de bordes */
#define EDGE_DETECTION_KERNEL_MASKY "1 2 1 0 0 0 -1 -2 -1"
/** Columnas de los kernel de deteccion de bordes */
#define EDGE_DETECTION_KERNEL_ROWS 3
/** Filas de los kernel de deteccion de bordes */
#define EDGE_DETECTION_KERNEL_COLUMNS 3
/** Ecuacion para la operacion norma */
#define NORM_FILTER_OPERATION "sqrt(b1^2+b2^2)"

#define LIBRARY_FILTER_PAGE 5
/** Inicializa los recursos de la ventana */
void InitFilterSelectionXmlResource();

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(FilterSelectionPartEvent, FilterSelectionPart)
      IMPLEMENT_EVENT_CALLBACK(OnUiUpdate, OnUiUpdate(event), wxUpdateUIEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor
 */
FilterSelectionPart::FilterSelectionPart(LibraryManager* pManager, int Filters) :
                                 FilterPart(pManager),
                                 NEW_EVENT_OBJECT(FilterSelectionPartEvent),
                                 filters_(Filters){
   windowTitle_ = _(caption_FILTER_SELECTION);

   // Guardo los filtros soportados
   int i = 0;
   if ((filters_ & LOWPASS_FILTER) != 0)
      suportedFilters_.insert(
            std::pair<FilterProcess::FilterType, int>(FilterProcess::LowPassFilter,
                                                      i++));
   if ((filters_ & HIGHPASS_FILTER) != 0)
      suportedFilters_.insert(
            std::pair<FilterProcess::FilterType, int>(FilterProcess::HighPassFilter,
                                                      i++));
   if ((filters_ & EDGEDETECTION_FILTER) != 0)
      suportedFilters_.insert(
            std::pair<FilterProcess::FilterType, int>(
                  FilterProcess::EdgeDetectionFilter, i++));
   if ((filters_ & MAYORITY_FILTER) != 0)
      suportedFilters_.insert(
            std::pair<FilterProcess::FilterType, int>(FilterProcess::MaxFilter, i++));
   if ((filters_ & MINORITY_FILTER) != 0)
      suportedFilters_.insert(
            std::pair<FilterProcess::FilterType, int>(FilterProcess::MinFilter, i++));
   // No se utilizan los filtros de biblioteca para filtros post-clasificacion
   if (!(filters_ & MINORITY_FILTER) && !(filters_ & MAYORITY_FILTER)) {
      // Filtro utilizado por la biblioteca
      suportedFilters_.insert(
            std::pair<FilterProcess::FilterType, int>(FilterProcess::LibraryFilter, i++));
   }
}

/**
 * Destructor
 */
FilterSelectionPart::~FilterSelectionPart() {
   DELETE_EVENT_OBJECT;
}

/**
 *	Crea la ventana de la parte y contecta el evento UIUpdate.
 *	@return true si pudo cargar la ventana de la herramienta.
 *	@return false si fallo la carga de la ventana de la herramienta.
 */
bool FilterSelectionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_FILTER_SELECTION_PART"));
   if (!pToolWindow_) {
      return false;
   }   
   wxChoicebook *pchoicebook =
         XRCCTRL(*pToolWindow_, wxT("ID_FILTER_CHOICEBOOK"), wxChoicebook);
   // FilterSelectionPart muestra solo los filtros indicados en filters_
   if (suportedFilters_.find(FilterProcess::MaxFilter) == suportedFilters_.end())
      pchoicebook->DeletePage(4);
   if (suportedFilters_.find(FilterProcess::MinFilter) == suportedFilters_.end())
      pchoicebook->DeletePage(3);
   if (suportedFilters_.find(FilterProcess::EdgeDetectionFilter) == suportedFilters_.end())
      pchoicebook->DeletePage(2);
   if (suportedFilters_.find(FilterProcess::HighPassFilter) == suportedFilters_.end())
      pchoicebook->DeletePage(1);
   if (suportedFilters_.find(FilterProcess::LowPassFilter) == suportedFilters_.end())
      pchoicebook->DeletePage(0);

   pchoicebook->SetSelection(0);
   pToolWindow_->Connect(wxEVT_UPDATE_UI,
                         wxUpdateUIEventHandler(FilterSelectionPartEvent::OnUiUpdate),
                         NULL, pEventHandler_);

   return true;
}

/**
 * Obtiene el indice del filtro seleccionado y lo guarda en la propiedad interna
 * Invoca al metodo que devuelve una lista con el filtro segun las opciones
 * seleccionadaas con una banda como parametro, y guarda en filtro en la
 * propiedad interna.
 */
bool FilterSelectionPart::CommitChanges(){
   selectedFilter_ = GetSelectedFilterType();
   if (pPal_) {
         pPal_->AddAttribute<FilterProcess::FilterType>(FilterProcess::FilterTypeKey,
                                                        selectedFilter_);
   }
   return FilterPart::CommitChanges();
}

/**
 * Selecciona la pagina que corresponde al filtro guardado en la propiedad.
 * Para el filtro pasabajos y pasaaltos selecciona el radiobutton que se
 * ponde con el tamanio del kernel de la propiedad.
 * @return true si pudo pasar los datos de las propiedades a la propiedad interna
 * @return false si falla obtener el control de la pagina correspondiente al
 * filtro seleccionado.
 */
bool FilterSelectionPart::RollbackChanges() {
   int rows, columns;
   GetKernelSize(rows, columns);
   SetSelectedFilterType(selectedFilter_);
   SetSelectedKernelSize(selectedFilter_, rows, columns);
   return true;
}

/**
 * Asigna el filtro pasabajos a la propiedad (filtro por defecto).
 * Asigna el filtro por defecto.
 * Invoca a rollbackchanges para pasar los valores de las propiedades a los
 * controles wx.
 */
void FilterSelectionPart::SetInitialValues() {
   std::map<FilterProcess::FilterType, int>::iterator it = suportedFilters_.begin();
   if (it != suportedFilters_.end())
      selectedFilter_ = it->first;
   filterList_ = GetFilterMatrix(selectedFilter_, INITIAL_SIZE);
   if (pPal_) {
      pPal_->AddAttribute<FilterProcess::FilterType>(
               FilterProcess::FilterTypeKey, selectedFilter_);
   }
   //Seteo filtro activo y un par de cosas mas.
   CommitChanges();
}

/**
 * Obtiene el icono de la parte.
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 */
void FilterSelectionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_FILTER_SELECTION, ToolBitmap);
}

/**
 * Analiza si se modifico el tipo de filtro o el tamanio del mismo desde el ultimo
 * commit.
 * @param[in] Event evento UIUpdate
 * \note la comparacion de los kernel se podria evitar para los casos en que los
 * kernel son fijos (pasaaltos, pasabajos, deteccion de bordes)
 */
void FilterSelectionPart::OnUiUpdate(wxUpdateUIEvent &Event) {
   modified_ = false;
   FilterProcess::FilterType selectedfilter =  GetSelectedFilterType();
   if (selectedFilter_ == selectedfilter) {
      // Obtiene las dimensiones de la matriz interna y la del control wx
      int controlfilterrow, controlfiltercolumn, propertyfilterrow,
            propertyfiltercolumn;
      GetSelectedKernelSize(selectedfilter, controlfilterrow, controlfiltercolumn);
      GetKernelSize(propertyfilterrow, propertyfiltercolumn);

      // Compara las dimensiones de la matriz
      if (controlfilterrow != propertyfilterrow
            || controlfiltercolumn != propertyfiltercolumn)
         modified_ = true;
   } else {
      modified_ = true;
   }
   modified_ = pItemSelector_ ? modified_ || pItemSelector_->HasChanged() : modified_;
   Event.Skip();
}

/**
 * Devuelve el tipo de filtro seleccionado en la interfaz grafica.
 * @return filtertype : tipo de filtro seleccionado
 */
FilterProcess::FilterType FilterSelectionPart::GetSelectedFilterType() {
   /**if (pItemSelector_)
      return FilterProcess::LibraryFilter;**/
   FilterProcess::FilterType filtertype = FilterProcess::LowPassFilter;
   int filterindex = USE_CONTROL(*pToolWindow_, wxT("ID_FILTER_CHOICEBOOK"),
         wxChoicebook, GetSelection(), -1);

   std::map<FilterProcess::FilterType, int>::iterator it = suportedFilters_.begin();
   for (; it != suportedFilters_.end(); it++) {
      if (it->second == filterindex)
         filtertype = it->first;
   }

   return filtertype;
}

/**
 * Configura el tipo de filtro seleccionado en el control wx
 * @param[in] Type tipo de filtro a seleccionar
 */
void FilterSelectionPart::SetSelectedFilterType(FilterProcess::FilterType Type) {
   std::map<FilterProcess::FilterType, int>::iterator it = suportedFilters_.find(Type);
   if (it != suportedFilters_.end())
      GET_CONTROL(*pToolWindow_, wxT("ID_FILTER_CHOICEBOOK"), wxChoicebook)
                                          ->ChangeSelection(it->second);
}

/**
 * Configura la dimension del kernel en interfaz grafica para uno de los
 * algoritmos
 * @param[in] Type el algoritmo a configurar
 * @param[in] Size la nueva dimension en interfaz grafica
 */
void FilterSelectionPart::SetSelectedKernelSize(FilterProcess::FilterType Type,
                                                   int Rows, int Columns) {
   std::map<FilterProcess::FilterType, int>::iterator it = suportedFilters_.find(Type);
   if (it == suportedFilters_.end() || Type == FilterProcess::EdgeDetectionFilter)
      return;

   int index = 0;
   switch (Rows) {
      case 5:
         index = 1;
         break;
      case 7:
         index = 2;
         break;
   }

   wxWindow *ppagewindow = USE_CONTROL(*pToolWindow_, wxT("ID_FILTER_CHOICEBOOK"),
                                       wxChoicebook, GetPage(it->second), NULL);
   if (ppagewindow)
      GET_CONTROL(*ppagewindow, wxT("ID_KERNEL_SIZE"), wxRadioBox)
                                                     ->SetSelection(index);
}


/**
 *	Devuelve tamanio de filas y columnas de la matriz seleccionada en
 *	el control wx.
 *	@param[out] Rows filas de la matriz
 *	@param[out] Columns columnas de la matriz
 */
void FilterSelectionPart::GetSelectedKernelSize(FilterProcess::FilterType Type,
                                                int &Rows, int &Columns) {
   std::map<FilterProcess::FilterType, int>::iterator it = suportedFilters_.find(Type);
   if (it == suportedFilters_.end() || it->first == FilterProcess::LibraryFilter)
      return;

   int kernelsize = -1;
   if (Type != FilterProcess::EdgeDetectionFilter) {
      wxWindow *ppagewindow = USE_CONTROL(*pToolWindow_,
                                      wxT("ID_FILTER_CHOICEBOOK"),
                                      wxChoicebook, GetPage(it->second), NULL);
      if (ppagewindow)
         kernelsize = USE_CONTROL(*ppagewindow, wxT("ID_KERNEL_SIZE"),
                                            wxRadioBox, GetSelection(), -1);
   }

   switch (kernelsize) {
      default:
      case 0:
         Rows = 3;
         Columns = 3;
         break;
      case 1:
         Rows = 5;
         Columns = 5;
         break;
      case 2:
         Rows = 7;
         Columns = 7;
         break;
   }
}

/**
 * Retorna el filtro seleccionado en el ultimo commit exitoso
 * @return FilterSelectionPart::FilterType tipo de filtro seleccionado
 */
FilterProcess::FilterType FilterSelectionPart::GetFilterType() {
   return selectedFilter_;
}

/**
 * Este metodo invoca al metodo que crea el filtro, segun el indice seleccionado
 * en el control wx. Devuelve una lista con el filtro obtenido en base a los
 * parametros seleccionados en los controles wx.
 * @param[in] Type tipo de filtro que se quiere crear
 * @param[in] Size dimension del filtro que se quiere aplicar.
 * @return filterlist lista con el filtro creado segun parametros de los controles
 *
 */
std::list<std::vector<std::vector<double> > > FilterSelectionPart::GetFilterMatrix(
      FilterProcess::FilterType Type , int Size) {
   std::list<std::vector<std::vector<double> > > filterlist;
   switch (Type) {
      default:
      case FilterProcess::LowPassFilter:
         filterlist = CreateLowPassFilter(Size);
         break;

      case FilterProcess::HighPassFilter:
         filterlist = CreateHighPassFilter(Size);
         break;

      case FilterProcess::EdgeDetectionFilter:
         filterlist = CreateEdgeDetectionFilter();
         break;
   }
   return filterlist;
}




/**
 * Crea una matriz cuadrada, cuyo tamanio se determina el tamanio seleccionado
 * en el control wx. La matriz tiene en todas las posiciones el valor 1/N, siendo
 * N la cantidad de elementos de la matriz.
 * Agrega la matriz a una lista.
 * @param[in] Size dimension del filtro que se quiere crear
 * @return filterlist lista que contiene el filtro
 */
std::list<std::vector<std::vector<double> > >
                        FilterSelectionPart::CreateLowPassFilter(int Size) {
   std::list<std::vector<std::vector<double> > > filterlist;

   std::vector<double> filterrow;
   std::vector<std::vector<double> > filtermatrix;

   float value = 1.0 / (Size * Size);
   filterrow.assign(Size, value);
   filtermatrix.assign(Size, filterrow);

   filterlist.push_back(filtermatrix);

   return filterlist;
}

/**
 * Crea una matriz cuadrada, cuyo tamanio se determina el tamanio seleccionado
 * en el control wx. La matriz tiene en todas las posiciones el valor -1, al
 * centro de la matriz se le asigna N-1, siendo N la cantidad de elementos de
 * la matriz.
 * Agrega la matriz a una lista.
 * @param[in] Size dimension del filtro que se quiere crear
 * @return filterlist lista que contiene la matrice (filtro)
 */
std::list<std::vector<std::vector<double> > >
                        FilterSelectionPart::CreateHighPassFilter(int Size) {
   std::list<std::vector<std::vector<double> > > filterlist;
   std::vector<double> filterrow;
   std::vector<std::vector<double> > filtermatrix;

   filterrow.assign(Size, -1);
   filtermatrix.assign(Size, filterrow);
   filtermatrix[Size / 2][Size / 2] = (Size * Size) - 1;

   filterlist.push_back(filtermatrix);

   return filterlist;
}

/**
 * Devuelve la lista con los filtros de deteccion de bordes.
 * Crea dos matrices de 3x3 y las carga con valores fijos (de dos defines).
 * Agrega ambas matrices a una lista.
 * @return filterlist lista con dos matrices (kernel de 3x3)
 */
std::list<std::vector<std::vector<double> > > FilterSelectionPart::CreateEdgeDetectionFilter() {
   // Crea string con la lista de valores de cada matriz
   std::string edgedetectionxmask(EDGE_DETECTION_KERNEL_MASKX);
   std::string edgedetectionymask(EDGE_DETECTION_KERNEL_MASKY);
   // Transforma cada string en un vector, cada posicion un valor de la matriz
   std::vector<std::string> edgedetectionxvector = tokenizer(edgedetectionxmask, " ");
   std::vector<std::string> edgedetectionyvector = tokenizer(edgedetectionymask, " ");
   // Crea un iterador apuntando al primer elemento de cada vector
   std::vector<std::string>::iterator itxmask = edgedetectionxvector.begin();
   std::vector<std::string>::iterator itymask = edgedetectionyvector.begin();
   // Crea dos matrices, uno para cada filtro (mascara x e y)
   std::vector<std::vector<double> > filtermatrixx;
   std::vector<std::vector<double> > filtermatrixy;
   // Crea dos vectores, uno para cada matriz resultante
   std::vector<double> filterrowx;
   std::vector<double> filterrowy;

   for (int i = 0; i < EDGE_DETECTION_KERNEL_ROWS; i++) {
      filterrowx.clear();
      filterrowy.clear();
      for (int j = 0; j < EDGE_DETECTION_KERNEL_COLUMNS; j++) {
         filterrowx.push_back(StringToNumber<double>(*itxmask));
         filterrowy.push_back(StringToNumber<double>(*itymask));
         ++itxmask;
         ++itymask;
      }
      filtermatrixx.push_back(filterrowx);
      filtermatrixy.push_back(filterrowy);
   }

   std::list<std::vector<std::vector<double> > > filterlist;

   filterlist.push_back(filtermatrixx);
   filterlist.push_back(filtermatrixy);

   return filterlist;
}

/**
 * Obtiene el nodo ecuacion segun el filtro seleccioado. Recibe como parametro
 * la cantidad de bandas sobre las que se aplica la ecuacion.
 * @param[in] BandCount cantidad de bandas de del elemento de salida.
 * @return pequationnode nodo ecuacion.
 * \note el unico filtro que genera una ecuacion es el de deteccion de bordes.
 */
wxXmlNode * FilterSelectionPart::GetBandsEquationNode(int BandCount) {
   wxXmlNode *pequationnode;

   switch (selectedFilter_) {
      case FilterProcess::EdgeDetectionFilter:
         pequationnode = CreateEdgeDetectionFilterEquationNode(BandCount);
         break;
      default:
         pequationnode = NULL;
         break;
   }

   return pequationnode;
}

/**
 * Genera el nodo ecuacion para el filtro de deteccion de bordes.
 * @param[in] BandCount cantidad de bandas de del elemento de salida.
 * @return pequationnode nodo ecuacion.
 */
wxXmlNode * FilterSelectionPart::CreateEdgeDetectionFilterEquationNode(int BandCount) {
   BandMathRenderer::Parameters parameters;
   BandMathRenderer::Parameters::Equation equation;
   equation.equation_ = wxT(NORM_FILTER_OPERATION);
   equation.bandNames_.insert(std::pair<std::string, int>(wxT("b1"), 0));
   equation.bandNames_.insert(std::pair<std::string, int>(wxT("b2"), 1));
   parameters.equations_.push_back(equation);
   return BandMathRenderer::GetXmlNode(parameters);
}

}  // namespace suri
