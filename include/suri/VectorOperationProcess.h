/*! \file VectorOperationProcess.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTOROPERATIONPROCESS_H_
#define VECTOROPERATIONPROCESS_H_

// Includes Estandar
// Includes Suri
#include "FileVectorExporterProcess.h"
#include "suri/ProcessAdaptLayer.h"
#include "VectorOperation.h"
// Includes Wx
// Defines

namespace suri {
// forwards

class VectorOperationProcess : public suri::FileVectorExporterProcess {
public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
   /** Constructor */
   VectorOperationProcess(std::vector<DatasourceInterface*>& InputDatasources,
                          const Subset &ViewerSubset, Element* &pOutputElement,
                          DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~VectorOperationProcess();
private:
   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
    **/
   std::vector<wxXmlNode*> DoGetXmlParameters() const;
   /** Obtiene la lista de elementos a renderizar en el proceso */
   DatasourceContainerInterface* GetRenderizationList();
   /** Crea una fuente de datos vectorial con el buffer
    *  basandose en una copia de la fuente original */
   VectorDatasource* GetProcessedDatasource();
   /** Genera un archivo shp en el directorio temporal del sistema */
   std::string GetTempFile();
   VectorOperation::OperationType operation_; /** Tipo de operacion */
   std::vector<DatasourceInterface*> datasources_; /** Fuentes de datos para procesar */
   double distancevalue_; /** Distancia de buffer */
   std::string distancevaluestr_; /** String con distancia de buffer */
   std::string bufferfield_; /** Campo de distancia de buffer */
   int nrings_; /** Numero de anillos concentricos */
   std::string nringsstr_; /** String con numero de anillos concentricos */
   double conversionfactor_; /** Factor de conversion */
   std::string conversionfactorstr_; /** String con factor de conversion */
   std::string datasourcefields_; /** String con campo y fuente de datos a la que corresponden */
};

} /** namespace suri */

#endif /* VECTOROPERATIONPROCESS_H_ */
