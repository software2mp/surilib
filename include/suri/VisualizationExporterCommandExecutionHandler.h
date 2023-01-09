/*! \file VisualizationExporterCommandExecutionHandler.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VISUALIZATIONEXPORTERCOMMANDEXECUTIONHANDLER_H_
#define VISUALIZATIONEXPORTERCOMMANDEXECUTIONHANDLER_H_

// Includes Suri
#include "suri/CommandExecutionHandlerInterface.h"

namespace suri {

// forwards
class DataViewManager;
class ProcessAdaptLayer;
class RasterElement;
class VectorElement;
class Element;

namespace core {

/** Clase encargada de administrar la ejecucion del comando VisualizationExporterProcess **/
class VisualizationExporterCommandExecutionHandler : public CommandExecutionHandlerInterface {
   /* Constructor Copia */
   VisualizationExporterCommandExecutionHandler(const VisualizationExporterCommandExecutionHandler&);
public:
   /** Constructor **/
   VisualizationExporterCommandExecutionHandler(DataViewManager* pDataView);
   /** Destructor **/
   virtual ~VisualizationExporterCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    * @return true en caso de poder manejar la ejecucion del comando
    * @return false en caso contrario
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   /** Configura atributos del proceso a traves del pal */
   void SetAttributes(RasterElement* pRaster, ProcessAdaptLayer &Pal);
   /** Recorre los elementos y agrega a un vector los que se encuentran activos */
   std::vector<Element*> GetInputElements();
   DataViewManager* pDataView_;
};

} /** namespace core */
} /** namespace suri */

#endif /* VISUALIZATIONEXPORTERCOMMANDEXECUTIONHANDLER_H_ */
