/*! \file BufferPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef BUFFERPART_H_
#define BUFFERPART_H_

// Includes Estandar
// Includes Suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "VectorOperation.h"
// Includes Wx
// Defines

namespace suri {

// fordwards
class World;
class DataViewManager;

/** Declara clase BufferPartEvent */
DECLARE_EVENT_OBJECT(BufferPartEvent);

/** Part que permite configurar el calculo de buffer para capas vectoriales */
class BufferPart : public Part, public ProcessAtributeProvider {
   /** Declara friend la clase de evento BufferPartEvent */
   FRIEND_EVENT_OBJECT(BufferPartEvent);
public:
   /** Constructor */
   BufferPart(DataViewManager *pDataViewManager, World* pInputWorld);
   /** Destructor */
   virtual ~BufferPart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update() {}
   /** Retorna el icono de la herramienta. */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Devuelve si la parte contiene datos validos. */
   virtual bool HasValidData();

protected:
   /** Crea la ventana de la parte y conecta los eventos */
   bool CreateToolWindow();
   /** Habilita/Deshabilita los controles correspondientes al radio button */
   void SetStatus(bool isManual);
   /** Accion de seleccionar el radio button manual */
   void OnRadioButtonManual(wxCommandEvent &Event);
   /** Accion de seleccionar el radio button del campo */
   void OnRadioButtonField(wxCommandEvent &Event);
   /** Accion de seleccionar una opcion en el Choice de distancias */
   void OnChoiceSelection(wxCommandEvent &Event);
   /** Accion de seleccionar una opcion en el Choice de unidades */
   void OnUnitSelection(wxCommandEvent &Event);
   /** Accion de ingresar un valor en el TextCtrl de distancia */
   void OnDistanceTexCtrlChanged(wxCommandEvent &Event);
   /** Obtiene la distancia */
   double GetInternalDistance();
   /** Obtiene la distancia */
   double GetDistanceFromTextCtrl();
   /** Obtiene la cantidad de anillos concentricos */
   int GetInternalQuantity();
   /** Carga los choice del part */
   void LoadChoiceFields();
   /** Obtiene el nombre del campo seleccionado en el choice */
   std::string GetFieldFromChoice();
   /** Obtiene el nombre del campo seleccionado en el choice */
   std::string GetField();
   /** Obtiene el factor de conversion de la unidad a metros */
   double GetConversionFactor();
   /** Setea el factor de conversion de la unidad a metros */
   void SetConversionFactor();
   /** Retorna el valor del factor de conversion de la unidad a metros */
   double GetConversionFactorValue(std::string SelectedUnit);
   /** Puntero al mundo */
   World *pInputWorld_;
   /** Administrador con informacion de proyecto */
   DataViewManager *pDataViewManager_;
   /** Objeto que conecta los eventos */
   EVENT_OBJECT_PROPERTY(BufferPartEvent);
   double distance_; /** Distancia del buffer */
   int quantity_; /** Cantidad de anillos concentricos */
   std::string field_; /** Nombre de la columna */
   bool isfromfield_; /** Indica si la distancia proviene de una columna */
   bool reloadfield_ ; /** Indica si es necesario cargar el nombre de la columna nuevamente */
   double conversionfactor_;
   VectorOperation::OperationType operation_; /** Tipo de operacion */
};

} /** namespace suri */

#endif /* BUFFERPART_H_ */
