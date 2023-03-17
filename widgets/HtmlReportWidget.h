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

#ifndef HTMLREPORTWIDGET_H_
#define HTMLREPORTWIDGET_H_

// Includes standard
#include <string>
#include <map>

// Includes suri
#include "suri/Widget.h"
#include "suri/StatisticsFunctions.h"

namespace suri {

// Forwards
class HtmlViewerPart;
class ReportInterface;

/** Declara clase ContextManagerToolbarEventHandler */
DECLARE_EVENT_OBJECT(HtmlReportWidgetEventHandler);

/**
 * Clase que muestra estadisticas por pantalla con botones para exportar
 * a disco en formatos html y exel.
 */
class HtmlReportWidget : public Widget {
public:
   /** Declara friend la clase de evento ContextManagerToolbarEventHandler */
   FRIEND_EVENT_OBJECT(HtmlReportWidgetEventHandler);
   /** Ctor */
   explicit HtmlReportWidget(const std::string &WindowTitle,
                           const std::string &OutputFormat = "html");
   /** Dtor */
   ~HtmlReportWidget();
   /** Crea la ventana */
   bool CreateToolWindow();
   /** Retorna un bitmap con el icono de la ventana. */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;

   /** Obtiene ventana que despliega html */
   HtmlViewerPart* GetHtmlViewer() const;
   /** Actualiza ventana que despliega html */
   void SetHtmlViewer(HtmlViewerPart *pHtmlViewerPart);

   /** Configura el reporte que a desplegar */
   void SetReport(ReportInterface* pReport);

   /** Evento de guardar a disco */
   void OnSaveToDiskButton();
   /** Configura el titulo de la ventana **/
   void SetWindowTitle(const std::string& Title);

private:
   std::string outputFormat_; /*! Formato de las estadisticas */
   ReportInterface* pReport_; /*! Reporte que hay que mostrar */
   HtmlViewerPart* pHtmlViewerPart_; /*! Part que se usa para desplegar las */
                             /** estadisticas en html */

   /** declara privado un puntero a la clase ContextManagerToolbarEventHandler */
   EVENT_OBJECT_PROPERTY(HtmlReportWidgetEventHandler);
};

}  // namespace

#endif /* HTMLREPORTWIDGET_H_ */
