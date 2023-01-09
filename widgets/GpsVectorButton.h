/*! \file GpsVectorButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef GPSTOGGLEBUTTON_H_
#define GPSTOGGLEBUTTON_H_

// Includes suri
#include "suri/Vector.h"

// Includes App
#include "VectorEditorButton.h"

// Defines
/** Flag para indicar que hay que agregar un boton 'marcar' (punto) */
#define BUTTON_MARK                 0x00000001
/** Flag para indicar que hay que agregar un boton 'eliminar' (punto) */
#define BUTTON_DELETE               0x00000002
/** Flag para indicar que hay que agregar un boton 'cerrar' (feature) */
#define BUTTON_CLOSE                0x00000004

// Forwards
class VectorElementEditor;

/** namespace suri */
namespace suri {
// Forwards
class ViewerWidget;
class MarkButton;
class DeleteButton;
class CloseButton;
class DataViewManager;

/** Clase para botones de la herramienta GPS. */
/**
 * Cuando se activa/desactiva el boton carga/saca los botones en NavigationWidget
 * \todo pasar toda la funcionalidad a VectorEditorButton.
 */
class GpsVectorButton : public VectorEditorButton {
public:
   /** ctor */
   GpsVectorButton(wxWindow *pToolbar, ViewerWidget *pViewer, const wxString &Bitmap,
                   VectorElementEditor* pVectorElementEditor,
                   DataViewManager* pDataViewManager = NULL,
                   wxString ButtonTooltip =
                         wxT(tooltip_BUTTON),
                   int Flag = BUTTON_MARK | BUTTON_DELETE | BUTTON_CLOSE);
   /** dtor */
   virtual ~GpsVectorButton();
   /** Setea el estado del boton. Ingresa/Saca boton de NavigationWidget. */
   virtual bool DoSetValue(bool State);

protected:
   /** Elimina los botones del NavigationWidget */
   virtual void RemoveButtonsFromNavigationWidget();

private:
   MarkButton* pMarkButton_; /*! Boton que permite agregar puntos */
   DeleteButton* pDeleteButton_; /*! Boton que elimina el ultimo punto agregado */
   CloseButton* pCloseButton_; /*! Boton que cierra vector */
   /*! informa que botones deberan mostarse en NavigationWidget cuando se active boton */
   int flag_;
   /** administrador de info de la aplicacion **/
   DataViewManager* pDataViewManager_;
};
}  // namespace suri

#endif /*GPSTOGGLEBUTTON_H_*/
