/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
