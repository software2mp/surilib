/*! \file SetupMeasurementPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SETUPMEASUREMENT_H_
#define SETUPMEASUREMENT_H_

#include "suri/Part.h"
#include "suri/DataViewManager.h"
#include "SimpleVectorStylePart.h"


namespace suri {

/** Clase encargada de administrar la configuracion de visualizacion de medicion de la aplicacion **/
class SetupMeasurementPart : public Part {
public:
   /** Ctor. */
   SetupMeasurementPart(DataViewManager* pDataViewManager);
   /** Dtor. */
   virtual ~SetupMeasurementPart();
   /** Crea el part (el interior de la ventana) */
   virtual bool CreateToolWindow();
   /** Asigna el icono para el part.  */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Inicializa los controles */
   virtual void SetInitialValues();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Actualiza el estado de la parte */
   virtual void Update();
private:
   /** Actualiza las opciones de configuracion en el archivo xml. */
   void WriteNewStyles();
   DataViewManager* pDataViewManager_;
   std::vector<std::string> parameters_;
   std::vector<std::string> panels_;
   // Parts.
   std::vector<ui::SimpleVectorStylePart*> pStyles_;
   // Estilos.
   std::string areaStyle_;
   std::string distanceStyle_;
};

} /* namespace suri */

#endif /* SETUPMEASUREMENT_H_ */
