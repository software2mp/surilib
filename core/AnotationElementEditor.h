/*! \file AnotationElementEditor.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DRAWANOTATIONBUTTON_H_
#define DRAWANOTATIONBUTTON_H_

// Includes standard

// Includes Suri
#include "suri/Vector.h"
#include "suri/Coordinates.h"
#include "VectorElementEditor.h"

// Includes Wx
#include "wx/wx.h"

// Includes App

// Includes OGR
#include "ogrsf_frmts.h"

// Defines
/** Estilo del vector para la anotacion */
#define ANOTATION_STYLE_PREFIX \
   "VECTORSTYLE[1,\"Anotacion_BN\",SYMBOL[\"Suri-symbol\",0,COLOR[0,0,0,0],SIZE[1]],LABEL[\"Suri-label\",1,"
/** Nombre del campo de la anotacion */
#define ANOTATION_STYLE_EXPRESSION "EXPRESSION[\"{" SUR_ANOTATION_FIELD "}\"]"
/** Estilo del tipo de letra para la anotacion */
#define ANOTATION_STYLE_SUFFIX \
   ",COLOR[0,0,0,255],BACKCOLOR[255,255,255,255],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[4]]]"
/** Estilo de la anotacion, combinacion de los otros estilos */
#define ANOTATION_POINT_1 ANOTATION_STYLE_PREFIX ANOTATION_STYLE_EXPRESSION ANOTATION_STYLE_SUFFIX

/** namespace suri */
namespace suri {
// Forwards
class ViewerWidget;
class AnotationElement;

/** Clase utilizada para poder ingresar una Anotacion. */
/**
 * Se representa mediante boton (presionable).
 * Una vez activo permite :
 * - agregar un texto de anotacion en una posicion dada.
 * \note Esta clase hereda de ToggleButton ya que se necesita que posea la
 * funcionalidad de ser excluyente con otros botones del mismo grupo.
 */
class AnotationElementEditor : public VectorElementEditor {
   /** Ctor. de Copia. */
   AnotationElementEditor(const AnotationElementEditor &);
public:
   /** Ctor. default. */
   AnotationElementEditor(ViewerWidget *pViewer, int ListGroup,
                      VectorEditor* pVectorEditor,
                      DataViewManager* pDataViewManager = NULL);
   /** Dtor. */
   virtual ~AnotationElementEditor();
   /** Salva el vector. */
   virtual bool CloseVector();
protected:
   /** Configura el punto a dibujar. */
   virtual void SetupElement();
   /** Agrega un punto al vector. */
   virtual bool AddPointToVector(const Coordinates &Point);
   /** Permite configurar el elemento antes de agregarlo a la lista */
   virtual bool ConfigureSavedElement(VectorElement* pVectorElement);
private:
   wxString anotation_; /*! Texto de la anotacion. */
   AnotationElement *pAnotationElement_; /*! Elemento anotacion. */
};
}  // namespace suri

#endif /*DRAWANOTATIONBUTTON_H_*/
