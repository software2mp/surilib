/*! \file GpsTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GPSTOOL_H_
#define GPSTOOL_H_

// Includes standard
// Includes Suri
#include "suri/Widget.h"
#include "DrawTool.h"

// Includes Wx

// Includes App

// Forwards
class VectorEditor;

// Defines

/** namespace suri */
namespace suri {
// Forwards
class Viewer2D;
class Button;
class VectorEditor;
class DataViewManager;
class DatasourceManagerInterface;

/** Permite editar vectores usando coordenadas */
/**
 * Herramienta que permite cargar puntos usando coordenadas
 * Muestra un boton para puntos, uno para lineas y otro para
 * poligonos.
 * A estos botones les carga estilos gps para diferenciarlos
 */
class GpsTool : public DrawTool {
   /** Ctor. de Copia. */
   GpsTool(const GpsTool &MeassureTool);

public:
   /** Ctor. default. */
   GpsTool(int VectorGroup, VectorEditor* pVectorEditor,
         DataViewManager* pDataViewManager =
         NULL, const suri::SuriObject::UuidType& ViewcontextId = SuriObject::NullUuid,
         wxWindow *pParent = NULL);
   /** Dtor. */
   virtual ~GpsTool();
   /** Creacion de la ventana */
   virtual bool CreateToolWindow();
   /** Retorna la ventana asociada a la herramienta */
   virtual wxWindow *GetToolWindow() const {
      return (const_cast<GpsTool*>(this))->GetWindow();
   }
protected:

private:
   int vectorGroup_; /*! grupo de la lista para los vectores */
   VectorEditor* pVectorEditor_; /*! VectorEditor que se usa para dibujar */
   DataViewManager* pDataViewManager_; /*! Visualizador del contexto */
   DatasourceManagerInterface* pDatasourceManager_; /*! Fuente de datos del contexto */
   suri::SuriObject::UuidType viewcontextId_; ///< Id que identifica cada contexto.
};
}  // namespace suri

#endif /*GPSTOOL_H_*/
