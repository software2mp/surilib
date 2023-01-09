/*! \file LibraryToolCommandCreator.h */                                                     
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 

#ifndef __LIBRARY_TOOL_COMMAND_CREATOR_H__
#define __LIBRARY_TOOL_COMMAND_CREATOR_H__

// Includes Estandard
#include <string>

// Includes Suri
#include "suri/CommandCreatorInterface.h"

namespace suri {

// forwards
class Command;
class ToolInterface;

namespace core {


/** Clase que encargada de configurar el Command para la administracion de la biblioteca de
 *  sistemas de referencia
 **/
class SpatialReferenceLibraryCommandCreator : public CommandCreatorInterface {
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};
/** Clase que encargada de configurar el Command para la administracion de la biblioteca de datums
 * de la aplicacion
 **/
class DatumLibraryCommandCreator : public CommandCreatorInterface {
private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};
/** Clase que encargada de configurar el Command para la administracion de la biblioteca de 
 *  elipsoides de la aplicacion
 **/
class EllipsoidLibraryCommandCreator : public CommandCreatorInterface {
private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/** Clase que encargada de configurar el Command para la administracion de la biblioteca de 
 *  tabla de intervalos de la aplicacion
 **/
class DensitySliceLibraryCommandCreator : public CommandCreatorInterface {
private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/** Clase que encargada de configurar el Command para la administracion de la biblioteca de 
 *  servidores wms de la aplicacion
 **/
class WmsLibraryCommandCreator : public CommandCreatorInterface {
private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/** Clase que encargada de configurar el Command para la administracion de la biblioteca de 
 *  estilos vectoriales de la aplicacion
 **/
class VectorStyleLibraryCommandCreator : public CommandCreatorInterface {
private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/** Clase que encargada de configurar el Command para la administracion de la biblioteca de 
 *  firmas espectrales vectoriales de la aplicacion
 **/
class SpectralSignLibraryCommandCreator : public CommandCreatorInterface {
private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase)
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/** Clase que encargada de configurar el Command para la administracion de la biblioteca de
 *  filtros de convolucion de la aplicacion
 **/
class ConvolutionfiltersLibraryCommandCreator : public CommandCreatorInterface {
private:
   /**
    * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
    * del comando sea del dominio de la clase.
    **/
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/**
 * Clase que encargada de configurar el Command para la administracion de la biblioteca de
 * ecuaciones de la aplicacion
 */
class EquationLibraryCommandCreator : public CommandCreatorInterface {
private:
   /**
    * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
    * del comando sea del dominio de la clase)
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

class IndexLibraryCommandCreator : public CommandCreatorInterface {
private:
   /**
    * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
    * del comando sea del dominio de la clase)
    */
   virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};
}  /** namespace core **/
}  /** namespace suri **/

#endif
