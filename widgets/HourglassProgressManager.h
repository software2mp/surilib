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

#ifndef HOURGLASSPROGRESSMANAGER_H_
#define HOURGLASSPROGRESSMANAGER_H_

// Includes suri
#include "suri/ProgressManager.h"

// Includes wx
#include "wx/wx.h"

// Forwards
class wxProgressDialog;
class wxGauge;

// Defines
#define __USE_PROGRESSBAR__

/** namespace suri */
namespace suri {
// Forwards
class ProgressWidget;

/** ProgressManager que pone reloj(mouse) y bloquea aplicacion cuando se registra una tarea */
/**
 * Cuando se registra una tarea pone el reloj, bloquea la aplicacion e inicia
 * ProgressWidget hasta que se libere.
 * Se pueden registrar varias tareas en paralelo
 * Para informar que avanzo una tarea se llama a Update.
 * \note establece las acciones que realiza el ProgressManager cuando
 * tiene tareas asignadas.
 */
class HourglassProgressManager : public ProgressManager {
   /** Ctor. de Copia. */
   HourglassProgressManager(const HourglassProgressManager &HourglassProgressManager);

public:
   /** Ctor. */
   HourglassProgressManager();
   /** Dtor */
   virtual ~HourglassProgressManager();
   /** Pone el cursor como relog y desactiva las ventanas de la aplicacion */
   virtual int GetProgressId(int Cycles, const std::string &Description);
   /** Si no quedan procesos pendientes activa las ventanas de la aplicacion y muestra flecha */
   virtual bool ReleaseProgressId(int ProgressId);
   /** Hace un yiald */
   virtual void Update(int ProgressId) const;
protected:
private:
#ifdef __USE_PROGRESSBAR__
   wxTimer timer_; /*! Timer para activar la barra de progreso */
   ProgressWidget *pProgressWidget_; /*! Dialogo de progreso */
#endif
};
}

#endif /*HOURGLASSPROGRESSMANAGER_H_*/
