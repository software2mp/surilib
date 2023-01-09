/*! \file LibraryToolCommandExecutionHandler.h */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */


#ifndef __LIBRARY_TOOL_COMMAND_EXEC_HANDLER_H__
#define __LIBRARY_TOOL_COMMAND_EXEC_HANDLER_H__

// Includes Estadard
#include <string>

// Includes Suri
#include "suri/CommandExecutionHandlerInterface.h"
#include "suri/DataViewManager.h"

namespace suri {

namespace core {

class LibraryToolCommandExecutionHandler : public CommandExecutionHandlerInterface {
	/** const copia */
	LibraryToolCommandExecutionHandler(const LibraryToolCommandExecutionHandler&);

public:
	/** constructor **/
	LibraryToolCommandExecutionHandler(DataViewManager* pDataView) : pDataView_(pDataView) {
	}	
	/** dtor **/
	~LibraryToolCommandExecutionHandler() {}
	/**
	 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
	 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    * @return true en caso de poder manejar la ejecucion del comando
    * @return false en caso contrario
    */
	virtual bool Execute(const Command* pCommand, GenericTool* pTool) {
		if (CanHandleCommand(pCommand, pTool)) {
			ExecuteLibrary(libraryName_, windowTitle_);
		} else {
			return false;
		}	
		return true;
	}

protected:
	/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
	virtual bool CanHandleCommand(const Command* pCommand, GenericTool* pTool)=0;
	/** Metodo que instancia y configura el administrador de bibliotecas a partir del nombre de la misma **/
	void ExecuteLibrary(const std::string& LibraryCode, const std::string& WindowTitle);
	DataViewManager* pDataView_;
	std::string libraryName_, windowTitle_;
};

/** Clase encargada de administrar la ejecucion del comando SpatialReferenceLibrary **/
class SpatialReferenceLibraryCommandExecutionHandler : public LibraryToolCommandExecutionHandler {

	/** constructor copia **/
	SpatialReferenceLibraryCommandExecutionHandler(const SpatialReferenceLibraryCommandExecutionHandler&);

public:
	/** Constructor **/
	SpatialReferenceLibraryCommandExecutionHandler(DataViewManager* pDataView); 

private:
	/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
	virtual bool CanHandleCommand(const Command* pCommand, GenericTool* pTool);
};

/** Clase encargada de administrar la ejecucion del comando DatumLibrary **/
class DatumLibraryCommandExecutionHandler : public LibraryToolCommandExecutionHandler {

	/** constructor copia **/
	DatumLibraryCommandExecutionHandler(const DatumLibraryCommandExecutionHandler&);

public:
	/** Constructor **/
	DatumLibraryCommandExecutionHandler(DataViewManager* pDataView);

private:
	/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
	virtual bool CanHandleCommand(const Command* pCommand, GenericTool* pTool);

};

/** Clase encargada de administrar la ejecucion del comando Ellipsoid **/
class EllipsoidLibraryCommandExecutionHandler : public LibraryToolCommandExecutionHandler {

	/** constructor copia **/
	EllipsoidLibraryCommandExecutionHandler(const EllipsoidLibraryCommandExecutionHandler&);

public:
	/** Constructor **/
	EllipsoidLibraryCommandExecutionHandler(DataViewManager* pDataView); 

private:
	/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
	virtual bool CanHandleCommand(const Command* pCommand, GenericTool* pTool);
};

/** Clase encargada de administrar la ejecucion del comando DensitySlice **/
class DensitySliceLibraryCommandExecutionHandler : public LibraryToolCommandExecutionHandler {

	/** constructor copia **/
	DensitySliceLibraryCommandExecutionHandler(const DensitySliceLibraryCommandExecutionHandler&);

public:
	/** Constructor **/
	DensitySliceLibraryCommandExecutionHandler(DataViewManager* pDataView);

private:
	/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
	virtual bool CanHandleCommand(const Command* pCommand, GenericTool* pTool);
};

/** Clase encargada de administrar la ejecucion del comando WmsLibrary **/
class WmsLibraryCommandExecutionHandler : public LibraryToolCommandExecutionHandler {

	/** constructor copia **/
	WmsLibraryCommandExecutionHandler(const WmsLibraryCommandExecutionHandler&);

public:
	/** Constructor **/
	WmsLibraryCommandExecutionHandler(DataViewManager* pDataView);

private:
	/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
	virtual bool CanHandleCommand(const Command* pCommand, GenericTool* pTool);
};

/** Clase encargada de administrar la ejecucion del comando VectorStyleLibrary **/
class VectorStyleLibraryCommandExecutionHandler : public LibraryToolCommandExecutionHandler {

	/** constructor copia **/
	VectorStyleLibraryCommandExecutionHandler(const VectorStyleLibraryCommandExecutionHandler&);

public:
	/** Constructor **/
	VectorStyleLibraryCommandExecutionHandler(DataViewManager* pDataView);

private:
	/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
	virtual bool CanHandleCommand(const Command* pCommand, GenericTool* pTool);
};

/** Clase encargada de administrar la ejecucion del comando ConvolutionFiltersLibrary **/
class ConvolutionFiltersLibraryCommandExecutionHandler : public LibraryToolCommandExecutionHandler {

   /** constructor copia **/
   ConvolutionFiltersLibraryCommandExecutionHandler(const ConvolutionFiltersLibraryCommandExecutionHandler&);

public:
   /** Constructor **/
   ConvolutionFiltersLibraryCommandExecutionHandler(DataViewManager* pDataView);

private:
   /** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
   virtual bool CanHandleCommand(const Command* pCommand, GenericTool* pTool);
};


/** Clase encargada de administrar la ejecucion del comando SpectralSignLibrary **/
class SpectralSignLibraryCommandExecutionHandler : public LibraryToolCommandExecutionHandler {

	/** constructor copia **/
	SpectralSignLibraryCommandExecutionHandler(const SpectralSignLibraryCommandExecutionHandler&);

public:
	/** Constructor **/
	SpectralSignLibraryCommandExecutionHandler(DataViewManager* pDataView);

private:
	/** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
	virtual bool CanHandleCommand(const Command* pCommand, GenericTool* pTool);
};

/** Clase encargada de administrar la ejecucion del comando SpectralSignLibrary **/
class EquationLibraryCommandExecutionHandler : public LibraryToolCommandExecutionHandler {

   /** constructor copia **/
   EquationLibraryCommandExecutionHandler(const EquationLibraryCommandExecutionHandler&);

public:
   /** Constructor **/
   explicit EquationLibraryCommandExecutionHandler(DataViewManager* pDataView);

private:
   /** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
   virtual bool CanHandleCommand(const Command* pCommand, GenericTool* pTool);
};

/** Clase encargada de administrar la ejecucion del comando SpectralSignLibrary **/
class IndexLibraryCommandExecutionHandler : public LibraryToolCommandExecutionHandler {

   /** constructor copia **/
   IndexLibraryCommandExecutionHandler(const IndexLibraryCommandExecutionHandler&);

public:
   /** Constructor **/
   explicit IndexLibraryCommandExecutionHandler(DataViewManager* pDataView);

private:
   /** Metodo responsable de determinar si el comando puede ser manejado por la instancia **/
   virtual bool CanHandleCommand(const Command* pCommand, GenericTool* pTool);
};

}  /** namespace core **/
}  /** namespace suri **/
#endif
