/*! \file MaxLikelihoodPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MAXLIKELIHOODPART_H_
#define MAXLIKELIHOODPART_H_

// Includes standard
// Includes Suri
#include "MaxLikelihoodAlgorithm.h"
#include "ParametersSelectionPart.h"
#include "suri/VectorStyle.h"
#include "ClusterClassificationPart.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(MaxLikelihoodPartWidgetEvent);

/** Genera nodo de renderizador que clasifica usando algoritmo maxima verosimilitud */
/**
 *	Esta clase al retornar la cantidad de clases asi como las porpiedades
 *	considera que la primer clase es siempre 'Sin Clase'.
 */
class MaxLikelihoodPart : public ClusterClassificationPart {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(MaxLikelihoodPartWidgetEvent);
   /** Ctor */
   MaxLikelihoodPart();
   /** Dtor */
   virtual ~MaxLikelihoodPart();

// --- Metodos de Part ---
   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Inicializa los datos mostrados */
   virtual void SetInitialValues();

// -- Metodos de ParametersSelectionPart ---
   /** Retorna el nombre del algoritmo que se usara para clasificar */
   virtual std::string GetAlgorithmName();

protected:
   /** Cambia el color del control */
   virtual void SetColourCtrlValue(VectorStyle::Color Color);
   /** Retorna el color del control */
   virtual VectorStyle::Color GetColourCtrlValue();
   /** Cambia el valor del slider */
   virtual void SetThresholdCtrlValue(double Threshold);
   /** Retorna el valor del slider */
   virtual double GetThresholdCtrlValue();
   /** modifica el texto que informa la posicion */
   virtual void OnScroll(wxScrollEvent& Event);

   /** Inicializo la clase */
   virtual MaxLikelihoodPart* InitializeClass() {
      return this;
   }
   EVENT_OBJECT_PROPERTY(MaxLikelihoodPartWidgetEvent); /*! Conecta los eventos */

   static const double DEFAULT_THRESHOLD;  /*! threshold inicial */
   static const VectorStyle::Color DEFAULT_COLOR;  /*! color inicial */
};
}  // namespace suri

#endif /* MAXLIKELIHOODPART_H_ */
