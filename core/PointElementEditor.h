/*! \file PointElementEditor.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DRAWPOINTBUTTON_H_
#define DRAWPOINTBUTTON_H_

// Includes standard

// Includes Suri
#include "VectorElementEditor.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Clase crear vectores de tipo punto. */
/**
 * Se representa mediante boton (presionable).
 * Una vez activo permite agregar un elemento vectorial HotLink con 1 punto
 * cada vez que se realiza un click-izquierdo en el visualizador.
 * \note Esta clase hereda de VectorElementEditor ya que se necesita que posea
 *       la funcionalidad de ser excluyente con otros botones del mismo grupo y
 *       permita agregar un punto al vector, en este caso de tipo punto.
 */
class PointElementEditor : public VectorElementEditor {
   /** Ctor. de Copia. */
   PointElementEditor(const PointElementEditor &PointElementEditor);

public:
   /** Ctor. default. */
   PointElementEditor(ViewerWidget *pViewer, int ListGroup, VectorEditor* pVectorEditor,
         DataViewManager* pDataViewManager = NULL,
         std::string CategoryName = DRAW_TABLE_NAME);
   /** Dtor. */
   virtual ~PointElementEditor();
protected:
   /** Configura el punto a dibujar. */
   virtual void SetupElement();
   /** Agrega un punto al vector. */
   virtual bool AddPointToVector(const Coordinates &Point);
};
}  // namespace suri

#endif /*DRAWPOINTBUTTON_H_*/
