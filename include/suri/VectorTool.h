/*! \file VectorTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORTOOL_H_
#define VECTORTOOL_H_

// Includes standard
// Includes Suri
#include "suri/Tool.h"
#include "suri/Widget.h"
#include "suri/ToolGroupManager.h"
// Includes Wx
#include "wx/wx.h"
#include "wx/bmpbuttn.h"
#include "wx/xrc/xmlres.h"
// Defines

/** Toolbar de edicion vectorial */
/**
 * Tiene todas las opciones de edicion vectorial
 *
 */
namespace suri {
class VectorEditor;

class VectorTool : public Tool, public Widget, public ToolGroupManager {
public:

   /** Ctor */
   VectorTool(ViewerWidget *pViewer, int AnotationGroup, int VectorGroup,
              VectorEditor* pVectorEditor, wxWindow *pParent = NULL);
   /** Dtor */
   virtual ~VectorTool();
   /** Crea la ventana de la parte */
   bool CreateToolWindow();
   /** Devuelve Puntero a la ventana asociada a la herramienta */
   virtual wxWindow *GetToolWindow() const;
   /** Devuelve el estado del Tool. */
   virtual bool IsEnabled();

private:
   VectorEditor* pVectorEditor_; /*! VectorEditor que se utilizan los */
   /* botones para agregar puntos a vectores */
   int anotationGroup_; /*! grupo de la lista para las anotaciones */
   int vectorGroup_; /*! grupo de la lista para los vectores */
};
}

#endif /* VECTORTOOL_H_ */
