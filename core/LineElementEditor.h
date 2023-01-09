/*! \file LineElementEditor.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DRAWLINEBUTTON_H_
#define DRAWLINEBUTTON_H_

// Includes standard

// Includes Suri
#include "VectorElementEditor.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Clase para crear vectores de tipo linea. */
/**
 * Se representa mediante boton (presionable).
 * Una vez activo permite :
 * - agregar un puntos al vector de tipo linea en curso.
 * - cerrar y salvar el vector en curso.
 * \note Esta clase hereda de VectorElementEditor ya que se necesita que posea
 * la funcionalidad de ser excluyente con otros botones del mismo grupo y
 * permita agregar un punto al vector, en este caso de tipo linea.
 */
class LineElementEditor : public VectorElementEditor {
   /** Ctor. de Copia. */
   LineElementEditor(const LineElementEditor &LineElementEditor);

public:
   /** Ctor. default. */
   LineElementEditor(ViewerWidget *pViewer, int ListGroup, VectorEditor* pVectorEditor,
         DataViewManager* pDataViewManager = NULL,
         std::string CategoryName = DRAW_TABLE_NAME);
   /** Dtor. */
   virtual ~LineElementEditor();
protected:
   /** configura la linea a dibujar. */
   virtual void SetupElement();
   /** Agrega un punto a la linea */
   virtual bool AddPointToVector(const Coordinates &Point);
};
}  // namespace suri

#endif /*DRAWLINEBUTTON_H_*/
