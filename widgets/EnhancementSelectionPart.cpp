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

#include <string>
#include <list>
#include <cstdio>
#include <memory>

// Inclues suri
#include "EnhancementSelectionPart.h"
#include "suri/ViewerWidget.h"
#include "StatsCanvas.h"
#include "Enhancement.h"
#include "suri/StatisticsFunctions.h"
#include "suri/Dimension.h"
#include "suri/RasterSpatialModel.h"
#include "suri/messages.h"
#include "RasterLayer.h"
#include "SRDStatistics.h"
#include "SRDHistogram.h"
#include "SREEnhancementFactory.h"
#include "SREEnhancement.h"
#include "suri/ParameterCollection.h"
#include "SREEnhancementUtils.h"
#include "suri/DataTypes.h"
#include "suri/StatisticsCalculator.h"

// Includes App
#include "resources.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/sizer.h"

// Defines

/** namespace suri */
namespace suri {

/**
 * Constructor
 * @param[in] pElement elemento sobre el que se aplica el realce
 */
EnhancementSelectionPart::EnhancementSelectionPart(RasterElement * pElement) :
      LutSelectionPart(pElement) {
   windowTitle_ = _(caption_ENHANCEMENT_CHOICE_SELECTION);
   // el mensaje de error se asigna en el constructor porque hay uno solo
   errorMessage_ = message_ENHANCEMENT_CHOICE_ERROR;
}


/**
 * Constructor
 * @param[in] pLayer elemento sobre el que se aplica el realce
 */
EnhancementSelectionPart::EnhancementSelectionPart(RasterLayer * pLayer) :
      LutSelectionPart(pLayer) {
   windowTitle_ = _(caption_ENHANCEMENT_CHOICE_SELECTION);
   // el mensaje de error se asigna en el constructor porque hay uno solo
   errorMessage_ = message_ENHANCEMENT_CHOICE_ERROR;
}

/** Destructor */
EnhancementSelectionPart::~EnhancementSelectionPart() {
}

/**
 * Crea la ventana de la herramienta.
 * @return true si pudo crear la ventana de la herramienta.
 * @return false si no pudo crear la ventana de la herramienta.
 */
bool EnhancementSelectionPart::CreateToolWindow() {
   bool returnvalue = ChoiceSelectionPart::CreateToolWindow();

   if (returnvalue) {
      // Muestra boton refresh
      GET_CONTROL(*pToolWindow_, wxT("ID_REFRESH_BUTTON"), wxBitmapButton)->Show(true);

      // Modifica texto del part
      GET_CONTROL(*pToolWindow_, wxT("ID_CHOICE_CHECKBOX"), wxCheckBox)
         ->SetLabel(caption_ENHANCEMENT_BOX_NAME);

      wxStaticBoxSizer* pbox = NULL;
      size_t staticboxposition = 0;
      if (pToolWindow_->GetSizer()
            && pToolWindow_->GetSizer()->GetItem(staticboxposition)
            && pToolWindow_->GetSizer()->GetItem(staticboxposition)->GetSizer()) {
         pbox = dynamic_cast<wxStaticBoxSizer*>(pToolWindow_->GetSizer()->GetItem(
               staticboxposition)->GetSizer());
      }
      if (pbox) {
         pbox->GetStaticBox()->SetLabel(caption_ENHANCEMENT_CHOICE_SELECTION);
      }
   }

   return returnvalue;
}

/**
 * Retorna una lista con los nombres de todas las luts soportadas
 * @return std::list con los nombres de las luts soportadas.
 */
std::list<std::string> EnhancementSelectionPart::GetLutNames() {
   namespace SRE = suri::raster::enhancement;

   std::list<std::string> enhancemetns;

   // Obtiene la lista de realces
   std::vector<std::string> enhancements =
         SRE::EnhancementFactory::GetInstance()->GetAvailableEnhancements();

   // Carga los realces en la propiedad
   std::vector<std::string>::const_iterator it;
   for (it = enhancements.begin(); it != enhancements.end(); ++it)
      enhancemetns.push_back(*it);

   return enhancemetns;
}

/**
 * Genera una Lut soportada por el sistema. El nombre debe estar entre los que
 * devuelve GetLutNames .
 * @param[in] LutName nombre de la lut que quiero
 * @param[in] Active indica si la lut esta activa. Si es false se retorna null
 * @param[out] pNewLutNode puntero al nodo generado por la lut de nombre
 * indicado
 * @return informa si pudo generar la lut.
 */
bool EnhancementSelectionPart::CreateLutNode(const std::string &LutName, bool Active,
                                             wxXmlNode* &pNewLutNode) {
   pNewLutNode = NULL;
   if (!Active) {
      return true;
   }

   namespace SRD = suri::raster::data;
   namespace SRE = suri::raster::enhancement;
   namespace SREU = suri::raster::enhancement::utils;

   SRD::StatisticsBase* pstats = NULL;
   SRD::HistogramBase* phist = NULL;
   GetRasterStats(pstats, phist);
	/** Fix para que no rompa la aplicacion si no es posible
	 *  calcular las estadisticas
	 */
   if (!pstats && !phist)
      return false;
   double* pmin = new double[phist->GetBandCount()];
   double* pmax = new double[phist->GetBandCount()];
   double* pmean = new double[phist->GetBandCount()];
   double* pstdev = new double[phist->GetBandCount()];

   for (int ix = 0, lenix = phist->GetBandCount(); ix < lenix; ++ix) {
      pmin[ix] = pstats->GetMin(ix);
      pmax[ix] = pstats->GetMax(ix);
      pmean[ix] = pstats->GetMean(ix);
      pstdev[ix] = pstats->GetStdev(ix);
   }

   std::auto_ptr<suri::ParameterCollection> params(new suri::ParameterCollection());

   params.get()->AddValue<int>("BandCount", phist->GetBandCount());
   params.get()->AddValue<int*>("NumBins", phist->GetNumBins());
   params.get()->AddValue<double**>("BAFP", phist->GetBinAccumFrequencyPercentage());
   params.get()->AddValue<int*>("AccumFreq", phist->GetBandAccumFrequency());
   params.get()->AddValue<double*>("Min", pmin);
   params.get()->AddValue<double*>("Max", pmax);
   params.get()->AddValue<double*>("Mean", pmean);
   params.get()->AddValue<double*>("Stdev", pstdev);

   SRE::Enhancement* pEnhancement = SRE::EnhancementFactory::GetInstance()->Create(
         LutName, *params.get());

   int** pLut = pEnhancement->CreateLUT(phist->GetBins());

   SREU::LutToXml(pNewLutNode, true, phist->GetBandCount(),
                  const_cast<std::string&>(LutName), phist->GetNumBins(),
                  phist->GetMin(), phist->GetMax(), pLut);

   delete[] pmin;
   delete[] pmax;
   delete[] pmean;
   delete[] pstdev;

   for (int ix = 0, lenix = phist->GetBandCount(); ix < lenix; ++ix) {
      delete[] pLut[ix];
      pLut[ix] = NULL;
   }

   delete[] pLut;
   pLut = NULL;

   return true;
}

/**
 * Crea el nuevo nodo del reenderizador a partir del nodo que habia antes en el
 * elemento. Si no hay nodo o no pudo modificar el estado de activacion
 * retorna false.
 * @param[in] pLutNode Nodo de la lut anterior
 * @param[in] Active Indica si la nueva lut debe estar activa
 * @param[out] pNewLutNode Nodo actualizado con los datos que selecciono el
 * usuario
 * @return informa si pudo actualizar el nodo
 */
bool EnhancementSelectionPart::CreateLutNode(const wxXmlNode* pLutNode, bool Active,
                                             wxXmlNode* &pNewLutNode) {
   pNewLutNode = NULL;
   // Creo una lut usando tabla en elemento
   LutArray lut;
   if (!LutArray::FromXml(pLutNode, lut)) {
      return !Active;
   }

   // Usa lut en elemento sin recalcular nada
   lut.active_ = Active;
   return lut.ToXml(pNewLutNode);
}

/**
 * Obtiene las estadisticas del elemento. Crea y configura un mundo, una lista
 * con el elemento y un statscanvas. Los asigna a un elemento rendercontroller y
 * obtiene las estadisticas del elemento.
 * @param[in] RequiredStats que estadisticas hay que calcular
 * @return stats estadisticas del elemento.
 * \deprecated Se utiliza en su lugar a GetRasterStats
 */
Statistics EnhancementSelectionPart::GetStatistics(
      Statistics::StatisticsFlag RequiredStats) {
   return suri::Statistics();
}

/** */
void EnhancementSelectionPart::GetRasterStats(
      suri::raster::data::StatisticsBase*& pStatistics,
      suri::raster::data::HistogramBase*& pHistogram) {
   RasterElement* pcurrentelem = dynamic_cast<RasterElement*>(
         pRasterLayer_ ? pRasterLayer_->GetElement() : pElement_);
   raster::data::StatisticsCalculator statscalculator(pcurrentelem);
   statscalculator.CalculateStatistics(pStatistics, pHistogram);
}

/**
 * Busca si el nodo es de una lut soportada y retorna el nombre. Si la Lut es
 * soportada y tiene nombre, el nombre retornado es el del nodo.
 * @param[in] pLutNode puntero al nodo de una lut
 * @param[out] LutName nombre de la lut. Si no existe
 * ninguna lut que corresponda con el nodo no se modifica
 * @return indica si el nodo coincide con una lut soportada.
 */
bool EnhancementSelectionPart::GetElementLutName(const wxXmlNode* pLutNode,
                                                 std::string &LutName) {
   namespace SRE = suri::raster::enhancement;

   LutName = "";
   // Obtiene la lista de realces
   std::vector<std::string> enhancements =
            SRE::EnhancementFactory::GetInstance()->GetAvailableEnhancements();

   wxString propval;
   pLutNode->GetPropVal(ENHANCEMENT_NODE_PROP_NAME, &propval);

   LutName = std::string(propval.mb_str());

   // Busca el nombre de la lut en enhancements
   std::vector<std::string>::iterator it = enhancements.begin();
   for (; it != enhancements.end() && *it != LutName; it++) ;

   return (it != enhancements.end());
}

/** Verifica si el nodo lut es de tipo correcto. */
bool EnhancementSelectionPart::IsValidLutType(const wxXmlNode* pLutNode) {
   // Genero LutArray con nodo
   LutArray lut;
   if (!LutArray::FromXml(pLutNode, lut)) {
      return false;
   }

   // Analizo que no se modifique el orden de las bandas.
   int inputband = 0;
   for (int i = 0; i < lut.GetCount(); i++) {
      lut.GetLookUpTable(i, inputband);
      if (inputband != i) {
         return false;
      }
   }

   return true;
}

/**
 * Retorna el estado de activacion de la tabla en el elemento
 * @param[in] pLutNode nodo de la tabla
 * @return bool que indica si la lut esta activa, si falla retorna false.
 */
bool EnhancementSelectionPart::IsActive(const wxXmlNode* pLutNode) {
   wxString propval;
   pLutNode->GetPropVal(ENHANCEMENT_NODE_PROP_ACTIVE, &propval);
   return (propval.compare(wxT(ACTIVE_PROPERTY_VALUE)) == 0);
}

/** Retorna el estado de activacion del elemento */
std::string EnhancementSelectionPart::GetNodeName() {
   return ENHANCEMENT_NODE;
}

/**
 * Configura el mundo para el renderizador. Si hay un raster activo asigna al
 * window y al extent del mundo recibido el subset interseccion entre el extent
 * y el window del muno activo. Si no hay raster activo asigna al mundo recibido
 * el extent del elemento.
 * @param[in] pWorld mundo que se quiere configurar
 */
void EnhancementSelectionPart::ConfigureWorld(World *pWorld) {
   World * pactiveworld = NULL;
   // Si el elemento esta activo, obtengo el mundo del viewer activo
   Element* pelement = pRasterLayer_ ? pRasterLayer_->GetElement() : pElement_;
   if (pelement->IsActive()) {
      // Obtengo el viewer activo
      ViewerWidget *pviewer = ViewerWidget::GetActiveViewer();
      if (pviewer) {
         pactiveworld = pviewer->GetWorld();
      }
   }

   // Le asigna al mundo recibido la referencia espacial del elemento
   pWorld->SetSpatialReference(pelement->GetSpatialReference().c_str());

   // Creo un subset para asignar el window y el world al mundo recibido, y uno
   // para obtener el tamanio del viewport
   Subset subset(0, 0, 0, 0), viewport(0, 0, 0, 0);
   // Si pude obtener el mundo del viewer activo asigna al subset la interseccion
   // entre el window y el world del mundo activo
   if (pactiveworld) {
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
      RasterElement* praster = dynamic_cast<RasterElement*>(pelement);
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
void EnhancementSelectionPart::ConfigureList(LayerList *pList) {
   pList->AddElement(CreateElement());
}

/**
 * Crea un raster a partir del elemento raster sobre el que se aplica la herramienta,
 * y le agrega la combinacion de bandas del elemento original.
 * @return pelement Elemento raster creado a partir del original
 * \attention el elemento devuelto y su eliminacion son responsabilidad del
 * solicitante.
 */
RasterElement * EnhancementSelectionPart::CreateElement() {
   Element* pcurrentelem = pRasterLayer_ ? pRasterLayer_->GetElement() : pElement_;

   std::string url = pcurrentelem->GetUrl().c_str();
   RasterElement *pelement = dynamic_cast<RasterElement*>(Element::Create(
         "RasterElement", url.c_str(), pcurrentelem->GetMetadataMap()));
   if (!pelement) {
      return pelement;
   }

   // Ruta al nodo combinacion de bandas
   wxString combinationnodepath = wxString(wxT(RENDERIZATION_NODE))
         + wxT(NODE_SEPARATION_TOKEN) + wxString(BAND_COMBINATION_NODE);

   // Creo un nodo a partir del nodo combinacion del elemento
   wxXmlNode *pbandcombination = new wxXmlNode(
         *pcurrentelem->GetNode(combinationnodepath));

   wxXmlNode *prnode = pelement->AddNode(pelement->GetNode(wxT("")),
                                         RENDERIZATION_NODE);
   pelement->AddNode(prnode, pbandcombination);

   Option::OptionsMapType options = pcurrentelem->GetMetadataMap().GetOption();
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
void EnhancementSelectionPart::RetrieveNoDataValue(bool& HasNoDataValue, double& NoDataValue) {
   HasNoDataValue = false;
   NoDataValue = 0.0;

   Element* pelem = pRasterLayer_ ? pRasterLayer_->GetElement() : dynamic_cast<Element*>(pElement_);
   if (pelem) {
      wxXmlNode* pdatanotvalidvalue = pelem->GetNode(PATH_NOT_VALID_DATA_VALUE_NODE_PIPES);
      if (pdatanotvalidvalue) {
         pdatanotvalidvalue = pdatanotvalidvalue->GetChildren();
         if (pdatanotvalidvalue->GetType() == wxXML_TEXT_NODE) {
            wxString ctt = pdatanotvalidvalue->GetContent();
            HasNoDataValue = true;
            NoDataValue = StringToNumber<double>(ctt.mb_str());
         }
      }
   }
}

}

