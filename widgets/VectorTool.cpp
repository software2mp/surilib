/*! \file VectorTool.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
// Includes Suri
#include "suri/ToggleButton.h"
#include "suri/VectorEditor.h"
#include "suri/ViewerWidget.h"
#include "PolygonElementEditor.h"
#include "PixelVectorButton.h"
#include "resources.h"
// Includes Wx
// Defines

// todo provisorio mover a messages.h
#define tooltip_SELECT_VECTOR "Seleccionar vector"
#define tooltip_EDIT_TABLE "Editar tabla de vector"
#define tooltip_EDIT_GEOMETRY "Editar geometria de vector"

#include "suri/VectorTool.h"

namespace suri {
/** Ctor */
VectorTool::VectorTool(ViewerWidget *pViewer, int AnotationGroup, int VectorGroup,
                       VectorEditor* pVectorEditor, wxWindow *pParent) :
      Widget(pParent), pVectorEditor_(pVectorEditor), anotationGroup_(AnotationGroup),
      vectorGroup_(VectorGroup) {
   if( Configuration::GetParameter("app_short_name", "") == wxT("SoPI") )
      windowTitle_ = caption_DRAW_TOOL;
   else
      windowTitle_ = caption_DRAW_CREATION_TOOL;
   wantedWidth_ = 200;
   wantedHeight_ = 20;
   toolName_ = GetWindowTitle().c_str();
}

/** Dtor */
VectorTool::~VectorTool() {
}

/**
 *  Creacion de la ventana
 *  @return bool true en caso de exito, false en caso contrario.
 */
bool VectorTool::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_VECTOR_TOOLBAR"));
   InitializeToolbar(pToolWindow_, pViewer_, pList_);

   pViewer_->Link(this);

   return true;
}

/**
 * Retorna la ventana
 * @return wxWindow * Ventana
 */
wxWindow *VectorTool::GetToolWindow() const {
   return (const_cast<VectorTool*>(this))->GetWindow();
}

/**
 * Devuelve el estado del Tool.
 * @return bool true si esta habilitado, false en caso contrario
 */
bool VectorTool::IsEnabled() {
   return true;
}
}
