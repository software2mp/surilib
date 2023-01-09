/*! \file ProcessHistoryPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef __PROCESS_HISTORY_PART_H__
#define __PROCESS_HISTORY_PART_H__

#include "suri/Part.h"

namespace suri {

// Forwards
class DatasourceInterface;
class ProcessHistoryPartEventHandler;

/** 
 * Clase que se encarga de la visualizacion del historial de procesamiento de la fuente
 * de datos asociada al Part
 **/
class ProcessHistoryPart : public Part {
   /** ctor copia **/
   ProcessHistoryPart(const ProcessHistoryPart&);

public:
   /** ctor **/
   explicit ProcessHistoryPart(DatasourceInterface* pDatasource);
   /** dtor **/
   virtual ~ProcessHistoryPart();
   /**
    * Indica si el Part tiene cambios para salvar.
    */
   virtual bool HasChanged();

   /**
    * Salva los cambios realizados en el Part.
    */
   virtual bool CommitChanges();

   /**
    * Restaura los valores originales del Part.
    */
   virtual bool RollbackChanges();

   /**
    * Inicializa el part.
    */
   virtual void SetInitialValues();

   /**
    * Actualiza el estado de la parte.
    */
   virtual void Update();

   /**
    * Crea la ventana de la parte.
    */
   virtual bool CreateToolWindow();

   /**
    * Retorna el icono de la herramienta.
    */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

   /**
    * Handler para el evento de click del boton guardar.
    */
   void OnSaveButtonClicked(wxCommandEvent &Event);

private:
   DatasourceInterface* pDatasource_;
   ProcessHistoryPartEventHandler* pEventHandler_;
   friend class ProcessHistoryPartEventHandler;
   /** Metodo que es llamado cuando se presiona el boton de exportar historial **/
   void OnExportButtonClicked(wxCommandEvent& Event);
};


}  /** namespace suri **/

#endif

