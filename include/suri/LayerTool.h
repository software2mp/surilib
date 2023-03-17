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

#ifndef LAYERTOOL_H_
#define LAYERTOOL_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/GenericTool.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Comandos de Procesamientos de Capas
 */
class LayerTool : public suri::GenericTool {
public:
   /** Constructor */
   explicit LayerTool(DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~LayerTool();
   /** Ejecucion del comando */
   virtual void Execute(const Command* pToolCommand);
   typedef enum  {
      NoGroup = -1, PointGroup = 0, LineGroup = 1, PolygonGroup = 2, RasterGroup = 3
   } LayerGroup;
   typedef std::multimap<LayerGroup, DatasourceInterface*> LayerGroupMap;

protected:
   /** Creacion del Comando */
   virtual Command* CreateCommand(const std::string &CommandName);
   /** Destruccion del Comando */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Creacion del Widget */
   virtual Widget* CreateWidget(const std::string &Widget);
private:
   /**  */
   DataViewManager* pDataViewManager_;
   /** Ejecuta el comando para agregar una capa */
   void ExecuteAddLayer() const;
   /** Ejecuta el comando para agregar una capa remota */
   void ExecuteAddRemoteLayer() const;
   /** Ejecuta el comando para agregar una capa csv */
   void ExecuteAddCsvLayer() const;
   /** Ejecuta el comando para agregar una capa de terreno */
   void ExecuteAddTerrainLayer() const;
   /** Ejecuta el comando para agregar una capa de terreno con un filtro */
   void AddTerrainLayerElement(std::string& filter) const;
   /** Ejecuta el comando para agregar una capa raster con un filtro */
   void AddLayerElement(std::string& filter) const;
   /** Ejecuta el comando para quitar la capa seleccionada */
   void ExecuteRemoveLayer() const;
   /** Ejecuta el comando para mostrar una capa */
   void ExecuteDisplayLayers() const;
   /** Ejecuta el comando para ocultar una capa */
   void ExecuteHideLayers() const;
   /** Ejecuta el comando para mostrar una capa en todos los contextos */
   void ExecuteDisplayLayersInAllViewcontexts() const;
   /** Ejecuta el comando para ocultar una capa en todos los contextos */
   void ExecuteHideLayersInAllViewcontexts() const;
   /** Activa en todos los viewcontext las capas seleccionadas en el viewcontext seleccionado */
   void ActivateLayersInAllViewcontexts(bool Activate) const;
   /** Activa o desactiva los layers seleccionados del viewcontext seleccionado  */
   void ActivateSelectedLayersInSelectedViewcontext(bool Activate) const;
   /** Activa o desactiva los layers seleccionados en un viewcontext en particular */
   void ActivateSelectedLayersInViewcontext(ViewcontextInterface* pViewcontext,
                                            bool Activate) const;
   /** Despliega el widget para mostrar y editar las propiedades de la capa seleccionada */
   void ExecuteShowLayerProperties() const;
   /** Ejecuta el comando para crear un grupo */
   void ExecuteCreateGroup() const;
   /** Ejecuta el comando para exportar una capa raster */
   void ExecuteExportLayer() const;
   /** Ejecuta el comando para exportar una capa vectorial **/
   void ExecuteExportVectorLayer() const;
   /** Ejecuta el comando para exportar la seleccion de una capa */
   void ExecuteExportLayerSelection() const;
   /** Ejecuta apilar bandas */
   void ExecuteStackBands() const;
   /** Despliega el widget para mostrar y editar las propiedades de visualizacion */
   void ExecuteVisualization() const;
   /** importador de capa como area de entrenamiento **/
   void ExecuteClassConverter() const;
   /** Ejecuta el comando para calcular el buffer para capas vectoriales **/
   void ExecuteExportBufferVectorLayer() const;
   /** Ejecuta el comando para fusionar capas vectoriales **/
   void ExecuteMergeLayers() const;
   /** Ejecuta el comando para calcular operaciones vectoriales **/
   void ExecuteExportVectorOperationLayer() const;
   /** Metodo que invoca al importador generico raster **/
   void ExecuteImportGenericRaster() const;
   /** Muestra pantalla para que el usuario ingrese un nombre de grupo. */
   bool GetGroupName(std::string &GroupName) const;
   LayerGroupMap mapLayerGroup(const wxArrayString& pathArray) const;
#ifdef __DEBUG__
   /** Ejecuta el comando para agregar una recinto */
   void ExecuteAddEnclosure() const;
   /** Ejecuta el comando para agregar un raster clasificado */
   void ExecuteAddClassifiedRaster() const;
   /** Ejecuta el comando para limpiar una recinto */
   void ExecuteCleanEnclosure() const;
   /** Ejecuta el comando para limpiar un raster clasificado */
   void ExecuteCleanClassifiedRaster() const;
   /** Ejecuta el comando para limpiar una recinto */
   void ExecuteValidateEnclosure() const;
   /** Ejecuta el comando para limpiar un raster clasificado */
   void ExecuteValidateClassifiedRaster() const;
   /** Ejecuta la traduccion segun el id 0 = LayerElement, 1 Datasource-Element**/
   void ExecuteTranslation(int IdTranslation = 0, bool Direct = true, bool Apply = false)const;
   /** Ejecuta la traduccion layer-Element **/
   void ExecuteLayerElementTranslation(bool Direct = true, bool Apply = false)const;
   /** Ejecuta la traduccion Datasource-Element **/
   void ExecuteDatasourceElementTranslation(bool Direct = true, bool Apply = false)const;
   /** Aplica dos traducciones, de forma de poder cruzar las fuente de datos asociadas
    *  entre dos capas
    */
   void ExecuteSwitchTranslation()const;
   /**
    * Ejecuta el componente CGU en modo ReadOnly o ReadWrite
    */
   void ExecuteShowCgu(bool ReadOnly = true) const;
   /** Metodo que carga una lista de gcp a partir de un archivo*/
   void ExecuteGcpLoaderFromTxt() const;

   /** Activa/Desactiva los botones de la toolbar**/
   void ExecuteActiveCommands(const std::string& CommandNameCaller) const;
   /** Habilita/Deshabilita los botones de la toolbar**/
   void ExecuteEnableCommands(const std::string& CommandNameCaller) const;
#endif
};

} /** namespace suri */

#endif /* LAYERTOOL_H_ */
