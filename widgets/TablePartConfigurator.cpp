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

// Includes Estandar
#include <string>
#include <vector>
#include <sstream>

// Includes Suri
#include "TablePart.h"
#include "TableTool.h"
#include "suri/ToolSupport.h"
#include "DefaultTable.h"
#include "suri/MemoryDriver.h"
#include "suri/BufferedDriver.h"
#include "suri/GcpDriver.h"
#include "suri/Table.h"
#include "suri/VectorEditorDriver.h"
#include "TablePartConfigurator.h"
#include "suri/messages.h"
#include "suri/Configuration.h"
#include "suri/DataViewManager.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DatasourceInterface.h"
#include "suri/VectorEditionTable.h"
#include "VectorDatasourceSelectionSource.h"
#include "VectorDatasource.h"
#include "suri/GeoreferenceGcpDriver.h"
#include "suri/CoregisterGcpDriver.h"

// Includes Wx

// Defines

// forwards

namespace suri {
/** Opcion para edicion vectorial */
const std::string TablePartConfigurator::VectorTableEditorOption = "vector_table_editor";
/** Opcion de configuracion de editar de tabla **/
const std::string TablePartConfigurator::TableEditorKeyOption = "TableEditor";
const std::string TablePartConfigurator::CorregisterTableEditorOption =
      "coregister_table_editor";
const std::string TablePartConfigurator::TablePartFileNameKey = "FileName";
/** Raster DN */
const std::string TablePartConfigurator::RasterDnTableEditorOption =
      "rasterdn_table_editor";
const std::string TablePartConfigurator::BandTableEditorOption =
      "band_table_editor";
const std::string TablePartConfigurator::GeoreferenceTableEditorOption =
      "georeference_table_editor";
/** Opcion de configuracion de driver de corregistro/georreferenciacion */
const std::string TablePartConfigurator::TableSourceSrs = "table_source_srs";
const std::string TablePartConfigurator::TableSourceRasterModel =
      "table_source_raster_model";
const std::string TablePartConfigurator::TableDestinationSrs = "table_destination_srs";
const std::string TablePartConfigurator::TableDestinationRasterModel =
      "table_destination_raster_model";
/** Clave para esconder columnas
 *  si se posee mas de una columna a esconder se separa con coma
 *  ej. esconder columna a y columna c => "a,b" **/
const std::string TablePartConfigurator::HiddenColumnsKeyOption = "hidden_cols";
/** Clave para columnas de solo lectura
 *  si se posee mas de una columna se separa con coma
 *  ej. readonly columna a y columna c => "a,b" **/
const std::string TablePartConfigurator::RoColumnsKeyOption = "ro_cols";
/** Clave para habilitar la consulta en el componente de tabla **/
const std::string TablePartConfigurator::SearchKeyOption = "search_enabled";
const std::string TablePartConfigurator::VectorDatasourceUrl = "vdsurl";
/**
 * Constructor
 */
TablePartConfigurator::TablePartConfigurator() {
}

/**
 * Destructor
 */
TablePartConfigurator::~TablePartConfigurator() {
}

/**
 * Inicializa
 * @param pDataViewManager administrador de informacion de la aplicacion
 * @param ConfigurationOptions opciones de configuracion
 * @return
 */
TablePart* TablePartConfigurator::Initialize(DataViewManager* pDataViewManager,
                                             const Option& ConfigurationOptions) {
   std::string parameter = ConfigurationOptions.GetOption(TableEditorKeyOption);
   Subject* psubject;
   DriverInterface* pdriver = this->RetrieveDriver(ConfigurationOptions,
                                                   pDataViewManager, psubject);
   if (pdriver == NULL)
      return NULL;

   Table* ptable = RetrieveTable(pDataViewManager, parameter);
   if (ptable == NULL)
      ptable = new DefaultTable();
   ptable->SetDriver(pdriver);
   ptable->SetPermission(RetrievePermissionKey(parameter));
   if (psubject)
      ptable->SetDataChangeNotifier(psubject);

   TablePart* ptablepart = Initialize(pDataViewManager, ptable, ConfigurationOptions);
   ConfigureColumns(ConfigurationOptions, ptable, ptablepart);
   GcpDriver* pgcpdriver = dynamic_cast<GcpDriver*>(pdriver);
   if (pgcpdriver) {
      ptablepart->Attach(pgcpdriver);
      pgcpdriver->Attach(ptablepart);
      pgcpdriver->LazyInit();
   }

   return ptablepart;
}

/**
 * Inicializa table part
 * @param pDataViewManager administrador de informacion de la aplicacion
 * @param[in] pTable tabla que quiero configurar
 * @param[in] ConfigurationOptions opciones de configuracion.
 */
TablePart* TablePartConfigurator::Initialize(DataViewManager* pDataViewManager,
                                             Table* pTable,
                                             const Option& ConfigurationOptions) {
   std::string parameter = ConfigurationOptions.GetOption(TableEditorKeyOption);
   std::string driverconfig = "tool_";
   driverconfig.append(parameter);
   driverconfig.append("_driver");
   std::string driver = "MemoryDriver";
   driver = Configuration::GetParameter(driverconfig, driver);
   bool includeorder = (driver.compare("GcpDriver") == 0
         || driver.compare("GeoreferenceGcpDriver") == 0);
   bool includerefsys = driver.compare("GeoreferenceGcpDriver") == 0;
   bool enablesearch = !ConfigurationOptions.GetOption(SearchKeyOption).empty();
   TablePart* ppart = new TablePart(pTable, includeorder, includerefsys,
                                    pDataViewManager, enablesearch);
   ConfigureColumns(ConfigurationOptions, pTable, ppart);
   ppart->ChangeWindowTitle(this->RetrieveWindowTitle(parameter));
   ppart->SetWindowIcon(this->RetrieveWindowIcon(parameter));
   TableTool* ptool = new TableTool(pDataViewManager, ppart);
   ppart->SetTableTool(ptool);
   ppart->SetSelectionNotifier(
         pDataViewManager ? pDataViewManager->GetFeatureSelection() : NULL);
   ptool->SetTablePart(ppart);
   return ppart;
}

/** Configura las columnas de la tabla de acuerdo a las opciones de config **/
void TablePartConfigurator::ConfigureColumns(const Option& ConfigurationOptions,
                                             Table* pTable, TablePart* pTablePart) {
   std::string columnsarray = ConfigurationOptions.GetOption(HiddenColumnsKeyOption);
   if (!columnsarray.empty())
      ConfigureHiddenColumns(columnsarray, pTable, pTablePart);
   std::string rocolumnsarray = ConfigurationOptions.GetOption(RoColumnsKeyOption);
   if (!columnsarray.empty())
      ConfigureReadOnlyColumns(rocolumnsarray, pTablePart);
}

/**
 *  Configura las columnas ocultas en TablePart segun el string que se pasa por parametro
 *  El formato del array de columnas es el nombre de las columnas separado por ,
 *  @param ColumnsArray columas a ocultar
 *  @param pTable tabla para la obtencion de indices de columas
 *  @param pTablePart part que posee la visualizacion de la tabla. Es donde
 *  se configura el ocultamiento de la tabla
 */
void TablePartConfigurator::ConfigureHiddenColumns(const std::string& ColumnsArray,
                                                   Table* pTable,
                                                   TablePart* pTablePart) {
   std::vector<std::string> columnsnames = tokenizer(ColumnsArray, ",");
   std::vector<std::string>::iterator it = columnsnames.begin();
   for (; it != columnsnames.end(); ++it) {
      int col = pTable->GetColumnByName(*it);
      if (col >= 0) {
         pTablePart->HiddeColumn(col);
      }
   }
}

/** Configura las columnas de solo lectura
 *  El formato del array de columnas es el nombre de las columnas separado por ,
 *  @param ColumnsArray columas de solo lectura
 *  @param pTablePart control grafico de la tabla **/
void TablePartConfigurator::ConfigureReadOnlyColumns(const std::string& RoColumnsArray,
                                                     TablePart* pTablePart) {
   std::vector<std::string> columnsnames = tokenizer(RoColumnsArray, ",");
   std::vector<std::string>::iterator it = columnsnames.begin();
   for (; it != columnsnames.end(); ++it) {
      pTablePart->SetReadOnlyColumnView(*it);
   }
}

/**
 * Obtiene los comandos de la configuracion
 * @return
 */
std::vector<std::string> TablePartConfigurator::RetrieveCommands(
      const std::string& Configuration) {
   std::string parameter = "tool_";
   parameter.append(Configuration);
   parameter.append("_commands");
   std::string commands = Configuration::GetParameter(parameter, std::string(""));
   return tokenizer(commands, ",");
}

/**
 * Obtiene los permisos de la configuracion
 * @return
 */
int TablePartConfigurator::RetrievePermissionKey(const std::string& Configuration) {
   // TODO(Sebastian - TCK #2218): Mejorar con manejo de permisos con nombres
   int permissionKey = 0;
   std::string parameter = "tool_";
   parameter.append(Configuration);
   parameter.append("_permissions");
   return Configuration::GetParameter(parameter, permissionKey);
}

/**
 * Obtiene el drives de la configuracion
 * @return
 */
DriverInterface* TablePartConfigurator::RetrieveDriver(
      const Option& ParameterOption, DataViewManager* pDataViewManager,
      Subject* &pSubject) {
   std::string configuration = ParameterOption.GetOption(TableEditorKeyOption);
   std::string driver = "MemoryDriver";
   std::string parameter = "tool_";
   parameter.append(configuration);
   parameter.append("_driver");
   driver = Configuration::GetParameter(parameter, driver);
   DriverInterface* pdriver = NULL;
   if (driver.compare("MemoryDriver") == 0) {
      pdriver = new MemoryDriver();
   } else if (driver.compare("VectorEditorDriver") == 0) {
      VectorElement* prasterfromlayer = ToolSupport::GetVector(
            pDataViewManager->GetViewcontextManager());
      if (!prasterfromlayer)
         return NULL;
      LayerInterface* player =
            pDataViewManager->GetViewcontextManager()->GetAssociatedLayerByElementId(
                  prasterfromlayer->GetUid());
      if (!player)
         return NULL;
      DatasourceInterface* pdatasource =
            pDataViewManager->GetDatasourceManager()->GetDatasource(
                  player->GetAssociatedDatasourceId());
      if (!pdatasource)
         return NULL;

      pdriver = new VectorEditorDriver(pdatasource);
   } else if (driver.compare("GcpDriver") == 0) {
      std::string filename = ParameterOption.GetOption(TablePartFileNameKey);

      // Se obtienen los SRS y raster model de los puntos de control.
      std::string sourcesrs = ParameterOption.GetOption(TableSourceSrs);
      std::string destinationsrs = ParameterOption.GetOption(TableDestinationSrs);
      std::string sourcerastermodel = ParameterOption.GetOption(TableSourceRasterModel);
      std::string destinationrastermodel = ParameterOption.GetOption(
            TableDestinationRasterModel);

      GcpDriver* pgcpdriver =
            filename.empty() ? new CoregisterGcpDriver() :
                               new CoregisterGcpDriver(filename, sourcesrs,
                                                       destinationsrs,
                                                       sourcerastermodel,
                                                       destinationrastermodel);
      pgcpdriver->Init(!filename.empty());
      pdriver = pgcpdriver;
      pSubject = pgcpdriver;
   } else if (driver.compare("GeoreferenceGcpDriver") == 0) {
      std::string filename = ParameterOption.GetOption(TablePartFileNameKey);
      // Se obtienen los SRS y raster model de los puntos de control.
      std::string sourcesrs = ParameterOption.GetOption(TableSourceSrs);
      std::string destinationsrs = ParameterOption.GetOption(TableDestinationSrs);
      std::string sourcerastermodel = ParameterOption.GetOption(TableSourceRasterModel);
      std::string destinationrastermodel = ParameterOption.GetOption(
            TableDestinationRasterModel);

      GeoreferenceGcpDriver* pgcpdriver =
            filename.empty() ? new GeoreferenceGcpDriver() :
                               new GeoreferenceGcpDriver(filename, sourcesrs,
                                                         destinationsrs,
                                                         sourcerastermodel,
                                                         destinationrastermodel);
      pgcpdriver->Init(!filename.empty());
      pdriver = pgcpdriver;
      pSubject = pgcpdriver;
   } else {
      return pdriver;
   }
   int buffered = 0;
   std::string buffparam = "tool_";
   buffparam.append(configuration);
   buffparam.append("_buffered_driver");
   buffered = Configuration::GetParameter(buffparam, buffered);
   if (buffered) {
      BufferedDriver* pbuffereddriver = new BufferedDriver(pdriver);
      pdriver = pbuffereddriver;
      pSubject = pbuffereddriver;
   }

   return pdriver;
}

/**
 * Obtiene el titulo de la ventana
 * @param Configuration
 * @return
 */
std::string TablePartConfigurator::RetrieveWindowTitle(
      const std::string& Configuration) {
   std::string parameter = "tool_";
   parameter.append(Configuration);
   parameter.append("_windowtitle");
   std::string title = Configuration::GetParameter(parameter, std::string(""));
   return (title.compare("") == 0) ? caption_VECTOR_TABLE_PART : title;
}

/**
 * Obtiene el path del icono de la ventana
 * @param Configuration
 * @return
 */
std::string TablePartConfigurator::RetrieveWindowIcon(
      const std::string& Configuration) {
   std::string parameter = "tool_";
   parameter.append(Configuration);
   parameter.append("_windowicon");
   return Configuration::GetParameter(parameter, std::string(""));
}

/**
 * Metodo auxiliar que obtiene el tipo de tabla correspondiente
 * a la configuracion de la aplicacion
 * @param Parameter
 * @return Table* nueva instancia correspondiente al tipo de instancia que indica
 * la configuracion
 * @return DefaultTable* en caso de no encontrar la configuracion deseada
 */
Table* TablePartConfigurator::RetrieveTable(DataViewManager* pDataViewManager,
                                            const std::string& Parameter) {
   std::string parameter = "tool_";
   parameter.append(Parameter);
   parameter.append("_instance");
   std::string typetable = Configuration::GetParameter(parameter, std::string(""));
   Table* ptable = NULL;
   if (typetable.compare("BasicTable") == 0) {
      ptable = new DefaultTable();
   } else if (typetable.compare("VectorTable") == 0) {
      VectorElement* prasterfromlayer = ToolSupport::GetVector(
            pDataViewManager->GetViewcontextManager());
      if (!prasterfromlayer)
         return NULL;
      LayerInterface* player =
            pDataViewManager->GetViewcontextManager()->GetAssociatedLayerByElementId(
                  prasterfromlayer->GetUid());
      if (!player)
         return NULL;
      DatasourceInterface* pdatasource =
            pDataViewManager->GetDatasourceManager()->GetDatasource(
                  player->GetAssociatedDatasourceId());
      if (!pdatasource)
         return NULL;
      ptable = new VectorEditionTable(pdatasource);
   } else {
      ptable = new DefaultTable();
   }
   return ptable;
}

/** Metodo que devuelve las opciones de configuracion para edicion vectorial **/
Option TablePartConfigurator::GetVectorTableEditorConfig() {
   Option configoption;
   configoption.SetOption(TableEditorKeyOption, VectorTableEditorOption);
   configoption.SetOption(HiddenColumnsKeyOption,
                          VectorEditorDriver::GeometryColumnName);
   configoption.SetOption(RoColumnsKeyOption, VectorEditorDriver::FeatureIdColumnName);
   configoption.SetOption(SearchKeyOption, "true");
   return configoption;
}

/** Metodo que devuelve las opciones de configuracion para edicion vectorial **/
Option TablePartConfigurator::GetCorregisterTableEditorConfig() {
   Option configoption;
   configoption.SetOption(TableEditorKeyOption, CorregisterTableEditorOption);
   std::string hiddencolums = CoregisterGcpDriver::SourceGeometryColumn;
   hiddencolums.append(",");
   hiddencolums.append(CoregisterGcpDriver::DestinationGeometryColumn);
   configoption.SetOption(HiddenColumnsKeyOption, hiddencolums);
   ConfigureGcpColumns(configoption);
   return configoption;
}

/**
 * Metodo que devuelve las opciones de configuracion para la tabla de DNs
 * de elementos raster.
 */
Option TablePartConfigurator::GetRasterDnTableEditorConfig() {
   Option configoption;
   configoption.SetOption(TableEditorKeyOption, RasterDnTableEditorOption);
   return configoption;
}

/**
 * Metodo que devuelve las opciones de configuracion para la tabla bandas.
 */
Option TablePartConfigurator::GetBandTableEditorConfig() {
   Option configoption;
   configoption.SetOption(TableEditorKeyOption, BandTableEditorOption);
   return configoption;
}

/**
 * Metodo que devuelve las opciones de configuracion para georreferenciacion.
 */
Option TablePartConfigurator::GetGeoreferenceTableEditorConfig() {
   Option configoption;
   configoption.SetOption(TableEditorKeyOption, GeoreferenceTableEditorOption);
   configoption.SetOption(HiddenColumnsKeyOption,
                          GeoreferenceGcpDriver::kGeometryColumn);
   ConfigureGcpColumns(configoption);
   return configoption;
}

/** metodo auxiliar que conigura las columnas para los dirvers gcps**/
void TablePartConfigurator::ConfigureGcpColumns(Option& ConfigOption) {
   std::string readonlycolumns = GcpDriver::XErrorColumnName;
   readonlycolumns.append(",");
   readonlycolumns.append(GcpDriver::YErrorColumnName);
   readonlycolumns.append(",");
   readonlycolumns.append(GcpDriver::XmodelColumnName);
   readonlycolumns.append(",");
   readonlycolumns.append(GcpDriver::YmodelColumnName);
   readonlycolumns.append(",");
   readonlycolumns.append(GcpDriver::CuadraticErrorColumnName);

   ConfigOption.SetOption(RoColumnsKeyOption, readonlycolumns);
}
} /** namespace suri */
