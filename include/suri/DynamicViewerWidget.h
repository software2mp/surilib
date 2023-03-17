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

#ifndef DYNAMICVIEWERWIDGET_H_
#define DYNAMICVIEWERWIDGET_H_

// Includes Estandar
// Includes Suri
#include "ViewerWidget.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class DataViewManager;
class ViewportManagerInterface;
class MouseEventInterface;
class PaintEventInterface;
class CommandEventInterface;

/**
 * Clase que encapsula el cambio dinamico del modo de visualizacion (2D/3D).
 * Contiene al visualizador acorde al modo de visualizacion seleccionado y
 * encapsula el cambio asociado al modo de visualizacion seleccionado.
 * TCK DIS: #8277
 **/
class DynamicViewerWidget : public ViewerWidget {
public:
   /** Constructor */
   explicit DynamicViewerWidget(wxWindow *pParent = NULL, DataViewManager* pDataViewManager = NULL);
   /** Destructor */
   virtual ~DynamicViewerWidget();
   typedef enum {
      Gis = 0, Raster, Z
   } VisualizationMode;
   /** Creacion de la ventana */
   virtual bool CreateToolWindow();
   /** Resetea el estado del visualizador */
   virtual void ResetViewer();
   /** Obtiene la transformacion asociada **/
   virtual CoordinatesTransformation* GetTransformation();
   /** Obtiene el modo de visualizacion configurado. **/
   VisualizationMode GetVisualizationMode() const;
   /** Configura el modo de visualizacion**/
   bool ChangeVisualizationMode(VisualizationMode Mode);
   /** Despliega un elemento (puede ser funcion de RenderizationControl) */
   virtual void DisplayElement(Element *pElement);
   /** Oculta un elemento (puede ser funcion de RenderizationControl) */
   virtual void HideElement(Element *pElement);
   /** El elemento ha sido modificado */
   virtual void UpdateElement(Element *pElement);
   /** Se le setea la lista de elementos */
   virtual void DoSetElementList(LayerList *pList);
   /** Hace ZoomIn */
   void ZoomIn(int X, int Y, int Factor = 2, int Z = 0);
   /** Hace ZoomOut */
   void ZoomOut(int X, int Y, int Factor = 2, int Z = 0);
   /** Cambia la ventana del mundo */
   void ChangeWindow(const Subset &NewWindow, bool PreventUpdate = false);
   /** Setea el WorldExtentManager */
   void SetWorldExtentManager(WorldExtentManager* pExtentManager);
   /** Retorna el WorldExtentManager */
   WorldExtentManager* GetWorldExtentManager();
   /** Metodo de actualizacion de la lista, el mundo y/o los elementos. */
   void UpdateViewers(Model *pModel);
   /** Verifica el estado de la renderizacion. */
   void CheckRenderizationStatus();
   virtual SuriObject::UuidType GetId() const;
   virtual SuriObject::UuidIntType GetIntId() const;
   /** Retorna el id del modelo de elevacion */
   suri::Element::UidType GetDemId() const;
   /** Setea el id del modelo de elevacion */
   bool ChangeDem(suri::Element::UidType Dem);
   /** Obtiene el mundo */
   virtual World *GetWorld() const;
   /** Retorna el visualizador especificado por parametro */
   ViewerWidget* GetViewer(VisualizationMode Mode);
   /** Retorna el visualizador activo */
   ViewerWidget* GetViewer();
   /** establece el titulo de la ventana */
   virtual void SetWindowTitle(const wxString& WindowTitle);
   /** Obtiene la lista */
   virtual LayerList* GetList() const;
   /** Marca el "foco" en el miniframe */
   virtual void Focus(bool Focused = true);
   /** Agrega un handler para eventos de tipo mouse */
   virtual void PushMouseEvent(MouseEventInterface *pMouseEvent);
   /** Agrega un handler para eventos de tipo paint */
   virtual void PushPaintEvent(PaintEventInterface *pPaintEvent, bool DeepSearch = true);
   /** Agrega un handler para eventos de tipo command */
   virtual void PushCommandEvent(CommandEventInterface *pCommandEvent);
   /** Elimina el handler de eventos de tipo mouse */
   virtual MouseEventInterface *PopMouseEvent(MouseEventInterface *pMouseEvent = NULL);
   /** Elimina el handler de eventos de tipo paint */
   virtual PaintEventInterface *PopPaintEvent(PaintEventInterface *pPaintEvent = NULL);
   /** Elimina el handler de eventos command */
   virtual CommandEventInterface *PopCommandEvent(CommandEventInterface *pCommandEvent = NULL);
   /** Desactiva los elementos del visualizador activo */
   void DeactivateViewerElements();

protected:
	/** Metodo polimorfico que se llama cuando se hace foco sobre un Widget **/
	virtual void DoOnFocus(bool Focused);
   /** Permite obtener la ventana con las herramientas del viewer */
   virtual wxWindow *DoGetViewerTools(wxWindow *pParent);
   /** modo de visualization activo **/
   VisualizationMode activeMode_;
   /** mapa de asociacion de vieweres a modo seleccionado **/
   /** Un viewer 2d tiene dos modos de visualizacion =>
    * Si tengo la misma ref ocurre algun problema? **/
   std::map<VisualizationMode, ViewerWidget*> viewers_;
	suri::Element::UidType demid_;
};

}  /** namespace suri **/
#endif /* DYNAMICVIEWERWIDGET_H_ */
