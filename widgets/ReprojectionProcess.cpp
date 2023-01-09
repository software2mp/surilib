/*! \file ReprojectionProcess.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "ReprojectionProcess.h"
#include "suri/DataViewManager.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/XmlFunctions.h"

// Includes Wx
// Defines
#define DEFAULT_DELTA 10
// forwards

namespace suri {

/** Nombre del proceso **/
const std::string ReprojectionProcess::kProcessName = "ReprojectionProcess";
/** Claves para atributos de configuracion del proceso */
const std::string ReprojectionProcess::ReprojectionParamsKeyAttr = "ReprojParams";
const std::string ReprojectionProcess::ReprojectionKeyAtttr = "ReprojBool";
const std::string ReprojectionProcess::AskGcpFileAttr = "GcpFile";
const std::string ReprojectionProcess::ShowResizePartAttr = "ShowResizePart";

/**
 * Constructor
 * @param pInputElement
 * @param ViewportSubset
 * @param pOutputElement
 * @param LibraryId
 * @param pManager
 */
ReprojectionProcess::ReprojectionProcess(Element *pInputElement,
                                         const Subset &ViewportSubset,
                                         const SuriObject::UuidType& LibraryId,
                                         DataViewManager* pDataViewManager,
                                         bool AskGcpFile, bool Reproject) :
      FileExporterProcess(pInputElement, ViewportSubset, pDataViewManager),
      pReprojectionParameters_(new ReprojectionParameters()), reprojec_(Reproject),
      askGcpFile_(AskGcpFile), libraryId_(LibraryId) {
   SetProcessName(kProcessName);
   this->showResizePart_ = true;
   RasterElement* praster = dynamic_cast<RasterElement*>(pInputElement);
   if (praster) {
      pReprojectionParameters_->rasterModelIn_ = praster->GetRasterModel();
      if (pReprojectionParameters_->rasterModelIn_.empty()) {
         DatasourceInterface* pdatasource =
               pDataViewManager->GetDatasourceManager()->GetDatasourceByElementId(
                     pInputElement->GetUid());
         pReprojectionParameters_->rasterModelIn_ =
               dynamic_cast<RasterElement*>(pdatasource->GetElement())->GetRasterModel();
      }
      if (!pReprojectionParameters_->pRasterSpatialModelIn_)
         pReprojectionParameters_->pRasterSpatialModelIn_ = RasterSpatialModel::Create(
               pReprojectionParameters_->rasterModelIn_);
      pReprojectionParameters_->srWktIn_ = pReprojectionParameters_->srWktOut_ =
            praster->GetSpatialReference().c_str();
      if (AskGcpFile) {
         pReprojectionParameters_->algorithm_ = suri::TransformationFactory::kPolynomial;
      } else {
         pReprojectionParameters_->algorithm_ = suri::TransformationFactory::kExact;
      }
      suri::ParameterCollection params;
      params.AddValue<std::string>(suri::TransformationFactory::kParamWktIn,
                                   pReprojectionParameters_->srWktIn_);
      params.AddValue<std::string>(suri::TransformationFactory::kParamWktOut,
                                   pReprojectionParameters_->srWktOut_);
      suri::TransformationFactory* pfactory =
            suri::TransformationFactoryBuilder::Build();
      pReprojectionParameters_->pTranform_ = pfactory->Create(
            suri::TransformationFactory::kExact, params);
      pReprojectionParameters_->pWorldIn_ = pInputWorld_;

      praster->GetRasterSize(pReprojectionParameters_->width_, pReprojectionParameters_->height_);

   }
}

/** dtor **/
ReprojectionProcess::~ReprojectionProcess() {
   delete pReprojectionParameters_;
}

/** Actualiza el estado de los parts que administra la clase */
void ReprojectionProcess::DataUpdate() {
   FileExporterProcess::DataUpdate();
}

/** Pemite configurar el RasterElement que se va a renderizar */
bool ReprojectionProcess::ConfigureRaster(RasterElement *pRasterElement) {
   FileExporterProcess::ConfigureRaster(pRasterElement);
   wxXmlNode* prnode = pRasterElement->GetNode(RENDERIZATION_NODE);

   wxXmlNode *preprojnode = pRasterElement->AddNode(prnode, REPROJECTION_NODE);
   pRasterElement->AddNode(preprojnode, REPROJECTION_TYPE_NODE,
                        wxT(pReprojectionParameters_->algorithm_));

   pRasterElement->AddNode(preprojnode, REPROJECTION_TYPE_ALGORITHM_NODE,
                           wxT(pReprojectionParameters_->algorithm_));

   pRasterElement->AddNode(preprojnode, REPROJECTION_TYPE_ORDER_NODE,
                              wxT(suri::IntToString(pReprojectionParameters_->algorithmOrder_)));
   pRasterElement->AddNode(preprojnode, REPROJECTION_TYPE_DELTA_NODE,
                                 wxT(suri::IntToString(DEFAULT_DELTA)));
   if (!pReprojectionParameters_->gcpListFileName_.empty()) {
      wxXmlNode *pgcpnode = pRasterElement->AddNode(preprojnode,
                                                    REPROJECTION_TYPE_GCPLIST_NODE);
      pRasterElement->AddNode(pgcpnode, REPROJECTION_TYPE_GCPLIST_URL_NODE,
                              pReprojectionParameters_->gcpListFileName_);
   }
#ifdef __UNUSED_CODE__
   wxXmlNode *pinterpnode = pRasterElement->AddNode(prnode, INTERPOLATION_NODE, wxT(""),
                                    wxT(INTERPOLATION_PROPERTY_ZOOM_IN),
                                    wxT(INTERPOLATION_PROPERTY_VALUE_NEAREST_NEIGHBOUR));
   if (!pinterpnode) {
      REPORT_AND_FAIL_VALUE("D:Error al agregar la interpolacion al raster", NULL);
   }

   pinterpnode->AddProperty(wxT(INTERPOLATION_PROPERTY_ZOOM_OUT),
                            wxT(INTERPOLATION_PROPERTY_VALUE_NEAREST_NEIGHBOUR));
#endif
   return true;
}

/** Configura el subset espacial */
World *ReprojectionProcess::GetWorld() {
   Subset inputsubset = GetInputSubset();
   pInputWorld_->SetWindow(inputsubset);
   if (pAdaptLayer_)
      pAdaptLayer_->GetAttribute<World*>(ProcessAdaptLayer::OutputWorldKeyAttr,
                                         pOutputWorld_);
   return pOutputWorld_;
}

/** Permite configurar la salida del proceso antes de correrlo */
bool ReprojectionProcess::ConfigureProcess() {
   bool result = FileExporterProcess::ConfigureProcess();
   return (result && pReprojectionParameters_->pTranform_ != NULL);
}

/** Una vez ejecutado el proceso hace visible la salida */
bool ReprojectionProcess::ConfigureOutput() {
   // En RunProcess de RasterProcess se hace un delete del world con lo que
   bool result = FileExporterProcess::ConfigureOutput();
   return result;
}

/** Devuelve el Sistema de Referencia de llegada. **/
std::string ReprojectionProcess::GetOutputSpatialReference() {
   return pReprojectionParameters_->srWktOut_;
}

/**  Devuelve el subset de llegada.  **/
Subset ReprojectionProcess::GetOutputSubset() {
   return RasterProcess::GetOutputSubset();
}

/** Devuelve el Modelo Raster de llegada */
RasterSpatialModel* ReprojectionProcess::GetOutputRasterSpatialModel() {
   return RasterProcess::GetOutputRasterSpatialModel();
}

/** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso**/
void ReprojectionProcess::DoOnProcessAdaptLayerSet() {
   FileExporterProcess::DoOnProcessAdaptLayerSet();
   if (pAdaptLayer_) {
      pAdaptLayer_->AddAttribute<SuriObject::UuidType>(ProcessAdaptLayer::LibraryIdKeyAttr,
                                             libraryId_);
      pAdaptLayer_->AddAttribute<bool>(ReprojectionProcess::ReprojectionKeyAtttr, reprojec_);
      pAdaptLayer_->AddAttribute<bool>(ReprojectionProcess::AskGcpFileAttr, askGcpFile_);
      pAdaptLayer_->AddAttribute<bool>(ReprojectionProcess::ShowResizePartAttr, showResizePart_);
      pAdaptLayer_->AddAttribute<ReprojectionParameters*>(
            ReprojectionProcess::ReprojectionParamsKeyAttr, pReprojectionParameters_);
      pAdaptLayer_->AddAttribute<World*>(
               ProcessAdaptLayer::OutputWorldKeyAttr, pOutputWorld_);
   }
}   

} /** namespace suri */
