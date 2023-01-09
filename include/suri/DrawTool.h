/*! \file DrawTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DRAWTOOL_H_
#define DRAWTOOL_H_

// Includes standard
#include <set>

// Includes Suri
#include "suri/Widget.h"
#include "suri/ViewerWidget.h"
#include "suri/ToolGroupManager.h"
#include "suri/Tool.h"

// Includes Wx

// Includes App

// Defines

// Forwards

/** namespace suri */
namespace suri {
// Forwards
class VectorEditor;
class Viewer2D;
class VectorEditorButton;
class SaveVectorButton;
class CloseVectorButton;
class DataViewManager;

/** Clase herraminta de dibujo. */
/**
 * Crea un panel donde se insertan los botones de las herramientas de dibujo:
 *    - Anotaciones
 *    - Puntos
 *    - Lineas
 *    - Poligonos
 * y agrega un boton para salvar y descartar el vector en edicion.
 * Maneja la logica de activacion/desactivacion de la herramienta.
 */
class DrawTool : public Tool, public Widget, public ToolGroupManager {
   /** Ctor. de Copia. */
   DrawTool(const DrawTool &DrawTool);

public:
   /** Ctor. default. */
   DrawTool(int AnotationGroup, int VectorGroup, VectorEditor* pVectorEditor,
		   DataViewManager* pDataViewManager = NULL,
		   const suri::SuriObject::UuidType& ViewcontextId = SuriObject::NullUuid,
		   wxWindow *pParent = NULL);
   /** Dtor. */
   virtual ~DrawTool();
   /** Creacion de la ventana */
   virtual bool CreateToolWindow();
   /** Retorna la ventana asociada a la herramienta */
   /**
    * @return Puntero a la ventana asociada a la herramienta o NULL en caso de
    *       de no tener ventana asociada.
    */
   virtual wxWindow *GetToolWindow() const {
      return const_cast<DrawTool*>(this)->GetWindow();
   }
   /** Metodo virtual para indicar que la herramienta se ha des/activado */
   virtual bool DoSetActive(bool Active);
   /** Indica si debe activarse o no. */
   virtual bool IsEnabled();
   /** Metodo que se ejecuta cuando se desactiva un boton de la herramienta */
   virtual void ButtonDeactivated(Button *pButton);
   /** Metodo que se ejecuta cuando se activa un boton de la herramienta */
   virtual void ButtonActivated(Button *pButton);
protected:
   /** Inicializa los botones para la edicion vectorial de diferentes geometrias **/
   void InitializeVectorEditionButtons();
   /** Ctor para clases que hereden de DrawTool. */
   DrawTool(wxWindow *pParent = NULL,
            DataViewManager* pDataViewManager = NULL,
            const suri::SuriObject::UuidType& ViewcontextId = SuriObject::NullUuid);
   VectorEditor* pVectorEditor_; /*! VectorEditor que se utilizan los */
   /* botones para agregar puntos a vectores */
   SaveVectorButton *pSaveButton_; /*! Boton que salva el vector en edicion */
   CloseVectorButton *pCloseButton_; /*! Boton que descarta el vector en edicion */
   std::set<Button*> toolButtons_; /*! botones de la herramienta */
   std::set<VectorEditorButton*> drawVectorButtons_; /*! botones de dibujo de */
   DataViewManager* pDataViewManager_; /*! Visualizador de datos. */
   SuriObject::UuidType viewcontextId_; /*! Id que identifica cada contexto. */

private:
   int anotationGroup_; /*! grupo de la lista para las anotaciones */
   int vectorGroup_; /*! grupo de la lista para los vectores */
   /** se cachea la transformacion a utilizar para evitar la reconstruccion **/
   CoordinatesTransformation *pCoordsTransform_;
   /** se cachea el sistema de referencia para evitar la reconstruccion
    * de la transformacion
    */
   std::string lastSpatialReference_;
};
}  // namespace suri

#endif /* DRAWTOOL_H_ */
