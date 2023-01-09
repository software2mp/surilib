/*! \file MapEditionTask.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MAPEDITIONTASK_H_
#define MAPEDITIONTASK_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/TaskInterface.h"
#include "suri/MapTool.h"
#include "suri/World.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class LayerList;
class RasterElement;

/*!
 * Clase que representa la tarea de creacion de mapas
 */
class MapEditionTask : public TaskInterface {
   /** Ctor. copia **/
   MapEditionTask(const MapEditionTask&);

public:
   /** Constructor **/
   MapEditionTask(World* pWorld, wxWindow* pParent);
   /** Destructor **/
   virtual ~MapEditionTask();
   /** Comienza la tarea */
   virtual bool Start();
   /** Finaliza la tarea. Intenta guardar los cambios. */
   virtual bool End();
   /** Cancela la tarea sin guardar */
   virtual bool Cancel();
   /** Indica si la tarea esta activa */
   virtual bool IsActive() const;
   /** Muestra/oculta la leyenda*/
   bool ShowLegend(bool Show);
   /** Muestra/oculta el norte*/
   bool ShowNorth(bool Show);
   /** Muestrala grilla con los grados que se indican por parametro*/
   bool ShowGrid(const std::string& GridConfig);
   /** Muestra/oculta la escala*/
   bool ShowScale(bool Show);
   /** Indica cual es la lista de elementos activa **/
   void SetActiveElementList(LayerList* pList);
   /** Nombre de la tarea */
   static const std::string TASKNAME;
   /** Nombre clave que se utiliza par la lista de elementos **/
   static const std::string ELEMENTLIST;

private:
   /** bool que indica si comenzo la tarea **/
   bool started_;
   /** Herramienta para la creacion de mapas **/
   MapTool* pMapTool_;
   /** Actualiza lso parametros del renderizador en funcion de las herramientas
    *  de mapas habilitadas */
   void UpdateMapRendererParameters();
   /** bool que indica si se muestra la legenda en el mapa */
   bool legend_;
   /** bool que indica si se muestra la escala en el mapa */
   bool scale_;
   /** bool que indica si se muestra el norte en el mapa */
   bool north_;
   /** String con la configuracion actual de la grilla **/
   std::string gridConfig_;
};

} /** namespace suri */

#endif /* MAPEDITIONTASK_H_ */
