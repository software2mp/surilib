/*! \file ClassificationProcessPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CLASSIFICATIONPROCESSPARTPROVIDER_H_
#define CLASSIFICATIONPROCESSPARTPROVIDER_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "FileExporterProcessPartProvider.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// Forwards
class ClassificationPart;
class LayerList;

/** Clase encargada de proveer los parts para la configuracion del proceso de
 *  clasificacion
 */
class ClassificationProcessPartProvider : public suri::FileExporterProcessPartProvider {
   /** ctor copia **/
   ClassificationProcessPartProvider(const ClassificationProcessPartProvider&);

public:
   /** dtor **/
   virtual ~ClassificationProcessPartProvider();
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);
   /** Metodo para la creacion de un Part provider
    *  @return instancia del creador correspondiente
    *  @return NULL en caso contrario
    */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);

private:
   /** En funcion de los parametros de entrada selecciona el part de clasificacion acorde**/
   ClassificationPart* GetClassificationPart();
   std::string classificationType_;
   /** ctor **/
   explicit ClassificationProcessPartProvider(ProcessAdaptLayer* pPal);
};

} /** namespace suri */

#endif /* CLASSIFICATIONPROCESSPARTPROVIDER_H_ */
