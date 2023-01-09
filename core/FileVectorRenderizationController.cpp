/*! \file FileVectorRenderizationController.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/FileVectorRenderizationController.h"
#include "suri/Subset.h"
#include "suri/World.h"
#include "suri/FileVectorCanvas.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
FileVectorRenderizationController::FileVectorRenderizationController() {
}

/** Destructor **/
FileVectorRenderizationController::~FileVectorRenderizationController() {
}

/** Modifica el mundo de manera de recorrer el extent completo y asi renderizar */
bool FileVectorRenderizationController::Render() {
//   if (!pWorld_)
//      return false;
   if (!pRenderizationList_ || fileName_.empty()) {
      return false;
   }
   Subset subset;
   pWorld_->GetWindow(subset);
   FileVectorCanvas* pcanvas = new FileVectorCanvas(pRenderizationList_, subset, 
                              pWorld_->GetSpatialReference(), fileName_);
   if (pcanvas) {
      SetOutputCanvas(pcanvas);
   }
   return VectorRenderizationController::Render();
}
/**
 * Setter del nombre del archivo de salida
 * @param[in] Filename Nombre del archivo de salida
 */
void FileVectorRenderizationController::SetFilename(const std::string &Filename) {
   fileName_ = Filename;
}

/**
 * Setter de las opciones de imagen de salida
 * @param[in] Option opciones de imagen
 */
void FileVectorRenderizationController::SetOption(const Option& Options) {
   option_ = Options;
}

} /** namespace suri */
