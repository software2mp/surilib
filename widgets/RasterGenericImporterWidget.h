/*! \file RasterGenericImporterWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERGENERICIMPORTERWIDGET_H_
#define RASTERGENERICIMPORTERWIDGET_H_

// Includes Estandar
#include <string>
#include <map>
// Includes Suri
#include "suri/DialogWidget.h"
#include "RasterOffsetConfigurationWidget.h"
#include "suri/Option.h"

// Includes Wx
// Defines
// forwards
class wxFileDirPickerEvent;

namespace suri {
// forwards
class RasterGenericImporterWidgetHandler;
class DatasourceManagerInterface;

/**
 * Clase que representa el importador generico raster. Es el responsable
 * de generar el header correspondiente para archivos raster de tipo
 * bip, bil y bsq.
 * Para esto se seguira la especificacion de los formatos que se explica en
 * la ayuda de arqgis:
 * http://help.arcgis.com/es/arcgisdesktop/10.0/help/index.html#//009t00000010000000
 * y la especificacion de esri
 * http://downloads.esri.com/support/whitepapers/other_/eximgav.pdf
 * El resultado de la importacion es una fuente de datos raster
 */
class RasterGenericImporterWidget : public suri::DialogWidget {
public:
   /** ctor **/
   explicit RasterGenericImporterWidget(DatasourceManagerInterface* pDsManager);
   /** dtor **/
   virtual ~RasterGenericImporterWidget();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();

private:
   friend class RasterGenericImporterWidgetHandler;
   RasterGenericImporterWidgetHandler* pEventHandler_;
   /** Metodo que se llama cuando se selecciona un nuevo archivo **/
   void OnFileSelectionChange(wxFileDirPickerEvent &Event);
   /** Metodo que se llama cuando se presiona Cancel **/
   void OnCancelClick(wxCommandEvent& Event);
   /** Metodo que se llama cuando se presiona Aceptar **/
   void OnOkClick(wxCommandEvent& Event);
   /** Metodo que se llama cuando se actualiza algun textctrl del widget **/
   void OnFieldUpdate(wxCommandEvent& Event);
   /** Metodo que se llama cuando se presiona el boton de offset **/
   void OnOffsetClick(wxCommandEvent& Event);
   /**
    * Metodo auxiliar que realiza una configuracion sobre los controles graficos
    * del widget
    */
   void ConfigureControls();
   /**
    * Metodo auxiliar que obtiene la informacion de metainformacion
    * que configura el usuario
    */
   void GetMetadataOptions(Option& Metadata);
   /**
    *  Metodo auxiliar que actualiza el size requerido de la imagen en funcion de
    *  la configuracion indicada.
    */
   void UpdateSizeReq();
   std::map<std::string, std::string> keyValue_;
   RasterOffsetConfigurationWidget::RasterOffsetInfo offsetInfo_;
   DatasourceManagerInterface* pDsManager_;
   std::map<std::string, std::string> dataTypeMap_;
};

} /** namespace suri */

#endif /* RASTERGENERICIMPORTERWIDGET_H_ */
