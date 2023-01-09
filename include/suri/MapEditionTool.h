/*! \file MapEditionTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MAPEDITIONTOOL_H_
#define MAPEDITIONTOOL_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/GenericTool.h"
// Includes Wx
// Defines
// forwards

namespace suri {
// forwards
class MapEditionTask;
class TaskConfig;

/*!
 *  Clase que representa una tarea de edicion de mapa.
 */
class MapEditionTool : public suri::GenericTool {
   /** ctor copia **/
   MapEditionTool(const MapEditionTool&);

public:
   /** Constructor **/
   MapEditionTool(DataViewManager* pDataViewManager, wxWindow* pParent);
   /** Destructor **/
   virtual ~MapEditionTool();
   /** Ejecucion del comando */
   virtual void Execute(const Command* pToolCommand);

protected:
   /** Creacion del Comando */
   virtual Command* CreateCommand(const std::string &CommandName);
   /** Destruccion del Comando */
   virtual void DestroyCommand(Command* &pCommand) const;
   /** Creacion del Widget */
   virtual Widget* CreateWidget(const std::string &Widget);
   /** Inicia la tarea de edicion de mapa */
   bool StartTask();
   /** Finaliza la tarea de edicion de mapa **/
   bool EndTask();

private:
   /** Metodo que se ejecuta cuando se apreta boton de mostrar leyenda */
   void ExecuteShowLegend(bool Show);
   /** Metodo que se ejecuta cuando se apreta boton de mostrar norte */
   void ExecuteShowNorth(bool Show);
   /** Metodo que se ejecuta cuando se apreta boton de mostrar escala */
   void ExecuteShowScale(bool Show);
   /** Metodo que se ejecuta cuando se apreta choice de grilla */
   void ExecuteShowGrid(const std::string& GridValue);
   /** Metodo que se ejecuta cuando se apreta boton de iniciar/finalizar tarea */
   void ExecuteTaskMenu(bool Active);
   /** Ventana para renderizar el mapa */
   wxWindow* pParent_;
   /** Referencia a la tarea */
   MapEditionTask* pMapTask_;
   /** Configurador para la tarea de mapa */
   TaskConfig* pTaskConfig_;
   /** Referencia al command de escala para poder desactivarlo/activarlo
    *  en funcion de la capa desplegada (las georreferenciados no tienen escala)
    */
   Command* pScaleCommand_;
   SuriObject::UuidType blockedDatasource_;
};

} /** namespace suri */

#endif /* MAPEDITIONTOOL_H_ */
