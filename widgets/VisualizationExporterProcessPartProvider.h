/*! \file VisualizationExporterProcessPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VISUALIZATIONEXPORTERPROCESSPARTPROVIDER_H_
#define VISUALIZATIONEXPORTERPROCESSPARTPROVIDER_H_

// Includes Suri
#include "FileExporterProcessPartProvider.h"
#include "ReprojectionProcess.h"

namespace suri {

/**
 * Clase encargada de proveer los parts que configuran un proceso que exporta
 * el resultado a un archivo
 */
class VisualizationExporterProcessPartProvider : public FileExporterProcessPartProvider {
   /** Constructor copia */
   VisualizationExporterProcessPartProvider(const VisualizationExporterProcessPartProvider&);
   /** Constructor */
   explicit VisualizationExporterProcessPartProvider(ProcessAdaptLayer* pPal);

public:
   /**
    * Metodo para la creacion de un Part provider
    * @return instancia del creador correspondiente
    * @return NULL en caso contrario
    */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);
   /** Destructor */
   virtual ~VisualizationExporterProcessPartProvider();
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);

private:
   /** Parametros de reproyeccion */
   ReprojectionProcess::ReprojectionParameters* pReprojectionParameters_;
};

} /** namespace suri */

#endif /* VISUALIZATIONEXPORTERPROCESSPARTPROVIDER_H_ */
