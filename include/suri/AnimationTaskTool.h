/*! \file AnimationTaskTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ANIMATIONTASKTOOL_H_
#define ANIMATIONTASKTOOL_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/GenericTool.h"
#include "suri/Command.h"
#include "AnimationTask.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// Forwards
class TaskConfig;

/*!
 *  Clase que representa las herramientas disponibles para la tarea de animacion
 */
class AnimationTaskTool : public suri::GenericTool {
   /** Constructor copia */
   AnimationTaskTool(const AnimationTaskTool&);

public:
   AnimationTaskTool(DataViewManager* pDataViewManager, wxWindow* pParent);
   virtual ~AnimationTaskTool();
   /** Ejecucion del comando */
   virtual void Execute(const Command* pToolCommand);

protected:
   /** Creacion del Comando */
   virtual Command* CreateCommand(const std::string &CommandName);
   /** Destruccion del Comando */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Creacion del Widget */
   virtual Widget* CreateWidget(const std::string &Widget);
   /** Inicia la tarea de edicion de mapa */
   bool StartTask();
   /** Finaliza la tarea de edicion de mapa **/
   bool EndTask();
   /** Avanza un frame hacia atras**/
   void ExecuteBackwardAnimation();
   /** Avanza un frame hacia adelante **/
   void ExecuteForwardAnimation();
   /** Inicia/frena una animacion **/
   void ExecuteAnimate(bool Start = true);
   /** Indica la velocidad de animacion **/
   void ExecuteSetFrameRate(int Rate);
   /** Metodo que se ejecuta cuando se apreta boton de iniciar/finalizar tarea */
   void ExecuteTaskMenu(bool Active);

private:
   /** Ventana para renderizar el mapa */
   wxWindow* pParent_;
   /** Referencia a la tarea */
   AnimationTask* pAnimTask_;
   /** Configurador para la tarea de mapa */
   TaskConfig* pTaskConfig_;
};

} /** namespace suri */

#endif /* ANIMATIONCREATIONTOOL_H_ */
