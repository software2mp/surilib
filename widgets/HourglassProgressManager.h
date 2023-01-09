/*! \file HourglassProgressManager.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
