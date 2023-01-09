/*! \file DatasourceContainerInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DATASOURCECONTAINERINTERFACE_H_
#define DATASOURCECONTAINERINTERFACE_H_

// Includes Estandar
#include <list>
#include <vector>

// Includes Suri
#include "SuriObject.h"
#include "DatasourceInterface.h"
#include "LayerList.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Define la interfaz para un contenedor de fuentes de datos
 */
class DatasourceContainerInterface {
public:
   typedef std::list<SuriObject::UuidType> UuidListType; /*! Lista con ids datasources contenidos */

   /** Constructor */
   DatasourceContainerInterface() {}
   /** Derstructor */
   virtual ~DatasourceContainerInterface() {}
   /** Agrega una fuente de datos */
   virtual bool AddDatasource(DatasourceInterface* pDatasource)=0;
   /** Quita una fuente de datos */
   virtual bool RemoveDatasource(const SuriObject::UuidType& Id)=0;
   /** Obtiene una fuente de datos \deprecated */
   virtual DatasourceInterface* GetDatasourceByElementId(
                                const SuriObject::UuidType& ElementId)const=0;
   /** Obtiene una fuente de datos */
   virtual DatasourceInterface* GetDatasource(const SuriObject::UuidType& Id)const=0;
   /** Retorna una lista de Ids de las fuentes de datos */
   virtual UuidListType GetDatasourceList()const=0;
   /** Cambia el orden de elemento de la lista hacia la posicion siguiente al elemento destino */
   virtual bool ChangeOrder(const SuriObject::UuidType& Id,
                            const SuriObject::UuidType& DestinationId)=0;
   /** Mueve un elemento al final de la lista */
   virtual bool MoveToEnd(DatasourceInterface *pDatasource)=0;
   /** Devuelve una lista con los Ids de la fuente de datos */
   virtual std::vector<SuriObject::UuidType> GetDatasourceIds() const=0;
   /** Verifica que no exista el Datasource en la lista */
   virtual bool Exists(DatasourceInterface* pDatasource)=0;

private:
   /** Constructor de copia */
   DatasourceContainerInterface(const DatasourceContainerInterface&);
};

} /** namespace suri */

#endif /* DATASOURCECONTAINERINTERFACE_H_ */
