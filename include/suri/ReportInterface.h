/** \file ReportInterface.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef REPORTINTERFACE_H_
#define REPORTINTERFACE_H_

// Includes standard
#include <string>

// Includes suri
#include "suri/SuriObject.h"

namespace suri {

/**
 * Clase de la que deben heredar todos los reportes que son resultado
 * de un proceso.
 */
class ReportInterface : public SuriObject {
public:
   /** Dtor */
   virtual ~ReportInterface() {}

   /** 
    * Retorna un string con el contenido del reporte 
    * @param[in] ReportFormat formato del reporte. Ej xml, html
    * @return reporte en formato pedido.
    */
   virtual std::string GetContentAsText(const std::string &ReportFormat) = 0;
};

} /* namespace suri */

#endif /* REPORTINTERFACE_H_ */
