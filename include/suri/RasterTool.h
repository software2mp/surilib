/*! \file RasterTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERTOOL_H_
#define RASTERTOOL_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "GenericTool.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Ejecuta los comandos relacionados con operaciones raster
 */
class RasterTool : public suri::GenericTool {
public:
   /** Constructor */
   explicit RasterTool(DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~RasterTool();
   /** Ejecucion del comando */
   virtual void Execute(const Command* pToolCommand);

protected:
   /** Creacion del Comando */
   virtual Command* CreateCommand(const std::string &CommandName);
   /** Destruccion del Comando */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Creacion del Widget */
   virtual Widget* CreateWidget(const std::string &Widget);

private:
   /** Despliega el widget para corregistro */
   void ExecuteScaleImage() const;
   /** Despliega el widget para georreferenciacion */
   void ExecuteBandStacking() const;
};

} /** namespace suri */

#endif /* RASTERTOOL_H_ */
