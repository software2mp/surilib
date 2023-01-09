/*! \file AnotationPropertiesPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ANOTATIONPROPERTIESPART_H_
#define ANOTATIONPROPERTIESPART_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"
#include "suri/Element.h"
#include "suri/VectorStyle.h"

// Includes Wx

// Includes App
#include "AnotationElement.h"

// Defines
/** Si esta definido permite editar el texto de la anotacion */
#define __SHOW_TEXTCTRL_FOR_EDIT_ANOTATION__
#undef __SHOW_TEXTCTRL_FOR_EDIT_ANOTATION__

/** namespace suri */
namespace suri {

// Forwards
class VectorLayer;

/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(AnotationPropertiesPartEvent);

/** Clase que permite visualizar/editar las propiedades especificas de una Anotacion. */
/**
 * Esta clase genera una ventana que permite visualizar/editar las propiedades
 * especificas de una Anotacion, a saber:
 *    - Texto: texto de la anotacion.
 *    - Fuente: tipo y tamanio.
 *    - Color: de fuente y de fondo.
 *    - Efectos de la fuente: negrita, italica, subrayado.
 * Muestra un preview de como quedaria el texto de la Anotacion al
 * editar dichas propiedades.
 */
class AnotationPropertiesPart : public Part {
   /** Ctor. de Copia */
   AnotationPropertiesPart(const AnotationPropertiesPart &AnotationProp);
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(AnotationPropertiesPartEvent);
   /** Ctor. legacy*/
   explicit AnotationPropertiesPart(VectorElement *pElement);
   /** Ctor. */
   explicit AnotationPropertiesPart(VectorLayer *pVectorLayer);
   /** Ctor. legacy que Permite especificar si el panel de seleccion de campo
    * sera visible o no */
   AnotationPropertiesPart(VectorElement *pElement, bool ShowFieldSelector);
   /** Ctor. que Permite especificar si el panel de seleccion de campo sera visible o no */
   AnotationPropertiesPart(VectorLayer *pVectorLayer, bool ShowFieldSelector);
   /** Dtor. */
   virtual ~AnotationPropertiesPart();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Inicializa los controles */
   virtual void SetInitialValues();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Devuelve si la parte tiene datos validos */
   virtual bool HasValidData();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Evento UIUpdate */
   virtual void OnUIUpdate(wxUpdateUIEvent &Event);
   /** Evento OnCheckboxClicked */
   virtual void OnCheckboxLabelEnableClicked(wxCommandEvent &Event);
   /** Actualiza el VectorStyle a partir del elemento(pElement). */
   void UpdateStyle();
   /** Obtiene el icono del part */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   /** Actualiza el estado de la parte */
   virtual void Update() { }

private:
   /** carga los controles(combos, listas, etc) con los posibles valores. */
   void LoadControls();
   /** setea las prop. de la letra. */
   bool SetLabel(VectorStyle* pVectorStyle);
   /** setea el punto de anclaje. */
   bool SetAnchor(VectorStyle* pVectorStyle);
   /** setea el punto donde debe establecerse la anotacion */
   bool SetPlacement(VectorStyle* pVectorStyle);
   /** setea la expresion */
   bool SetExpresion(VectorStyle* pVectorStyle);
   /** setea el angulo de inclinacion. */
   bool SetAngle(VectorStyle* pVectorStyle);
   /** setea el desplazamiento en X */
   bool SetDx(VectorStyle* pVectorStyle);
   /** setea el desplazamiento en Y */
   bool SetDy(VectorStyle* pVectorStyle);
   /** setea el texto de la anotacion. */
   bool SetAnotationText();
   /** Obtiene los valores configurados por el usuario **/
   void GetValuesFromControl(int &SelectedFont, int &SizeFont, bool &Bold, bool &Italic,
                             bool &Underline, wxColour& ColorFont, bool &Opaque,
                             wxColour& ColorBkg, bool &Validate, bool &LabelChecked,
                             int &SelectedField, bool &BackgroundChecked, int &Anchor);
   /** obtiene el label asociado al estilo **/
   bool GetLabel(VectorStyle::Label* &pLabel, VectorStyle* pVectorStyle);
   EVENT_OBJECT_PROPERTY(AnotationPropertiesPartEvent); /*! Objeto para conectar */
   /* eventos */
   VectorElement *pElement_; /*! elemento anotacion. */
   VectorLayer *pVectorLayer_; /*! capa vectorial de anotacion. */
   VectorStyle *pVecStyle_; /*! Estilo de decoracion del vector */
	std::vector<std::string> vstyles_;		
   std::vector<std::string> fieldList_; /* Lista de campos de las etiquetas - no anotacion*/
   int selectedField_; /* Campo seleccionado de la lista */
   bool updatePreview_; /*! flag que indica cuando actualizar el preview. */
// TODO (Sebastian - TCK #835): Analizar si se puede hacer mas prolijo con alguna refactorizacion
   bool hasFieldCombo_; /*! indica si debe mostrar combo de seleccion de campo */
   /** Flag que indica si se desea hacer visible la etiqueta **/
   bool isLabelChecked_;

#ifdef __SHOW_TEXTCTRL_FOR_EDIT_ANOTATION__
   wxString textAnotation_; /*! texto de la anotacion. */
#endif
};
}  // namespace suri

#endif /*ANOTATIONPROPERTIES_H_*/
