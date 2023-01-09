/*! \file SnapPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SNAPPART_H_
#define SNAPPART_H_

// Includes Suri
#include "suri/DialogWidget.h"

namespace suri {

// forwards
class SnapTool;

namespace ui {

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(SnapPartEvent);

/**
 * Clase que permite configurar la herramienta de snapping
 */
class SnapPart : public suri::DialogWidget {
public:
   /** constructor */
   SnapPart();
   /** Destructor */
   virtual ~SnapPart();
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();
   /** Setea una referencia a la herramienta de snapping existente */
   void SetTool(SnapTool* Tool);

private:
   /** Inicializa el choice de modos */
   void InitializeModeValues();
   /** Inicializa el choice de unidades */
   void InitializeUnitValues();
   /** Evento click sobre boton aceptar */
   void OnOkButton(wxCommandEvent &Event);
   /** Evento click sobre boton cancelar */
   void OnCancelButton(wxCommandEvent &Event);
   /** Setea el modo en la herramienta de snapping */
   void SetMode();
   /** Setea la unidad en la herramienta de snapping */
   void SetUnit();
   /** Setea la tolerancia en la herramienta de snapping */
   void SetTolerance();
   /** Conecta los eventos a los controles */
   void ConnectEvents();
   /** Herramienta de snapping */
   SnapTool* pSnapTool_;
   EVENT_OBJECT_PROPERTY(SnapPartEvent);
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(SnapPartEvent);
};

} /** namespace ui */
} /** namespace suri */

#endif /* SNAPPART_H_ */
