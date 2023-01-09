/*! \file XrcWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef XRCWIDGET_H_
#define XRCWIDGET_H_

// Includes standard

// Includes Suri
#include "suri/Widget.h"

// Includes Wx

// Includes App

// Defines

namespace suri {
class Part;

/** Widget que se configura con XRC. Da una interfaz generica para acceder a controles */
/**
 * Configura la interfaz del widget a partir de un XRC. Da una interfaz
 * para el manejo de los valores en los controles que contiene(permite abstraerse de la
 * interfaz de wx).
 * La pantalla que despliegue XrcWidget cambiara en cada instancia,
 * dependiendo del XRC que se le pase.
 */
class XrcWidget : public Widget {
public:
   /** Ctor */
   XrcWidget(Part *pParentPart, const std::string &XrcId,
             const std::string &WidgetTitle);
   /** Dtor */
   virtual ~XrcWidget();

   /** Crea la ventana usando el xrc */
   virtual bool CreateToolWindow();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
protected:
private:
   Part *pParentPart_; /*! Part que se usa para configurar el Widget despues de crearlo */
   std::string xrcId_; /*! Nombre del part que quiero crear. */
};

/** Retorna el contenido del control como string */
bool GetCtrlValue(Widget* pWidget, const std::string &ControlName, std::string &Value);
/** Retorna el contenido del control como entero */
bool GetCtrlValue(Widget* pWidget, const std::string &ControlName, int &Value);
/** Retorna el contenido del control como double */
bool GetCtrlValue(Widget* pWidget, const std::string &ControlName, double &Value);

/** Modifica el contenido del control con string */
bool SetCtrlValue(Widget* pWidget, const std::string &ControlName,
                  const std::string &NewValue);
/** Modifica el contenido del control con entero */
bool SetCtrlValue(Widget* pWidget, const std::string &ControlName, int NewValue);
/** Modifica el contenido del control con double */
bool SetCtrlValue(Widget* pWidget, const std::string &ControlName, double NewValue);

}  // namespace suri

#endif /* XRCWIDGET_H_ */
