/*! \file LibraryTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYTOOL_H_
#define LIBRARYTOOL_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "GenericTool.h"
#include "suri/Command.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Comandos de Biblioteca
 */
class LibraryTool : public GenericTool {
   /** ctor copia **/
   LibraryTool(const LibraryTool&);

public:
   /** ctor **/
   explicit LibraryTool(DataViewManager* pDataViewManager);
   /** dtor **/
   virtual ~LibraryTool();
   /** Ejecucion del comando */
   virtual void Execute(const Command* pToolCommand);

private:
   /** Metodo auxiliar que inicializa el widget full fledged con la biblioteca
    *  correspondiente al command*/
   void ExecuteLibrary(const std::string& LibraryCommandName) const;
   /** */
   virtual Command *CreateCommand(const std::string &CommandName);
   /** */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** */
   virtual Widget *CreateWidget(const std::string &Widget);
};

} /** namespace suri */

#endif /* LIBRARYTOOL_H_ */
