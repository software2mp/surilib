/*! \file DatasourceManipulator.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DATASOURCEMANIPULATOR_H_
#define DATASOURCEMANIPULATOR_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "DatasourceInterface.h"
#include "suri/Option.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * La clase Manipulator provee una interfaz que permite manipular diferentes tipos de objetos
 * de manera prolija y con abstraccion de cada tipo en particular
 */
class DatasourceManipulatorInterface {
public:
   /** Constructor */
   DatasourceManipulatorInterface() {}
   /** Destructor */
   virtual ~DatasourceManipulatorInterface() {}
   /** Crea una nueva fuente de datos que cumple con los requisitos de validacion de Enclosure */
   virtual DatasourceInterface* Create(const std::string& FileName,
                                       const Option& option) const = 0;
   /** Efectua la logica de agregado para un Datasource validado */
   virtual bool AddNature(DatasourceInterface* pDatasource) const = 0;
   /** Efectua la logica de limpieza para un Datasource validado */
   virtual bool CleanNature(DatasourceInterface* pDatasource) const = 0;
   /** Efectua la logica de copia de un Datasource a otro */
   virtual bool CopyNature(DatasourceInterface* pSourceDatasource,
                 DatasourceInterface* pDestinationDatasource) const = 0;
};

} /** namespace suri */

#endif /* DATASOURCEMANIPULATOR_H_ */
