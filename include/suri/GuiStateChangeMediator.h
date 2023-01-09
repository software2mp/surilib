/** \file GuiStateChangeMediator.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GUISTATECHANGEMEDIATOR_H_
#define GUISTATECHANGEMEDIATOR_H_

// Includes stadard
#include <map>
#include <string>

// Includes suri
#include "suri/GuiStateInterface.h"
#include "suri/ObserverInterface.h"
#include "suri/Command.h"

namespace suri {

class Widget;

/**
 * Recibe eventos y modifica el estado de la aplicacion
 */
class GuiStateChangeMediator : public ObserverInterface {
public:
   /** Ctor */
   GuiStateChangeMediator(Widget *pMainWidget);
   /** Dtor */
   virtual ~GuiStateChangeMediator();

   /** Configura el estado del mediador */
   void SetState(GuiStateInterface* pGuiStateInterface);

   /** Elimina todos los commands registrados. */
   void ResetCommandList();
   /** Elimina todos los commands registrados. */
   void RemoveCommand(const std::string &CommandId);
   /** Registra un comando en el tool indicado. */
   void RegisterCommand(const std::string &CommandGuiId, Command* pCommand);
   /** Actualiza estado de la aplicacion a partir del evento */
   virtual bool StateChangeEvent(const std::string EventId,
                                       const std::string SourceId);

   /** Modifica el estado de habilitacion de la herramienta */
   virtual bool EnableCommand(const std::string CommandId, bool NewState);
   /** Modifica el estado de activacion de la herramienta */
   virtual bool ActivateCommand(const std::string CommandId, bool NewState);


   // --- Metodos auxiliares - mantienen la interfaz grafica consistente ---
   /** Actualiza el estado de activacion de la herramienta en la interfaz grafica */
   virtual bool UpdateCommandGuiState(const std::string CommandId);
   /** Modifica el estado de activacion de la herramienta */
   virtual bool DesactivateConflictingCommands(const std::string CommandId);

   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification);

   // Eventos
   static const std::string StartTask;
   static const std::string EndTask;
   static const std::string Initialize;
   static const std::string Activate;
   static const std::string Deactivate;
   static const std::string DeleteCommand;

   // Sources
   static const std::string VectorEditionTask;
   static const std::string ClassificationTask;
   static const std::string CoregisterTask;
   static const std::string GeoreferenceTask;
   static const std::string AnimationTask;
   static const std::string MapEditionTask;

private:
   /** Modifica estado del item en menu en caso de existir. */
   virtual bool EnableCommandMenuItem(const std::string CommandLocations,
                                                             bool NewState);
   /** Modifica estado del item en menu en caso de existir. */
   virtual bool ActivateCommandMenuItem(const std::string CommandLocations,
                                                             bool NewState);

   /** Mapa con los comandos registrados */
   typedef std::map<std::string, Command*> CommandMap;
   CommandMap commands_;
   /** Estado de la interfaz. Clase con logica que responde a un evento */
   GuiStateInterface* pGuiStateInterface_;
   /** Widget de la ventana principal */
   Widget *pMainWidget_;
};

} /* namespace suri */
#endif /* GUISTATECHANGEMEDIATOR_H_ */
