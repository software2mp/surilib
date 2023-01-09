/** \file GeneralPixelInformationPart.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GENERALPIXELINFORMATIONPART_H_
#define GENERALPIXELINFORMATIONPART_H_

#include "suri/Part.h"

namespace suri {

class ParameterCollection;

/**
 * Clase para visualizar informacion general de todas las
 * capas intersecadas.
 */
class GeneralPixelInformationPart : public suri::Part {
public:

   /**
    * Ctor
    */
   GeneralPixelInformationPart(ParameterCollection& Params);

   /**
    * Dtor
    */
   virtual ~GeneralPixelInformationPart();

   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();

   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();

   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();

   /** Inicializa el part */
   virtual void SetInitialValues();

   /** Actualiza el estado del Part */
   virtual void Update();

private:
   static const std::string kGeneralPixelInformationXrcName;
   static const std::string kGeneralPixelInformationTitle;

   int layersCount_;  // cantidad total de capas
   int layersWlCount_;  // cantidad total de capas con hipervinculo
   int rasterCount_;  // cantidad de capas raster
   int vectorCount_;  // cantidad de capas vectoriales
};

}  // namespace suri

#endif /* GENERALPIXELINFORMATIONPART_H_ */
