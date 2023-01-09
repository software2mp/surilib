/*! \file AnimationTask.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ANIMATIONTASK_H_
#define ANIMATIONTASK_H_

// Includes Estandar
#include <string>
#include <list>

// Includes Suri
#include "suri/TaskInterface.h"
#include "wxmacros.h"
#include "suri/NodePath.h"

// Includes Wx
// Defines
// forwards
class wxTimer;
class wxWindow;

namespace suri {
// forwards
class LayerList;
class DataViewManager;
/** Macro para declarar el objeto de los eventos */
class AnimationTaskEvent;
class Element;

/*!
 * Clase que representa una tarea de animacion
 */
class AnimationTask : public suri::TaskInterface {
   /** Constructor copia **/
   AnimationTask(const AnimationTask&);

public:
   /** Constructor **/
   AnimationTask(wxWindow* pParent, DataViewManager* pDataManager, int ListGroup = 6);
   /** Destructor **/
   virtual ~AnimationTask();
   /** Comienza la tarea */
   virtual bool Start();
   /** Finaliza la tarea. Intenta guardar los cambios. */
   virtual bool End();
   /** Cancela la tarea sin guardar */
   virtual bool Cancel();
   /** Indica si la tarea esta activa */
   virtual bool IsActive() const;
   /** Avanza un frame hacia atras**/
   bool BackwardAnimation();
   /** Avanza un frame hacia adelante **/
   bool ForwardAnimation();
   /** Inicia/frena una animacion **/
   bool Animate(bool Start = true);
   /** Indica la velocidad de animacion **/
   bool SetFrameRate(int Rate);
   /** Nombre de la tarea */
   static const std::string TASKNAME;

private:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(AnimationTaskEvent);
   /*! Objeto para conectar eventos */
   EVENT_OBJECT_PROPERTY(AnimationTaskEvent);
   /** Evento OnTimer que cambia la el elemento activo */
   void OnTimer();
   /** Obtiene el elemento a mostrar **/
   Element* GetNextElement();
   /** Muestra el frame que corresponde en el instante de animacion **/
   void ShowNextFrame();
   /** Configura el bloqueo las fuente de datos involucradas en la tarea**/
   void ConfigureBlocking(bool Block = true);
   /** bool que indica si la tarea se encuentra iniciada**/
   bool started_;
   /** Administrador de informacion de la aplicacion **/
   DataViewManager* pDataManager_;
   /*! Timer utilizado para el evento OnSize */
   wxTimer *pAnimationTimer_;
   /** frames por segundo que se utiliza para la animacion **/
   int framesPerSecond_;
   /** frame en que se encuentra la animacion **/
   int currentFrame_;
   /** Referencia al elemento al que corresponde el frame de animacion */
   Element* pCurrentElement_;
   /** bool que indica si se debe frenar el timer **/
   bool stop_;
   /** Grupo sobre el cual se desea animar **/
   int group_;
   /** bool que indica si se encuentra renderizando para saltear el timer **/
   bool rendering_;
   /** lista de nodos seleccionados al iniciar animacion **/
   std::list<NodePath> selectlist_;
   wxWindow* pParent_;
};

} /** namespace suri */

#endif /* ANIMATIONTASK_H_ */
