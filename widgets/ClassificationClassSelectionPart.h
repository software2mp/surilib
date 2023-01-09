/*! \file ClassificationClassSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CLASSIFICATIONCLASSSELECTION_H_
#define CLASSIFICATIONCLASSSELECTION_H_

// Includes standard

// Includes Suri
#include "suri/PartCollection.h"
#include "suri/LayerList.h"

// Includes Wx

// Includes App

// Defines

namespace suri {
/** Permite seleccionar las clases sobre las que se quiere calcular la estadistica */
/**
 * Permite seleccionar una clase.
 * Ademas, permite enmascarar los valores no validos de la imagen de
 * entrada.
 * Por ultimo permite seleccionar el color y nombre de la clase 'sin clase'
 */
class ClassificationClassSelectionPart : public PartCollection {
public:
   /** Ctor */
   ClassificationClassSelectionPart(LayerList* pLayerList,
                                    bool NonClassifiedClassParametersSelector = true);
   /** Dtor */
   virtual ~ClassificationClassSelectionPart();

   /** Modifica contenido de la ventana */
// Oculta selector de propiedades de la clase 'sin clase'
   bool ConfigureWidget();

private:
   bool nonClassifiedClassParametersSelector_; /*! Flag. Si es true se */
   /*! agrega part para seleccionar areas de entrenamiento */
};
}

#endif /* CLASSIFICATIONCLASSSELECTION_H_ */
