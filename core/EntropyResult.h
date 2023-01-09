/*! \file EntropyResult.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ENTROPYRESULT_H_
#define ENTROPYRESULT_H_

// Includes Estandar
// Includes Suri
#include "suri/Serializable.h"
#include "suri/Subset.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class EntropyResult : public Serializable{
public:
   /**
    * Constructor
    */
   EntropyResult(double entropy, int ulx, int uly, int lrx, int lry);
   /**
    * Destructor
    */
   virtual ~EntropyResult();
   /**
    * Devuelve un XML con los contenidos de la instancia serializados
    */
   wxXmlNode* SerializeXml() const;
   /**
    * Resultado del calculo de entropia
    */
   double entropy_;
   /**
    * Coordenadas de la baldosa sobre la que se calcula la entropia.
    */
   Subset subset_;
};

} /** namespace suri */

#endif /* ENTROPYRESULT_H_ */
