/*! \file FileRenderization.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILERENDERIZATION_H_
#define FILERENDERIZATION_H_

/**
 *
 */

// Includes estandar
#include <string>

// Includes Suri
#include "suri/Option.h"
#include "MovingWindowController.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Controlador para la renderizacion a un archivo */
/**
 * Salva un canvas a un archivo usando el nombre de archivo y las opciones
 * recibidas.
 */
class FileRenderization : public MovingWindowController {
public:
   /** Ctor */
   FileRenderization();
   /** Dtor */
   virtual ~FileRenderization();
   /** Modifica el mundo de manera de recorrer el extent completo y asi renderizar */
   virtual bool Render();
   /** Setter del nombre del archivo de salida */
   void SetFilename(const std::string &Filename);
   /** Setter de las opciones de imagen de salida */
   void SetOption(const Option& Options);
   void SetFeathering(const Subset& Intersection, const std::vector<void*>& Data,
                                                       unsigned char NoDataValue);
   void SetNoDataValue(unsigned char Value) { noDataValue_ = Value;}
protected:
private:
   std::string fileName_; /*! Nombre del archivo de salida */
   Option option_; /*! Opciones de imagen de la salida */
   Subset intersection_;
   unsigned char noDataValue_;
   std::vector<void*> featheringData_;
};
}

#endif /* FILERENDERIZATION_H_ */
