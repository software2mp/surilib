/*! \file Progress.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/Progress.h"

// Include suri
#include "suri/ProgressManager.h"

// Includes wx
#include "wx/wx.h"

/** namespace suri */
namespace suri {
/**
 * Ctor. Pide un progressId a ProgressManager
 * @param[in] Cycles cantidad de ciclos que necesita la tarea
 * @param[in] Description descripcion de la tarea
 */
Progress::Progress(int Cycles, const std::string &Description) {
   progressId_ = ProgressManager::GetInstance()->GetProgressId(Cycles, Description);
}

/** Dtor. Libera al ProgressManager */
Progress::~Progress() {
   ProgressManager::GetInstance()->ReleaseProgressId(progressId_);
}

/**
 * realiza tick sobre ProgressManager
 * @return informa si tuvo exito.
 */
bool Progress::Update() {
   return ProgressManager::GetInstance()->Tick(progressId_);
}
}
