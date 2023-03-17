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

#ifndef RASTEROFFSETCONFIGURATIONWIDGET_H_
#define RASTEROFFSETCONFIGURATIONWIDGET_H_



// Includes Estandar
// Includes Suri

#include <suri/DialogWidget.h>

class wxCommandEvent;

// Includes Wx
// Defines
// forwards

namespace suri {
// forward
class RasterOffsetConfigurationWidgetHandler;

class RasterOffsetConfigurationWidget : public suri::DialogWidget {
public:
   /** Estructura con la informacion de offset **/
   struct RasterOffsetInfo {
      struct OffsetInfo {
         OffsetInfo() : headerOffset_(0), tailOffset_(0) { }

         int headerOffset_;
         int tailOffset_;

         bool operator ==(const OffsetInfo& B) {
            return headerOffset_ == B.headerOffset_ && tailOffset_ == B.tailOffset_;
         }

         bool operator !=(const OffsetInfo& B) {
            return !(*this == B);
         }
      };

      OffsetInfo fileOffset_;
      OffsetInfo bandOffset_;
      OffsetInfo LineOffset_;

      bool operator ==(const RasterOffsetInfo& B) {
         return fileOffset_ == B.fileOffset_
               && bandOffset_ == B.bandOffset_
               && LineOffset_ == B.LineOffset_;
      }

      bool operator !=(const RasterOffsetInfo& B) {
         return !(*this == B);
      }
   };
   /** Constructor ***/
   explicit RasterOffsetConfigurationWidget(RasterOffsetInfo& OffsetInfo);
   virtual ~RasterOffsetConfigurationWidget();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();

private:
   friend class RasterOffsetConfigurationWidgetHandler;
   RasterOffsetConfigurationWidgetHandler* pEventHandler_;
   RasterOffsetInfo& offsetInfo_;
   /** Metodo que se llama cuando se presiona Cancel **/
   void OnCancelClick(wxCommandEvent& Event);
   /** Metodo que se llama cuando se presiona Aceptar **/
   void OnOkClick(wxCommandEvent& Event);
   /**
    * Metodo que carga los controles graficos a partir del offset
    *  que se configuro en la clase
    */
   void LoadGuiControls();
};

} /** namespace suri */

#endif /* RASTEROFFSETCONFIGURATIONWIDGET_H_ */
