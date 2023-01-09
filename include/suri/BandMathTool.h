/*! \file BandMathTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef BANDMATHTOOL_H_
#define BANDMATHTOOL_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "GenericTool.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Ejecuta los comandos correspondientes al Algebra de Bandas
 */
class BandMathTool : public suri::GenericTool {
public:
   /** Constructor */
   explicit BandMathTool(DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~BandMathTool();
   /** Ejecucion del comando */
   virtual void Execute(const Command* pToolCommand);

protected:
   /** Creacion del Comando */
   virtual Command *CreateCommand(const std::string &CommandName);
   /** Destruccion del Comando */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Creacion del Widget */
   virtual Widget *CreateWidget(const std::string &Widget);

private:
   /** Despliega el widget para aplicar filtros */
   void ExecuteApplyFilter() const;
   /** Despliega el widget de indices */
   void ExecuteIndexes() const;
   /** Despliega el widget de algebra de bandas */
   void ExecuteBandMath() const;
};

} /** namespace suri */

#endif /* BANDMATHTOOL_H_ */
