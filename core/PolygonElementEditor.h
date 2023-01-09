/*! \file PolygonElementEditor.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DRAWPOLYGONBUTTON_H_
#define DRAWPOLYGONBUTTON_H_

// Includes standard

// Includes Suri

// Includes Wx

// Includes App
#include "VectorElementEditor.h"

// Defines

/** namespace suri */
namespace suri {
/** Clase para crear vectores de tipo poligono */
/**
 * Se representa mediante boton (presionable).
 * Una vez activo permite :
 * - agregar un puntos al vector de tipo poligono en curso.
 * - cerrar y salvar el vector en curso.
 * \note Esta clase hereda de VectorElementEditor ya que se necesita que posea
 *       la funcionalidad de ser excluyente con otros botones del mismo grupo
 *       y permita agregar un punto al vector, en este caso de tipo poligono.
 */
class PolygonElementEditor : public VectorElementEditor {
   /** Ctor. de Copia. */
   PolygonElementEditor(const PolygonElementEditor &PolygonElementEditor);

public:
   /** Ctor. default */
   PolygonElementEditor(ViewerWidget *pViewer, int ListGroup,
                        VectorEditor* pVectorEditor,
                        DataViewManager* pDataViewManager = NULL,
                        std::string CategoryName = DRAW_TABLE_NAME);
   /** Dtor. */
   virtual ~PolygonElementEditor();
protected:
   /** configura el poligono a dibujar. */
   virtual void SetupElement();
   /** Agrega un punto al vector. */
   virtual bool AddPointToVector(const Coordinates &Point);
};
}  // namespace suri

#endif /*DRAWPOLYGONBUTTON_H_*/
