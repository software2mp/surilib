/*! \file VectorOperationPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTOROPERATIONPART_H_
#define VECTOROPERATIONPART_H_

// Includes Estandar
// Includes Suri
#include "suri/ProcessAtributeProvider.h"
#include "suri/Part.h"
#include "suri/UseFeatureSelection.h"
#include "VectorOperation.h"
// Includes Wx
// Defines
// forwards

namespace suri {
/** Declara clase BufferPartEvent */
DECLARE_EVENT_OBJECT(VectorOperationPartEvent);

class VectorOperationPart : public UseFeatureSelection, public suri::ProcessAtributeProvider, public suri::Part {
   FRIEND_EVENT_OBJECT(VectorOperationPartEvent);
public:
   /** Constructor */
   VectorOperationPart();
   /** Destructor */
   virtual ~VectorOperationPart();
   /** Crea la ventana de la parte y contecta los eventos */
   bool CreateToolWindow();
   /** Accion de seleccionar una opcion en el Choice de unidades */
   void OnUnitSelection(wxCommandEvent &Event);
   /**
    * Retorna el icono de la herramienta.
    */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;

private:
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update() {}
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Carga los choice del part */
   void LoadChoiceFields();
   /** Accion de seleccionar una opcion en el Choice de primer capa */
   void OnFirstLayerSelection(wxCommandEvent &Event);
   /** Accion de seleccionar una opcion en el Choice de segunda capa */
   void OnSecondLayerSelection(wxCommandEvent &Event);
   /** Devuelve si la parte contiene datos validos. */
   bool HasValidData();
   /** Objeto que conecta los eventos */
   EVENT_OBJECT_PROPERTY(VectorOperationPartEvent);
   VectorOperation::OperationType operation_; /** Tipo de operacion */
   int firstchoice_; /** Id del elemento del primer choice */
   int secondchoice_; /** Id del elemento del segundo choice */
   std::vector<DatasourceInterface*> datasources_; /** Fuentes de datos a procesar */
   std::vector<DatasourceInterface*> inputdatasources_; /** Fuentes de datos de entrada */
   bool hasvaliddata_; /** Devuelve si la parte contiene datos validos */
};

} /** namespace suri */

#endif /* VECTOROPERATIONPART_H_ */
