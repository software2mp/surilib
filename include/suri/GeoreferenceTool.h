/** \file GeoreferenceTool.h
 *
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOREFERENCETOOL_H_
#define GEOREFERENCETOOL_H_

#include <string>

#include "suri/GenericTool.h"

namespace suri {
   
class DataViewManager;
class Command;
class Widget;
class TaskConfig;
class GeoreferenceTask;

/**
 * 
 */
class GeoreferenceTool : public GenericTool {
public:

   /**
    * Ctor.
    */
   explicit GeoreferenceTool(DataViewManager* pDataViewManager);
   
   /**
    * Dtor
    */ 
   virtual ~GeoreferenceTool();

   /**
    * Ejecucion del comando
    */
   virtual void Execute(const Command* pToolCommand);

protected:
   /**
    * Creacion del Comando
    */
   virtual Command* CreateCommand(const std::string& CommandName);

   /**
    * Destruccion del Comando
    */
   virtual void DestroyCommand(Command*& pCommand) const;

   /**
    * Creacion del Widget
    */
   virtual Widget* CreateWidget(const std::string& Widget);

private:
   GeoreferenceTask* pGeoreferenceTask_;  // Tarea de georreferenciacion.
   Command* pTaskCommand_;  // Comando asociado a la tarea de edicion vectorial
   TaskConfig* pTaskConfig_;  // TaskConfig que permite configurar el Layout

   /**
    * Inicia la tarea de georreferenciacion
    */
   bool StartTask(const std::string &FileName);

   /**
    * Crea una capa vectorial en memoria de tipo correcto
    */
   void ExecuteNewGcpFile();
   /** Abre un archivo de gcp existente e inicia la tarea de toma de puntos**/
   void ExecuteOpenGcpFile();
   /**
    * Inicia la tarea de edicion vectorial con vector seleccionado
    */
   void ExecuteStartTask();

   /**
    * Finaliza la tarea de edicion vectorial
    */
   void ExecuteEndTask();

   /**
    * Finaliza la edicion grafica de features, no finaliza la tarea.
    */
   void ExecuteEndAddGcps() const;
};

}  // namespace suri

#endif  // GEOREFERENCETOOL_H_
