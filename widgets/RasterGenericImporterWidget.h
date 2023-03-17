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
