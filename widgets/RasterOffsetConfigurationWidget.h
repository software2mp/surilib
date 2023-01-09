/*! \file RasterOffsetConfigurationWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
