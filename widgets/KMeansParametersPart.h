/*! \file KMeansParametersPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
// Includes Suri
#include "suri/StatisticsFunctions.h"
#include "ClassificationAlgorithmInterface.h"
// Includes Wx
// Includes App
// Defines

#ifndef KMEANSPARAMETERSPART_H_
#define KMEANSPARAMETERSPART_H_

#include "NonSupervisedParameterSelectionPart.h"

namespace suri {
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(KMeansParametersPartEvent);

class KMeansParametersPart : public suri::NonSupervisedParameterSelectionPart {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(KMeansParametersPartEvent);

   KMeansParametersPart();
   virtual ~KMeansParametersPart();

// --- Metodos de Part ---
   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa los datos mostrados */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();

// -- Metodos de ParametersSelectionPart ---
   /** Elimina todas las clases cargadas. */
   virtual bool Reset();
   /** Retorna el nombre del algoritmo que se usara para clasificar */
   virtual std::string GetAlgorithmName();
   /** Agrega clases que se usan para clasificar */
   virtual bool AddClass(int ClassId, Statistics Stats);
   /** Retorna un puntero al clasificador creado por part */
   virtual ClassificationAlgorithmInterface* GetClassificationManager();
   /** Retorna la cantidad de clases */
   virtual int GetClassCount();
   /** Retorna las propiedades de la clase en la posicion indicada. Las Posiciones van de 0 a */
// GetClassCount()-1.
   virtual bool GetClassProperties(ClassInformation &Properties);

   /** modifica el texto que informa la posicion */
   virtual void OnScroll(wxScrollEvent& Event);
protected:
   EVENT_OBJECT_PROPERTY(KMeansParametersPartEvent); /*! Objeto que conecta los */
   /* eventos */
};
}

#endif /* KMEANSPARAMETERSPART_H_ */
