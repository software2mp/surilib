/*! \file PrincipalComponentAnalysisTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PRINCIPALCOMPONENTANALYSISTOOL_H_
#define PRINCIPALCOMPONENTANALYSISTOOL_H_

#include <string>

#include "suri/GenericTool.h"

namespace suri {

class PrincipalComponentAnalysisTool : public GenericTool {
public:
   /**
    * Ctor.
    */
   explicit PrincipalComponentAnalysisTool(DataViewManager* pDataViewManager);

   /**
    * Dtor.
    */
   virtual ~PrincipalComponentAnalysisTool();

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
    * Ejecuta el proceso de analisis de componentes principales.
    */
   void ExecutePCAnalysis();
};

}  // namespace suri

#endif  // PRINCIPALCOMPONENTANALYSISTOOL_H_
