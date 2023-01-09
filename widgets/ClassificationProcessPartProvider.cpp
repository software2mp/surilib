/*! \file ClassificationProcessPartProvider.cpp */
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
#include "ClassificationProcessPartProvider.h"
#include "ClassificationPart.h"
#include "ParametricClassificationPart.h"
#include "KMeansPart.h"
#include "ClassFussionParametersPart.h"
#include "suri/ClassificationProcess.h"
#include "ClassificationGetter.h"
#include "ThresholdClassSelectionPart.h"
#include "SpectralAngleMapperPart.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** Ctor **/
ClassificationProcessPartProvider::ClassificationProcessPartProvider(
      ProcessAdaptLayer* pPal) : FileExporterProcessPartProvider(pPal) {
   if (pPal) {
      pPal->GetAttribute<std::string>(ClassificationProcess::ClassificationType,
                                      classificationType_);
   }
}

/** Dtor **/
ClassificationProcessPartProvider::~ClassificationProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool ClassificationProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   ClassificationPart* pclasifpart = GetClassificationPart();
   if (!pclasifpart)
      return false;
   ClassificationGetter* pgetter = pclasifpart;
   pPal_->AddAttribute<ClassificationGetter*>(
         ClassificationProcess::ClassGetterAttrKey, pgetter);
   pclasifpart->SetProcessLayer(pPal_);
   if (!FileExporterProcessPartProvider::GetParts(ProcessParts))
      return false;
   std::vector<Part*>::iterator it = ProcessParts.begin();
   ProcessParts.insert(it, pclasifpart);
   return true;
}

/** En funcion de los parametros de entrada selecciona el part de clasificacion acorde**/
ClassificationPart* ClassificationProcessPartProvider::GetClassificationPart() {
   DataViewManager* pdvmgr = NULL;
   bool datafound = pPal_->GetAttribute<DataViewManager*>(
         ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   std::vector<Element*> inputelements;
   bool elemsfound = pPal_->GetAttribute< std::vector<Element*> >(
         ProcessAdaptLayer::InputElementsKeyAttr, inputelements);
   LayerList* playerlist = NULL;
   bool listfound = pPal_->GetAttribute<LayerList*>(
         ProcessAdaptLayer::InputElementsKeyAttr, playerlist);

   if (!datafound || !elemsfound || !listfound)
      return NULL;
   ClassificationPart* pclasifpart = NULL;
   if (classificationType_ == "ParametricClassificationPart") {
      pclasifpart = new ParametricClassificationPart(
            playerlist, inputelements[0], pdvmgr,
            ParametricClassificationPart::Supervised);
   } else if (classificationType_ == "KMeansPart") {
      // Clasificacion no supervisada correspondiente a la version 1.1.0
      pclasifpart = new KMeansPart(inputelements[0]);
   } else if (classificationType_ == "ClassFussionParametersPart") {
      pclasifpart = new ClassFussionParametersPart(inputelements[0]);
   } else if (classificationType_ == "ThresholdClassification") {
      pclasifpart = new ThresholdClassSelectionPart(pdvmgr, inputelements[0]);
   } else if (classificationType_ == "SpectralAngleMapper") {
      pclasifpart = new SpectralAngleMapperPart(pdvmgr->GetLibraryManager());
   }
   return pclasifpart;
}

/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* ClassificationProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   ClassificationProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("ClassificationProcess") == 0 ||
         ProcessName.compare("KMeansClassificationProcess") == 0)
      pprovider = new ClassificationProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
