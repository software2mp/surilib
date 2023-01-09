/*! \file FileVectorRenderizationController.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILEVECTORRENDERIZATIONCONTROLLER_H_
#define FILEVECTORRENDERIZATIONCONTROLLER_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "VectorRenderizationController.h"
#include "Option.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Controlador para la renderizacion a un archivo de una capa vectorial
 */
class FileVectorRenderizationController : public VectorRenderizationController {
public:
   /** ctor **/
   FileVectorRenderizationController();
   /** dtor **/
   virtual ~FileVectorRenderizationController();
   /** Modifica el mundo de manera de recorrer el extent completo y asi renderizar */
   virtual bool Render();
   /** Setter del nombre del archivo de salida */
   void SetFilename(const std::string &Filename);
   /** Setter de las opciones de imagen de salida */
   void SetOption(const Option& Options);

private:
   std::string fileName_; /*! Nombre del archivo de salida */
   Option option_; /*! Opciones de imagen de la salida */
};

} /** namespace suri */

#endif /* FILEVECTORRENDERIZATIONCONTROLLER_H_ */
