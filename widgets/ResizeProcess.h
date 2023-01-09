/*! \file ResizeProcess.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIB_SURI_WIDGETS_RESIZEPROCESS_H_
#define LIB_SURI_WIDGETS_RESIZEPROCESS_H_


// Includes Estandar
#include <string>
// Includes Suri
#include "suri/RasterProcess.h"
#include "ReprojectionProcess.h"
// Includes Wx
// Defines
// forwards

class GDALRasterBand;

namespace suri {
class ResizeProcess:public suri::ReprojectionProcess {
protected:
      void Resize(const std::string& SrcFilename,
                  const std::string& DestFilename,
                  World* pWorld);

public:
    ResizeProcess(suri::Element* pInputElement, const suri::Subset& ViewportSubset,
                         const SuriObject::UuidType& LibraryId,
                         DataViewManager* pDataViewManager,
                         bool AskGcpFile = false, bool Reproject = true);
    virtual ~ResizeProcess() {}

    /** Ctor. de Copia. */
    ResizeProcess(const ResizeProcess &);
    bool RunProcess();
};
}
#endif /* LIB_SURI_WIDGETS_RESIZEPROCESS_H_ */
