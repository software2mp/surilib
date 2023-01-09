/*! \file VectorOperationProcessPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTOROPERATIONPARTPROVIDER_H_
#define VECTOROPERATIONPARTPROVIDER_H_

// Includes Estandar
// Includes Suri
#include "FileVectorExporterProcessPartProvider.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Clase encargada de proveer los parts para la configuracion del proceso
 * de operaciones vectoriales
 */
class VectorOperationProcessPartProvider : public suri::FileVectorExporterProcessPartProvider {
   /** Constructor copia **/
   VectorOperationProcessPartProvider(const VectorOperationProcessPartProvider&);
public:
   /** Metodo para la creacion de un Part provider */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);
   /** Destructor */
   virtual ~VectorOperationProcessPartProvider();
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);

protected:
   /** Constructor **/
   explicit VectorOperationProcessPartProvider(ProcessAdaptLayer* pPal);
};

} /** namespace suri */

#endif /* VECTOROPERATIONPARTPROVIDER_H_ */
