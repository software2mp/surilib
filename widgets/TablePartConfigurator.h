/*! \file TablePartConfigurator.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TABLEPARTCONFIGURATOR_H_
#define TABLEPARTCONFIGURATOR_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "TablePart.h"
#include "PermissionList.h"
#include "suri/DriverInterface.h"
#include "suri/DataViewManager.h"
#include "suri/Option.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Configurador de TablePart
 */
class TablePartConfigurator {
public:
   /** Opcion para edicion vectorial */
   static const std::string VectorTableEditorOption;
   /** Opcion de configuracion de editar de tabla **/
   static const std::string TableEditorKeyOption;
   /** Opcion para driver de corregistro **/
   static const std::string CorregisterTableEditorOption;
   static const std::string TablePartFileNameKey;

   static const std::string RasterDnTableEditorOption;

   static const std::string BandTableEditorOption;

   static const std::string GeoreferenceTableEditorOption;

   static const std::string TableSourceSrs;
   static const std::string TableSourceRasterModel;
   static const std::string TableDestinationSrs;
   static const std::string TableDestinationRasterModel;
   /** Clave para esconder columnas
    *  si se posee mas de una columna a esconder se separa con coma
    *  ej. esconder columna a y columna c => "a,b" **/
   static const std::string HiddenColumnsKeyOption;
   /** Clave para columnas de solo lectura
    *  si se posee mas de una columna se separa con coma
    *  ej. readonly columna a y columna c => "a,b" **/
   static const std::string RoColumnsKeyOption;
   /** Clave para habilitar la consulta en el componente de tabla **/
   static const std::string SearchKeyOption;
   static const std::string VectorDatasourceUrl;
   /** Constructor */
   TablePartConfigurator();
   /** Destructor */
   virtual ~TablePartConfigurator();
   /** Inicializa */
   TablePart* Initialize(DataViewManager* pDataViewManager,
                         const Option& ConfigurationOptions);
   /** Inicializa */
   TablePart* Initialize(DataViewManager* pDataViewManager, Table* pTable,
                         const Option& ConfigurationOptions);
   /** Metodo que devuelve las opciones de configuracion para edicion vectorial **/
   static Option GetVectorTableEditorConfig();
   /** Metodo que devuelve las opciones de configuracion para edicion vectorial **/
   static Option GetCorregisterTableEditorConfig();

   /**
    * Metodo que devuelve las opciones de configuracion para la tabla de DNs
    * de elementos raster.
    */
   static Option GetRasterDnTableEditorConfig();

   /**
    * Metodo que devuelve las opciones de configuracion para la tabla bandas.
    */
   static Option GetBandTableEditorConfig();

   /**
    * Metodo que devuelve las opciones de configuracion para georreferenciacion.
    */
   static Option GetGeoreferenceTableEditorConfig();

private:
   /** Obtiene los comandos de la configuracion */
   std::vector<std::string> RetrieveCommands(const std::string& Configuration);
   /** Obtiene los permisos de la configuracion */
   int RetrievePermissionKey(const std::string& Configuration);
   /** Obtiene el driver de la configuracion */
   DriverInterface* RetrieveDriver(const Option& ParameterOption,
                         DataViewManager* pDataViewManager, Subject* &pSubject);
   /** Metodo auxiliar que obtiene el tipo de tabla correspondiente
    * a la configuracion de la aplicacion**/
   Table* RetrieveTable(DataViewManager* pDataViewManager, const std::string& Parameter);
   /** Obtiene el titulo de la ventana */
   std::string RetrieveWindowTitle(const std::string& Configuration);
   /** Obtiene el path del icono de la ventana */
   std::string RetrieveWindowIcon(const std::string& Configuration);
   /**
    *  Configura las columnas ocultas en TablePart segun el string que se pasa por parametro
    *  El formato del array de columnas es el nombre de las columnas separado por ,
    *  @param ColumnsArray columas a ocultar
    *  @param pTable tabla para la obtencion de indices de columas
    *  @param pTablePart part que posee la visualizacion de la tabla. Es donde
    *  se configura el ocultamiento de la tabla
    */
   void ConfigureHiddenColumns(const std::string& ColumnsArray, Table* pTable,
                               TablePart* pTablePart);
   /** Configura las columnas de solo lectura
    *  El formato del array de columnas es el nombre de las columnas separado por ,
    *  @param ColumnsArray columas de solo lectura
    *  @param pTable tabla para la obtencion de indices de columas**/
   void ConfigureReadOnlyColumns(const std::string& RoColumnsArray,
                                 TablePart* pTablePart);
   /** Configura las columnas de la tabla de acuerdo a las opciones de config **/
   void ConfigureColumns(const Option& ConfigurationOptions, Table* pTable,
                         TablePart* pTablePart);
   /** metodo auxiliar que conigura las columnas para los dirvers gcps**/
   static void ConfigureGcpColumns(Option& ConfigOption);
};

} /** namespace suri */

#endif /* TABLEPARTCONFIGURATOR_H_ */
