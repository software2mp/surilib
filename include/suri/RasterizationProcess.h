/*! \file RasterizationProcess.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef RASTERIZATIONPROCESS_H_
#define RASTERIZATIONPROCESS_H_

// Includes standard
#include <vector>

// Includes suri
#include "FileExporterProcess.h"
#include "ClassInformation.h"

/** namespace suri */
namespace suri {
// Forward
class DatasourceManagerInterface;

class RasterizationProcess: public FileExporterProcess {

public:
   /** Ctor de copia */
   RasterizationProcess(RasterizationProcess &);
   /** Constructor */
   RasterizationProcess(std::vector<Element*> InputElements, const Subset &WindowSubset,
                        DataViewManager* pDataViewManager = NULL,
                        bool ShowSpectralSelectionPart = false);
   /** Dtor */
   virtual ~RasterizationProcess();
   /** Nombre del proceso **/
   static const std::string kProcessName;

   virtual World *GetOutputWorld();

   bool  ConfigureProcess();
   bool RunProcess();
protected:
   // ------ Metodos que permiten configurar el proceso ------
   /** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso**/
   virtual void DoOnProcessAdaptLayerSet();
private:

};
}

#endif /* RASTERIZATIONPROCESS_H_ */
