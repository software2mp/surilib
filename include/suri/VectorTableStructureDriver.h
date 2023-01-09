/*! \file VectorTableStructureDriver.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORTABLESTRUCTUREDRIVER_H_
#define VECTORTABLESTRUCTUREDRIVER_H_

// Includes Estandar
// Includes Suri
#include "DatasourceInterface.h"
#include "suri/VectorEditor.h"
#include "suri/MemoryDriver.h"
#include "VectorDatasource.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase que representa un driver encargado de mostrar en una tabla la estructura
 * de la tabla asociada a una fuente de datos vectorial
 */
class VectorTableStructureDriver : public suri::MemoryDriver {
   /** ctor copia **/
   VectorTableStructureDriver(const VectorTableStructureDriver&);

public:
   /** constructor **/
   explicit VectorTableStructureDriver(DatasourceInterface* pDatasource);
   /** destructor **/
   virtual ~VectorTableStructureDriver();
   /** numero de columna que corresponde al nombre del campo **/
   static const int FieldNameColumn;
   /** numero de columna que corresponde al tipo del campo **/
   static const int FieldTypeColumn;

private:
   /** fuente de datos que se representa en el driver **/
   VectorDatasource* pVectorDatasource_;
   SuriObject::UuidType id_; /*! Id del Datasource asociado*/
   VectorEditor vectorEditor_; /*! Editor del vecto */
   /** Metodo auxiliar que inicializa la tabla a partir de la fuente de datos**/
   void InitializeTable();
};

} /** namespace suri */

#endif /* VECTORTABLESTRUCTUREDRIVER_H_ */
