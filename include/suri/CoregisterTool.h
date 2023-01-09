/** \file CorregisterTool.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CORREGISTERTOOL_H_
#define CORREGISTERTOOL_H_

// Includes suri
#include "suri/GenericTool.h"

namespace suri {

//fordwards
class CoregisterTask;

/** Tool con los botones de corregistro */
class CoregisterTool : public GenericTool {
public:
   /** Ctor */
   CoregisterTool(DataViewManager* pDataViewManager);
   /** Dtor */
   virtual ~CoregisterTool();

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
   /** Inicia la tarea de coregistro */
   bool StartTask(const std::string &FileName);

   /** Crea una capa vectorial en memoria de tipo correcto */
   void ExecuteNewGcpFile();
   /** Genera un archivo GCP con puntos autodetectados */
   void ExecuteAutoGenerateGcpFile();
   /** Abre un archivo de gcp existente e inicia la tarea de toma de puntos**/
   void ExecuteOpenGcpFile();
   /** Inicia la tarea de edicion vectorial con vector seleccionado */
   void ExecuteStartTask();
   /** Finaliza la tarea de edicion vectorial */
   void ExecuteEndTask();

   /** Finaliza la edicion grafica de features, no finaliza la tarea. */
   void ExecuteEndAddGcps() const;
   /**
    * Flag que indica si mostrar las opciones de configuracion para el
    * algoritmo de generacion de gcps
    */
   bool gcpAutoGenerateEnabled_;

   /** Tarea de corregistro */
   CoregisterTask* pCoregisterTask_;
   /** Comando asociado a la tarea de edicion vectorial */
   Command* pTaskCommand_;
   /** TaskConfig que permite configurar el Layout */
   suri::TaskConfig *pTaskConfig_;
};

} /* namespace suri */
#endif /* CORREGISTERTOOL_H_ */
