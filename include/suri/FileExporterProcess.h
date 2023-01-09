/*! \file FileExporterWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILEEXPORTERWIDGET_H_
#define FILEEXPORTERWIDGET_H_

// Includes standard
#include <vector>
#include <string>

// Includes Suri
#include "RasterProcess.h"
#include "suri/Element.h"
#include "suri/LayerList.h"
#include "suri/RasterElement.h"
#include "suri/ProcessCreatorInterface.h"

// Includea wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"

/** namespace suri */
namespace suri {

// forwards
class DataViewManager;

/** Clase para exportar una lista de elementos a un archivo */
/**
 * Widget que hereda de ExporterWidget y agrega (a las partes de seleccion de
 * subset espacial y archivo de salida ) un part para seleccionar un subset
 * espectral. Agrega al elemento el nodo con las bandas seleccionadas y agrega
 * el elemento a la lista de elementos a renderizar.
 */
class FileExporterProcess : public RasterProcess {
   /** Ctor. de copia */
   FileExporterProcess(const FileExporterProcess &FileExporterWidget);

public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
   /** ctor a partir de la capa de adaptacion de procesos **/
   explicit FileExporterProcess(DataViewManager* pDataViewManager);
   /** Ctor. */
   explicit FileExporterProcess(Element *pElement, const Subset &WindowSubset,
                       DataViewManager* pDataViewManager);

   /** Ctor. */
   explicit FileExporterProcess(std::vector<Element*> InputElements, const Subset &ViewerSubset,
                       DataViewManager* pDataViewManager);
   /** Dtor. */
   virtual ~FileExporterProcess();
   /** Actualiza el estado del part con informacion de otros parts */
   virtual void DataUpdate();

protected:

   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
   **/
   virtual std::vector<wxXmlNode*> DoGetXmlParameters() const {
      return std::vector<wxXmlNode*>();
   }

   /** Metodo que especializa la configuracion del ProcessAdaptLayer para
    *  los procesos especificos
    */
   virtual void DoOnProcessAdaptLayerSet();
   // ------ Metodos que administran la ejecucion del proceso ------
   /** Configura el proceso para que la salida sea el raster configurado */
   virtual bool ConfigureProcess();

   // ------ Metodos que permiten configurar el proceso raster ------
   /** Pemite configurar el RasterElement que se va a renderizar */
   virtual bool ConfigureRaster(RasterElement *pRasterElement);
   /** Configura el elemento creado con la herramienta */
   virtual bool ConfigureOutput();
   /** Otiene el tamano del objeto a salvar */
   virtual wxLongLong GetNeededSpace();

   // ------ Metodos que leen el part de seleccion de archivo de salida ------
   /** Obtiene el nombre y formato del archivo y lo asigna al objeto salvacion */
   virtual void GetSelectedOutput(std::string &Filename, Option &Options);

   /** Atributos **/

   /* Tipo de dato del objeto a salvar, se usa
    * para calcular el tamano de la imagen de salida
    */
   std::string dataType_;

private:
   /** Verifica que haya espacio para guardar el archivo */
   bool CheckNeededDiskSpace(const wxString &Path, wxLongLong NeededSpace);
   /** Verifica que el archivo de entrada sea distinto al de salida */
   bool IsValidOutput(const std::string &Filename);
};

namespace process {

class FileExporterProcessCreator : public ProcessCreatorInterface {

public:
   /** Crea una instancia del proceso asociado al creador concreto
    * @return ProcessInterface nueva instancia de proceso
   **/
   virtual ProcessInterface* Create(DataViewManager* pDataViewManager) const {
      return new FileExporterProcess(pDataView_);
   }

private:
   ProcessAdaptLayer* pAdaptLayer_;
   DataViewManager* pDataView_;
};
}  /** namespace process **/

} // suri
#endif /* FILEEXPORTERWIDGET_H_ */
