/*! \file StatisticsTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef STATISTICSTOOL_H_
#define STATISTICSTOOL_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/GenericTool.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Ejecuta los comandos correspondientes a las tareas de Estadisticas
 */
class StatisticsTool : public suri::GenericTool {
public:
   /** Constructor */
   explicit StatisticsTool(DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~StatisticsTool();
   /** Ejecucion del comando */
   virtual void Execute(const Command* pToolCommand);

protected:
   /** Creacion del Comando */
   virtual Command* CreateCommand(const std::string &CommandName);
   /** Destruccion del Comando */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Creacion del Widget */
   virtual Widget* CreateWidget(const std::string &Widget);

private:
   /** Despliegue el widget de estadisticas globales */
   void ExecuteGlobalStatistics() const;
   /** Despliegue el widget de estadisticas de clase */
   void ExecuteClassStatistics() const;
   /** Despliegue el widget de exportador de estadisticas de clase */
   void ExecuteClassStatisticsExporter() const;
};

} /** namespace suri */

#endif /* STATISTICSTOOL_H_ */
