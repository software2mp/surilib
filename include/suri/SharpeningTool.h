/*! \file SharpeningTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SHARPENINGTOOL_H_
#define SHARPENINGTOOL_H_

#include <string>

#include "GenericTool.h"

namespace suri {

/**
 * Herramienta para ejecucion mejoras de definicion de imagenes.
 */
class SharpeningTool : public suri::GenericTool {
public:
   /**
    * Ctor.
    */
   explicit SharpeningTool(DataViewManager* pDataViewManager);

   /**
    * Dtor.
    */
   virtual ~SharpeningTool();

   /**
    * Ejecuta un comando (ToolInterface::Execute)
    */
   virtual void Execute(const Command* pToolCommand);

protected:
   /**
    * Metodo para la creacion de comandos.
    */
   virtual Command* CreateCommand(const std::string& CommandName);

   /**
    * Metodo para la destruccion de comandos.
    */
   virtual void DestroyCommand(Command*& pCommand) const;

   /**
    * Metodo para la creacion de widgets de la herramienta.
    */
   virtual Widget* CreateWidget(const std::string& Widget);

private:
   /**
    * Ejecuta el procesos de fusion de bandas con pancromatica (HSV por el momento)
    */
   void ExecuteSharpening();
};

}  // namespace suri

#endif  // SHARPENINGTOOL_H_
