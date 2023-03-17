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

#ifndef TableImportXyPart_H_
#define TableImportXyPart_H_

// Includes Estandar
// Includes Suri
#include "suri/DialogWidget.h"
#include "LibraryItemEditorPart.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/LibraryItemSelectionInterface.h"
#include "suri/BasicLibraryItemSelector.h"
// Includes Wx
#include "wx/event.h"
// Defines
// forwards

namespace suri {
/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(TableImportXyPartEvent);

class TableImportXyPart : public DialogWidget, public LibraryItemSelectionInterface {
public:
   /** Constructor */
   TableImportXyPart(LibraryManager* pManager, DatasourceManagerInterface* dm);
   /** Destructor */
   virtual ~TableImportXyPart();
   /** Creacion de Ventana  */
   virtual bool CreateToolWindow();
   void SetActiveItem(const LibraryItem* pItem);

private:
   EVENT_OBJECT_PROPERTY(TableImportXyPartEvent);
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(TableImportXyPartEvent);
   /** Evento click sobre Boton Aceptar */
   void OnButtonOk(wxCommandEvent &Event);
   /** Evento click sobre Boton Cancelar */
   void OnButtonCancel(wxCommandEvent &Event);
   /** Evento click sobre RadioButton Tabulacion */
   void OnRadioButtonTab(wxCommandEvent &Event);
   /** Evento click sobre RadioButton Coma */
   void OnRadioButtonComa(wxCommandEvent &Event);
   /** Evento click sobre RadioButton Espacio */
   void OnRadioButtonSpace(wxCommandEvent &Event);
   /** Evento click sobre RadioButton Otro */
   void OnRadioButtonOther(wxCommandEvent &Event);
   /** Maneja el estado del TextCtrl del separador */
   void SetState(bool isEnabled);
   /** Crea las columnas del archivo a importarse. El valor de X e Y es FLOAT mientras */
   // bool CreateColumns(std::string Path, VectorEditionTable* Table);
   /** Establece el valor obtenido del archivo en la celda correspondiente */
   // bool SetValues(std::string Filename, std::string Line, VectorEditionTable* Table);
   /** Obtiene el icono del part */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   /** Agrega la fuente de datos utilizando el driver vrt de gdal */
   // void AddDatasourceWithDriver(std::string Path);
   /** Agrega la fuente de datos parseando el archivo y creando un shapefile */
   // void AddDatasourceWithShapefile(std::string Path);
   /** Retorna el atributo WKT_ATTRIBUTE_NAME del espacio de referencia seleccionado */
   std::string GetSpatialReference();
   /** Retorna el nombre de un archivo a partir de un path */
   std::string GetFilename(std::string Path);
   /** Retorna el caracter de separacion correspondiente al sistema operativo */
   // std::string GetSeparator();
   /** Retorna un string aleatorio de 32 caracteres */
   // std::string GetRandomString();
   /** Contiene el separador */
   std::string token_;
   /** Boolean que indica si se indica un token custom **/
   bool isenabled_;
   /** Controlador de la fuente de datos */
   DatasourceManagerInterface* dm_;
   /** Espacio de referencia */
   std::string spatialReference_;
   LibraryClient* pClient_;
   BasicLibraryItemSelector* pItemSelector_;
};

} /** namespace suri */

#endif /* TableImportXyPart_H_ */
