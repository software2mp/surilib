/*! \file PixelVectorButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef PIXELVECTORBUTTON_H_
#define PIXELVECTORBUTTON_H_

#include "VectorEditorButton.h"

// Forwards
class VectorElementEditor;

/** namespace suri */
namespace suri {
// Forwards
class ViewerWidget;

/** Dibuja vectores con coordenadas que obtiene de eventos de boton */
/**
 * Obtiene la posicion en el mundo del pixel sobre el que se hizo click.
 * Transforma las coordeandas al sistema del vector. Le pasa estas coordenadas
 * a VectorElementEditor para agregar puntos a elemento.
 * Segun que tipo de VectorElementEditor se pasa en el constructor el tipo de
 * vector que crea el boton sera diferente.
 */
class PixelVectorButton : public VectorEditorButton {
   /** Ctor. de Copia. */
   PixelVectorButton(const PixelVectorButton &PixelVectorButton);
public:
   /** Ctor. default */
   PixelVectorButton(wxWindow *pToolbar, ViewerWidget *pViewer, const wxString &Bitmap,
                     VectorElementEditor* pVectorElementEditor, wxString ButtonTooltip,
                     int Flags);
   /** Ctor. default */
   PixelVectorButton(wxWindow *pToolbar, ViewerWidget *pViewer, const wxString &Bitmap,
                      GuiGeometryCreator::GeometryType GeoType, wxString ButtonTooltip,
                      ViewportManagerInterface* pViewportManager = NULL,
                      ViewcontextManagerInterface* pViewcontextManager = NULL,
                      int Flags = BUTTON_NULL);
   /** Indica si se encuentra activo el editor **/
   virtual bool IsActive() const;

protected:
   /** Setea el estado del boton. */
   virtual bool DoSetValue(bool State);
   /** Metodo ejecutado al levantar/soltar el boton izquierdo del mouse. */
   virtual void DoOnLeftUp(wxMouseEvent &Event);
   /** Metodo ejecutado al levantar/soltar el boton derecho del mouse. */
   virtual void DoOnRightUp(wxMouseEvent &Event);
   /** Elimina el ultimo punto agregado al feature. */
   virtual void DoOnMiddleUp(wxMouseEvent &Event);
   /** Tipo de geometria sobre la cual se trabaja. */
   GuiGeometryCreator::GeometryType geoType_;
   /** Visualizador de datos. */
   DataViewManager* pDataViewManager_;
};
}  // namespace suri

#endif /*PIXELVECTORBUTTON_H_*/
