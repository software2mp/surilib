/*! \file ExporterWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <vector>
#include <string>
#include <memory>

// Includes Suri
#include "BandInfo.h"
#include "suri/Element.h"
#include "suri/ElementVectorSerializer.h"
#include "suri/Dimension.h"
#include "suri/RasterSpatialModel.h"
#include "FileRenderization.h"
#include "MovingWindowController.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Progress.h"
#include "CombinationInfoGenerator.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/NotebookWidget.h"
#include "suri/ParameterCollection.h"
#include "suri/RasterElement.h"
#include "suri/RasterProcess.h"
#include "suri/Subset.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/DataViewManager.h"
#include "suri/World.h"
#include "suri/WorldSerializer.h"
#include "XmlMetadataHandler.h"

// Includes wx
#include "wx/sstream.h"

// Defines
/** Cantidad de ciclos de la herramienta para la barra de progreso */
#define EXPORT_OPERATION_COUNT 3
#define PROCESS_DESCRIPTION "Proceso"

/** namespace suri */
namespace suri {

/** Nombre del proceso **/
const std::string RasterProcess::kProcessName = "RasterProcess";

/** ctor a partir de la capa de adaptacion de procesos **/
RasterProcess::RasterProcess(DataViewManager* pDataViewManager) :
                              pInputElement_(NULL),
                              pInputWorld_(new World), pOutputWorld_(new World),
                              pRasterModelIn_(NULL),
      pRasterModelOut_(NULL), elementSubset_(Subset()), viewerSubset_(Subset()),
      pRenderizationObject_(NULL), showMaskPart_(false),
      pDataViewManager_(pDataViewManager) {
}

/**
 * Constructor
 * Guarda la referencia espacial, el modelo espacial y el extent del elemento
 * recibido.
 * @param[in] pInputElement : puntero al elemento a exportar
 * @param[in] ViewerSubset:
 * @param[out] pOutputElement : referencia al elemento generado por la herramienta
 */
RasterProcess::RasterProcess(Element *pInputElement, const Subset &ViewerSubset,
                             DataViewManager* pDataViewManager) :
                             pInputElement_(NULL),
                             pInputWorld_(new World), pOutputWorld_(new World),
                             pRasterModelIn_(NULL),
      pRasterModelOut_(NULL), elementSubset_(Subset()), viewerSubset_(ViewerSubset),
      pRenderizationObject_(NULL), showMaskPart_(false),
      pDataViewManager_(pDataViewManager) {
   std::vector<Element*> elements;
   SetProcessName(kProcessName);
   elements.push_back(pInputElement);
   Initialize(elements);
}

RasterProcess::RasterProcess(const std::vector<Element*>& ElementsP,
                             const Subset &ViewerSubset, DataViewManager* pDataViewManager) :
      pInputElement_(NULL), pInputWorld_(new World), pOutputWorld_(new World),
      pRasterModelIn_(NULL),
      pRasterModelOut_(NULL), elementSubset_(Subset()), viewerSubset_(ViewerSubset),
      pRenderizationObject_(NULL), showMaskPart_(true),
      pDataViewManager_(pDataViewManager) {
   SetProcessName(kProcessName);
   Initialize(ElementsP);
}


/**
 * Destructor
 */
RasterProcess::~RasterProcess() {
   delete pRenderizationObject_;
   delete pInputWorld_;
   delete pOutputWorld_;
   RasterSpatialModel::Destroy(pRasterModelIn_);
   RasterSpatialModel::Destroy(pRasterModelOut_);
}

/**
 * Actualiza el estado de los parts que administra la clase con
 * informacion configurada en otros parts
 */
void RasterProcess::DataUpdate() {
#ifdef __UNUSED_CODE__

#ifdef __USE_SPATIAL_PART__
   if (!pSpatialPart_)
      return;
   pSpatialPart_->SetEnabled(HasValidSpectralData());
#else
   if (!pSubsetInputPart_)
         return;
      pSubsetInputPart_->SetEnabled(HasValidSpectralData());
#endif
#endif
}

/**
 * Configura el proceso, lo corre y genera la salida llamando a ConfigureOutput.
 * Utiliza varios metodos para que las clases que heredan puedan 
 * modificar el comportamiento
 * @return bool que indica si pudo ejecutar el proceso correctamente
 */
bool RasterProcess::RunProcess() {
   bool saveresult = true;

   if (!pRenderizationObject_) {
      return false;
   }

   // Despliego barra de progreso
   Progress progression(EXPORT_OPERATION_COUNT, PROCESS_DESCRIPTION);

   LayerList *plist = GetElementList();
   progression.Update();

   if (!plist)
      return false;

   pRenderizationObject_->SetRenderizationList(plist);

   LayerList *pmasklist = GetMaskList();
   pRenderizationObject_->SetMaskList(pmasklist);

   World * pworld = GetOutputWorld();
   if (!pworld) {
      message_.assign(message_SAVE_ERROR wxT(" ") message_GET_WORLD_ERROR);
      return false;
   }


   pRenderizationObject_->SetWorld(pworld);

   // Llamado a configure
   ConfigureProcess();
   // guarda
   if (!pRenderizationObject_->Render()) {
      message_ = wxString(message_RASTER_OPERATION_ERROR wxT("."));
      // \TODO mejorar el mensaje de error
      saveresult = false;
   }
   progression.Update();

   // Elimina la lista, que debe existir por fuera del pSaveObject_->Save
   if (saveresult)
      saveresult = ConfigureOutput();

   // No son mas necesarios el mundo y la lista para el SaveObject
   pRenderizationObject_->SetMaskList(NULL);
   pRenderizationObject_->SetRenderizationList(NULL);
   pRenderizationObject_->SetWorld(NULL);

   delete plist;
   delete pmasklist;

   RasterSpatialModel::Destroy(pRasterModelOut_);
   // Es necesario ponerlo en NULL ya que sino queda apuntado a cualquier lado
   progression.Update();

   return saveresult;
}

/**
 * Crea un elemento con el filename del elemento recibido
 * Llama a ConfigureRaster para permitir a las clases que heredan configurar
 * el elemento.
 * Crea un lista y el agrega el elemento.
 * \attention La lista devuelta (y su destruccion) es responsabilidad del que la recibe.
 * @return playerlist : devuelve la lista con el elemento. Devuelve null si falla
 * la creacion del elemento raster o su configuracion.
 */
LayerList *RasterProcess::GetElementList() {
   LayerList *playerlist = new LayerList();
   // Se copia el elemento porque el original esta en la LayerList y
   // tiene eventos conectados
   Element* inputElem = GetInputElement();
   RasterElement* praster = dynamic_cast<RasterElement*>(Element::Create(
         "RasterElement", inputElem->GetNode(), inputElem->GetMetadataMap()));

   // Si falla la creacion/configuracion del elemento retorna NULL
   if (!praster || !ConfigureRasterBands(praster) || !ConfigureRaster(praster)) {
      SHOW_ERROR(message_RASTER_CONFIGURATION_ERROR);
      delete playerlist;
      delete praster;
      return NULL;
   }
   /** 
    * Se elimina el nodo del renderizador multiplexor para evitar problemas en la renderizacion
    * en el procesamiento
   **/
   wxXmlNode* pmultnode =
                  praster->GetNode(RENDERIZATION_NODE NODE_SEPARATION_TOKEN BAND_MULTIPLEXOR_NODE);
   if (pmultnode)
      praster->RemoveNode(praster->GetNode(RENDERIZATION_NODE), pmultnode);
   // como esta el raster. Tiene interpolacion? => agregarselo a lo ultimo.
   praster->Activate(true);
   playerlist->AddElement(praster);

   return playerlist;
}

/**
 *  Devuelve una lista con el elemento que se quiere utilizar como mascara.
 *  Las clases que deseen utilizar mascaras deben reimplementar este metodo
 */
LayerList *RasterProcess::GetMaskList() {
   LayerList* playerlist = NULL;
   if (pAdaptLayer_
         && pAdaptLayer_->GetAttribute<LayerList*>(ProcessAdaptLayer::MaskListKeyAttr,
                                                   playerlist))
      return playerlist;
   return NULL;
}
/**
 * Configura el nodo de seleccion de bandas en el raster
 * @param[in] pRasterElement puntero al elemento a configurar
 * @return true siempre
 */
bool RasterProcess::ConfigureRaster(RasterElement *pRasterElement) {
   return true;
}

/**
 * Se llama antes de renderizar pero despues de configurar el mundo, 
 * permite configurar el nodo renderizacion de los elementos en la lista.
 * @return bool que indica si tuvo exito
 */
bool RasterProcess::ConfigureProcess() {
   return true;
}

/** 
 * Configura la salida con el resultado de la operacion.
 * @return bool que indica si tuvo exito
 */
bool RasterProcess::ConfigureOutput() {
   return true;
}

/**
 * Metodo que configura las bandas en el elemento a renderizar usando datos
 * ingresados por el usuario
 * @param[in] pRaster : RasterElement que se va a renderizar
 * @return true si tuvo exito al configurar las bandas
 * @return true si showSpectralSelectionPart_=false, en este caso se deben
 * configurar las bandas en ConfigureRaster.
 * @return false si ocurrio un error al actualizar las bandas.
 */
bool RasterProcess::ConfigureRasterBands(RasterElement *pRaster) {
   if (!pRaster)
      return false;

   wxXmlNode *prnode = pRaster->AddNode(pRaster->GetNode(wxT("")), RENDERIZATION_NODE);
   CombinationInfo* pcombination = NULL;
   bool spectral = true;
   bool merge = false;
   if (pAdaptLayer_) {
      pAdaptLayer_->GetAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, spectral);
      merge = pAdaptLayer_->GetAttribute<CombinationInfo*>(
                     ProcessAdaptLayer::CombinationInfoKeyAttr, pcombination);
   }
   if (spectral && merge) {
      // Agrego los nodos con la info de las URLs y el nodo de apilamiento
      // conteniendo los nodos de combinacion de bandas.
      CombinationInfo* pcomb = CombinationInfo::Clone(pcombination);
      CombinationInfoGenerator::Merge(pcomb, pRaster);
   } else {
      pRaster->AddNode(prnode, CreateBandCombinationNode());
   }

#ifdef __UNUSED_CODE__
   // Zoom
   wxXmlNode *pinterpnode = pRaster->AddNode(
                  prnode, INTERPOLATION_NODE, wxT(""),
                  wxT(INTERPOLATION_PROPERTY_ZOOM_IN),
                  wxT(INTERPOLATION_PROPERTY_VALUE_NEAREST_NEIGHBOUR));
   if (!pinterpnode)
      REPORT_AND_FAIL_VALUE("D:Error al agregar la interpolacion al raster", false);

   // Tipo de ZOOM
   pinterpnode->AddProperty(wxT(INTERPOLATION_PROPERTY_ZOOM_OUT),
                            wxT(INTERPOLATION_PROPERTY_VALUE_NEAREST_NEIGHBOUR));
#endif

   return true;
}

/**
 * Crea un nodo combinacion de bandas con la seleccion de bandas.
 * Carga las bandas en un string y crea un nodo combinacion.
 * @return pCombinationNode nodo xml con la combinacion de bandas seleccionada
 *
 * \deprecated El part es responsable por devolver el/los nodo(s) como
 *    corresponde (ver RasterProcess::ConfigureRasterBands)
 */
wxXmlNode * RasterProcess::CreateBandCombinationNode() {
   std::string bandlist;
   wxXmlNode * pCombinationNode = NULL;
   std::vector<int> bandvector = GetSelectedRasterBands();
   std::vector<int>::iterator iter = bandvector.begin();
   for (; iter != bandvector.end(); ++iter) {
      if (!bandlist.empty()) {
         bandlist.append(" ");
      }
      std::ostringstream auxname; auxname << *iter;
      bandlist.append(auxname.str());
   }

   pCombinationNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                    wxT(BAND_COMBINATION_NODE));
   pCombinationNode->AddChild(new wxXmlNode(pCombinationNode,
                              wxXML_TEXT_NODE, wxEmptyString, wxT(bandlist)));

   return pCombinationNode;
}


/**
 * Retorna las bandas selecciondas por el usuario. Si el proceso no tiene
 * pagina de seleccion espectral retorna todas las bandas.
 * @return vector con las bandas que se usan para proceso.
 *
 * \todo (danieltesta-#2961): actualizar el proceso para cuando este terminado
 *    el soporte para multiples capas.
 * \deprecated El proceso raster ya no tiene la responsabilidad de obtener
 *    las bandas desde el part (ver RasterProcess::CreateBandCombinationNode)
 */
std::vector<int> RasterProcess::GetSelectedRasterBands() const {
   std::vector<BandInfo> bands;

   if (!pAdaptLayer_
         || !pAdaptLayer_->GetAttribute<std::vector<BandInfo> >(
               ProcessAdaptLayer::BandInfoKeyAttr, bands)) {
      std::vector<int> allbands;
      for (int i = 0; i < bandCount_; i++)
         allbands.push_back(i);
      return allbands;
   }

   std::vector<int> retbands;
   for (int ix = 0, lenix = bands.size(); ix < lenix; ++ix) {
      // solo se agregan las bandas validas 
      int valid = 1;
      Element* pelement = GetInputElement();
      if (bands[ix].GetElement())
         bands[ix].GetElement()->GetBandValidity(valid, bands[ix].GetPosition());
      else if (dynamic_cast<RasterElement*>(pelement))
         dynamic_cast<RasterElement*>(pelement)->GetBandValidity(valid, bands[ix].GetPosition());
      if (valid != 0)
         retbands.push_back(bands[ix].GetPosition());
   }

   return retbands;
}

/**
 * Indica si las bandas se configuraron correctamente
 * @return bool que indica si las bandas seleccionadas en pantalla son validas.
 * \note Analiza las bandas selecciondas en pantalla, no las del ultimo commit.
 * @return true si el proceso no tiene pagina de seleccion espectral
 */
bool RasterProcess::HasValidSpectralData() {
#ifdef __UNUSED_CODE__
   if (!pSpectralPart_)
      return true;

   return pSpectralPart_->HasValidData();
#endif
   return true;
}

/**
 * Metodo que configura el mundo del objeto para salvar.
 * Carga en el extent del elemento el window del mundo seleccionado en la parte
 * de seleccion de subset espacial.
 * Transforma el tamanio del raster en coordenadas P-L y lo asigna al tamanio
 * del viewport.
 * @return puntero a World configurado con parametros ingresados por el usuario
 *
 */
World* RasterProcess::GetOutputWorld() {
   Subset window(0, 0, 0, 0);
   if (!pOutputWorld_)
      pOutputWorld_ = new World;
   World* pworld = NULL;
   if (!pAdaptLayer_
         || !pAdaptLayer_->GetAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr,
                                               pworld)) {
      return NULL;
   }
   pworld->GetWindow(window);
   /** configuro el sistema de referencia de llegada **/
   std::string spatialreference = GetOutputSpatialReference();
   pOutputWorld_->SetSpatialReference(spatialreference);

   /** configura el subset de llegada **/
   window = GetOutputSubset();
   /** configura el modelo raster de llegada */
   RasterSpatialModel* prastermodel = GetOutputRasterSpatialModel();
   if (!prastermodel)
      return NULL;
   // Lo asigna al window del mundo.
   pOutputWorld_->SetWindow(window);
   // Iguala extent del mundo al window
   pOutputWorld_->SetWorld(window);

   // Transforma las coordenadas del window a ccordenadas P-L
   prastermodel->InverseTransform(window.lr_);
   prastermodel->InverseTransform(window.ul_);

   // Auxiliar para obtener alto y ancho del window
   Dimension auxmatriz(window);

   // Asigna el tamanio del raster en P-L al tamanio del viewport
   int width = SURI_ROUND(int, auxmatriz.GetWidth());
   int height = SURI_ROUND(int, auxmatriz.GetHeight());
   pOutputWorld_->SetViewport(width, height);

   return pOutputWorld_;
}

/**
 * Retorna el subset de la imagen de entrada que quiero exportar
 * TODO (Ramiro - TCK #2473) - Codigo parche: Refactorizar RasterProcess.
 */
Subset RasterProcess::GetInputSubset() {
   Subset window(0, 0, 0, 0);
   if (!pAdaptLayer_
         || !pAdaptLayer_->GetAttribute<Subset>(ProcessAdaptLayer::InputSubsetWindowKeyAttr,
                                               window)) {
      return Subset(0, 0, 0, 0);
   }
   // Uso GetWorld()->GetWindow(window) porque quiero el subset en sistema
   // de la imagen de entrada y no el elegido por el usuario.
   return window;
}

/** Devuelve el Sistema de Referencia de llegada.
 *  Template method: esto metodo se debe reimplementar si el proceso
 *  requiere que el SR de salida sea diferente al de llegada
 *  @return string con el wkt del SR de llegada **/
std::string RasterProcess::GetOutputSpatialReference() {
   return spatialReference_;
}

/** Devuelve el subset de llegada.
 *  Template method: esto metodo se debe reimplementar si el proceso
 *  requiere que el Subset de salida sea diferente al de llegada
 *  @return Subset de llegada **/
Subset RasterProcess::GetOutputSubset() {
   Subset window(0, 0, 0, 0);
   if (!pAdaptLayer_
         || pAdaptLayer_->GetAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr,
                                               window)) {
      return window;
   }
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                   pInputWorld_->GetSpatialReference());
   params.AddValue<std::string>(TransformationFactory::kParamWktOut, spatialReference_);
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pcoordinatestransform =
         pfactory->Create(TransformationFactory::kExact, params);
   TransformationFactoryBuilder::Release(pfactory);
   if (pcoordinatestransform) {
      pcoordinatestransform->Transform(window);
      delete pcoordinatestransform;
   }
   return window;
}

/** Devuelve el Modelo Raster de llegada
 *  Template method: esto metodo se debe reimplementar si el proceso
 *  requiere que el SR de salida sea diferente al de llegada
 *  @return RasterSpatialModel* Puntero al modelo raster de llegada
 **/
RasterSpatialModel* RasterProcess::GetOutputRasterSpatialModel() {
   if (!pRasterModelOut_) {
       RasterElement* pelement = dynamic_cast<RasterElement*>(GetInputElement());
       pRasterModelOut_ = RasterSpatialModel::Create(pelement->GetRasterModel());
   }
   return pRasterModelOut_;
}

void RasterProcess::Initialize(const std::vector<Element*>& ElementsP) {
   // Elementos recibidos
   inputElements_ = ElementsP;

   // Guarda la referencia espacial del elemento
   spatialReference_ = GetInputElement()->GetSpatialReference().c_str();

   RasterElement *pelement = dynamic_cast<RasterElement*>(GetInputElement());
   if (pelement) {
      pelement->GetElementExtent(elementSubset_);
      pRasterModelIn_ = RasterSpatialModel::Create(pelement->GetRasterModel());
      bandCount_ = pelement->GetBandCount();
   }

   /** Crear el mundo con las propiedades del elemento recibido */
   pInputWorld_->SetSpatialReference(spatialReference_);
   pInputWorld_->SetWorld(elementSubset_);
   pInputWorld_->SetWindow(elementSubset_);

   showMaskPart_ = true;
   showSpectralSelectionPart_ = true;
}

/** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso**/
void RasterProcess::OnProcessAdaptLayerSet() {
   if (pAdaptLayer_) {
      World* pinputworld = NULL;
      if (!pAdaptLayer_->GetAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr,
                                         pinputworld)) {
         pAdaptLayer_->AddAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr,
                                         pInputWorld_);
      } else {
         pInputWorld_ = pinputworld;
      }
      pAdaptLayer_->AddSerializer(ProcessAdaptLayer::InputWorldKeyAttr, new WorldSerializer());
      std::vector<Element*> inputelements;
      if (!pAdaptLayer_->GetAttribute<std::vector<Element*> >(
            ProcessAdaptLayer::InputElementsKeyAttr, inputelements)) {
         pAdaptLayer_->AddAttribute<std::vector<Element*> >(
            ProcessAdaptLayer::InputElementsKeyAttr, inputElements_);
      } else {
         inputElements_ = inputelements;
      }
      bool mask = true;
      if (!pAdaptLayer_->GetAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr,
                                           showMaskPart_)) {
         pAdaptLayer_->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr,
                                          showMaskPart_);
      } else {
         showMaskPart_ = mask;
      }
      Subset viewersubset;
      if (!pAdaptLayer_->GetAttribute<Subset>(ProcessAdaptLayer::InputSubsetWindowKeyAttr,               
                                          viewersubset)) {
      pAdaptLayer_->AddAttribute<Subset>(ProcessAdaptLayer::InputSubsetWindowKeyAttr,
                                         viewerSubset_);
      } else {
         viewerSubset_ = viewersubset;
      }
      pAdaptLayer_->AddAttribute<DataViewManager*>(
            ProcessAdaptLayer::DataViewManagerKeyAttr, pDataViewManager_);
      Initialize(this->inputElements_);
      DoOnProcessAdaptLayerSet();
   }
}

/** Metodo que especializa la configuracion del ProcessAdaptLayer para
 *  los procesos especificos
 */
void RasterProcess::DoOnProcessAdaptLayerSet() {
}

/**
 *
 * Serializacion
 *
 */

/**
 * Metodo que a partir de un xml que contiene los parametros del proceso configura el mismo.
 * @param[in] XmStr string en formato xml que contiene los parametros necesarios para la
 * correcta ejecucion del proceso
 * @return true en caso de poder configurar correctamente el proceso en funcion de los parametros
 * @return false en caso contrario
 **/
bool RasterProcess::ConfigureProcessFromXmlString(const std::string& XmlStr) {
   wxStringInputStream ss(XmlStr);
   wxXmlDocument doc(ss);
   wxXmlNode* pRoot = doc.GetRoot();
   this->pAdaptLayer_ = ProcessAdaptLayer::DeserializeXml(pRoot->GetChildren());
   return (pAdaptLayer_ != NULL);
}

/** Metodo que obtiene los parametros asociados al proceso en formato XML.
 * Esto permite que el codigo cliente pueda hacer eso del xml resultante
 * (ej. para la exportacion del mismo)
 * @return string en formato xml con los parametros asociados al proceso
 * @return string vacio en caso contrario.
**/
std::string RasterProcess::GetParametersAsXmlString() const {
   std::vector<Element*> elList;
   bool hasList = this->pAdaptLayer_->GetAttribute<std::vector<Element*> >(
         ProcessAdaptLayer::InputElementsKeyAttr, elList);
   if (hasList){
      this->pAdaptLayer_->AddSerializer(ProcessAdaptLayer::InputElementsKeyAttr,
                                        new ElementVectorSerializer());
   }
   return this->pAdaptLayer_->GetAttributesAsXmlString();
}

/**
 * Exporta los metadatos de una capa a un archivo
 * @param[in] pDatasource fuente de datos que posee el elemento con los metadatos a exportar
 * @return true en caso de que pueda exportar correctamente los metadatos, false caso contrario
 */
bool RasterProcess::ExportMetadataToFile(DatasourceInterface* pDatasource, std::string Filename) {
   bool exportmetadata = false;
   pAdaptLayer_->GetAttribute<bool>(ProcessAdaptLayer::ExportMetadataKeyAttr, exportmetadata);
   if (exportmetadata) {
      MetadataExporterInterface* pexporter = new XmlMetadataHandler();
      return pexporter->ExportMetadata(pDatasource, pDatasource->GetElement()->GetUrl().c_str());
   }
   return false;
}

}  /** namespace suri **/
