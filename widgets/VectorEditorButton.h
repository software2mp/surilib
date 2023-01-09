/*! \file VectorEditorButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTOREDITORBUTTON_H_
#define VECTOREDITORBUTTON_H_

// Includes suri
#include "suri/ToggleButton.h"
#include "suri/Subject.h"
#include "VectorEditionComponentInterface.h"

// Includes app
#include "VectorElementEditor.h"
#include "GeometryElementEditor.h"

/** namespace suri */
namespace suri {

// forwards
class ViewportManagerInterface;
class ViewcontextManagerInterface;

/** Clase que heredan los botones que desean editar vectores. */
/**
 * Clase que se crea para unir la funcionalidad de PixelVectorButton y
 * GPSVectorButton. Sirve para que la clase DrawTool (de donde heredan todas las
 * herramientas de edicion de vectores) use botones genericos.
 * \todo Esta clase tiene que incluir la funcionalidad para agregar puntos,
 *       eliminar puntos y cerrar geometrias, y las clases que heredan definen
 *       como llaman a estos metodos (PixelVectorButton los llama ante eventos
 *       de mouse, GPSVectorButton los llama ante eventos en el widget de
 *       ingreso de coordenadas).
 */
class VectorEditorButton : public ToggleButton,
                              public VectorEditionComponentInterface,
                              public Subject {
public:
   /** Ctor. */
   VectorEditorButton(wxWindow *pToolbar, ViewerWidget *pViewer, const wxString &Bitmap,
                      VectorElementEditor* pVectorElementEditor, wxString ButtonTooltip,
                      int Flags = BUTTON_NULL);
   /** Ctor. default */
   VectorEditorButton(wxWindow *pToolbar, ViewerWidget *pViewer, const wxString &Bitmap,
                      GuiGeometryCreator::GeometryType GeoType, wxString ButtonTooltip,
                      ViewportManagerInterface* pViewportManager = NULL,
                      ViewcontextManagerInterface* pViewcontextManager = NULL,
                      int Flags = BUTTON_NULL);

   /** Dtor. */
   virtual ~VectorEditorButton();
   /** Retorna el VectorElementEditor que usa el boton */
   virtual VectorElementEditor *GetVectorElementEditor();
   /** Retorna el VectorElementEditor que usa el boton */
   virtual GeometryElementEditor* GetGeometryElementEditor();

   /** Indica si se encuentra activo el editor **/
   virtual bool IsActive() const { return false;}
   /** Metodo que se llama cuando se inicializa la edicion vectorial. **/
   virtual void OnVectorEditionStarted() {
   }
   /**  Metodo que se llama cuando finaliza la edicion vectorial.*/
   virtual void OnVectorEditionFinished() {
   }

protected:
   ViewerWidget* pViewer_; /*! View de donde se obtiene el NavigationWidget */
   /*! VectorElementEditor que se utilza para agregar puntos a los vectores */
   VectorElementEditor* pVectorElementEditor_;
   GeometryElementEditor* pGeoEditor_;

};
}  // namespace suri

#endif /* VECTOREDITORBUTTON_H_ */
