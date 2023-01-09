/*! \file VectorEditorButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "VectorEditorButton.h"
#include "GeometryElementEditor.h"

/** namespace suri */
namespace suri {
/**
 * Constructor.
 * @param[in] pToolbar lugar donde se agrega el boton
 * @param[in] pViewer viewer donde se editan los botones
 * @param[in] Bitmap icono de la herramienta
 * @param[in] pVectorElementEditor clase que se usa para editar el vector
 * @param[in] ButtonTooltip tooltip del boton
 * @param[in] Flags Mascara que define los eventos de mouse que se conectan sobre
 *            la ventana del visualizador
 * @return instancia de la calse VectorEditorButton
 */
VectorEditorButton::VectorEditorButton(wxWindow *pToolbar, ViewerWidget *pViewer,
                                       const wxString &Bitmap,
                                       VectorElementEditor* pVectorElementEditor,
                                       wxString ButtonTooltip, int Flags) :
      ToggleButton(pToolbar, Bitmap, ButtonTooltip, Flags), pViewer_(pViewer),
      pVectorElementEditor_(pVectorElementEditor), pGeoEditor_(NULL) {
   windowTitle_ = ButtonTooltip;
}

/**
 * Constructor.
 * @param[in] pToolbar lugar donde se agrega el boton
 * @param[in] pViewer viewer donde se editan los botones
 * @param[in] Bitmap icono de la herramienta
 * @param[in] GeoType tipo de geometria que edita el boton
 * @param[in] ButtonTooltip tooltip del boton
 * @param[in] pViewportManager
 * @param[in] pViewcontextManager
 * @param[in] Flags Mascara que define los eventos de mouse que se conectan sobre
 *            la ventana del visualizador
 * @return instancia de la calse VectorEditorButton
 */
VectorEditorButton::VectorEditorButton(wxWindow *pToolbar, ViewerWidget *pViewer,
                                       const wxString &Bitmap,
                                       GuiGeometryCreator::GeometryType GeoType,
                                       wxString ButtonTooltip,
                                       ViewportManagerInterface* pViewportManager,
                                       ViewcontextManagerInterface* pViewcontextManager,
                                       int Flags) :
      ToggleButton(pToolbar, Bitmap, ButtonTooltip, Flags), pViewer_(pViewer),
      pVectorElementEditor_(NULL),
      pGeoEditor_(
            new GeometryElementEditor(GeoType, pViewer, pViewportManager,
                                      pViewcontextManager)) {
   windowTitle_ = ButtonTooltip;
}

/** Destructor */
VectorEditorButton::~VectorEditorButton() {
   delete pGeoEditor_;
}

/**
 * Devuelve el VectorElementEditor del boton.
 * @return pVectorElementEditor
 */
VectorElementEditor *VectorEditorButton::GetVectorElementEditor() {
   return pVectorElementEditor_;
}

/** Retorna el VectorElementEditor que usa el boton */
GeometryElementEditor* VectorEditorButton::GetGeometryElementEditor() {
   return pGeoEditor_;
}
}  // namespace suri

