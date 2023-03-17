/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#ifndef EXPORTERWIDGET_H_
#define EXPORTERWIDGET_H_

// Includes standard
#include <vector>
#include <string>

// Includes Suri
#include "suri/Element.h"
#include "suri/LayerList.h"
#include "suri/World.h"
#include "suri/Option.h"
#include "suri/RasterSpatialModel.h"
#include "FileRenderization.h"
#include "MovingWindowController.h"
#include "suri/LibraryManager.h"
#include "suri/ProcessInterface.h"

// Includes wx
#include "wx/longlong.h"


/** namespace suri */
namespace suri {

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(ExporterWidgetEvent);

// Forward
class RenderizationController;
class FileRenderization;
class RasterElement;
class DataViewManager;

/** Clase que permite configurar y exportar una seleccion de elementos */
/**
 *	Clase abstracta, contenedor de partes para distintas herramienta.
 *	Crea y agrega una parte para seleccion de formato y nombre del archivo de
 *	salida, y una parte para seleccion del subset espacial del elemento raster.
 */
class RasterProcess : public ProcessInterface {
   /** Ctor. de Copia. */
   RasterProcess(const RasterProcess &);

public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
   /** ctor a partir de la capa de adaptacion de procesos **/
   explicit RasterProcess(DataViewManager* pDataViewManager);
   /** Ctor. */
   explicit RasterProcess(Element *pInputElement, const Subset &ViewerSubset,
                 DataViewManager* pDataViewManager);
   /** Constructor para admitir un proceso raster con mas de un elemento **/
   explicit RasterProcess(const std::vector<Element*>& ElementsP,
                          const Subset &ViewerSubset,
                          DataViewManager* pDataViewManager);
   /** Dtor. */
   virtual ~RasterProcess();

   /** Actualiza el estado de los parts que administra la clase */
   virtual void DataUpdate();

   // ------ Metodos que administran la ejecucion del proceso ------
   /** Corre el proceso y genera la salida usando los metodos de configuracion */
   virtual bool RunProcess();
  /** 
    * Metodo que a partir de un xml que contiene los parametros del proceso configura el mismo.
    * @param[in] XmStr string en formato xml que contiene los parametros necesarios para la 
    * correcta ejecucion del proceso
    * @return true en caso de poder configurar correctamente el proceso en funcion de los parametros
    * @return false en caso contrario
   **/
   virtual bool ConfigureProcessFromXmlString(const std::string& XmlStr);
   /** Metodo que obtiene los parametros asociados al proceso en formato XML.
    * Esto permite que el codigo cliente pueda hacer eso del xml resultante
    * (ej. para la exportacion del mismo)
    * @return string en formato xml con los parametros asociados al proceso
    * @return string vacio en caso contrario.
   **/
   std::string GetParametersAsXmlString() const;
   /** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso.
      *  Template method **/
    virtual void OnProcessAdaptLayerSet();

protected:

   /** Metodo que especializa la configuracion del ProcessAdaptLayer para
    *  los procesos especificos
    */
   virtual void DoOnProcessAdaptLayerSet();
   /** Obtiene la lista de elementos que se a renderizar con el proceso */
   virtual LayerList *GetElementList();
   /** Devuelve una lista con el elemento que se quiere utilizar como mascara.*/
   virtual LayerList *GetMaskList();
   // ------ Metodos que permiten configurar el proceso ------
   /** Pemite configurar el RasterElement que se va a renderizar */
   virtual bool ConfigureRaster(RasterElement *pRasterElement);
   /** Permite configurar la salida del proceso antes de correrlo */
   virtual bool ConfigureProcess();
   /** Una vez ejecutado el proceso hace visible la salida */
   virtual bool ConfigureOutput();
   // ------ Metodos que permiten redefinir bandas de entrada ------
   /** Retorna las bandas selecciondas por el usuario */
   virtual std::vector<int> GetSelectedRasterBands() const;
   /** Indica si las bandas se configuraron correctamente */
   virtual bool HasValidSpectralData();

   // ------ Metodos que configuran seleccion espectral ------
   /** Asigna las bandas a renderizar en el elemento a procesar */
   virtual bool ConfigureRasterBands(RasterElement *pRaster);
   /** Crea un nodo combinacion de bandas con la seleccion de bandas. */
   wxXmlNode* CreateBandCombinationNode();

   // ------ Metodo que lee el part de seleccion espacial ------
   /** Configura el subset espacial */
   virtual World *GetOutputWorld();
   /** Retorna el subset de la imagen de entrada que quiero exportar */
   virtual Subset GetInputSubset();

   /** Devuelve el Sistema de Referencia de llegada. **/
   virtual std::string GetOutputSpatialReference();
   /**  Devuelve el subset de llegada.  **/
   virtual Subset GetOutputSubset();
   /** Devuelve el Modelo Raster de llegada */
   virtual RasterSpatialModel* GetOutputRasterSpatialModel();
   /** Exporta los metadatos de una capa a un archivo */
   virtual bool ExportMetadataToFile(DatasourceInterface* pDatasource, std::string Filename);
   Element* pInputElement_; /*! elemento a exportar (entrada de datos). */

   std::vector<Element*> inputElements_;

   World *pInputWorld_; /*! Puntero al mundo de salida*/
   World *pOutputWorld_; /*! Puntero al mundo de llegada*/
   RasterSpatialModel* pRasterModelIn_; /*! Modelo espacial del elemento recibido */
   RasterSpatialModel* pRasterModelOut_; /*! Modelo espacial de llegada */

   std::string spatialReference_; /*! Referencia espacial del elemento recibido */
   Subset elementSubset_; /*! Extent del elemento recibido */
   Subset viewerSubset_; /*! Subset de la imagen en el viewport */

   RenderizationController *pRenderizationObject_; /*! Objeto para salvar */

   std::string message_; /*! Mensajes de error */

   // TODO(Javier - Sin TCK): Analizar
   bool showMaskPart_; /*! Flag que indica si se debe mostrar part de mascara */
   bool showSpectralSelectionPart_; /*! Determina si se debe mostrar el part */
                                    /** de seleccion espectral */

   virtual Element* GetInputElement() const { return inputElements_[0]; }
   DataViewManager* pDataViewManager_; /*! Administrador general de la aplicacion */
   int bandCount_; /*! Cantidad de bandas en imagen de entrada */

private:
   /** Inicializa el objeto */
   void Initialize(const std::vector<Element*>& ElementsP);
};
}
#endif /* EXPORTERWIDGET_H_ */
