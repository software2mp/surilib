/*! \file VectorProcess.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORPROCESS_H_
#define VECTORPROCESS_H_

// Includes Estandar
#include <vector>

// Includes Suri
#include "ProcessInterface.h"
#include "VectorRenderizationController.h"
#include "DatasourceInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class Part;

/*!
 * Clase que representa un proceso sobre capas vectoriales.
 */
class VectorProcess : public suri::ProcessInterface {
   /** ctor copia **/
   VectorProcess(const VectorProcess&);

public:
   /** constructor **/
   VectorProcess();
   /** destructor **/
   virtual ~VectorProcess();
#ifdef __UNUSED_CODE__
   /** Agrega las partes. Sacar este metodo cuando se desacoplen los procesos */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);
#endif
   // ------ Metodos que administran la ejecucion del proceso ------
   /** Corre el proceso y genera la salida usando los metodos de configuracion */
   virtual bool RunProcess();
   /** Permite configurar la salida del proceso antes de correrlo */
   virtual bool ConfigureProcess();
   /** Una vez ejecutado el proceso hace visible la salida */
   virtual bool ConfigureOutput();

protected:
   /** Controlador para la renderizacion de capas vectoriales **/
   VectorRenderizationController* pController_;
   /** Obtiene la lista de elementos a renderizar en el proceso **/
   virtual DatasourceContainerInterface* GetRenderizationList()=0;
   /** Configura el subset espacial */
   virtual World* GetWorld()=0;
   /** Exporta los metadatos de una capa a un archivo */
   virtual bool ExportMetadataToFile(DatasourceInterface* pDatasource, std::string Filename);
};

} /** namespace suri */

#endif /* VECTORPROCESS_H_ */
