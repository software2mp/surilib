/*! \file ReprojectionProcessPartProvider.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "ReprojectionProcessPartProvider.h"
#include "TransformationModelParametersPart.h"
#include "RasterOutputSizePart.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/DataViewManager.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
ReprojectionProcessPartProvider::ReprojectionProcessPartProvider(
      ProcessAdaptLayer* pPal) :
      FileExporterProcessPartProvider(pPal) {
   pReprojectionParameters_ = NULL;

   if (pPal) {
      pPal->GetAttribute<bool>(ReprojectionProcess::ReprojectionKeyAtttr, reprojection_);
      pPal->GetAttribute<bool>(ReprojectionProcess::AskGcpFileAttr, askGcpFile_);
      pPal->GetAttribute<bool>(ReprojectionProcess::ShowResizePartAttr, showResizePart_);
      pPal->GetAttribute<ReprojectionProcess::ReprojectionParameters*>(
            ReprojectionProcess::ReprojectionParamsKeyAttr, pReprojectionParameters_);
   }
}

/** Destructor **/
ReprojectionProcessPartProvider::~ReprojectionProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool ReprojectionProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   DataViewManager* pdvmgr = NULL;
   bool datafound = pPal_->GetAttribute<DataViewManager*>(
         ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   SuriObject::UuidType libraryid = SuriObject::NullUuid;
   bool libfound = pPal_->GetAttribute<SuriObject::UuidType>(
         ProcessAdaptLayer::LibraryIdKeyAttr, libraryid);
   World* pworld = NULL;
   bool inworldfound = pPal_->GetAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr,
                                                   pworld);
   World* poutputworld = NULL;
   bool outworldfound = pPal_->GetAttribute<World*>(
         ProcessAdaptLayer::OutputWorldKeyAttr, poutputworld);
   if (!datafound || !libfound || !inworldfound || !outworldfound)
      return false;

   TransformationModelParametersPart* ptransfpart = NULL;
   if (reprojection_) {
      int flags = !askGcpFile_ ? 
      TransformationModelParametersPart::Polynomial | TransformationModelParametersPart::Exact : 
      TransformationModelParametersPart::GcpFile | TransformationModelParametersPart::Polynomial;
      ptransfpart = new TransformationModelParametersPart(libraryid,
                                                          pdvmgr->GetLibraryManager(),
                                                          pReprojectionParameters_,
                                                          flags);
      ptransfpart->SetProcessLayer(pPal_);
      ProcessParts.push_back(ptransfpart);
   }

   bool bval = false;
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr, bval);

   if (!FileExporterProcessPartProvider::GetParts(ProcessParts)) return false;

   for (int ix = 0, lenix = ProcessParts.size(); ix <lenix; ++ix) {
      ProcessAtributeProvider* pprov = dynamic_cast<ProcessAtributeProvider*>(ProcessParts[ix]);
      if (pprov != NULL && pprov->GetProcessLayer() == NULL) {
         pprov->SetProcessLayer(pPal_);
      }
   }
   if (showResizePart_){
	   RasterOutputSizePart* prasteroutpart = new RasterOutputSizePart(
	   		pReprojectionParameters_, pworld, poutputworld);
	   prasteroutpart->SetProcessLayer(pPal_);
	   if (ptransfpart != NULL)
	   	ptransfpart->Attach(prasteroutpart);
	   ProcessParts.insert(ProcessParts.end() - 1, prasteroutpart);
   }
   return true;
}

/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* ReprojectionProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   ReprojectionProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("ReprojectionProcess") == 0 || ProcessName.compare("GeoreferenceProcess") == 0)
      pprovider = new ReprojectionProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
