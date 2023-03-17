/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
