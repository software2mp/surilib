/** \file HtmlReportWidget.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
