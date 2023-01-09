/*! \file GeometryElementEditor.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOMETRYELEMENTEDITOR_H_
#define GEOMETRYELEMENTEDITOR_H_


#include "GuiGeometryCreator.h"
#include "suri/PaintEventInterface.h"
#include "suri/ViewportEvent.h"
#include "GeometryCollection.h"
#include "suri/VectorStyle.h"
#include "GeometryRenderer.h"
#include "suri/DataViewManager.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Configuration.h"
// Includes OGR
#include "ogrsf_frmts.h"

// Defines
/** Primer estilo de dibujo para la linea */
#define G_DRAW_LINE_1 \
   "VECTORSTYLE[2,\"Linea amarillo\",PEN[\"Suri-pen\",1,COLOR[255,255,0,255],WIDTH[2]]]"
/** Segundo estilo de dibujo para la linea */
#define G_DRAW_LINE_2 "VECTORSTYLE[2,\"Linea rojo\",PEN[\"Suri-pen\",1,COLOR[255,0,0,255],WIDTH[2]]]"
/** Tercer estilo de dibujo para la linea */
#define G_DRAW_LINE_3 \
   "VECTORSTYLE[2,\"Linea violeta\",PEN[\"Suri-pen\",1,COLOR[128,0,255,255],WIDTH[2]]]"
/** Nombre por defecto para el elemento */
#define LINEA_FILE_NAME "Linea_1"




// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards


namespace suri {

// Forwards
class VectorEditor;

/**  Clase que encapsula la creacion de gometrias utilizando el nuevo sistema
 * de edicion vectorial. */
class GeometryElementEditor : public ViewportEvent {
public:
   GeometryElementEditor(GuiGeometryCreator::GeometryType Type, ViewerWidget* pViewer,
                         ViewportManagerInterface* pViewportManager,
                         ViewcontextManagerInterface* pViewcontextManager);
   virtual ~GeometryElementEditor();

   virtual bool IsVectorOpen();

   virtual bool ConfigureSavedElement(VectorElement* pVectorElement);

   virtual bool SaveVector(const std::string &FileName = "");

   /*  virtual VectorElement *GetCurrentElement();*/

   virtual bool CanDeleteShapefile(const std::string &Filename);

   virtual bool DeletePoint(int Position);

   virtual int GetPointCount();

   virtual bool DeleteFeature(long FeatureID);

   virtual bool DeleteShapefile(const std::string &Filename);

   /**
    * Resetea el estado del editor.
    */
   virtual void Reset();

   /**
    * Se ejecuta cuando el usuario hace click sobre el visualizador.
    */
   virtual void SetPoint(double Pixel, double Line, wxMouseEvent& Event);

   /**
    * Se ejecuta cuando el usuario se mueve en el visualizador.
    */
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent& Event);

   /**
    * Se ejecuta cuando el usuario comienza drag sobre visualizador.
    */
   virtual void OnStartDrag(const Coordinates& InitialCoordinate);

   /**
    * Se ejecuta cuando el usuario finaliza drag.
    */
   virtual void OnEndDrag(const Coordinates& InitialCoordinate,
                          const Coordinates& CurrentCoordinate, wxMouseEvent& Event);

   /**
    * Se ejecuta cada vez que el usuario mueve el mouse mientras realiza drag.
    */
   virtual void OnDrag(const Coordinates& InitialCoordinate,
                       const Coordinates& CurrentCoordinate, wxMouseEvent& Event);

   /**
    * Al pintar se dibuja geometria en edicion.
    */
   virtual void OnPaint(wxPaintEvent&Event, wxDC &dc);

   /**
    * Indica que quiere recibir el evento OnPaint.
    */
   virtual PaintEventInterface::PaintEventFlags GetPaintFlags() const;

   /**
    *
    */
   virtual bool CanProcessEvent(wxEvent &Event) const;

   /**
    * Preguntar a gabi.
    */
   //virtual bool SaveMemoryDatasourceToDisk(const std::string &Filename);

   /**
    *
    */
   virtual void Start();

   /**
    *
    */
   virtual bool CloseVector();

   /**
    *
    */
   virtual void Stop();

   // Inicialmente era protected.
   virtual void CloseFeature();
   /** Indica si se encuentra activo el editor **/
   bool IsActive() const {
      return active_;
   }

   bool IsEditing() const {
      return pGuiGeomCreator_ ? pGuiGeomCreator_->IsEditing() : false;
   }

   virtual void OnLeftUp(wxMouseEvent &Event);

   virtual void Finish();

protected:
   /** Salva el vector a SaveVector. */
   //virtual void CloseFeature();

private:
   std::vector<Geometry*> modifgeoms_;
   GuiGeometryCreator* pGuiGeomCreator_;
   GuiGeometryCreator::GeometryType type_;
   VectorEditor* pVectorEditor_;
   ViewerWidget* pViewer_;
   std::string elementUrl_; /*! URL que se pasa al elemento */
   static int GeoElementCounter_;
   bool active_;
   bool editing_;
   /** feature id en edicion **/
   long currentFid_;
   void Initialize();
   /** Finaliza y actualiza la edicion de la geometria en curso */
   void EndFeatureEdition();
   bool SaveFeatures(const std::string& FileName);
};
} /** namespace suri **/
#endif /* GEOMETRYELEMENTEDITOR_H_ */
