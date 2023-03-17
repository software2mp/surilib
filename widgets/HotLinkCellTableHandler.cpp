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
// Includes Suri
#include "HotLinkCellTableHandler.h"
#include "suri/Table.h"
#include "suri/Extent.h"
#include "suri/PartContainerWidget.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

// Includes Wx
// Defines
// forwards

// Includes ogr
#include "ogr_geometry.h"
#include "ogr_feature.h"
#include "ogrsf_frmts.h"
#include "wx/mimetype.h"
namespace suri {

HotLinkCellTableHandler::HotLinkCellTableHandler() {
}

HotLinkCellTableHandler::~HotLinkCellTableHandler() {
}

/**
 * Metodo que se encarga del manejo de click sobre la celda. Para el caso del handler
 * de hotlink, este se activa realizando ctrl+click en vez de click
 * @param rowNumber : fila a la que partenece dicha celda
 * @param colNumber : columna a la que pertenece dicha celda
 * @param pGrid : puntero que permite el formateo de las grillas, en caso de que se necesite
 * @param pTable : puntero que permite extraer informacion de la tabla en caso de ser necesario
 * @param KeyDown: Tecla presionada al momento del click
 */
bool HotLinkCellTableHandler::DoHandle(int rowNumber, int colNumber,
                                     wxGrid* pGrid, Table* pTable,
                                     KeyDown KeyDown, MouseEvent MEvent) {
	if ( KeyDown != suri::CellTableHandler::CTRL || MEvent != CellTableHandler::LCLICK)
		return false;
	// Actua segun el evento de click sobre la celda
	std::string cellvalue;
   /** Sumo uno porque las columnas empiezan de 1 en este caso.
    *  TCK #4947 */
   pTable->GetCellValue((colNumber+1), rowNumber, cellvalue);
   DisplayHotLink(cellvalue);
   pGrid->SetCellTextColour(rowNumber, (colNumber+1), *wxRED);
   return true;
	}

void HotLinkCellTableHandler::DisplayHotLink(const std::string &Url) {
   ///wxFileDialog dlg(NULL);
   ///dlg.ShowModal();
   if (Url.find("http") != std::string::npos) {
      wxLaunchDefaultBrowser(Url.c_str());
      return;
   }
   wxFileName fnhotlink(Url.c_str());
   if (!fnhotlink.FileExists()) {
      // SHOW_ERROR(message_NO_FILE_s ,Url.c_str());
      return;
   }
   // Verifico que tenga extension
   wxString hotlinkextention = fnhotlink.GetExt();
   /**
    * Creo mime type.
    * La documentacion de wx recomiendan usar la instancia global de
    * wxMimeTypesManager.
    */
   wxFileType *photlinkmimetype;
   if (!hotlinkextention.IsEmpty()) {
      photlinkmimetype = wxTheMimeTypesManager->GetFileTypeFromExtension(
            hotlinkextention);
   } else {  // Si el archivo no tiene extension uso el mime por defecto
      photlinkmimetype = wxTheMimeTypesManager->GetFileTypeFromMimeType(
            wxString("text/plain"));
   }

   if (!photlinkmimetype) {
      // SHOW_ERROR(message_UNKNOW_EXTENTION_s, Url.c_str());
      return;
   }

   wxString filepath = fnhotlink.GetFullPath();
#ifdef __WXGTK__
   // Ejecuto app
   /* Nota: hay un posible bug en el metodo GetOpenCommand cuando el path del
    * archivo que recibe tiene espacios ("C:\mi imagen.jpg"). Para corregirlo
    * se agregan comillas para poder escapar los espacios.
    * No se agregan comillas en windows porque genera comandos que no son
    * ejecutables (y no esta el problema de los nombres con espacio en blanco)
    * Ver http://trac.wxwidgets.org/ticket/4607
    * */
   if (filepath.Find(wxT(" ")) != wxNOT_FOUND) {
      filepath = wxT("\"") + filepath + wxT("\"");
   }
#endif
   wxString commandstr = photlinkmimetype->GetOpenCommand(filepath);
#ifdef __WXGTK__
   if (commandstr.StartsWith(wxT("\"\"")) || commandstr.EndsWith(wxT("\"\""))) {
      commandstr = photlinkmimetype->GetOpenCommand(fnhotlink.GetFullPath());
   }
#endif
   if (!commandstr.IsEmpty()) {
      wxExecute(commandstr);
   } else {
      SHOW_ERROR(message_NO_APP_ERROR_s, Url.c_str());
   }

   delete photlinkmimetype;
}


} /** namespace suri */
