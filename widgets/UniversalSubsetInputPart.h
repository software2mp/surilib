/*! \file UniversalSubsetInputPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef UNIVERSALSUBSETINPUTPART_H_
#define UNIVERSALSUBSETINPUTPART_H_

// Includes Estandar

// Includes Suri
#include "UniversalCoordinateInputPart.h"
#include "suri/World.h"
#include "SpatialSelectionPart.h"
#include "suri/ProcessAtributeProvider.h"

// Includes Wx

// Defines

// forwards

namespace suri {

// forwards
class UniversalSubsetInputPartEvent;

/*!
 * Componente universal para Seleccion de Subset.
 */
class UniversalSubsetInputPart : public suri::UniversalCoordinateInputPart,
                                 public ProcessAtributeProvider {
   /** ctor copia **/
   UniversalSubsetInputPart(const UniversalSubsetInputPart&);

public:
   /** ctor **/
   UniversalSubsetInputPart(DataViewManager* pDataviewManager, LibraryManager *pManager,
                            World* pWorld, const Subset& InputSubset,
                            ProcessAdaptLayer* pProcessAdaptLayer = NULL);
   /** dtor **/
   virtual ~UniversalSubsetInputPart();
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool GetCoordinate(Coordinates &Coordinate);
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool SetCoordinate(Coordinates Coordinate);
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool GetSubset(Subset& OutputSubset);
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool SetSubset(Subset& NewSubset);
   /** Devuelve el world que se encuentra activo en el part **/
   World* GetWorld();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Informa si el subset seleccionado es valido */
   virtual bool HasValidData();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Inicializa el part */
   void SetInitialValues();

private:
   /**
    * Metodo auxiliar privado que agrega comportamiento adicional al commit de la
    * clase base. Este comportamiento adicional es necesario cuando
    * el commit involucra objetos adicionales a los items de biblioteca
    * del editor (ej. reproyeccion, en donde se necesita actualizar los params
    * de reproyeccion ademas de leer el item de biblioteca)
    * @return true en caso de realizar correctamente el commit "interno"
    * @return false en caso de que ocurra algun error durante el commit
    */
   virtual bool OnCommitChanges();
   /**
    * Metodo auxiliar privado que agrega comportamiento adicional al rollback de la
    * clase base. Este comportamiento adicional es necesario cuando
    * el rollback involucra objetos adicionales a los items de biblioteca
    * del editor (ej. parts compuesto en donde se necesita llamar a los rollbacks de
    * los mismos)
    * @return true en caso de realizar correctamente el rollback "interno"
    * @return false en caso de que ocurra algun error durante el rollback
    */
   virtual bool OnRollback();
   /** Metodo auxiliar que actualiza el widget correspondiente a el ingreso de coordenadas
    *  que corresponda*/
   virtual bool UpdateCoordinatesInputWidget(bool ReplaceWidget = false);
   /**
    *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
    *  Este metodo es el que realmente tienen que implementar las clases hijas para
    * lograr la especializacion a la hora de iniciar el part (metodo que se llama
    * en CreateToolWindow, a modo de Template Method).
    **/
   virtual bool ConfigureFeatures();
   friend class UniversalSubsetInputPartEvent;
   /** Evento que se ejecuta al des/activar el checkbox **/
   void OnCheckboxEventHandler(wxCommandEvent &Event);
   SpatialSelectionPart* pSubsetSelectionPart_;
   World* pWorld_;
   Subset inputSubset_;
   ProcessAdaptLayer* pProcessAdaptLayer_;
   UniversalSubsetInputPartEvent* pCheckEventHandler_;
   SpatialSelectionPart* pRasterSubsetSelectionPart_;
   bool raster_;
};

} /** namespace suri */

#endif /* UNIVERSALSUBSETINPUTPART_H_ */
