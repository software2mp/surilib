/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXMACROS_H_
#define WXMACROS_H_

// Includes suri
#include "logmacros.h"

// Defines
// ------------------------------------------------------------------------------
#ifndef GET_CONTROL
/** Para llamar un metodo de los controles del XRC */
#define GET_CONTROL(Parent, ControlId, ControlType)                             \
   if ( XRCCTRL(Parent, ControlId, ControlType) )                                 \
      XRCCTRL(Parent, ControlId, ControlType)
#endif

// ------------------------------------------------------------------------------
#ifndef USE_CONTROL
#  include "wx/xrc/xmlres.h"
/** Para llamar un metodo, con valor de retorno, de los controles del XRC */
#define USE_CONTROL(Parent, ControlId, ControlType, Member, FailValue)            \
   ( XRCCTRL(Parent, ControlId, ControlType) ?                                   \
     XRCCTRL(Parent, ControlId, ControlType)->Member : FailValue )
#endif
// ------------------------------------------------------------------------------

#ifndef GET_BITMAP_RESOURCE
#include "wx/filesys.h"
#include "wx/artprov.h"
/** Flag que impide que se esten registrando cada vez que se llama la macro */
extern "C" bool wxImageHandlersInitialized;
/** Obtiene un bitmap almacenado en el filesyste en memoria */
#  define GET_BITMAP_RESOURCE(Url, Bitmap) {                               \
      if ( !wxImageHandlersInitialized ) {                                    \
         wxInitAllImageHandlers();                                            \
         wxImageHandlersInitialized = true; }                                 \
      wxFSFile *pfsfile = wxFileSystem().OpenFile(Url, wxFS_READ|wxFS_SEEKABLE);\
      if ( pfsfile == NULL )                                                  \
      {                                                                       \
         if (!std::string(Url).empty())                                        \
            SHOW_ERROR(message_CANT_CREATE_BITMAP_s, Url);                    \
         Bitmap = wxArtProvider::GetBitmap(wxART_MISSING_IMAGE);              \
      }                                                                       \
      else                                                                    \
      {                                                                       \
         wxImage img(*(pfsfile->GetStream() ) );                              \
         Bitmap = wxBitmap(img);                                              \
      }                                                                       \
      delete pfsfile;                                                         \
}
#endif

#ifndef ESCALE_BITMAP
#include "wx/filesys.h"
#include "wx/artprov.h"
/** Cambia tamanio de bitmap */
#  define ESCALE_BITMAP(Bitmap, Width, Height) {                           \
      if ( Bitmap.GetWidth()!=Width || Bitmap.GetHeight()!=Height )           \
      {                                                                       \
         wxImage img = Bitmap.ConvertToImage();                               \
         img.Rescale(Width, Height, wxIMAGE_QUALITY_HIGH);                    \
         Bitmap = wxBitmap(img);                                              \
      }                                                                       \
}
#endif

#define SHOWQUERY(wxWindow, Query, ...)                                       \
   wxMessageDialog(wxWindow, Query,                                           \
                   suri::Configuration::GetParameter("app_short_name",        \
                                                     ""), ## __VA_ARGS__).ShowModal()

// ------------------------------------------------------------------------------
#ifndef CREATE_EVENT_OBJECT
#define CREATE_EVENT_OBJECT
// ------------------------------------------------------------------------------
/** Declara el objeto de eventos para los que heredan de CBaseTool */
#define DECLARE_EVENT_OBJECT(ClassName) class ClassName
// ------------------------------------------------------------------------------
#define EVENT_OBJECT_PROPERTY(ClassName) ClassName *pEventHandler_

// ------------------------------------------------------------------------------
#define FRIEND_EVENT_OBJECT(ClassName) friend class ClassName

// ------------------------------------------------------------------------------
/** Implementa el objeto de eventos */
#define START_IMPLEMENT_EVENT_OBJECT(ClassName, ParentType)                    \
   class ClassName : public wxEvtHandler                                      \
   {                                                                          \
private:                                                                   \
      ParentType *pParent_;      /*Herramienta padre*/                           \
public:                                                                    \
      ClassName(ParentType *pParent) : pParent_(pParent) {}                      \
      virtual ~ClassName() {REPORT_DEBUG("D: Destruccion " # ClassName "(%p)",  \
                                         this); pParent_=NULL; }

// ------------------------------------------------------------------------------
/** Implementa los callbacks del objeto de eventos */
#define IMPLEMENT_EVENT_CALLBACK(Name, CallbackFunction, CallbackEventType)     \
   void Name(CallbackEventType& event)                                        \
   {if ( !pParent_ ) { return; } event.Skip(); pParent_->CallbackFunction; }

// ------------------------------------------------------------------------------
/** Cierra la implementacion del objeto de eventos */
#define END_IMPLEMENT_EVENT_OBJECT };

// ------------------------------------------------------------------------------
#define NEW_EVENT_OBJECT(ClassName) pEventHandler_(new ClassName(this) )

// ------------------------------------------------------------------------------
#define DELETE_EVENT_OBJECT REPORT_DEBUG("D: Elimino EventHandler(%p)",       \
                                            pEventHandler_); delete pEventHandler_; pEventHandler_= \
   NULL

// ------------------------------------------------------------------------------
// Deprecada: Usar llamada directa a Connect

#define CONNECT_TOOL_EVENT(ClassName, HandlerFunction, wxEventId,               \
                              wxEventTypeHandler)  \
   if ( pToolWindow_ ) {                                                        \
      pToolWindow_->Connect(wxEventId,                                        \
                            wxEventTypeHandler(ClassName::HandlerFunction), NULL, pEventHandler_); } \
   else                                                                       \
      REPORT_DEBUG("D:Error al conectar el evento, pToolWindow_==NULL")

// ------------------------------------------------------------------------------
// Deprecada: Usar llamada directa a Disconnect
#define DISCONNECT_TOOL_EVENT(ClassName, HandlerFunction, wxEventId,            \
                                 wxEventTypeHandler)  \
   if ( pToolWindow_ ) {                                                        \
      pToolWindow_->Disconnect(wxEventId,                                     \
                               wxEventTypeHandler(ClassName::HandlerFunction), NULL, pEventHandler_); } \
   else                                                                       \
      REPORT_DEBUG("D:Error al desconectar el evento, pToolWindow_==NULL")

#endif  // CREATE_EVENT_OBJECT
// ------------------------------------------------------------------------------
#ifndef WS2S
/** Convierte un wxString a un std::string */
#define WS2S(as) (std::string((as).mb_str(wxConvUTF8)))
#endif

#endif   // WXMACROS_H_
