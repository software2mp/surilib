/*! \file FileVectorExporterProcessPartProvider.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "FileVectorExporterProcessPartProvider.h"
#include "VectorFormatSelectionPart.h"
#include "TransformationModelParametersPart.h"
#include "suri/DataViewManager.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** ctor **/
FileVectorExporterProcessPartProvider::FileVectorExporterProcessPartProvider(
      ProcessAdaptLayer* pPal) :
      VectorProcessPartProvider(pPal) {
}

/** Destructor **/
FileVectorExporterProcessPartProvider::~FileVectorExporterProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool FileVectorExporterProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   bool ok = VectorProcessPartProvider::GetParts(ProcessParts);
   if (ok) {
      DataViewManager* pdvmgr = NULL;
      pPal_->GetAttribute<DataViewManager*>(ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
      bool srslection = false;
      pPal_->GetAttribute<bool>(ProcessAdaptLayer::IncludeTransformationParametersKeyAttr, srslection);
      if (srslection) {
         std::string spatianreferencein;
         pPal_->GetAttribute<std::string>(suri::TransformationFactory::kParamWktIn, spatianreferencein);
         std::string spatianreferenceout;
         pPal_->GetAttribute<std::string>(suri::TransformationFactory::kParamWktOut, spatianreferenceout);
         ReprojectionProcess::ReprojectionParameters* preprojprocess =
               new ReprojectionProcess::ReprojectionParameters;
         preprojprocess->srWktIn_ = spatianreferencein;
         preprojprocess->srWktOut_ = spatianreferenceout;
         SuriObject::UuidType libraryid = SuriObject::NullClassId;
         pPal_->GetAttribute<SuriObject::UuidType>(ProcessAdaptLayer::LibraryIdKeyAttr, libraryid);
         TransformationModelParametersPart* ptransf = 
                              new TransformationModelParametersPart(libraryid,
                              pdvmgr->GetLibraryManager(), 
                              preprojprocess,
                              TransformationModelParametersPart::Exact);
         ptransf->SetProcessLayer(pPal_);
         ProcessParts.push_back(ptransf);

      }
      VectorFormatSelectionPart* pformat = new VectorFormatSelectionPart();
      pformat->SetProcessLayer(pPal_);
      ProcessParts.push_back(pformat);
   }
   return ok;
}

/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* FileVectorExporterProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   FileVectorExporterProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("FileVectorExporterProcess") == 0)
      pprovider = new FileVectorExporterProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
