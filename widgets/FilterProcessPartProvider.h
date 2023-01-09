/*! \file FilterProcessPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILTERPROCESSPARTPROVIDER_H_
#define FILTERPROCESSPARTPROVIDER_H_

// Includes Estandar
// Includes Suri
#include "FileExporterProcessPartProvider.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase encargada de proveer los Parts para la configuracion del proceso de filtro **/
class FilterProcessPartProvider : public suri::FileExporterProcessPartProvider {
   /** ctor copia **/
   FilterProcessPartProvider(const FilterProcessPartProvider&);

public:
   /** Dtor **/
   virtual ~FilterProcessPartProvider();
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);
   /** Metodo para la creacion de un Part provider
    *  @return instancia del creador correspondiente
    *  @return NULL en caso contrario
    */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);

private:
   /** Ctor **/
   explicit FilterProcessPartProvider(ProcessAdaptLayer* pPal);
   int flags_; /*! Tipo de filtro que se quiere correr */
};

} /** namespace suri */

#endif /* FILTERPROCESSPARTPROVIDER_H_ */
