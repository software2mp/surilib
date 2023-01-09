/*! \file DefaultDatasourceList.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DEFAULTDATASOURCELIST_H_
#define DEFAULTDATASOURCELIST_H_

// Includes Estandar
#include <list>
#include <vector>

// Includes Suri
#include "suri/DatasourceContainerInterface.h"
#include "suri/DatasourceInterface.h"
#include "suri/LayerList.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Define una implementacion concreta utilizada por defecto en la aplicacion de DatasourceList
 */
class DefaultDatasourceList : public DatasourceContainerInterface {
public:
 	typedef std::list<DatasourceInterface *> DatasourceList; /*! Def Lista de fuente de datos */
  /** Constructor */
   DefaultDatasourceList();
   /** Destructor */
   virtual ~DefaultDatasourceList();
   /** Agrega una fuente de datos */
   virtual bool AddDatasource(DatasourceInterface* pDatasource);
   /** Quita una fuente de datos */
   virtual bool RemoveDatasource(const SuriObject::UuidType& Id);
   /** Obtiene una fuente de datos */
   virtual DatasourceInterface* GetDatasource(const SuriObject::UuidType& Id)const;
   /** Obtiene una fuente de datos \deprecated */
   virtual DatasourceInterface* GetDatasourceByElementId(
                                const SuriObject::UuidType& ElementId)const;
   /** Retorna una lista de Ids de las fuentes de datos */
   virtual UuidListType GetDatasourceList()const;
   /** Cambia el orden de  elemento de la lista hacia la posicion siguiente al elemento destino */
   virtual bool ChangeOrder(const SuriObject::UuidType& Id,
                            const SuriObject::UuidType& DestinationId);
   /** Mueve un elemento al final de la lista */
   virtual bool MoveToEnd(DatasourceInterface* pDatasource);
   /** Devuelve una lista con los Ids de la fuente de datos */
   virtual std::vector<SuriObject::UuidType> GetDatasourceIds()const;
   /** Verifica que no exista el Datasource en la lista */
   virtual bool Exists(DatasourceInterface* pDatasource);
	/** consulta si tiene mas fuente de datos **/
	bool HasNextDatasource(DatasourceList::const_iterator& Iterator) const {
		return Iterator != datasourceList_.end();
	}
	DatasourceList::const_iterator GetIterator() const {
		return datasourceList_.begin();                  
	}

private:
   /** Constructor de copia */
   DefaultDatasourceList(const DefaultDatasourceList &);
   DatasourceList datasourceList_; /*! Lista de fuente de datos */
};

} /** namespace suri */

#endif /* DEFAULTDATASOURCELIST_H_ */
