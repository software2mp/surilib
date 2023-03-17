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

#include "wxToggleBitmapButton.h"

// Includes suri
#include "wxmacros.h"
#include "resources.h"

// Defines
#if !defined(wxUSE_TOGGLEBTN) || !defined(__WXGTK__)

/**
 * Se fija el alto del boton para que no se modifique el layout cuando
 * se presiona/des-presiona el boton.
 * Nota: En la nueva version de wx, existe un boton toggle que no tendria
 * este problema.
 */
#define __FIX_TOGGLE_BITMAP_BUTTON_HEIGHT__

/** Contstructor del boton */
/**
 * Inicializa ventana y conecta los eventos de wxEVT_LEFT_DOWN y wxEVT_UPDATE_UI
 * @param[in] pToolbar lugar donde se agrega el boton
 * @param[in] pId identificador del boton
 * @param[in] pBmp bitmap que se va a mostrar
 * @param[in] X posicion (left) a partir de la cual dibujar el icono sobre el boton.
 * @param[in] Y posicion (upper) a partir de la cual dibujar el icono sobre el boton.
 */
wxToggleBitmapButton::wxToggleBitmapButton(wxWindow* pToolbar, wxWindowID pId,
      const wxBitmap& pBmp, wxCoord X, wxCoord Y) :
wxBitmapButton(pToolbar, pId, pBmp, wxDefaultPosition, wxSize(-1,
#ifdef __FIX_TOGGLE_BITMAP_BUTTON_HEIGHT__
                                                        pBmp.GetHeight() + 8) )
#else
                                                        -1) )
#endif
{
   bmp_ = pBmp;

   // Por defecto tiene AUTODRAW (usa el graficador de botones estandar del SO)
   SetWindowStyle(wxBU_AUTODRAW);
   // Setea el icono
   SetBitmapLabel(bmp_);
   // Setea el estado como No presionado
   pressed_=false;
   // Setea la posician upper left del acono
   x_=X;
   y_=Y;
   enabled_=true;

   // Conecta el evento encargado de cambiar el estado del botan ante un click
   Connect(wxEVT_LEFT_DOWN,
         wxCommandEventHandler(wxToggleBitmapButton::OnLeftDown), NULL);

   Connect(wxEVT_LEAVE_WINDOW,
         wxMouseEventHandler(wxToggleBitmapButton::OnLeaveWindow), NULL);

   Connect(wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(wxToggleBitmapButton::OnButtonClicked), NULL);

   Connect(wxEVT_UPDATE_UI,
         wxUpdateUIEventHandler(wxToggleBitmapButton::OnUIUpdate), NULL);

   cliked_=false;
}

/** Destructor del boton. */
/**
 * \attention dado que hereda de un wxWindow no debe ser detruido por codigo
 *            sino que lo hace WX.
 */
wxToggleBitmapButton::~wxToggleBitmapButton()
{
}

/** Cambia el estado del boton */
void wxToggleBitmapButton::SetValue(bool State)
{
   // Si el boton esta presionado lo despresiona y setea el AUTODRAW
   if ( !State )
   {
      SetWindowStyle(wxBU_AUTODRAW);
      SetBitmapLabel(bmp_);
      SetBitmapFocus(bmp_);
      Update();
      Refresh();
      pressed_=false;
   }
   // Si el botan esta no presionado lo presiona y grafica el acono presionado
   else
   {
      // COMENTADO PORQUE SE HACE EN ONUIUPDATE
      SetWindowStyle(0);// NO AUTODRAW
      // Dibuja el fondo presionado, luego el acono
      wxBitmap bmpdeselected = DrawIcon(false);
      // Dibuja el fondo presionado, el recuadro de foco, luego el acono
      wxBitmap bmpselected = DrawIcon(true);
      SetBitmapLabel(bmpdeselected);
      SetBitmapFocus(bmpselected);
      Update();
      Refresh();
      pressed_=true;
   }
}

/** Devuelve el estado del boton */
/**
 * @return true el boton esta presionado
 * @return false el boton NO esta presionado
 */
bool wxToggleBitmapButton::GetValue()
{
   // Devuelve el estado del botan
   return pressed_;
}

/** Cambia el estado del boton y actualiza la visualizacion */
/**
 * @param[in] Event evento de mouse cuando se presiona el boton izquierdo
 */
void wxToggleBitmapButton::OnLeftDown(wxCommandEvent& Event)
{
   // Cambia el estado
   SetValue(!GetValue() );
   cliked_=true;
   // Permite disparar otros eventos conectados al botan
   Event.Skip();
}

/** Cambia dibujo del boton */
/**
 * @param[in] Event evento que se genera cuando se actualiza la ui
 */
void wxToggleBitmapButton::OnUIUpdate(wxUpdateUIEvent& Event)
{
   if ( pressed_ && IsEnabled()!=enabled_ )
   {
      SetWindowStyle(0);  // NO AUTODRAW
      // Dibuja el fondo presionado, luego el acono
      wxBitmap bmpdeselected = DrawIcon(false);
      // Dibuja el fondo presionado, el recuadro de foco, luego el acono
      wxBitmap bmpselected = DrawIcon(true);
      SetBitmapLabel(bmpdeselected);
      SetBitmapFocus(bmpselected);
      Update();
      Refresh();// Permite disparar otros eventos conectados al botan
      enabled_=IsEnabled();
   }
   Event.Skip();
}

/** Evento asociado a wxUI_UPDATE que cambia dibujo del boton */
void wxToggleBitmapButton::OnLeaveWindow(wxMouseEvent& Event)
{
   /* \note la condicion Event.LeftIsDown() se agrega ya que cuando se esta
    * utilizando la herramienta de Mapa sucede lo siguiente:
    *  * Se activa alguno de los botones de la herramienta.
    *  * Se dispara una renderizacion.
    *  * Mientras se estan renderizando los elementos, se ejecuta el metodo
    *  Progress::Update, el cual desactiva todas las ventanas de la aplicacion.
    *  Al desactivar todas las ventanas hace que el boton de la herramienta de
    *  Mapa pierda el foco con lo cual se dispara un evento de OnLeaveWindow que
    *  en este caso invierte el estado del boton.
    *  * Resultado: los botones de la herramienta de mapa no quedaban presionados,
    *  a pesar de que mostraban por pantalla los elementos.
    */
   if ( cliked_ && Event.LeftIsDown() )
   {
      cliked_=false;
      // Cambia el estado
      SetValue(!GetValue() );
   }
}

/** Evento asociado a wxUI_UPDATE que cambia dibujo del boton */
void wxToggleBitmapButton::OnButtonClicked(wxCommandEvent& Event)
{
   cliked_=false;
   Event.Skip();
}

/**
 * Dibuja el icono correspondiente (solo para estado presionado, de
 * acuerdo a si esta "Selected" (tiene el foco) o no
 * @param[in] Selected informa si el boton esta seleccionado o no.
 */
wxBitmap wxToggleBitmapButton::DrawIcon(bool Selected)
{
   wxBitmap bmpbackground;
   wxMemoryDC* pdc = new wxMemoryDC();

   // Toma como bitmap base el fondo
   GET_BITMAP_RESOURCE(wxT(tool_TOGGLEBUTTON_TOGGLED), bmpbackground);
   wxImage imgbackground = bmpbackground.ConvertToImage();
   imgbackground.Rescale(bmp_.GetWidth() + 8, bmp_.GetHeight() + 8, wxIMAGE_QUALITY_HIGH);
   bmpbackground = wxBitmap(imgbackground);
   pdc->SelectObject(bmpbackground);

   // Dibuja el rectangulo de foco
   if ( Selected )
   {
      wxBitmap bmpselected;
      GET_BITMAP_RESOURCE(wxT(tool_TOGGLEBUTTON_SELECTED), bmpselected);
      wxImage imgselected = bmpselected.ConvertToImage();
      imgselected.Rescale(bmp_.GetWidth() + 8, bmp_.GetHeight() + 8, wxIMAGE_QUALITY_HIGH);
      pdc->DrawBitmap(wxBitmap(imgselected), 3, 3, true);
   }

   // Dibuja el acono semitransparente usando el Alpha channel a la mitad
   if ( !this->IsEnabled() )
   {
      wxImage img = bmp_.ConvertToImage();
      if ( !img.HasAlpha() ) {
         img.InitAlpha();
      }
      int imgw=img.GetWidth();
      int imgh=img.GetHeight();
      for ( int i=0; i<imgw; i++ )
      {
         for ( int j=0; j<imgh; j++ )
         {
            if ( img.GetRed(i, j)!=img.GetMaskRed() || img.GetGreen(i, j)
                  !=img.GetMaskGreen() || img.GetBlue(i, j)!=img.GetMaskBlue() ) {
               img.SetAlpha(i, j, 128);
            }
         }
      }

      pdc->DrawBitmap(wxBitmap(img), x_, y_, true);
   } else
   {
      pdc->DrawBitmap(bmp_, x_, y_, true);
   }

   delete pdc;
   return bmpbackground;
}

#endif   // wxUSE_TOGGLEBTN
