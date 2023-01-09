/*! \file NavigationInputWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef NAVIGATIONINPUTWIDGET_H_
#define NAVIGATIONINPUTWIDGET_H_

// Includes suri
#include "suri/Widget.h"
#include "suri/Coordinates.h"
#include "suri/CoordinatesNavigationInputInterface.h"

/** namespace suri */
namespace suri {
class ViewerWidget;

/** Interfaz que deben cumplir las herramientas que ingresan coordenadas */
class NavigationInputWidget : public Widget, public CoordinatesNavigationInputInterface {
public:
   /** Ctor */
   NavigationInputWidget() {
   }
   /** Dtor */
   virtual ~NavigationInputWidget() {
   }

   /** Creacion de la ventana */
   virtual bool CreateToolWindow()=0;
   /** Titulo de la ventana (en caso de pParent!=NULL */
   virtual const wxString GetWindowTitle() const=0;
   /** Retorna la ventana asociada a la herramienta */
   virtual wxWindow *GetToolWindow() const {
      return pToolWindow_;
   }
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool GetCoordinate(Coordinates &Coordinate)=0;
   /** Actualiza el parent */
   virtual void SetParent(wxWindow *pParent) {
      pParentWindow_ = pParent;
   }
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool SetCoordinate(Coordinates Coordinate)=0;
   virtual Widget* GetInputWidget() {
      return this;
   }
};
}

#endif /* NAVIGATIONINPUTWIDGET_H_ */
