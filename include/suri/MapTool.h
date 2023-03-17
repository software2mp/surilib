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

#ifndef MAPTOOL_H_
#define MAPTOOL_H_

// Includes standard
#include <string>
#include <vector>

// Includes Suri
#include "suri/Widget.h"
#include "suri/VectorElement.h"
#include "suri/ToggleButton.h"
#include "suri/ToolGroupManager.h"
#include "suri/Tool.h"
#include "MapRenderer.h"
#include "suri/ParameterCollection.h"
#include "suri/CoordinatesTransformation.h"

// Includes Wx
// Includes App
// Defines

// forwards ws
class wxCommandEvent;
class wxUpdateUIEvent;

/** namespace suri */
namespace suri {
// Forwards
class World;
class LayerList;
class Button;
class MapElement;

/**
 * Macro para declarar el objeto de los eventos.
 */
DECLARE_EVENT_OBJECT(MapEvent);

/** Herramienta de overlay de mapa.
 *  Herramienta de contexto que dibuja las leyendas de mapa sobre el viewer.
 *  Crea un renderizador que dibuja los elementos deseados, salvo las grillas
 * para las cuales utiliza una serie de archivos vectoriales.
 *  Las leyendas que dibuja son:
 *   - Grilla (Latitud Longitud)
 *   - Escala
 *   - Norte
 *   - Leyenda (de las capas vectoriales)
 */
class MapTool : public Tool, public Widget, public ToolGroupManager {
   /**
    *  Ctor. de Copia.
    */
   MapTool(const MapTool &MapTool);

public:
   /**
    * Declara el objeto de eventos como clase friend.
    */
   FRIEND_EVENT_OBJECT(MapEvent);

   /**
    *  Ctor. default.
    */
   MapTool(const World *pWorld, std::vector<int> VecGroup, wxWindow *pParent = NULL);
   /**
    *  Dtor.
    */
   virtual ~MapTool();

   /**
    *  Creacion de la ventana.
    */
   virtual bool CreateToolWindow();

   /**
    *  Titulo de la ventana (en caso de pParent!=NULL)
    */
   virtual const wxString GetWindowTitle() const;

   /**
    * Retorna la ventana asociada a la herramienta.
    * @return Puntero a la ventana asociada a la herramienta o NULL en caso de
    *       de no tener ventana asociada.
    */
   virtual wxWindow *GetToolWindow() const;

   /**
    * Metodo virtual para indicar que la herramienta se ha des/activado.
    *
    */
   virtual bool DoSetActive(bool Active);

   /**
    * Des/Activa los distintos elementos de la herramienta
    * (norte, escala, leyenda).
    */
   void DoOnButtonClicked(wxCommandEvent &Event);

   /**
    * Indica si esta habilitado o no.
    *
    */
   virtual bool IsEnabled();
   /**
    * Metodo que actualiza la grilla del mapa en funcion de los grados indicados por parametro
    * @param GridValue configuracion de la grilla
    */
   void UpdateGrid(const std::string& GridValue);
   /** Obtiene los parametros actuales del renderizador de mapa */
   MapRenderer::Parameters GetMapRendererParameters();
   /** Actualiza los parametros del render de mapa */
   void UpdateMapRendererParameters(MapRenderer::Parameters& Parameters);
   /** Indica si la grilla se encuentra activa */
   bool IsGridActive() const;

protected:
   /**
    * Evento disparado al cliquear sobre un checkbox.
    */
   void OnGridComboChanged(wxCommandEvent &Event);

   /**
    * Crea el elemento mapa que maneja la herramienta.
    *
    */
   void CreateMapElement();
private:
   /**
    *  Obtiene el url de una grilla
    */
   bool GetGridUrl(const std::string &GridId, std::string &Url);
   EVENT_OBJECT_PROPERTY(MapEvent); /*! Objeto para conectar eventos */
#ifdef __UNUSED_CODE__
   void ResetControls();
#endif

   const World *pWorld_; /*! Mundo */
   MapElement *pMapElement_; /*! Elemento usado */
   VectorElement *pGridElement_; /*! Elemento usado */
// int listGroup_; /*! grupo donde se dibujan las referencias. */
   std::vector<int> vecGroup_; /*! grupo donde se dibujan las referencias. */
   ToggleButton *pNorthButton_; /*! puntero a boton norte */
   ToggleButton *pScaleButton_; /*! puntero a boton de escala */
   ToggleButton *pLegendButton_; /*! puntero a boton de leyenda */
   /** cacheo de parametros **/
   ParameterCollection params_;
   /** transformacion de coordenadas **/
   CoordinatesTransformation *pcoordtrans_;
};
} // end namespace suri

#endif /*MAPTOOL_H_*/
