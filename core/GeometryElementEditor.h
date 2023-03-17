/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
