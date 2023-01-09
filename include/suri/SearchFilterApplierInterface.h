/*! \file SearchFilterApplierInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SEARCHFILTERAPPLIERINTERFACE_H_
#define SEARCHFILTERAPPLIERINTERFACE_H_

// Includes Estandar
#include <string>
// Includes Suri
// Includes Wx
// Defines
// forwards
namespace suri {

class SearchFilterApplierInterface {
public:
   /** Aplica el filtro recibido por parametro sobre la tabla */
   virtual void SetFilter(const std::string &Statement)=0;
   virtual void QueryBuilder()=0;
};

} //namespace suri

#endif /* SEARCHFILTERAPPLIERINTERFACE_H_ */
