/** \file ReprojectionProcess2.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef REPROJECTIONPROCESS2_H_
#define REPROJECTIONPROCESS2_H_

#include <string>
#include <vector>

#include "suri/ProcessInterface.h"
#include "GeoreferenceProcess.h"
#include "suri/Element.h"
#include "suri/Subset.h"
#include "suri/DataViewManager.h"
#include "ReprojectionProcess.h"
#include "BandInfo.h"
#include "suri/Subset.h"

namespace suri {

/**
 *
 */
class ReprojectionProcess2 : public GeoreferenceProcess {
public:
   static const std::string kProcessName;
   static const std::string kOutputElementType;

   /**
    * Ctor.
    */
   ReprojectionProcess2(Element* pInputElement, const Subset& ViewportSubset,
                        Element*& pOutputElement, const SuriObject::UuidType& LibraryId,
                        DataViewManager* pDataViewManager);

   /**
    * Dtor
    */
   virtual ~ReprojectionProcess2();

   /**
    * Corre el proceso y genera la salida usando los metodos de configuracion.
    */
   virtual bool RunProcess();
};

}  // namespace suri

#endif  // REPROJECTIONPROCESS2_H_
