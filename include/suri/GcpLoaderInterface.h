/*! \file GcpLoaderInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GCPLOADERINTERFACE_H_
#define GCPLOADERINTERFACE_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "GcpList.h"
#include "suri/ParserResult.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Interfaz que representa el comportamiento de una clase encarga
 * de crear una lista de gcp a partir de un archivo.
 */
class GcpLoaderInterface {
public:
   /** ctor **/
   GcpLoaderInterface();
   /** dtor **/
   virtual ~GcpLoaderInterface();
   /** A partir del filename que se indica por parametro,
    * crea una lista de gcp correspondiente.
    * @param[in] FileName nombre del archivo con el path
    * @param[in] pList lista de puntos de control en la cual se cargaran los datos
    * @return true en caso de poder cargar correctamente los puntos de control
    * @return false en caso contrario (ej. pList == NULL)
    */
   virtual ParserResult Load(const std::string& FileName, GcpList& List) = 0;
   /** Guarda la lista de gcps en el path indicado por parametro.
    * @param[in] FileName nombre del archivo con el path en donde se guardara la lista
    * @param[in] pList lista de puntos de control que contiene la informacion a guardar
    * @return true en caso de poder guardar correctamente los puntos de control
    * @return false en caso contrario
    */
   virtual bool Save(const std::string& FileName, const GcpList& List) = 0;
};

} /** namespace suri */

#endif /* GCPLOADERINTERFACE_H_ */
