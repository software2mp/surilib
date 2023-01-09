/*! \file wxToggleBitmapButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXTOGGLEBITMAPBUTTON_H_
#define WXTOGGLEBITMAPBUTTON_H_

// Includes de wx
#include "wx/wx.h"

/** declara evento toogle como evento click */
#if defined(wxUSE_TOGGLEBTN) && defined(__WXGTK__)
#  include "wx/tglbtn.h"
#else
#  ifdef wxEVT_COMMAND_TOGGLEBUTTON_CLICKED
#     error "Evento wxEVT_COMMAND_TOGGLEBUTTON_CLICKED ya definido, hay conflicto"
#  else
#     define wxEVT_COMMAND_TOGGLEBUTTON_CLICKED wxEVT_COMMAND_BUTTON_CLICKED
#  endif

/** Clase que implementa un wxBitmapButton que soporta 2 estados. */
/**
 * Esta es la clase que implementa un wxBitmapButton que soporta
 * 2 estados posibles.
 * Por el momento soporta solo botones de 25x25 pixels, e oconos de hasta 20x20
 */
class wxToggleBitmapButton : public wxBitmapButton
{
public:
   /** Contstructor del boton */
   wxToggleBitmapButton(wxWindow* pToolbar, wxWindowID pId, const wxBitmap& pBmp,
         wxCoord X=5, wxCoord Y=5);
   /** Destructor del boton. Atencion que dado que hereda de un wxWindow no debe ser detruido por */
   // codigo sino que lo hace WX
   virtual ~wxToggleBitmapButton();
   /** Cambia el estado del boton */
   void SetValue(bool state);
   /** Devuelve TRUE si el boton esto presionado, FALSE sino */
   bool GetValue();

private:
   /** Evento asociado a wxEVT_LEFT_DOWN que cambia el estado del boton y actualiza la visualiz. */
   void OnLeftDown(wxCommandEvent& Event);
   /** Evento asociado a wxUI_UPDATE que cambia dibujo del boton */
   void OnUIUpdate(wxUpdateUIEvent& Event);
   /** Evento asociado a wxUI_UPDATE que cambia dibujo del boton */
   void OnLeaveWindow(wxMouseEvent& Event);
   /** Evento asociado a wxUI_UPDATE que cambia dibujo del boton */
   void OnButtonClicked(wxCommandEvent& Event);
   /** Dibuja el icono correspondiente (solo si esta presionado, de acuerdo a si esto "Selected" */
   // (tiene el foco) o no
   wxBitmap DrawIcon(bool Selected);

   bool pressed_; /*! bool que indica cual de los dos estados tiene el boton. */
   /* TRUE=presionado, FALSE=no presionado */
   wxBitmap bmp_; /*! wxbitmap que indica el ocono a dibujar sobre el boton */
   wxCoord x_; /*! posicion (left) a partir de la cual dibujar el icono */
   /* sobre el boton. Se puede jugar con este valor para que */
   /* quede centrado */
   wxCoord y_; /*! posicion (upper) a partir de la cual dibujar el icono */
   /* sobre el boton. Se puede jugar con este valor para que */
   /* quede centrado */
   bool enabled_; /*! Recuerda si el boton esta activo */
   bool cliked_; /*! informa si se hizo click antes de salir de la ventana */
};

#endif   // wxUSE_TOGGLEBTN

#endif /*WXTOGGLEBITMAPBUTTON_H_*/
