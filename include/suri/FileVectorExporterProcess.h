/*! \file FileVectorExporterProcess.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILEVECTORPROCESS_H_
#define FILEVECTORPROCESS_H_

#include <vector>
#include <string>

#include "VectorProcess.h"
#include "suri/DatasourceInterface.h"
#include "Subset.h"
#include "DataViewManager.h"
#include "suri/World.h"

namespace suri {

class Element;
class FileVectorRenderizationController;
class SpatialSubsetSelectionPart;
class VectorFormatSelectionPart;

/*!
 * Clase que representa un proceso sobre capas vectoriales
 * que es renderizado sobre un archivo.
 */
class FileVectorExporterProcess : public suri::VectorProcess {
   /** ctor copia **/
   FileVectorExporterProcess(const FileVectorExporterProcess&);

public:
   /** ctor **/
   FileVectorExporterProcess(std::vector<DatasourceInterface*>& InputDatasources,
                             const Subset &ViewerSubset, Element* &pOutputElement,
                             DataViewManager* pDataViewManager);

   /** dtor **/
   virtual ~FileVectorExporterProcess();

#ifdef __UNUSED_CODE__
   /** Agrega las partes. Sacar este metodo cuando se desacoplen los procesos */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);
#endif

   /** Permite configurar la salida del proceso antes de correrlo */
   virtual bool ConfigureProcess();

   /** Una vez ejecutado el proceso hace visible la salida */
   virtual bool ConfigureOutput();

   /** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso**/
   virtual void OnProcessAdaptLayerSet();

   /** 
    * Metodo que a partir de un xml que contiene los parametros del proceso configura el mismo.
    * @param[in] XmStr string en formato xml que contiene los parametros necesarios para la 
    * correcta ejecucion del proceso
    * @return true en caso de poder configurar correctamente el proceso en funcion de los parametros
    * @return false en caso contrario
   **/
   virtual bool ConfigureProcessFromXmlString(const std::string& XmlStr);

   static const std::string EnableSelectionExportAttr;

protected:   
   /** Metodo que obtiene los parametros asociados al proceso en formato XML. 
    * Esto permite que el codigo cliente pueda hacer eso del xml resultante 
    * (ej. para la exportacion del mismo)
    * @return string en formato xml con los parametros asociados al proceso
    * @return string vacio en caso contrario. 
   **/
   virtual std::string GetParametersAsXmlString()const;

   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
   **/
   virtual std::vector<wxXmlNode*> DoGetXmlParameters() const {
      return std::vector<wxXmlNode*>();
   }

   /** Inicializa la instancia **/
   void Initialize();

   /** Obtiene la lista de elementos a renderizar en el proceso **/
   virtual DatasourceContainerInterface* GetRenderizationList();

   /** Configura el subset espacial */
   virtual World* GetWorld();

   /** Obtiene el nombre y formato del archivo y lo asigna al objeto salvacion */
   virtual void GetSelectedOutput(std::string &FileName, Option &Options);

   std::vector<DatasourceInterface*> inputDatasources_;
   Subset viewerSubset_;
   Element* &pOutputElement_;
   DataViewManager* pDataViewManager_;

#ifdef __UNUSED_CODE__
   /** Part para selecion del subset de llegada **/
   SpatialSubsetSelectionPart* pSubsetSeelctionPart_;
   /** Part para seleccion de archivo de salida */
   VectorFormatSelectionPart* pFileSelectPart_;
#endif

   // Puntero al mundo de salida.
   World *pInputWorld_;

   // controlador para la renderizacion de la exportacion.
   FileVectorRenderizationController* pFileController_;

   /**
    * Cambia el nodo de renderizacion para poder acceder a la exportacion.
    */
   void SanitizeDatasource(DatasourceInterface* pDatasource);
};

}  // namespace suri

#endif  // FILEVECTORPROCESS_H_
