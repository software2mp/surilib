/** \file TaskConfigBuilder.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TASKCONFIGBUILDER_H_
#define TASKCONFIGBUILDER_H_

// Includes Estandard
#include <string>

namespace suri {
// Forward Declarations
class TaskConfig;
class ParameterCollection;

/**
 * Define una interfaz para la creacion de configuraciones.
 */
class TaskConfigBuilder {

public:
   /** Constructor **/
   TaskConfigBuilder();
   /** Destructor **/
   virtual ~TaskConfigBuilder();
   /**
    * Metodo a implementar por los builders para devolver una
    * configuracion lista para usar.
    */
   virtual TaskConfig* Build(const ParameterCollection& ParamsP) = 0;
   static const std::string kDataViewParam;
   static const std::string kTaskParam;
   static const std::string kVectorDatasourceParam;
   static const std::string kFileNameParam;
   static const std::string kToolNameParam;
   static const std::string KRasterParam;
};

}  // namespace suri

#endif  // TASKCONFIGBUILDER_H_
